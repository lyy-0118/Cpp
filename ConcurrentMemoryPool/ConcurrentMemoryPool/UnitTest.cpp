#include"ConcurrentAlloc.h"
void Alloc1() {
    for (int i = 0; i < 5; i++) {
		ConcurrentAlloc(9);
    }
}

void Alloc2() {
    for (int i = 0; i < 5; i++) {
        ConcurrentAlloc(17);
    }
}

//验证一下定义的TLS变量不需要加锁就能帮每个线程申请独有的ThreadCache对象
void AllocTest() {
    std::thread t1(Alloc1);
    std::thread t2(Alloc2);
    t1.join();
    t2.join();
}
void ConcurrentAllocTest1()
{
    void* ptr1 = ConcurrentAlloc(5);
    void* ptr2 = ConcurrentAlloc(8);
    void* ptr3 = ConcurrentAlloc(4);
    void* ptr4 = ConcurrentAlloc(6);
    void* ptr5 = ConcurrentAlloc(3);

    cout << ptr1 << endl;
    cout << ptr2 << endl;
    cout << ptr3 << endl;
    cout << ptr4 << endl;
    cout << ptr5 << endl;
}

//这里for循环目的是申请完cc中0号桶中的那个span，
//然后再申请一块的时候tc中没有，向cc申请，cc中也没有，
// 向pc中申请，pc中有一个127页的span，分出来一页给cc，自己留下126页的span
void ConcurrentAllocTest2() {
    for (int i = 0; i < 1024; ++i) {
        void* ptr = ConcurrentAlloc(5);
        cout << ptr << endl;
    }
	void* ptr = ConcurrentAlloc(5);
    cout << "--------" << ptr << endl;
}
int main()
{
    ConcurrentAllocTest2();
    return 0;
}