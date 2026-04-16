#pragma once
#include "ThreadCache.h"
#include "PageCache.h"
//其实就是tcmalloc,线程调用这个函数申请空间
void* ConcurrentAlloc(size_t size) {
	if(size > MAX_BYTES) {
		//（256,1024] 找pc
		size_t alignSize = SizeClass::RoundUp(size);  //先按照页大小对齐
		size_t k = alignSize >> PAGE_SHIFT; //算出来对齐之后要多少页

		PageCache::GetInstance()->_pageMtx.lock(); // 对pc中的span进行操作，加锁
		Span* span = PageCache::GetInstance()->NewSpan(k); // 直接向pc要
		PageCache::GetInstance()->_pageMtx.unlock(); // 解锁
		void* ptr = (void*)(span->_pageID << PAGE_SHIFT); // 通过获得到的span来提供空间
		return ptr;
	}
	else {  //申请空间小于等于 256KB 就走原先的逻辑 
		if(pTLSThreadCache == nullptr) { //如果当前线程还没有ThreadCache对象，就创建一个
			pTLSThreadCache = new ThreadCache();
		}
		return pTLSThreadCache->Allocate(size); //否则就直接用当前线程的ThreadCache对象来申请内存
	}
}

//线程调用这个函数释放空间
void ConcurrentFree(void* obj, size_t size) {
	assert(obj); // 释放的对象不能为空

	if(size > MAX_BYTES) {
		//（256,1024] 找pc
		// 通过obj找到对应的span，因为前面申请空间的
	    // 时候已经保证了维护的空间首页地址已经映射过了
		Span* span = PageCache::GetInstance()->MapObjectToSpan(obj);
		PageCache::GetInstance()->_pageMtx.lock(); // 记得加锁解锁
		PageCache::GetInstance()->ReleaseSpanToPageCache(span); // 直接通过span释放空间
		PageCache::GetInstance()->_pageMtx.unlock(); // 记得加锁解锁
	}
	else {
		if(pTLSThreadCache == nullptr) { //如果当前线程还没有ThreadCache对象，就创建一个
			pTLSThreadCache = new ThreadCache();
		}
		pTLSThreadCache->Deallocate(obj, size); //否则就直接用当前线程的ThreadCache对象来释放内存
	}
}