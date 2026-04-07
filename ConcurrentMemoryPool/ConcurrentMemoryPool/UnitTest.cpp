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
int main()
{
    AllocTest();
    return 0;
}