#pragma once
#define _CRT_SECURE_NO_WARNINGS 1

//#include<string.h>
#include<iostream>
#include<assert.h>
using namespace std;
namespace lyy {
	class string {
		friend ostream& operator<<(ostream& out, const string& s);
		friend istream& operator>>(istream& in, string& s);
	public:
		static const size_t npos = -1;
		typedef char* iterator;
		typedef const char* const_iterator;
		string(const char* s = "") {
			_size = strlen(s);
			_capacity = _size;
			_str = new char[_capacity + 1]; //多开一个空间用于存放\0
			strcpy(_str, s);
		}
		string(const string& s) {
			_size = strlen(s._str);
			_capacity = _size;
			_str = new char[_capacity + 1];
			strcpy(_str, s._str);
		}
		string& operator=(const string& s) {
			if (this != &s) {
				char* pst = new char[strlen(s._str) + 1];
				strcpy(pst, s._str);
				delete[]_str;  //删除原来存储的字符串
				_str = pst;
				_size = strlen(_str);
			}
			return *this;
		}
		~string() {
			delete[] _str;
			_size = _capacity = 0;
			_str = nullptr;
		}
		const char* c_str()const {
			return _str;
		}

		// capacity
		size_t size()const {
			return _size;
		}
		size_t capacity()const {
			return _capacity;
		}
		bool empty()const{
			if (_size == 0) return true;
			return false;
		}
		void resize(size_t n, char c = '\0');//改变字符串长度 剩余空间用c字符填充
		void reserve(size_t n);  //扩容

		iterator begin() {
			return _str;
		}
		iterator end() {
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

		// modify
		void push_back(char c);  //尾插
		string& append(const string& s); //附加一个字符串
		void append(const char* s);      //附加一个字符串
		string& operator+=(char c);  
		string& operator+=(const char* s);
		void clear() {   //不改变容量 只清除字符
			_str[0] = '\0';
			_size = 0;
		}

		// access
        char& operator[](size_t pos) {
			assert(pos < _size);
			return _str[pos];
		}
		const char& operator[](size_t pos)const{
			assert(pos < _size);
			return _str[pos];
		}


		void erase(size_t pos, size_t len = npos);  //从pos位置 清除len长度的字符（从后向前覆盖）
		void insert(char c, size_t pos);     //在pos位置插入一个字符 整体后移一位
		void insert(const char* s, size_t pos); //在pos位置插入一个字符串 整体后移len位
		size_t find(char ch, size_t pos=0);     //从pos位置开始寻找字符
		size_t find(const char*s , size_t pos=0);
		string substr(size_t pos, size_t len);  //返回字串
	private:
		char* _str;
		size_t _size;
		size_t _capacity;
	};

	//relational operators
	bool operator<(const string& s1, const string& s2);
	bool operator<=(const string& s1, const string& s2);
	bool operator>(const string& s1, const string& s2);
	bool operator>=(const string& s1, const string& s2);
	bool operator==(const string& s1, const string& s2);
	bool operator!=(const string& s1, const string& s2);
}

