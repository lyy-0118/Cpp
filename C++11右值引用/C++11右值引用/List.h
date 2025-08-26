#pragma once
#include<assert.h>

//#include"Iterator.h"

namespace bit
{
	template<class T>
	struct list_node
	{
		T _data;
		list_node<T>* _next;
		list_node<T>* _prev;

		/*list_node(const T& data = T())
			:_data(data)
			,_next(nullptr)
			,_prev(nullptr)
		{}

		list_node(T&& data)
			:_data(forward<T>(data))
			, _next(nullptr)
			, _prev(nullptr)
		{}*/

		list_node() = default;

		template<class X>
		list_node(X&& data)
			:_data(forward<X>(data))
			, _next(nullptr)
			, _prev(nullptr)
		{}
	};

	template<class T, class Ref, class Ptr>
	struct list_iterator
	{
		typedef list_node<T> Node;
		typedef list_iterator<T, Ref, Ptr> Self;
		Node* _node;

		list_iterator(Node* node)
			:_node(node)
		{}

		Ref operator*()
		{
			return _node->_data;
		}

		Ptr operator->()
		{
			return &_node->_data;
		}

		Self& operator++()
		{
			_node = _node->_next;
			return *this;
		}

		Self& operator--()
		{
			_node = _node->_prev;
			return *this;
		}

		Self operator++(int)
		{
			Self tmp(*this);
			_node = _node->_next;

			return tmp;
		}

		Self& operator--(int)
		{
			Self tmp(*this);
			_node = _node->_prev;

			return tmp;
		}
		
		bool operator!=(const Self& s) const
		{
			return _node != s._node;
		}

		bool operator==(const Self& s) const
		{
			return _node == s._node;
		}
	};


	/*template<class T>
	struct list_const_iterator
	{
		typedef list_node<T> Node;
		typedef list_const_iterator<T> Self;
		Node* _node;

		list_const_iterator(Node* node)
			:_node(node)
		{}

		const T& operator*()
		{
			return _node->_data;
		}

		const T* operator->()
		{
			return &_node->_data;
		}

		Self& operator++()
		{
			_node = _node->_next;
			return *this;
		}

		Self& operator--()
		{
			_node = _node->_prev;
			return *this;
		}

		Self operator++(int)
		{
			Self tmp(*this);
			_node = _node->_next;

			return tmp;
		}

		Self& operator--(int)
		{
			Self tmp(*this);
			_node = _node->_prev;

			return tmp;
		}

		bool operator!=(const Self& s) const
		{
			return _node != s._node;
		}

		bool operator==(const Self& s) const
		{
			return _node == s._node;
		}
	};*/

	template<class T>
	class list
	{
		typedef list_node<T> Node;
	public:
		/*typedef list_iterator<T> iterator;
		typedef list_const_iterator<T> const_iterator;*/

		typedef list_iterator<T, T&, T*> iterator;
		typedef list_iterator<T, const T&, const T*> const_iterator;
		//typedef ReverseIterator<iterator, T&, T*> reverse_iterator;
		//typedef ReverseIterator<const_iterator, const T&, const T*> const_reverse_iterator;

		//reverse_iterator rbegin()
		//{
		//	return reverse_iterator(--end());
		//}

		//reverse_iterator rend()
		//{
		//	return reverse_iterator(end());
		//}
		
		// 对称
	/*	reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}

		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}

		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(end());
		}

		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(begin());
		}*/

		iterator begin()
		{
			return _head->_next;
		}

		iterator end()
		{
			return _head;
		}

		const_iterator begin() const
		{
			return _head->_next;
		}

		const_iterator end() const
		{
			return _head;
		}

		void empty_init()
		{
			_head = new Node;
			_head->_next = _head;
			_head->_prev = _head;
			_size = 0;
		}

		list()
		{
			empty_init();
		}

		list(initializer_list<T> il)
		{
			empty_init();
			for (auto& e : il)
			{
				push_back(e);
			}
		}

		// lt2(lt1)
		list(const list<T>& lt)
		{
			empty_init();

			for (auto& e : lt)
			{
				push_back(e);
			}
		}

		// lt1 = lt3
		list<T>& operator=(list<T> lt)
		{
			swap(lt);
			return *this;
		}

		~list()
		{
			clear();
			delete _head;
			_head = nullptr;
		}

		void clear()
		{
			auto it = begin();
			while (it != end())
			{
				it = erase(it);
			}
		}

		// 16:18继续
		void swap(list<T>& lt)
		{
			std::swap(_head, lt._head);
			std::swap(_size, lt._size);
		}

		/*void push_back(const T& x)
		{
			insert(end(), x);
		}

		void push_back(T&& x)
		{
			insert(end(), forward<T>(x));
		}*/

		// 万能引用
		template<class X>
		void push_back(X&& x)
		{
			insert(end(), forward<X>(x));
		}

		void push_front(const T& x)
		{
			insert(begin(), x);
		}

		//iterator insert(iterator pos, const T& x)
		//{
		//	Node* cur = pos._node;
		//	Node* prev = cur->_prev;
		//	Node* newnode = new Node(x);

		//	// prev newnode cur
		//	newnode->_next = cur;
		//	cur->_prev = newnode;
		//	newnode->_prev = prev;
		//	prev->_next = newnode;

		//	++_size;

		//	return newnode;
		//}

		//iterator insert(iterator pos, T&& x)
		//{
		//	Node* cur = pos._node;
		//	Node* prev = cur->_prev;
		//	Node* newnode = new Node(forward<T>(x));

		//	// prev newnode cur
		//	newnode->_next = cur;
		//	cur->_prev = newnode;
		//	newnode->_prev = prev;
		//	prev->_next = newnode;

		//	++_size;

		//	return newnode;
		//}

		template<class X>
		iterator insert(iterator pos, X&& x)
		{
			Node* cur = pos._node;
			Node* prev = cur->_prev;
			Node* newnode = new Node(forward<X>(x));

			// prev newnode cur
			newnode->_next = cur;
			cur->_prev = newnode;
			newnode->_prev = prev;
			prev->_next = newnode;

			++_size;

			return newnode;
		}

		void pop_back()
		{
			erase(--end());
		}

		void pop_front()
		{
			erase(begin());
		}

		iterator erase(iterator pos)
		{
			assert(pos != end());

			Node* prev = pos._node->_prev;
			Node* next = pos._node->_next;

			prev->_next = next;
			next->_prev = prev;
			delete pos._node;

			--_size;

			return next;
		}

		size_t size() const
		{
			return _size;
		}

		bool empty() const
		{
			return _size == 0;
		}
	private:
		Node* _head;
		size_t _size;
	};

	struct AA
	{
		int _a1 = 1;
		int _a2 = 1;
	};

	//// 按需实例化
	//// T* const ptr1
	//// const T* ptr2
	//template<class Container>
	//void print_container(const Container& con)
	//{
	//	// const iterator -> 迭代器本身不能修改
	//	// const_iterator -> 指向内容不能修改
	//	typename Container::const_iterator it = con.begin();
	//	//auto it = con.begin();
	//	while (it != con.end())
	//	{
	//		//*it += 10;

	//		cout << *it << " ";
	//		++it;
	//	}
	//	cout << endl;

	//	for (auto e : con)
	//	{
	//		cout << e << " ";
	//	}
	//	cout << endl;
	//}

	//void test_list1()
	//{
	//	list<int> lt;
	//	lt.push_back(1);
	//	lt.push_back(2);
	//	lt.push_back(3);
	//	lt.push_back(4);

	//	list<int>::iterator it = lt.begin();
	//	while (it != lt.end())
	//	{
	//		*it += 10;

	//		cout << *it << " ";
	//		++it;
	//	}
	//	cout << endl;

	//	for (auto e : lt)
	//	{
	//		cout << e << " ";
	//	}
	//	cout << endl;
	//	print_container(lt);

	//	list<AA> lta;
	//	lta.push_back(AA());
	//	lta.push_back(AA());
	//	lta.push_back(AA());
	//	lta.push_back(AA());
	//	list<AA>::iterator ita = lta.begin();
	//	while (ita != lta.end())
	//	{
	//		//cout << (*ita)._a1 << ":" << (*ita)._a2 << endl;
	//		// 特殊处理，本来应该是两个->才合理，为了可读性，省略了一个->
	//		cout << ita->_a1 << ":" << ita->_a2 << endl;
	//		cout << ita.operator->()->_a1 << ":" << ita.operator->()->_a2 << endl;

	//		++ita;
	//	}
	//	cout << endl;
	//}

	//void test_list2()
	//{
	//	list<int> lt;
	//	lt.push_back(1);
	//	lt.push_back(2);
	//	lt.push_back(3);
	//	lt.push_back(4);

	//	// insert以后迭代器不失效
	//	list<int>::iterator it = lt.begin();
	//	lt.insert(it, 10);
	//	*it += 100;

	//	print_container(lt);

	//	// erase以后迭代器失效
	//	// 删除所有的偶数
	//	it = lt.begin();
	//	while (it != lt.end())
	//	{
	//		if (*it % 2 == 0)
	//		{
	//			it = lt.erase(it);
	//		}
	//		else
	//		{
	//			++it;
	//		}
	//	}

	//	print_container(lt);
	//}

	//void test_list3()
	//{
	//	list<int> lt1;
	//	lt1.push_back(1);
	//	lt1.push_back(2);
	//	lt1.push_back(3);
	//	lt1.push_back(4);

	//	list<int> lt2(lt1);

	//	print_container(lt1);
	//	print_container(lt2);

	//	list<int> lt3;
	//	lt3.push_back(10);
	//	lt3.push_back(20);
	//	lt3.push_back(30);
	//	lt3.push_back(40);

	//	lt1 = lt3;
	//	print_container(lt1);
	//	print_container(lt3);
	//}

	//void func(const list<int>& lt)
	//{
	//	print_container(lt);
	//}

	//void test_list4()
	//{
	//	// 直接构造
	//	list<int> lt0({ 1,2,3,4,5,6 });
	//	// 隐式类型转换
	//	list<int> lt1 = { 1,2,3,4,5,6,7,8 };
	//	const list<int>& lt3 = { 1,2,3,4,5,6,7,8 };

	//	func(lt0);
	//	func({ 1,2,3,4,5,6 });

	//	print_container(lt1);
	//	
	//	//auto il = { 10, 20, 30 };
	///*	initializer_list<int> il = { 10, 20, 30 };
	//	cout << typeid(il).name() << endl;
	//	cout << sizeof(il) << endl;*/
	//}

	//void test_list5()
	//{
	//	bit::list<int> lt;
	//	lt.push_back(1);
	//	lt.push_back(2);
	//	lt.push_back(3);
	//	lt.push_back(4);

	//	bit::list<int>::reverse_iterator rit = lt.rbegin();
	//	while (rit != lt.rend())
	//	{
	//		*rit += 1;
	//		cout << *rit << " ";
	//		++rit;
	//	}
	//	cout << endl;

	//	const bit::list<int> lt1(lt);
	//	bit::list<int>::const_reverse_iterator rit1 = lt1.rbegin();
	//	while (rit1 != lt1.rend())
	//	{
	//		//*rit1 += 1;

	//		cout << *rit1 << " ";
	//		++rit1;
	//	}
	//	cout << endl;
	//}
}
