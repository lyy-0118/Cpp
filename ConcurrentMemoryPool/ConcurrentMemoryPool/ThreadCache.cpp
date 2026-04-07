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

void ThreadCache::Deallocate(void* obj, size_t size) {
	assert(obj); // 释放的对象不能为空
	assert(size <= MAX_BYTES);

	size_t index = SizeClass::Index(size);
	_freeLists[index].Push(obj);
}

void* ThreadCache::FetchFromCentralCache(size_t index, size_t alignSize) {
	// 通过MaxSize和NumMoveSize控制cc给tc提供多少块alignSize大小的空间
	size_t batchNum = std::min(_freeLists[index].MaxSize(), SizeClass::NumMoveSize(alignSize));

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
	_freeLists[index].PushRange(FreeList::ObjNext(start), end);
	return start;
}
