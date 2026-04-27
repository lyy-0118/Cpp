#include "ThreadCache.h"
#include "CentralCache.h"

#include <algorithm>

void* ThreadCache::Allocate(size_t size) {
	assert(size <= MAX_BYTES); // tc中单次申请内存不能超过256KB

	size_t alignSize = SizeClass::RoundUp(size);
	size_t index = SizeClass::Index(alignSize);

	// 先从对应的自由链表桶中申请空间
	if (!_freeLists[index].Empty()) {
		return _freeLists[index].Pop();
	}

	// 对应桶没有空间，向CentralCache申请
	return FetchFromCentralCache(index, alignSize);
}

// 释放内存，obj是要释放的对象，size是对象的大小
void ThreadCache::Deallocate(void* obj, size_t size) {
	assert(obj); // 释放的对象不能为空
	assert(size <= MAX_BYTES);

	size_t index = SizeClass::Index(size);  //找到size的自由链表
	_freeLists[index].Push(obj);    //把obj放回对应的自由链表桶中

	//当前桶中的块数大于等于单批次申请块数的时候返还空间给CC
	if (_freeLists[index].Size()> _freeLists[index].MaxSize()) {
		ListTooLong(_freeLists[index], size);
	}
}

void* ThreadCache::FetchFromCentralCache(size_t index , size_t alignSize) {
	// 慢启动上限值，第一次申请就是1块，之后每次申请的块数逐渐增加，直到达到上限值
	// 通过MaxSize和NumMoveSize控制cc给tc提供多少块alignSize大小的空间
#ifdef WIN32
	size_t batchNum = min(_freeLists[index].MaxSize(), SizeClass::NumMoveSize(alignSize));
#else
	//其他系统中的用std::min，WIN32中用min，linux中用std::min，linux中需要加上std命名空间
	size_t batchNum = std::min(_freeLists[index].MaxSize(), SizeClass::NumMoveSize(alignSize));
#endif //WIN32
	if (batchNum == _freeLists[index].MaxSize()) {
		_freeLists[index].MaxSize() += 1; // 下次多给一块
	}

	// 输出型参数，[start,end]是tc想要的空间
	void* start = nullptr;
	void* end = nullptr;

	// 实际得到的块数
	size_t actualNum = CentralCache::GetInstance()->FetchRangeObj(start, end, batchNum, alignSize);

	assert(actualNum >= 1);
	if (actualNum == 1) {
		assert(start == end);
		return start;
	}

	// actualNum > 1，给tc回填[start下一个, end]
	_freeLists[index].PushRange(FreeList::ObjNext(start), end, actualNum-1);
	return start;
}

//tc向cc归还list桶中的空间，list是要归还的自由链表，size是要归还的空间的大小
void ThreadCache::ListTooLong(FreeList& list, size_t size) {
	void* start = nullptr;
	void* end = nullptr;

	//start和end分别指向list中要归还给cc的空间的起始地址和结束地址，是输出型参数
	list.PopRange(start, end, list.MaxSize()); //从list中弹出SizeClass::NumMoveSize(size)块空间，[start,end]是弹出的空间

	//归还给cc
	CentralCache::GetInstance()->ReleaseListToSpans(start, size); //这里不需要传end，因为PopRange能保证end后面就是空，所以只需要判断一下next为不为空就能判断出是不是end了。
	
}