#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>

#include<time.h>
#include <assert.h>

#include <thread>
#include <mutex>

using std::cout;
using std::endl;

//C++中尽量避免用宏，与之替换的是static const
static const size_t NFREELIST = 208;  //自由链表的数量，208是根据内存池的设计来定的，具体数值可以根据实际需求调整
static const size_t MAX_BYTES = 256*1024; //ThreadCache单次申请的最大字节数
static const size_t PAGE_NUM = 129;       //PageCache中每次向系统申请的页数，129是根据内存池的设计来定的，具体数值可以根据实际需求调整
static const size_t PAGE_SHIFT = 13;      //一页为8KB,右移13位算页数

//size_t的大小会随着平台位数发生变化，32位下size_t是unsigned int（4字节），64位下是unsigned __int64（8字节）
typedef size_t PageID;

#ifdef _WIN32
#include<Windows.h> // Windows下的头文件
#else
// 这里是Linux相关的头文件，就不写出来了
#endif // _WIN32

// 直接去堆上按页申请空间
inline static void* SystemAlloc(size_t kpage)
{
#ifdef _WIN32 // Windows下的系统调用接口
	void* ptr = VirtualAlloc(0, kpage << PAGE_SHIFT, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
	// linux下brk mmap等
#endif

	if (ptr == nullptr)
		throw std::bad_alloc(); // 申请失败抛出异常

	return ptr;
}

// 直接去堆上释放空间
inline static void SystemFree(void* ptr)
{
#ifdef _WIN32
	VirtualFree(ptr, 0, MEM_RELEASE);
#else
	// sbrk unmmap等
#endif
}

//FreeList实现——ThreadCache中的哈希桶
class FreeList
{
public:
	FreeList() : _freeList(nullptr) {}

	size_t Size() const { //返回当前自由链表中块的数量
		return _size;
	}

	bool Empty() { //判断哈希桶是否为空
		return _freeList == nullptr;
	}
	//如果没有引用，返回的是一个右值，
	//因为ObjNext返回值是一个拷贝，是一个临时对象，
	//而临时对象具有常属性，不能被修改，也就是一个右值，右值无法进行赋值操作：
	static void*& ObjNext(void* obj)  //返回obj的下一个对象地址
	{
		return *(void**)obj;
	}

	void Push(void* obj)
	{
		assert(obj); //插入的对象不能为空
		//头插
		//*(void**)obj = _freeList;
		ObjNext(obj) = _freeList;
		_freeList = obj;

		++_size; //当前自由链表中块的数量加1
	}

	void* Pop()
	{
		//在链表头删并返回一个内存块obj
		assert(_freeList); //弹出的对象不能为空
		void* obj = _freeList;
		_freeList = ObjNext(_freeList);

		--_size; //当前自由链表中块的数量减1
		return obj;
	}

	//向自由链表中头插，且插入多块空间
	void PushRange(void* start, void* end,size_t size) {
		//把[start,end]这n块空间插入到自由链表中
		ObjNext(end) = _freeList; //end的下一个对象指向原来的链表头
		_freeList = start; //链表头指向start

		_size += size; //当前自由链表中块的数量加size
	}

	//从自由链表中弹出N个块空间，[start,end]是弹出的空间
	void PopRange(void*& start, void*& end, size_t n) {
		//从自由链表中弹出size块空间，[start,end]是弹出的空间
		assert(n <= _size); //弹出的块数不能超过当前自由链表中块的数量
		start = end = _freeList; //start指向链表头
		
		for(size_t i = 0;i < n-1;++i) {
			end = ObjNext(end); //end指向下一个对象
		}

		_freeList = ObjNext(end); //链表头指向end的下一个对象
		ObjNext(end) = nullptr;   //end的下一个对象指向nullptr，表示弹出的空间已经脱离了链表
		_size -= n; //当前自由链表中块的数量减size
	}

	//tc向cc申请内存，未达到上限时能申请到的最大块空间
	size_t& MaxSize() {
		return _maxSize;
	}
private:
	void* _freeList = nullptr;
	size_t _maxSize = 1; //当前自由链表申请未达到上限时，能申请的最大快空间是多少
						 //初始值给1，表示第一次申请的就是1块
						 //到了上限之后_maxSize这个值就作废了
	size_t _size = 0; //当前自由链表中块的大小，单位是字节，初始化为0，表示还没有块被分配到这个链表中
};

//当线程申请一个size时，要计算出对应对齐之后的字节数
//专门搞一个SizeClass类来计算对齐后的字节数，内部提供接口：
class SizeClass {
public:
	//根据size计算应该落到哪个自由链表桶
	static inline size_t _Index(size_t size, size_t align_shift)
	{							/*这里align_shift是指对齐数的二进制位数。比如size为2的时候对齐数
								为8，8就是2^3，所以此时align_shift就是3*/
		return ((size + ((size_t)1 << align_shift) - 1) >> align_shift) - 1;
		//这里_Index计算的是当前size所在区域的第几个下标，所以Index的返回值需要加上前面所有区域的哈希桶的个数
	}

	// 计算映射的哪一个自由链表桶
	static inline size_t Index(size_t size)
	{
		assert(size <= MAX_BYTES);
		// 每个区间有多少个链
		static int group_array[4] = { 16, 56, 56, 56 };
		if (size <= 128)
		{ // [1,128] 8B -->8B就是2^3B，对应二进制位为3位
			return _Index(size, 3); // 3是指对齐数的二进制位位数，这里8B就是2^3B，所以就是3
		}
		else if (size <= 1024)
		{ // [128+1,1024] 16B -->4位
			return _Index(size - 128, 4) + group_array[0];
		}
		else if (size <= 8 * 1024)
		{ // [1024+1,8*1024] 128B -->7位
			return _Index(size - 1024, 7) + group_array[1] + group_array[0];
		}
		else if (size <= 64 * 1024)
		{ // [8*1024+1,64*1024] 1024B -->10位
			return _Index(size - 8 * 1024, 10) + group_array[2] + group_array[1]
				+ group_array[0];
		}
		else if (size <= 256 * 1024)
		{ // [64*1024+1,256*1024] 8 * 1024B  -->13位
			return _Index(size - 64 * 1024, 13) + group_array[3] +
				group_array[2] + group_array[1] + group_array[0];
		}
		else
		{
			assert(false);
		}
		return -1;
	}
	//大佬写法，位运算实现向上取整，效率更高
	static size_t _RoundUp(size_t size, size_t alignNum) {
		//size是线程申请的字节数，alignNum是对应分区的对齐数，
		return ((size + alignNum - 1) & ~(alignNum - 1));
	}
	//static size_t _RoundUp(size_t size, size_t alignNum) {
	//	size_t res = 0;
	//	if (size % alignNum != 0) {
	//		//有余数，说明size不是alignNum的整数倍，需要向上取整
	//		res = (size / alignNum + 1) * alignNum;
	//	}
	//	else {
	//		//没有余数，说明size是alignNum的整数倍，不需要向上取整
	//		res = size;
	//	}
	//	return res;
	//}
	//把申请大小按规则向上对齐
	static size_t RoundUp(size_t size)  //计算对齐后的字节数
	{
		if (size <= 128) 
			return _RoundUp(size,8); //[1,128]字节的空间8字节对齐
		else if (size <= 1024) 
			return _RoundUp(size, 16); //[128+1,1024]字节的空间16字节对齐
		else if (size <= 8*1024) 
			return _RoundUp(size, 128); //[1024+1,8*1024]字节的空间128字节对齐
		else if (size <= 64*1024)
			return _RoundUp(size, 1024); //[8*1024+1,64*1024]字节的空间1024字节对齐
		else if(size <256*1024)
			return _RoundUp(size, 8 * 1024); //[64*1024+1,256*1024]字节的空间8 * 1024字节对齐
		else {
			//单次申请空间大于256KB，直接按照页来对齐
			//把 size 按 8192 字节（8KB）向上对齐
			//虽然这里跟上面的一样，但如果后续想要修改页大小的时候就直接修改PAGE_SHIFT
			return _RoundUp(size,1 << PAGE_SHIFT);
		}
	}

	//一次thread cache从中心缓存获取多少个size大小的对象，慢启动上限值
	static size_t NumMoveSize(size_t size) {
		assert(size > 0); //不能申请0大小的空间
		size_t num = MAX_BYTES / size;

		if (num > 512) {
			/*比如说单次申请的是8B，256KB除以8B得到的是一个三万多的数，
			那这样单次上限三万多块太多了，直接开到三万多可能会造成很多
			浪费的空间，不太现实，所以改小一点*/
			num = 512;
		}
		//如果说除了之后特别小，比2小，那么就调成2
		if (num < 2) {
			/*比如说单次申请的是256KB，那除得1，如果256KB上限一直是1，
			那这样有点太少了，可能线程要的是4个256KB，那将num改成2就
			可以少调用几次，也就会少几次开销，但是也不能太多，256KB空
			间是很大的，num太高了不太现实，可能会出现浪费*/
			num = 2;
		}
		//[2，512] ，一次批量移动多少个对象的(慢启动)上限值
		//小对象一次批量上限高
		//大对象一次批量上限低
		return num;
	}

	//计算一次向系统获取几个页
	/*根据对象大小 size，先估算一次该搬多少个对象，
	  再把这批对象占用的总字节数换算成页数；不足 1 页至少给 1 页*/
	static size_t NumMovePage(size_t size) {
		size_t num = NumMoveSize(size); //对于大小为 size 的对象，一次最多应该批量申请多少个对象。
		size_t npage = num * size;      //一次批量申请的总字节数

		npage >>= PAGE_SHIFT;   //把“总字节数”换算成“页数”
		if (npage == 0) {
			npage = 1;
		}
		return npage;
	}
};

// Span管理⼀个跨度的⼤块内存
// 管理以⻚为单位的⼤块内存
// 管理多个连续⻚⼤块内存跨度结构
struct Span {
public:
	PageID _pageID = 0; //页ID
	size_t _n = 0; //当前span管理的页数
	size_t _objSize = 0; //当前span管理的内存块大小

	void* _freeList = nullptr; //当前span管理的内存块链表头
	size_t _useCount = 0; //当前span管理的内存块被分配出去的数量

	Span* _next = nullptr; //当前span所在链表的下一个span
	Span* _prev = nullptr; //当前span所在链表的上一个span

	bool _isUse = false; //判断当前span是在cc还是pc中
};

//SpanList实现——CentralCache中的哈希桶
//带头双向循环链表
class SpanList {
public:
	SpanList() {
		//构造函数，弄一个哨兵节点，链表头指向哨兵节点
		_head = new Span();
		//哨兵节点的前驱和后继都指向自己，形成一个循环链表
		_head->_next = _head;
		_head->_prev = _head;
	}
	bool Empty() const {
		return _head->_next == _head;
	}
	Span* Begin() {
		return _head->_next;
	}
	Span* End() {
		return _head;
	}
	void PushFront(Span* ptr) {
		Insert(_head->_next, ptr);
	}
	//删除掉第一个Span
	Span* PopFront() {
		assert(!Empty()); //链表不能为空
		Span* first = _head->_next; //第一个span
		Erase(first); //删除第一个span
		return first; //返回被删除的span
	}

	void Insert(Span* pos, Span* ptr) {
		//在pos节点前插入ptr节点
		assert(pos);
		assert(ptr);

		Span* prev = pos->_prev; //pos节点的前驱节点
		prev->_next = ptr; //prev节点的后继指向ptr节点
		ptr->_prev = prev; //ptr节点的前驱指向prev节点
		ptr->_next = pos; //ptr节点的后继指向pos节点
		pos->_prev = ptr; //pos节点的前驱指向ptr节点
	}

	void Erase(Span* pos) {
		assert(pos); //pos节点不能为空
		assert(pos != _head); //pos节点不能是哨兵节点

		Span* prev = pos->_prev; //pos节点的前驱节点
		Span* next = pos->_next; //pos节点的后继节点

		prev->_next = next; //prev节点的后继指向next节点
		next->_prev = prev; //next节点的前驱指向prev节点

		//pos节点不需要delete删除，后续还要回收
		//回收相关逻辑...
	}
private:
	Span* _head = nullptr; //链表头指向哨兵节点
public:
	std::mutex _mtx; // 桶锁
};
