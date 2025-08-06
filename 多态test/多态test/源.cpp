#include<iostream>
using namespace std;
//class Base {
//public:
//	virtual void func1() { cout << "Base::func1" << endl; }
//	virtual void func2() { cout << "Base::func2" << endl; }
//	void func5() { cout << "Base::func5" << endl; }
//protected:
//	int a = 1;
//};
//class Derive : public Base
//{
//public:
//	// 重写基类的func1
//	virtual void func1() { cout << "Derive::func1" << endl; }
//	virtual void func3() { cout << "Derive::func1" << endl; }
//	void func4() { cout << "Derive::func4" << endl; }
//protected:
//	int b = 2;
//};
class Base {
public:
	virtual void func1() { cout << "Base::func1" << endl; }
	virtual void func2() { cout << "Base::func2" << endl; }
	void func5() { cout << "Base::func5" << endl; }
protected:
	int a = 1;
};
class Derive : public Base
{
public:
	// 重写基类的func1
	virtual void func1() { cout << "Derive::func1" << endl; }
	virtual void func3() { cout << "Derive::func1" << endl; }
	void func4() { cout << "Derive::func4" << endl; }
protected:
	int b = 2;
};
//class A
//
//{
//
//public:
//
//	virtual void f()
//
//	{
//
//		cout << "A::f()" << endl;
//
//	}
//
//};
//
//
//
//class B : public A
//
//{
//
//private:
//
//	virtual void f()
//
//	{
//
//		cout << "B::f()" << endl;
//
//	}
//
//};

//class A { 
//public: 
//	void test(float a) { cout << a; }
//};
//class B :public A { 
//public: 
//	void test(int b) 
//	{ cout << b; } 
//	int _b;
//}; 
//void main() {
//	A* a = new A; 
//	B* b = new B; 
//	a = b; 
//	a->test(1.1); //调用的还是A类的test()
//	cout<<a->_b; //不构成多态 访问不到B类的成员变量
//}
class Person
{
public:
	string _name; // 姓名
	string _sex; // 性别
	int _age; // 年龄
};
class Student : public Person
{
public:
	int _No; // 学号
};
//int main()
//{
//	Student sobj;
//	// 1.派⽣类对象可以赋值给基类的指针/引⽤
//	Person* pp = &sobj;
//	Person& rp = sobj;
//	cout << pp->_age;
//	// 派⽣类对象可以赋值给基类的对象是通过调⽤后⾯会讲解的基类的拷⻉构造完成的
//	Person pobj = sobj;
//	//2.基类对象不能赋值给派⽣类对象，这⾥会编译报错
//	//sobj = pobj;
//	return 0;
//}
class B1 {
public:
	virtual void fun1() {
		cout << "fun1()" << endl;
	}

};
class B2 {
public:
	virtual void fun3() {
		cout << "fun3()" << endl;
	}
};
class D:public B1,public B2{
	virtual void fun1() {
		cout << "D fun1()" << endl;
	}
	virtual void fun3() {
		cout << "D fun3()" << endl;
	}
	virtual void fun5() {
		cout << "fun5()" << endl;
	}
};
int main() {
	B1 b1;
	B2 b2;
	D d;
}

#if 0
int main()
{
	/*int i = 0;
	static int j = 1;
	int* p1 = new int;
	const char* p2 = "xxxxxxxx";
	printf("栈:%p\n", &i);
	printf("静态区:%p\n", &j);
	printf("堆:%p\n", p1);
	printf("常量区:%p\n", p2);
	Base b;
	Derive d;
	Base* p3 = &b;
	Derive* p4 = &d;
	printf("Person虚表地址:%p\n", *(int*)p3);
	printf("Student虚表地址:%p\n", *(int*)p4);
	printf("虚函数地址:%p\n", &Base::func1);
	printf("普通函数地址:%p\n", &Base::func5);*/

	/*A* pa = (A*)new B;

	pa->f();*/

	return 0;
}
#endif