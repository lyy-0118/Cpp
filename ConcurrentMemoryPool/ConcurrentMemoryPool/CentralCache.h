#pragma once
#include"Common.h"

//整个进程中CentralCache是只有一个的
//想要让所有tc都能访问到这一个cc对象
//用单例模式来实现CentralCache
class CentralCache {
public:
	//
	static CentralCache* GetInstance()
	{
		return &_sInst;
	}
	// 获取⼀个⾮空的span
	Span* GetOneSpan(SpanList& list, size_t byte_size);

	// 从cc获取⼀定数量的对象给thread cache
	size_t FetchRangeObj(void*& start, void*& end, size_t batchNum, size_t alignSize);
	
private:
	//单例，去掉构造、拷构和拷赋
	CentralCache()
	{ }

	CentralCache(const CentralCache& copy) = delete;
	CentralCache& operator = (const CentralCache& copy) = delete;

	SpanList _spanLists[NFREELIST]; //哈希桶，存储Span链表
	static CentralCache _sInst; //饿汉模式创建一个CentralCache
};
