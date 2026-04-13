#pragma once
#include "ThreadCache.h"
//其实就是tcmalloc,线程调用这个函数申请空间
void* ConcurrentAlloc(size_t size) {
	cout << "Thread " << std::this_thread::get_id() <<" "<<pTLSThreadCache << endl;
	if(size > MAX_BYTES) { //如果申请的空间超过了ThreadCache单次申请的最大字节数，就直接向系统申请内存
		return malloc(size);
	}
	else {
		if(pTLSThreadCache == nullptr) { //如果当前线程还没有ThreadCache对象，就创建一个
			pTLSThreadCache = new ThreadCache();
		}
		return pTLSThreadCache->Allocate(size); //否则就直接用当前线程的ThreadCache对象来申请内存
	}
}

//线程调用这个函数释放空间
void ConcurrentFree(void* obj, size_t size) {
	if(size > MAX_BYTES) { //如果释放的空间超过了ThreadCache单次申请的最大字节数，就直接向系统释放内存
		free(obj);
	}
	else {
		if(pTLSThreadCache == nullptr) { //如果当前线程还没有ThreadCache对象，就创建一个
			pTLSThreadCache = new ThreadCache();
		}
		pTLSThreadCache->Deallocate(obj, size); //否则就直接用当前线程的ThreadCache对象来释放内存
	}
}