#pragma once
#include<iostream>
#include<assert.h>
namespace lyy {
	template<class T>
	struct list_node { //结构体默认成员均为public
		list_node(const T& data = T())
			: _data(data)
			, _prev(nullptr)
			, _next(nullptr)
		{}

		T _data;
		list_node<T>* _prev;
		list_node<T>* _next;
	};
	template<class T,class Ref,class Ptr>
	struct list_iterator {   //结构体默认成员均为public
		typedef list_node<T> Node;
		typedef list_iterator<T,Ref,Ptr> Self;
		Node* _node;

		list_iterator(Node* node) 
			:_node(node)
		{}
		Ref operator*() {
			return _node->_data;
		}
		Ptr operator->() {
			return &_node->_data;
		}
		Self& operator++() {
			_node = _node->_next;
			return *this;
		}
		Self operator++(int) { //后置++(int)来区分
			Self tmp(*this);
			_node = _node->_next;
			return tmp;
		}
		Self& operator--() {
			_node = _node->_prev;
			return *this;
		}
		Self operator--(int) {
			Self tmp(*this);
			_node = _node->_prev;
			return tmp;
		}
		bool operator!=(const Self& s)const {
			return _node != s._node;
		}
		bool operator==(const Self& s)const {
			return _node == s._node;
		}
	};

	//template<class T>
	//struct list_const_iterator {   //结构体默认成员均为public
	//	typedef list_node<T> Node;
	//	typedef list_const_iterator<T> Self;
	//	Node* _node;

	//	list_const_iterator(Node* node)
	//		:_node(node)
	//	{}
	//	const T& operator*() {
	//		return _node->_data;
	//	}
	//	const T* operator->() {
	//		return &(_node->_data);
	//	}
	//	Self& operator++() {
	//		_node = _node->_next;
	//		return *this;
	//	}
	//	Self operator++(int) {
	//		Self tmp(*this);
	//		_node = _node->_next;
	//		return tmp;
	//	}
	//	Self& operator--() {
	//		_node = _node->_prev;
	//		return *this;
	//	}
	//	Self operator--(int) {
	//		Self tmp(*this);
	//		_node = _node->_prev;
	//		return tmp;
	//	}
	//	bool operator!=(const Self& s)const {
	//		return _node != s._node;
	//	}
	//	bool operator==(const Self& s)const {
	//		return _node == s._node;
	//	}
	//};
	
	template<class T>
	class list {
		typedef list_node<T> Node;
	public:
		/*typedef list_iterator<T> iterator;
		typedef list_const_iterator<T> const_iterator;*/
		typedef list_iterator<T,T&,T*> iterator;
		typedef list_iterator<T,const T&,const T*> const_iterator;

		iterator begin() {
			return _head->_next;  //头指针是哨兵位
		}
		iterator end() {
			return _head;
		}
		const_iterator begin() const{
			return _head->_next;  //头指针是哨兵位
		}
		const_iterator end() const{
			return _head;
		}
		size_t size()const {
			return _size;
		}
		bool empty()const {
			return _size == 0;
		}
		list() {
			/*_head = new Node;
			_head->_prev = _head;
			_head->_next = _head;
			_size = 0;*/
			empty_init();

		}
		void empty_init() {
			_head = new Node;
			_head->_next = _head;
			_head->_prev = _head;
			_size = 0;
		}

		template<class Iterator>
		list(Iterator first, Iterator last) {
			empty_init();
			while (first != last) {
				push_back(*first);
				++first;
			}
		}

		list(std::initializer_list<T> il)
		{
			empty_init();
			for (auto& e : il)
			{
				push_back(e);
			}
		}

		//拷贝构造 lt2(lt1)
		list(const list<T>& lt) {
			empty_init();
			for (auto& e : lt)
			{
				push_back(e);
			}
		}

		//赋值重载 lt3=lt1
		list<T>& operator=(list<T> it) {
			swap(it);
			return *this;
		}
		void swap(list<T>& lt) {
			std::swap(_head, lt._head);
			std::swap(_size, lt._size);
		}
		~list() {
			clear();
			delete _head;
			_head = nullptr;
		}

		// List Access
		T& front() {
			assert(!empty());
			return _head->_next->_data;
		}
		const T& front()const {
			assert(!empty());
			return _head->_next->_data;
		}
		T& back() {
			assert(!empty());
			return _head->_prev->_data;
		}
		const T& back()const {
			assert(!empty());
			return _head->_prev->_data;
		}

		iterator erase(iterator pos) {
			assert(pos != end());
			Node* cur = pos._node;
			Node* prev = cur->_prev;
			Node* next = cur->_next;

			prev->_next = next;
			next->_prev = prev;
			delete pos._node;  //释放旧节点
			--_size;

			return next;
		}
		void pop_back() {
			erase(--end());
		}
		void pop_front() {
			erase(begin());
		}
		void push_back(const T& val) {
			/*Node* node = new Node(val);
			node->_next = _head;
			node->_prev = _head->_prev;
			_head->_prev->_next = node;
			_head->_prev = node;
			++_size;*/
			insert(end(), val);
		}
		void push_front(const T& val) {
			insert(begin(), val);
		}
		void insert(iterator pos, const T& val) {
			Node* cur = pos._node; //得到当前位置的结点
			Node* prev = cur->_prev; //当前位置的前一个结点

			Node* newnode = new Node(val);
			
			newnode->_prev = prev;
			newnode->_next = cur;
			prev->_next = newnode;
			cur->_prev = newnode;

			++_size;
		}
		void clear() {
			//除头节点外都删除
			auto it = begin();
			while (it!=end()) {
				it = erase(it); //erase之后 原来的迭代器失效
			}
			_head->_next = _head;
			_head->_prev = _head;
		}
	private:
		Node* _head;
		size_t _size;
	};

	template<class Container>
	void print_container(const Container& con) {
		// const iterator -> 迭代器本身不能修改
		// const_iterator -> 指向内容不能修改
		typename Container::const_iterator it = con.begin();
		while (it != con.end()) {
			std::cout << *it << " ";
			++it;
		}
		std::cout << std::endl;
	}

}