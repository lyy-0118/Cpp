#include "PageCache.h"

//静态的成员不要在.h中定义，定义在.cpp中，防止链接错误
PageCache PageCache::_sInst; //单例对象

Span* PageCache::NewSpan(size_t k) {
	assert(k > 0 && k < PAGE_NUM); //申请的页数必须大于0
	//➀ k号桶有span
	if(!_spanLists[k].Empty()) {
		
		if (!_spanLists[k].Empty()) { //双重检查，防止在加锁之前其他线程已经把span取走了
			Span* span = _spanLists[k].PopFront(); //从k号桶中取出一个span返回

			//记录分配出去的span管理的页号和其地址的映射关系
			for(PageID i = 0; i < span->_n; ++i) {
				_idSpanMap[(PageID)(span->_pageID + i)] = span;
			}
			return span;
		}
	}
	//➁ k号桶中没有，往下找更大页的桶中有没有span，找到就把span拿出来拆开
	for (int i = k + 1; i < PAGE_NUM; ++i) {
		//[k+1,PAGE_NUM-1]号桶中找有没有span
		if (!_spanLists[i].Empty()) {
			//i号桶有span，对该span进行切分
			Span* nSpan = _spanLists[i].PopFront();
			//将nSpan切分成一个k页的和一个n-k页的span

			//k页的span
			//Span的空间是需要新建的，而不是用当前内存池中的空间
			Span* kSpan = new Span();
			kSpan->_pageID = nSpan->_pageID;
			kSpan->_n = k;

			//n-k页的span
			nSpan->_pageID += k;
			nSpan->_n -= k;
			//n-k页的放回对应哈希桶中
			_spanLists[nSpan->_n].PushFront(nSpan);
			//把n-k的span边缘页映射一下，方便后续合并
			_idSpanMap[(PageID)(nSpan->_pageID)] = nSpan;
			_idSpanMap[(PageID)(nSpan->_pageID) + nSpan->_n - 1] = kSpan;

			//记录kSpan的页号和span地址的映射关系
			for (PageID i = 0; i < kSpan->_n; ++i) {
				_idSpanMap[(PageID)(kSpan->_pageID + i)] = kSpan;
			}
			return kSpan; //返回k页的span
		}
	}
	//➂ k号桶中没有，且比k大的桶下面也没有，向系统申请
	void* ptr = SystemAlloc(PAGE_NUM-1); //向系统申请128页内存
	//开一个新span来管理这128页内存
	Span* span = new Span();
	span->_pageID = (PageID)ptr >> PAGE_SHIFT; //span管理的内存块的起始页号
	span->_n = PAGE_NUM - 1; //span管理的页数

	//放到对应哈希桶中
	_spanLists[span->_n].PushFront(span);
	//递归再次申请k页的span,这次递归一定会走到➁
	return NewSpan(k);

}

//通过页地址找到对应的span
Span* PageCache::MapObjectToSpan(void* obj) {
	PageID id = (PageID)obj >> PAGE_SHIFT; //通过对象地址计算出页号

	auto ret = _idSpanMap.find(id); //在映射表中找到页号对应的span

	//这里的逻辑是一定能保证通过块地址找到对应的span的，否则就出错了
	//因为每次从pc中申请span的时候都会把span管理的页号和span地址的映射关系记录到_idSpanMap中
	if(ret!=_idSpanMap.end()) {
		//这里的ret是一个迭代器，ret->first是页号，ret->second是span地址
		return ret->second;
	}
	else {
		assert(false); //如果找不到对应的span，说明程序有问题，直接断言失败
		return nullptr; //为了消除编译器的警告，实际不会执行到这里
	}
}

//管理cc归还的span
void PageCache::ReleaseSpanToPageCache(Span* span) {
	//向左不断合并
	while (1) {
		PageID leftId = span->_pageID - 1; //span左边的相邻页
		auto ret = _idSpanMap.find(leftId); //在映射表中找到左边页号对应的span
		
		//没有相邻span，break
		if(ret== _idSpanMap.end()) {
			break;
		}
		Span* leftSpan = ret->second; //左边的span地址
		//相邻span被分配给cc中在使用 break
		if(leftSpan->_isUse) {
			break;
		}
		//相邻span与当前span合并后超过128页，break
		if(span->_n + leftSpan->_n > PAGE_NUM - 1) {
			break;
		}
		//相邻span与当前span合并
		span->_pageID = leftSpan->_pageID; //合并后span的页号是左边span的页号
		span->_n += leftSpan->_n; //合并后span的页数是两者页数之和

		_spanLists[leftSpan->_n].Erase(leftSpan); //从空闲链表移除 leftSpan
		
		//不是释放左边那段页空间，
		//这里删掉的不是实际内存页，而是描述这段页的那个 Span 结构体对象
		delete leftSpan;
	}

	//向右不断合并
	while (1) {
		PageID rightId = span->_pageID + span->_n; //span右边的相邻页
		auto it = _idSpanMap.find(rightId); //在映射表中找到对应的span
		//没有相邻span，break
		if (it == _idSpanMap.end()) {
			break;
		}
		Span* rightSpan = it->second;
		//相邻span被分配给cc中在使用 break
		if (rightSpan->_isUse) {
			break;
		}
		//相邻span与当前span合并后超过128页，break
		if (rightSpan->_n + span->_n > PAGE_NUM - 1) {
			break;
		}
		//相邻span与当前span合并
		span->_n += rightSpan->_n;

		//把桶里面的span删掉
		_spanLists[rightSpan->_n].Erase(rightSpan);
		delete rightSpan;
	}
	//合并完，把当前span放到对应哈希桶中去
	_spanLists[span->_n].PushFront(span);
	span->_isUse = false; //cc返回到pc，isUse标记为false，表示这个span不在cc中被使用了

	//映射当前span的边缘页，方便后续合并
	_idSpanMap[(PageID)(span->_pageID)] = span;
	_idSpanMap[(PageID)(span->_pageID) + span->_n - 1] = span;
}