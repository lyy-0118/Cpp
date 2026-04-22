#pragma once
#include "Common.h"
#include "ObjectPool.h"
#include "PageMap.h"

class PageCache
{
public:
	//饿汉单例模式，整个进程中PageCache只有一个实例，所有线程都能访问到这个实例
	static PageCache* GetInstance() {
		return &_sInst;
	}

	Span* NewSpan(size_t n); //向系统申请n页内存，返回一个span管理这n页内存

	Span* MapObjectToSpan(void* obj);

	//管理cc归还的span
	void ReleaseSpanToPageCache(Span* span);
private:
	//单例模式，构造函数私有化，禁止拷贝和赋值
	PageCache() {}
	PageCache(const PageCache&) = delete;
	PageCache& operator=(const PageCache&) = delete;

	static PageCache _sInst; //饿汉模式创建一个PageCache实例
private:
	SpanList _spanLists[PAGE_NUM]; //PC中的哈希桶，管理不同大小的span链表
	
	//std::unordered_map<PageID, Span*> _idSpanMap; //页ID到span的映射表，方便通过页ID找到对应的span
	TCMalloc_PageMap1<32-PAGE_SHIFT> _idSpanMap;

	ObjectPool<Span> _spanPool;  //创建span的对象池
public:
	std::mutex _pageMtx; //全局锁，保护span链表的安全访问
};