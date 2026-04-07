#include "CentralCache.h"

#include <algorithm>
#include <cstdlib>

// 静态成员类内声明，类外定义和初始化
CentralCache CentralCache::_sInst;

// cc从管理空间非空的span取出一段batchNum个alignSize大小的块空间给tc
size_t CentralCache::FetchRangeObj(void*& start, void*& end, size_t batchNum, size_t alignSize) {
	// 获取当前size对应SpanList的索引
	size_t index = SizeClass::Index(alignSize);
	SpanList& list = _spanLists[index];

	std::lock_guard<std::mutex> lock(list._mtx); //锁住当前桶，保护span链表的安全访问

	// 获取一个管理空间非空的span
	Span* span = GetOneSpan(list, alignSize);
	assert(span);
	assert(span->_freeList);

	// 取空间[start,end]
	start = end = span->_freeList;
	size_t actualNum = 1; //记录实际取到的块数，初始值为1，因为至少能取到一块空间
	while (actualNum < batchNum && FreeList::ObjNext(end) != nullptr) {
		end = FreeList::ObjNext(end);
		++actualNum;
	}

	// 将[start,end]返回给tc后，调整span的链表指向
	span->_freeList = FreeList::ObjNext(end);
	// 返回的空间[end]的下一个对象指向nullptr，不要与原先span的链表连接在一起了
	FreeList::ObjNext(end) = nullptr;
	span->_useCount += actualNum;

	return actualNum;
}

Span* CentralCache::GetOneSpan(SpanList& list, size_t byte_size) {
	for (Span* cur = list.Begin(); cur != list.End(); cur = cur->_next) {
		if (cur->_freeList != nullptr) {
			return cur;
		}
	}

	// 当前桶没有可用span，创建一个新的span并切分对象
	Span* span = new Span();
	const size_t objSize = std::max(byte_size, sizeof(void*));
	const size_t bytes = 64 * 1024;
	const size_t nobj = std::max((size_t)1, bytes / objSize);

	char* mem = static_cast<char*>(std::malloc(nobj * objSize));
	if (mem == nullptr) {
		throw std::bad_alloc();
	}

	span->_freeList = mem;
	void* cur = mem;
	for (size_t i = 1; i < nobj; ++i) {
		void* next = mem + i * objSize;
		FreeList::ObjNext(cur) = next;
		cur = next;
	}
	FreeList::ObjNext(cur) = nullptr;

	list.PushFront(span);
	return span;
}
