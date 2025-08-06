#pragma once
#include<iostream>
#include<assert.h>
namespace lyy {
	template<class T>
	class vector {
		
	public:
		typedef T* iterator;
		typedef const T* const_iterator;

		size_t size()const {
			return _finish - _start;  //地址减去地址
		}
		size_t capacity()const {
			return _end_of_storage - _start;
		}
		iterator begin() {
			return _start;
		}
		iterator end() {
			return _finish;
		}
		const_iterator begin()const {
			return _start;
		}
		const_iterator end()const {
			return _finish;
		}
		void reserve(size_t n) {
			if (n > capacity()) {
				//扩容
				size_t old_size = size();
				T* tmp = new T[n];  //开辟新空间
				memcpy(tmp, _start, size() * sizeof(T));  //将原来的旧数据拷贝给新空间
				delete[] _start;	//销毁旧空间

				//指向新空间
				_start = tmp;
				_finish = tmp + old_size;
				_end_of_storage = tmp + n;
			}
		}
		bool empty() const{
			if (size() <= 0) {
				return true;
			}
			else {
				return false;
			}
		}
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
		T& operator[](size_t pos) {
			assert(pos >= 0 && pos < size());
			return _start[pos];
		}
		const T& operator[](size_t pos)const {
			assert(pos >= 0 && pos < size());
			return _start[pos];
		}
		iterator insert(iterator pos, const T& x) {
			if (_finish == _end_of_storage) {
				//扩容
				size_t len = pos - _start; //记录初始距离 
				reserve(capacity() == 0 ? 1 : 2 * capacity());
				//！！！此时迭代器_start已经指向新空间 
				// 需更新pos指向的位置
				pos = _start + len;   
			}
			iterator end = _finish - 1;
			while (end >= pos) {
				*(end + 1) = *end;
				--end;
			}
			_finish++;
			*pos = x;
			return pos;
		}
	private:
		iterator _start = nullptr;
		iterator _finish = nullptr;
		iterator _end_of_storage = nullptr;
	};

	template<class T>
	void print_vector(const vector<T>& v) {
		//typename vector<T>::const_iterator it = v.begin();
		//不能直接在未实例化的类模板取东西 编译器无法识别是静态成员还是类型 要加上typename表明是类型
		auto it = v.begin();
		while (it != v.end()) {
			std::cout << *it << " ";
			it++;
		}
		std::cout << std::endl;

		/*for (auto a : v) {
			std::cout << a <<" ";
		}
		std::cout << std::endl;*/
	}
}