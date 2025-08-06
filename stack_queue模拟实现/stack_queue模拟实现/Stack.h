#pragma once
#include<deque>
namespace lyy {
	template<class T,class Container=deque<T>>
	class Stack {
	public:
		void push(const T& x) {
			_con.push_back(x);
		}
		void pop() {
			_con.pop_back();
		}
		const T& top()const {
			return _con.back(); //Õ»¶¥
		}
		size_t size()const {
			return _con.size();
		}
		bool empty()const {
			return _con.empty();
		}
	private:
		Container _con;
	};
}