#pragma once
#include<iostream>
using namespace std;

namespace key {
	//结点结构体
	template<class K>
	struct BstNode {
		K _key;
		BstNode<K>* _left;
		BstNode<K>* _right;

		BstNode(const K& k)
			:_key(k)
			,_left(nullptr)
			,_right(nullptr)
		{}
	};

	template<class K>
	class BSTree {
		//typedef BstNode<K> Node
		using Node = BstNode<K>;
	public:
		BSTree() = default; //强制生成构造
		bool Insert(const K& key) {
			//树为空 直接插入
			if (!_root) {
				_root = new Node(key); //生成新节点
				return true;
			}
			//树不为空
			//先找到插入的空位置
			Node* parent = nullptr;
			Node* cur = _root;
			while (cur) {
				if (cur->_key < key) {
					parent = cur;
					cur = cur->_right; //往大结点移动;
				}
				else if (cur->_key > key) {
					parent = cur;
					cur = cur->_left; //往小结点移动
				}
				else {
					return false; //此处不考虑相等的情况
				}
			}
			//找到位置后 开辟新空间
			cur = new Node(key);
			if (parent->_key < key)
			{ //父小 放在父节点的右边
				parent->_right = cur;
			}
			else
			{ //父大 放在父节点的左边
				parent->_left = cur;
			}
			return true;
		}
		bool Find(const K& key) { //不支持查找相等的
			/*if (!_root) {
				return false;
			}*/
			Node* cur = _root; //树为空不进入循环直接返回False
			while (cur) {
				if (cur->_key < key) {
					//此时值小 往大的右走
					cur = cur->_right;
				}
				else if (cur->_key > key) {
					//此时值大 往左
					cur = cur->_left;
				}
				else {
					return true;
				}
			}
			return false;
		}
		bool Erase(const K& key) {
			Node* cur = _root;
			Node* parent = nullptr;
			while (cur) {
				if (cur->_key < key) {
					//此时值小 往大的右走
					parent = cur;
					cur = cur->_right;
				}
				else if (cur->_key > key) {
					//此时值大 往左
					parent = cur;
					cur = cur->_left;
				}
				else {
					/*找到了 有四种情况
					1.左为空 直删 父对应孩子指针指向右孩⼦
					2.右为空 直删 父对应孩子指针指向左孩⼦
					3.左右都有 找孩子中符合1或2的替换
					4.左右都为空 直删 归纳为1或2*/
					//1
					if (cur->_left == nullptr) {
						if (cur == _root) { //当前为根节点 将根节点替换为其右子树
							_root = cur->_right;
							return true;
						}
						else 
						{
							if (parent->_left == cur) {
								parent->_left = cur->_right;
							}
							else{
								parent->_right = cur->_right;
							}
						}
						delete cur;
					}
					//2
					else if (cur->_right == nullptr) {
						if (cur == _root) { //当前为根节点 将根节点替换为其左子树
							_root = cur->_left;
							return true;
						}
						else 
						{
							if (parent->_left == cur) {
								parent->_left = cur->_left;
							}
							else {
								parent->_right = cur->_left;
							}
						}
						delete cur;
					}
					//3
					else 
					{
						//左右都不为空
						//选择右子树最左结点
						Node* replace = cur->_right;
						Node* rp = cur; //raplace的父节点
						while (replace->_left) { //替换节点: 左为空
							rp = replace;
							replace = replace->_left;
						}

						cur->_key = replace->_key;

						//将r的右树嫁接给父节点rp
						if (rp->_left ==replace) {
							rp->_left = replace->_right;
						}
						else {
							rp->_right=replace->_right;
						}
						delete replace;
					}
					return true;
				}
			}
			return false;
		}
		void InOrder() {
			//前序遍历 左 根 右
			_InOrder(_root);
			cout << endl;
		}
	private:
		//前序遍历 左 根 右
		void _InOrder(Node* root)
		{
			if (root == nullptr)
			{
				return;
			}

			_InOrder(root->_left);
			cout << root->_key << " ";
			_InOrder(root->_right);
		}
	private:
		Node* _root = nullptr;
	};
}

namespace key_value {
	//结点结构体
	template<class K,class V>
	struct BstNode {
		K _key;
		V _value;
		BstNode<K,V>* _left;
		BstNode<K,V>* _right;

		BstNode(const K& k,const V& v)
			:_key(k)
			,_value(v)
			, _left(nullptr)
			, _right(nullptr)
		{}
	};

	template<class K,class V>
	class BSTree {
		//typedef BstNode<K,V> Node
		using Node = BstNode<K,V>;
	public:
		BSTree() = default; //强制生成默认构造

		//拷贝构造 深拷贝
		BSTree(const BSTree& bst) {
			_root = Copy(bst._root);
		}
		BSTree& operator=(BSTree bst) {
			/*Node* newRoot = bst._root;
			bst._root = _root;
			_root = newRoot;*/
			swap(_root, bst._root);
			return *this;
		}
		//析构
		~BSTree() {
			Destory(_root);
			_root = nullptr;
		}
		bool Insert(const K& key,const V& value) {
			//树为空 直接插入
			if (!_root) {
				_root = new Node(key,value); //生成新节点
				return true;
			}
			//树不为空
			//先找到插入的空位置
			Node* parent = nullptr;
			Node* cur = _root;
			while (cur) {
				if (cur->_key < key) {
					parent = cur;
					cur = cur->_right; //往大结点移动;
				}
				else if (cur->_key > key) {
					parent = cur;
					cur = cur->_left; //往小结点移动
				}
				else {
					return false; //此处不考虑相等的情况
				}
			}
			//找到位置后 开辟新空间
			cur = new Node(key, value);
			if (parent->_key < key)
			{ //父小 放在父节点的右边
				parent->_right = cur;
			}
			else
			{ //父大 放在父节点的左边
				parent->_left = cur;
			}
			return true;
		}
		Node* Find(const K& key) {
			/*if (!_root) {
				return false;
			}*/
			Node* cur = _root; //树为空不进入循环直接返回False
			while (cur) {
				if (cur->_key < key) {
					//此时值小 往大的右走
					cur = cur->_right;
				}
				else if (cur->_key > key) {
					//此时值大 往左
					cur = cur->_left;
				}
				else {
					return cur;
				}
			}
			return nullptr;
		}
		bool Erase(const K& key) {
			Node* cur = _root;
			Node* parent = nullptr;
			while (cur) {
				if (cur->_key < key) {
					//此时值小 往大的右走
					parent = cur;
					cur = cur->_right;
				}
				else if (cur->_key > key) {
					//此时值大 往左
					parent = cur;
					cur = cur->_left;
				}
				else {
					/*找到了 有四种情况
					1.左为空 直删 父对应孩子指针指向右孩⼦
					2.右为空 直删 父对应孩子指针指向左孩⼦
					3.左右都有 找孩子中符合1或2的替换
					4.左右都为空 直删 归纳为1或2*/
					//1
					if (cur->_left == nullptr) {
						if (cur == _root) { //当前为根节点 将根节点替换为其右子树
							_root = cur->_right;
							return true;
						}
						else
						{
							if (parent->_left == cur) {
								parent->_left = cur->_right;
							}
							else {
								parent->_right = cur->_right;
							}
						}
						delete cur;
					}
					//2
					else if (cur->_right == nullptr) {
						if (cur == _root) { //当前为根节点 将根节点替换为其左子树
							_root = cur->_left;
							return true;
						}
						else
						{
							if (parent->_left == cur) {
								parent->_left = cur->_left;
							}
							else {
								parent->_right = cur->_left;
							}
						}
						delete cur;
					}
					//3
					else
					{
						//左右都不为空
						//选择右子树最左结点
						Node* replace = cur->_right;
						Node* rp = cur;
						while (replace->_left) {
							rp = replace;
							replace = replace->_left;
						}

						cur->_key = replace->_key;

						if (rp->_left == replace) {
							rp->_left = replace->_right;
						}
						else {
							rp->_right = replace->_right;
						}
						delete replace;
					}
					return true;
				}
			}
			return false;
		}
		void InOrder() {
			//前序遍历 左 根 右
			_InOrder(_root);
			cout << endl;
		}
	private:
		//前序遍历 左 根 右
		void _InOrder(Node* root)
		{
			if (root == nullptr)
			{
				return;
			}

			_InOrder(root->_left);
			cout << root->_key << ":"<<root->_value<<" ";
			_InOrder(root->_right);
		}
		Node* Copy(Node* root) {
			if (root == nullptr) return nullptr;

			Node* newRoot = new Node(root->_key, root->_value);
			newRoot->_left = Copy(root->_left);
			newRoot->_right = Copy(root->_right);
			return newRoot;
		}
		void Destory(Node* root) {
			if (root == nullptr) return;
			Destory(root->_left);
			Destory(root->_right);
			delete root;
		}
	private:
		Node* _root = nullptr;
	};
}