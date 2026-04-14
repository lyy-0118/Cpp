#include "CentralCache.h"
#include "PageCache.h"
#include <algorithm>
#include <cstdlib>

// 静态成员类内声明，类外定义和初始化
CentralCache CentralCache::_sInst;

// cc从管理空间非空的span取出一段batchNum个alignSize大小的块空间给tc
size_t CentralCache::FetchRangeObj(void*& start, void*& end, size_t batchNum, size_t alignSize) {
	// 获取当前size对应SpanList的索引
	size_t index = SizeClass::Index(alignSize);
	SpanList& list = _spanLists[index];

	list._mtx.lock(); //锁住当前桶，保护span链表的安全访问

	// 获取一个管理空间非空的span
	Span* span = GetOneSpan(list, alignSize);
	assert(span);
	assert(span->_freeList);

	// 取空间[start,end]
	start = end = span->_freeList;
	size_t actualNum = 1; //记录实际取到的块数，初始值为1，因为至少能取到一块空间
	// batchNum是tc想要的块数，actualNum是cc实际能提供的块数
	// 如果batchNum大于actualNum，就继续往span的链表上取空间，直到达到batchNum或者span的链表上没有空间了
	while (actualNum < batchNum && FreeList::ObjNext(end) != nullptr) {
		end = FreeList::ObjNext(end);
		++actualNum;
	}

	// 将[start,end]返回给tc后，调整span的链表指向
	span->_freeList = FreeList::ObjNext(end);
	// 返回的空间[end]的下一个对象指向nullptr，不要与原先span的链表连接在一起了
	FreeList::ObjNext(end) = nullptr;
	span->_useCount += actualNum;  //给tc分了多少就给_useCount加多少

	list._mtx.unlock(); //解锁

	return actualNum;
}

// 参数list是cc中管理size大小的span链表，size是tc申请的空间大小
Span* CentralCache::GetOneSpan(SpanList& list, size_t size) {

	//在cc中找有没有管理空间非空的span，如果有就返回这个span
	Span* it = list.Begin();
	while (it!=list.End()) {
		if (it->_freeList != nullptr) {
			return it;  //找到了管理空间非空的span
		}
		else it = it->_next;   //没找到，继续找下一个span
	}
	//走到这里说明cc中没有管理空间非空的span了，就要向pc中申请全新的span

	//将size转换成页数，向pc申请span
	size_t k = SizeClass::NumMovePage(size);

	//先把cc的桶锁解掉，这样如果其他线程释放内存对象回来不会死锁
	list._mtx.unlock();

	//解决死锁的办法三：在调用NewSpan的地方加锁
	PageCache::GetInstance()->_pageMtx.lock();   //加锁，保护pc中span链表的安全访问
	Span* span = PageCache::GetInstance()->NewSpan(k); //调用NewSpan获取一个全新span
	PageCache::GetInstance()->_pageMtx.unlock(); //解锁

	//start地址为当前页号*页大小，end地址为start地址加上span管理的页数*页大小
	char* start = (char*)(span->_pageID << PAGE_SHIFT); //span管理的内存块的起始地址
	char* end = start + (span->n << PAGE_SHIFT); //span管理的内存块的结束地址

	//开始切分span管理的空间，不需要加锁，因为这个span是新申请的，还没有被其他线程访问过
	span->_freeList = start;
	void* tail = start;
	start += size; //切分出第一块空间，start指向下一块空间的起始地址
	//链接各个块
	while (start < end) {
		FreeList::ObjNext(tail) = start; //把tail的下一个对象指向start
		tail = start; //tail指向start
		start += size; //start指向下一块空间的起始地址
	}
	FreeList::ObjNext(tail) = nullptr; //最后一个块的下一个对象指向nullptr，表示链表结束

	//切好span后，把span挂到cc对应下标的桶里去
	list._mtx.lock(); //加锁
	list.PushFront(span);

	return span;
}
