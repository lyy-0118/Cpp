#pragma once
#include "Common.h"


template<class T>
class ObjectPool {
public:
	T* New() {
		T* obj = nullptr; //最终要返回的对象指针

		//如果自由链表有对象，直接取(之前分配的有还回来的对象)，否则从内存块中分配
		if (_freeList) {
			void* next = *(void**)_freeList; //next指向自由链表中下一个对象的指针
			obj = (T*)_freeList; //obj指向当前自由链表头部的对象
			_freeList = next; //更新自由链表头部为下一个对象
			// 头删操作
		}
		else {
			//内存块中剩余的字节数不足以分配一个对象时，申请新的内存块
			if (_leftBytes < sizeof(T)) {
				_leftBytes = 128 * 1024; //每次申请128KB的内存块
				//_memory =(char*)malloc(_leftBytes); //从堆上申请内存块

				// 右移13位，就是除以8KB，也就是得到的是16，这里就表示申请16页
				_memory = (char*)SystemAlloc(_leftBytes >> 13);

				if (_memory == nullptr) {
					throw std::bad_alloc(); // 申请失败抛出异常
				}
			}
			obj = (T*)_memory; //obj指向内存块的当前起始位置
			//类型T的大小可能小于指针大小，为后续delete保存指向下一块内存的地址，分配的对象大小至少为指针大小
			size_t objSize = sizeof(T) < sizeof(void*) ? sizeof(void*) : sizeof(T);
			_memory += objSize;
			_leftBytes -= objSize;
		}
		/*定位 new
		在已有内存 obj 上构造 T
		调用构造函数
		不分配内存*/
		new(obj) T;
		return obj;
	}
	void Delete(T* obj) {
		if (!obj) return;

		// 显⽰调⽤的T的析构函数进⾏清理
		obj->~T();

		// 将 obj 头插进⾃由链表
		*(void**)obj = _freeList; // 将当前自由链表头部的指针存储在 obj 中，形成链表连接
		_freeList = obj; // 更新自由链表头部为 obj

	}
private:
	char* _memory = nullptr; //指向内存块的指针
	size_t _leftBytes = 0;  //内存块中剩余的字节数
	void* _freeList = nullptr; //管理还回来的内存对象的自由链表
public:
	std::mutex _poolMtx; //防止threadcache申请时申请到空指针 

};
//【注】这里定长内存池是不会将空间释放的，
// 这样并不会造成内存泄漏的问题，因为这些空间由某个对象申请，不断地申请归还，
// 只要进程是正常结束的，最后还是会归还所有的空间，因为进程结束之后OS会自动回收整个进程的空间。

//struct TreeNode
//{
//	int _val;
//	TreeNode* _left;
//	TreeNode* _right;
//	TreeNode()
//		:_val(0)
//		, _left(nullptr)
//		, _right(nullptr)
//	{
//	}
//};
//
//void TestObjectPool()
//{
//	// 申请释放的轮次
//	const size_t Rounds = 3;
//	// 每轮申请释放多少次
//	const size_t N = 100000;
//	size_t begin1 = clock();
//	std::vector<TreeNode*> v1;
//	v1.reserve(N);
//	for (size_t j = 0; j < Rounds; ++j) {
//		for (int i = 0; i < N; ++i)
//		{
//			v1.push_back(new TreeNode);
//		}
//		for (int i = 0; i < N; ++i)
//		{
//			delete v1[i];
//		}
//		v1.clear();
//	}
//	size_t end1 = clock();
//	ObjectPool<TreeNode> TNPool;
//	size_t begin2 = clock();
//	std::vector<TreeNode*> v2;
//	v2.reserve(N);
//	for (size_t j = 0; j < Rounds; ++j)
//	{
//		for (int i = 0; i < N; ++i)
//		{
//			v2.push_back(TNPool.New());
//		}
//		for (int i = 0; i < 100000; ++i)
//		{
//			TNPool.Delete(v2[i]);
//		}
//		v2.clear();
//	}
//	size_t end2 = clock();
//	cout << "new cost time:" << end1 - begin1 << endl;
//	cout << "object pool cost time:" << end2 - begin2 << endl;
//}
//
////int main()
////{
////	TestObjectPool();
////    return 0;
////}