#pragma once
#include<iostream>
#include<assert.h>
namespace lyy {
	template<class T>
	class vector {
		// vector的迭代器是一个原生指针
		typedef T* iterator;
		typedef const T* const_iterator;
	public:
		size_t size()const {
			return _finish - _start;  //地址减去地址
		}
		size_t capacity()const {
			return _end_of_storage - _start;
		}
		iterator begin(){
			return _start;
		}
		iterator end(){
			return _finish;
		}
		const_iterator begin()const {
			return _start;
		}
		const_iterator end()const {
			return _finish;
		}

		//construct and destroy
		vector() = default;//c++11 前置生成默认构造

		vector(const vector<T>&v) {
			reserve(v.size());
	       	for (auto& e : v) {
				push_back(e);
			}
		}

		//类模板的成员函数，还可以继续是函数模版
		template <class InputIterator>
		vector(InputIterator first, InputIterator last)
		{
			while (first != last)
			{
				push_back(*first);
				++first;
			}
		}
		/*vector(size_t n, const T& val = T()) 
		 第一个参数类型size_t 当vector v(1,1) 创建对象时会走迭代器版本
		 （为避免 用下一种）
		{
			reserve(n);
			for (size_t i = 0; i < n; i++)
			{
				push_back(val);
			}
		}*/
		vector(int n, const T& value = T()) {
			reserve(n); //开辟新空间
			while (n--) {
				push_back(value);
			}
		}
		vector<T>& operator=(vector<T> v) { //赋值重载现代写法 让编译器拷贝构造
			swap(v);
			return *this;
		}
		~vector() {
			if (_start) {
				delete[] _start;
				_start = _finish = _end_of_storage = nullptr;
			}
		}
		void reserve(size_t n) {
			if (n > capacity()) {
				//扩容
				size_t old_size = size();
				T* tmp = new T[n];  //开辟新空间
				//将原来的旧数据拷贝给新空间
				//memcpy(tmp, _start, size() * sizeof(T)); 
				//memcpy把_start的地址浅拷贝给tmp tmp指向_start原来指向的空间 

				//正确方法
				for (size_t i = 0; i < old_size; i++) {
					tmp[i] = _start[i];
				}
				delete[] _start;	//销毁旧空间

				//指向新空间
				_start = tmp;
				_finish = tmp + old_size;
				_end_of_storage = tmp + n;
			}
		}
		bool empty() {
			if (size() <= 0) {
				return true;
			}
			else {
				return false;
			}
		}

		//modify
		void push_back(T x) {
			if (_finish == _end_of_storage) {
				//扩容
				reserve(capacity() == 0 ? 1 : 2 * capacity());
			}
			*_finish = x;
			++_finish;
		}
		void pop_back() {
			assert(!empty());
			--_finish;
		}
		void swap(vector<T>& v) {
			std::swap(_start, v._start);
			std::swap(_finish, v._finish);
			std::swap(_end_of_storage, v._end_of_storage);
		}

		//access
		T& operator[](size_t pos){
			assert(pos >= 0&&pos<size());
			return _start[pos];
		}
		const T& operator[](size_t pos)const {
			assert(pos >= 0 && pos < size());
			return _start[pos];
		}

		void insert(iterator pos,const T& x) {
			assert(pos >= _start && pos < _finish);
			if (_finish == _end_of_storage) {
				//扩容
				size_t len = pos - _start;
				reserve(capacity() == 0 ? 1 : 2 * capacity()); //扩容后迭代器失效 _start指向一个新空间
				pos = _start + len;  //更新pos迭代器
			}
			iterator end = _finish-1;
			while (end >= pos) {
				*(end + 1) = *end;
				--end;
			}
			_finish++;
		    *pos= x;
		}
		void erase(iterator pos) {
			assert(pos >= _start && pos < _finish);
			iterator it = pos+1;
			while (it != end()) {
				*(it - 1) = *it;
				it++;
			}
			--_finish;
		}
	private:
		iterator _start = nullptr;
		iterator _finish = nullptr;
		iterator _end_of_storage = nullptr;
	};

	template<class Container>
	void print_container(const Container& v)
	{
		/*auto it = v.begin();
		while (it != v.end())
		{
			cout << *it << " ";
			++it;
		}
		cout << endl;*/

		for (auto e : v)
		{
			std::cout << e << " ";
		}
		std::cout << std::endl;
	}
}