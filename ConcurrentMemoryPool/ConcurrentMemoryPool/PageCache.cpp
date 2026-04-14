#include "PageCache.h"

//静态的成员不要在.h中定义，定义在.cpp中，防止链接错误
PageCache PageCache::_sInst; //单例对象

Span* PageCache::NewSpan(size_t k) {
	assert(k > 0 && k < PAGE_NUM); //申请的页数必须大于0
	//➀ k号桶有span
	if(!_spanLists[k].Empty()) {
		
		if (!_spanLists[k].Empty()) { //双重检查，防止在加锁之前其他线程已经把span取走了
			return _spanLists[k].PopFront(); //从k号桶中取出一个span返回
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
			kSpan->n = k;

			//n-k页的span
			nSpan->_pageID += k;
			nSpan->n -= k;

			//n-k页的放回对应哈希桶中
			_spanLists[nSpan->n].PushFront(nSpan);
			return kSpan; //返回k页的span
		}
	}
	//➂ k号桶中没有，且比k大的桶下面也没有，向系统申请
	void* ptr = SystemAlloc(PAGE_NUM-1); //向系统申请128页内存
	//开一个新span来管理这128页内存
	Span* span = new Span();
	span->_pageID = (PageID)ptr >> PAGE_SHIFT; //span管理的内存块的起始页号
	span->n = PAGE_NUM - 1; //span管理的页数

	//放到对应哈希桶中
	_spanLists[span->n].PushFront(span);
	//递归再次申请k页的span,这次递归一定会走到➁
	return NewSpan(k);

}