#pragma once
#include "Common.h"

class ThreadCache
{
public:
	/*ThreadCache();
	~ThreadCache();*/
	void* Allocate(size_t size);   //线程申请size大小的空间
	void Deallocate(void* obj, size_t size);  //线程释放大小为size的obj的空间

	//ThreadCache中空间不够时，向CentralCache申请空间
	//参数index是指申请的空间在哈希表中的下标，alignSize是指申请的空间的对齐数
	void* FetchFromCentralCache(size_t index,size_t alignSize);

	//tc向cc归还list桶中的空间，list是要归还的自由链表，size是要归还的空间的大小
	void ListTooLong(FreeList& list, size_t size);

private:
	FreeList _freeLists[NFREELIST]; //哈希,每个桶表示一个自由链表
};

static _declspec(thread) ThreadCache* pTLSThreadCache=nullptr; //每个线程都有一个ThreadCache对象，线程局部存储
//注意要给成static，否则每个文件都包含这个头文件时，都会定义一个pTLSThreadCache变量，导致链接错误