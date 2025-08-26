#define _CRT_SECURE_NO_WARNINGS 1

#include<vector>
#include<iostream>
#include<map>
#include<string>
#include<assert.h>
using namespace std;

struct Point
{
	int _x;
	int _y;
};

//class Date
//{
//public:
//	Date(int year = 1, int month = 1, int day = 1)
//		:_year(year)
//		, _month(month)
//		, _day(day)
//	{
//		cout << "Date(int year, int month, int day)" << endl;
//	}
//
//	Date(const Date& d)
//		:_year(d._year)
//		, _month(d._month)
//		, _day(d._day)
//	{
//		cout << "Date(const Date& d)" << endl;
//	}
//private:
//	int _year;
//	int _month;
//	int _day;
//};
//
//int main()
//{
//	// C++98支持的
//	int a1[] = { 1, 2, 3, 4, 5 };
//	int a2[5] = { 0 };
//	Point p = { 1, 2 };
//
//	// C++11支持的
//	// 内置类型支持
//	int x1 = { 2 };
//	int x2 = 2;
//
//	// 自定义类型支持
//	// 这里本质是用{ 2025, 1, 1}构造一个Date临时对象
//	// 临时对象再去拷贝构造d1，编译器优化后合二为一变成{ 2025, 1, 1}直接构造初始化d1
//	// 运行一下，我们可以验证上面的理论，发现是没调用拷贝构造的
//	Date d1 = { 2025, 1, 1 };
//	Date d20(2025, 1, 1);
//
//	// 这里d2引用的是{ 2024, 7, 25 }构造的临时对象
//	const Date& d2 = { 2024, 7, 25 };
//
//	// 需要注意的是C++98支持单参数时类型转换，也可以不用{}
//	Date d3 = { 2025 }; // C++11
//	Date d4 = 2025;     // C++98
//	string str = "1111";
//
//	// 可以省略掉=
//	Point p1{ 1, 2 };
//	int x3{ 2 };
//	Date d6{ 2024, 7, 25 };
//	const Date& d7{ 2024, 7, 25 };
//
//	// 不支持，只有{}初始化，才能省略=
//	// Date d8 2025;
//
//	vector<Date> v;
//	v.push_back(d1);
//	v.push_back(Date(2025, 1, 1));
//	// 比起有名对象和匿名对象传参，这里{}更有性价比
//	v.push_back({ 2025, 1, 1 });
//	map<string, string> dict;
//	dict.insert({ "xxx", "yyyy" });
//
//	// 构造+拷贝构造+优化
//	//vector<int> v1 = { 1,2,3,4 };
//	//vector<int> v2 = { 10,20,30,1,1,1,1,1,1,1,1,1};
//	//const vector<int>& v4 = { 10,20,30,1,1,1,1,1,1,1,1,1 };
//
//	vector<int> v1{ 1,2,3,4 };
//	vector<int> v2{ 10,20,30,1,1,1,1,1,1,1,1,1 };
//	const vector<int>& v4 { 10,20,30,1,1,1,1,1,1,1,1,1 };
//
//	// 构造
//	vector<int> v3({ 10,20,30,1,1,1,1,1,1,1,1,1 });
//
//	initializer_list<int> il1 = { 10, 20, 30, 1, 1, 1, 1, 1, 1, 1, 1, 1};
//	int aa1[] = { 10, 20, 30, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
//
//	// initializer_list + {}pair初始化隐式类型转换
//	map<string, string> dict = { { "xxx", "yyyy" }, { "sort", "zzzz" } };
//
//	return 0;
//}

//template<class T>
//void func(const T& x)
//{}
//
//int main()
//{
//	// 左值：可以取地址
//	// 以下的p、b、c、*p、s、s[0]就是常见的左值
//	int* p = new int(0);
//	int b = 1;
//	const int c = b;
//	*p = 10;
//	string s("111111");
//	s[0] = 'x';
//
//	cout << &c << endl;
//	cout << (void*)&s[0] << endl;
//
//	// 左值引用给左值取别名
//	int& r1 = b;
//	int*& r2 = p;
//	int& r3 = *p;
//	string& r4 = s;
//	char& r5 = s[0];
//
//	// 右值：不能取地址
//	double x = 1.1, y = 2.2;
//	// 以下几个10、x + y、fmin(x, y)、string("11111")都是常见的右值
//	10;
//	x + y;
//	fmin(x, y);
//	string("11111");
//
//	//cout << &10 << endl;
//	//cout << &(x+y) << endl;
//	//cout << &(fmin(x, y)) << endl;
//	//cout << &string("11111") << endl;
//
//	// 右值引用给右值取别名
//	int&& rr1 = 10;
//	double&& rr2 = x + y;
//	double&& rr3 = fmin(x, y);
//	string&& rr4 = string("11111");
//
//	// 左值引用不能直接引用右值，但是const左值引用可以引用右值
//	const int& rx1 = 10;
//	const double& rx2 = x + y;
//	const double& rx3 = fmin(x, y);
//	const string& rx4 = string("11111");
//
//	// 右值引用不能直接引用左值，但是右值引用可以引用move(左值)
//	int&& rrx1 = move(b);
//	int*&& rrx2 = move(p);
//	int&& rrx3 = move(*p);
//	string&& rrx4 = move(s);
//	string&& rrx5 = (string&&)s;
//
//	// b、r1、rr1都是变量表达式，都是左值
//	cout << &b << endl;
//	cout << &r1 << endl;
//	cout << &rr1 << endl;
//
//	int i = 1;
//	int* ptr = (int*)i;
//
//	// int&& rr1 = 10;
//	// 这里要注意的是，rr1的属性是左值，所以不能再被右值引用绑定，除非move一下
//	int& r6 = rr1;
//
//	//int&& rrx6 = rr1;
//	int&& rrx6 = move(rr1);
//
//	return 0;
//}

//template<class T>
//void func(const T& x)
//{}

//void f(int& x)
//{
//	std::cout << "左值引用重载 f(" << x << ")\n";
//}
//void f(const int& x)
//{
//	std::cout << "到 const 的左值引用重载 f(" << x << ")\n";
//}
//
//void f(int&& x)
//{
//	std::cout << "右值引用重载 f(" << x << ")\n";
//}
//
//int main()
//{
//	/*int&& rr1 = 10;
//	int a = 20;
//	int& r2 = a;*/
//
//	int i = 1;
//	const int ci = 2;
//
//	f(i); // 调用 f(int&)
//	f(ci); // 调用 f(const int&)
//	f(3); // 调用 f(int&&)，如果没有 f(int&&) 重载则会调用 f(const int&)
//	f(std::move(i)); // 调用 f(int&&)
//
//	// 右值引用变量在用于表达式时是左值
//	// 右值引用本身的属性是左值
//	int&& x = 1;
//	f(x); // 调用 f(int& x)
//	f(std::move(x)); // 调用 f(int&& x)
//
//	return 0;
//}

namespace bit
{
	class string
	{
	public:
		typedef char* iterator;
		typedef const char* const_iterator;

		iterator begin()
		{
			return _str;
		}

		iterator end()
		{
			return _str + _size;
		}

		const_iterator begin() const
		{
			return _str;
		}

		const_iterator end() const
		{
			return _str + _size;
		}

		string(const char* str = "")
			:_size(strlen(str))
			, _capacity(_size)
		{
			cout << "string(char* str)-构造" << endl;
			_str = new char[_capacity + 1];
			strcpy(_str, str);
		}

		// 拷贝构造
		string(const string& s)
			:_str(nullptr)
		{
			cout << "string(const string& s) -- 拷贝构造" << endl;
			reserve(s._capacity);
			for (auto ch : s)
			{
				push_back(ch);
			}
		}

		void swap(string& ss)
		{
			::swap(_str, ss._str);
			::swap(_size, ss._size);
			::swap(_capacity, ss._capacity);
		}

		// 移动构造
		string(string&& s)
		{
			cout << "string(string&& s) -- 移动构造" << endl;
			// 转移掠夺你的资源
			swap(s);
		}

		string& operator=(const string& s)
		{
			cout << "string& operator=(const string& s) -- 拷贝赋值" <<
				endl;
			if (this != &s)
			{
				_str[0] = '\0';
				_size = 0;
				reserve(s._capacity);
				for (auto ch : s)
				{
					push_back(ch);
				}
			}
			return *this;
		}

		// 移动赋值
		string& operator=(string&& s)
		{
			cout << "string& operator=(string&& s) -- 移动赋值" << endl;
			swap(s);
			return *this;
		}

		~string()
		{
			//cout << "~string() -- 析构" << endl;
			delete[] _str;
			_str = nullptr;
		}
	
		char& operator[](size_t pos)
		{
			assert(pos < _size);
			return _str[pos];
		}

		void reserve(size_t n)
		{
			if (n > _capacity)
			{
				char* tmp = new char[n + 1];
				if (_str)
				{
					strcpy(tmp, _str);
					delete[] _str;
				}
				_str = tmp;
				_capacity = n;
			}
		}

		void push_back(char ch)
		{
			if (_size >= _capacity)
			{
				size_t newcapacity = _capacity == 0 ? 4 : _capacity *
					2;
				reserve(newcapacity);
			}
			_str[_size] = ch;
			++_size;
			_str[_size] = '\0';
		}

		string& operator+=(char ch)
		{
			push_back(ch);
			return *this;
		}

		const char* c_str() const
		{
			return _str;
		}

		size_t size() const
		{
			return _size;
		}
	private:
		char* _str = nullptr;
		size_t _size = 0;
		size_t _capacity = 0;
	};
}

class Solution {
public:
	// 传值返回需要拷贝
	bit::string addStrings(bit::string num1, bit::string num2) {
		bit::string str;
		int end1 = num1.size() - 1, end2 = num2.size() - 1;
		// 进位
		int next = 0;
		while (end1 >= 0 || end2 >= 0)
		{
			int val1 = end1 >= 0 ? num1[end1--] - '0' : 0;
			int val2 = end2 >= 0 ? num2[end2--] - '0' : 0;
			int ret = val1 + val2 + next;
			next = ret / 10;
			ret = ret % 10;
			str += ('0' + ret);
		}
		if (next == 1)
			str += '1';
		reverse(str.begin(), str.end());
		//cout << &str << endl;

		return str;
	}
};

//int main()
//{
//	bit::string ret;
//	// ...
//
//	ret = Solution().addStrings("11111111111111111111", "222222222222222222222222222");
//	cout << ret.c_str() << endl;
//	//cout << &ret<< endl;
//
//	return 0;
//}

//int main()
//{
//	//string ret = Solution().addStrings("11111111111111111111", "222222222222222222222222222");
//	//cout << ret << endl;
//
//	bit::string s1("11111111111111111");
//
//	bit::string s3 = s1;
//	bit::string s4 = bit::string("222222222");
//	bit::string s5 = move(s1);
//
//	return 0;
//}

//int main()
//{
//	const bit::string& lr = bit::string("111111");
//	bit::string&& rr = bit::string("111111");
//	cout << "xxxxxxxxxxxxxxxxxxxxx" << endl;
//
//	return 0;
//}

//class Solution {
//public:
//	// 这里的传值返回拷贝代价就太大了
//	/*vector<vector<int>> generate(int numRows) {
//		vector<vector<int>> vv(numRows);
//		for (int i = 0; i < numRows; ++i)
//		{
//			vv[i].resize(i + 1, 1);
//		}
//		for (int i = 2; i < numRows; ++i)
//		{
//			for (int j = 1; j < i; ++j)
//			{
//				vv[i][j] = vv[i - 1][j] + vv[i - 1][j - 1];
//			}
//		}
//		return vv;
//	}*/
//
//	void generate(int numRows, vector<vector<int>>& vv) {
//		vector<vector<int>> vv(numRows);
//		for (int i = 0; i < numRows; ++i)
//		{
//			vv[i].resize(i + 1, 1);
//		}
//		for (int i = 2; i < numRows; ++i)
//		{
//			for (int j = 1; j < i; ++j)
//			{
//				vv[i][j] = vv[i - 1][j] + vv[i - 1][j - 1];
//			}
//		}
//	}
//};

//int main()
//{
//	vector<vector<int>> ret = Solution().generate(100);
//	vector<vector<int>> ret;
//	Solution().generate(100, ret);
//
//	/*bit::string ret;
//	Solution().addStrings("1111111", "222222", ret);*/
//
//	return 0;
//}
//
//int main()
//{
//
//	return 0;
//}

//int main()
//{
//	bit::string ret = Solution().addStrings("11111111111111111111", "222222222222222222222222222");
//	cout << ret.c_str() << endl;
//
//	vector<vector<int>> ret = Solution().generate(100);
//
//	return 0;
//}

// 由于引用折叠限定，f1实例化以后总是一个左值引用
//template<class T>
//void f1(T& x)
//{}
//
//// 由于引用折叠限定，f2实例化后可以是左值引用，也可以是右值引用
//template<class T>
//void f2(T&& x)
//{}
//
//int main()
//{
//	typedef int& lref;
//	typedef int&& rref;
//	int n = 0;
//
//	// 引用折叠
//	lref& r1 = n; // r1 的类型是 int&
//	lref&& r2 = n; // r2 的类型是 int&
//	rref& r3 = n; // r3 的类型是 int&
//	rref&& r4 = 1; // r4 的类型是 int&&
//
//	// 没有折叠->实例化为void f1(int& x)
//	f1<int>(n);
//	//f1<int>(0); // 报错
//
//	// 折叠->实例化为void f1(int& x)
//	f1<int&>(n);
//	//f1<int&>(0); // 报错
//
//	// 折叠->实例化为void f1(int& x)
//	f1<int&&>(n);
//	//f1<int&&>(0); // 报错
//
//	// 折叠->实例化为void f1(const int& x)
//	f1<const int&>(n);
//	f1<const int&>(0);
//
//	// 折叠->实例化为void f1(const int& x)
//	f1<const int&&>(n);
//	f1<const int&&>(0);
//
//	// 没有折叠->实例化为void f2(int&& x)
//	//f2<int>(n); // 报错
//	f2<int>(0);
//
//	// 折叠->实例化为void f2(int& x)
//	f2<int&>(n);
//	//f2<int&>(0); // 报错
//
//	// 折叠->实例化为void f2(int&& x)
//	//f2<int&&>(n); // 报错
//	f2<int&&>(0);
//
//	return 0;
//}

// 万能引用 
//template<class T>
//void Function(T&& t)
//{
//	int a = 0;
//	T x = a;
//	//x++;
//
//	cout << &a << endl;
//	cout << &x << endl << endl;
//}
//
//int main()
//{
//	// 10是右值，推导出T为int，模板实例化为void Function(int&& t)
//	//Function(10);
//
//	int a;
//	// a是左值，推导出T为int&，引用折叠，模板实例化为void Function(int& t)
//	//Function(a); // 左值
//
//	// std::move(a)是右值，推导出T为int，模板实例化为void Function(int&& t)
//	//Function(std::move(a));
//
//	const int b = 8;
//	// b是左值，推导出T为const int&，引用折叠，模板实例化为void Function(const int& t)
//	// 所以Function内部会编译报错，x不能++
//	//Function(b);    // const 左值
//
//	// std::move(b)右值，推导出T为const int，模板实例化为void Function(const int&& t)
//	// 所以Function内部会编译报错，x不能++
//	Function(std::move(b)); // const 右值
//
//	return 0;
//}

//void Fun(int& x) { cout << "左值引用" << endl; }
//void Fun(const int& x) { cout << "const 左值引用" << endl; }
//void Fun(int&& x) { cout << "右值引用" << endl; }
//void Fun(const int&& x) { cout << "const 右值引用" << endl; }
//
//template < class T>
//void Function(T&& t)
//{
//	Fun(forward<T>(t));
//}
//
//int main()
//{
//	// 10是右值，推导出T为int，模板实例化为void Function(int&& t)
//	Function(10); // 右值
//
//	int a;
//	// a是左值，推导出T为int&，引用折叠，模板实例化为void Function(int& t)
//	Function(a); // 左值
//
//	// std::move(a)是右值，推导出T为int，模板实例化为void Function(int&& t)
//	Function(std::move(a)); // 右值
//
//	const int b = 8;
//	// a是左值，推导出T为const int&，引用折叠，模板实例化为void Function(const int&t)
//	Function(b);
//
//	// std::move(b)右值，推导出T为const int，模板实例化为void Function(const int&&t)
//	Function(std::move(b)); // const 右值
//
//	return 0;
//}

#include<list>
#include"List.h"

int main()
{
	bit::list<bit::string> lt;
	bit::string s1("11111111111");
	lt.push_back(s1);

	bit::string s2("33333333333");
	lt.push_back(move(s2));

	lt.push_back("22222222222");

	return 0;
}