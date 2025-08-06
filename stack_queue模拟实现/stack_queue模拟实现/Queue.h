#pragma once
#include<deque>
namespace lyy {
	template<class T, class Container = deque<T>>
	class Queue {
	public:
		void push(const T& x) {
			_con.push_back(x);
		}
		void pop() {
			_con.pop_front();
		}
		T& front(){
			return _con.front(); //队列首元素
		}
		const T& back()const {
			return _con.back(); //队列尾元素
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