#pragma once
#include<iostream>
using namespace std;

enum Color {
	RED,
	BLACK
};
template<class T>
struct RBTreeNode {
	T _data;
	RBTreeNode<T>* _parent;
	RBTreeNode<T>* _left;
	RBTreeNode<T>* _right;
	Color _cor;

	RBTreeNode(const T& data)
		:_data(data),
		_parent(nullptr),
		_left(nullptr),
		_right(nullptr)
	{}

};

template<class T, class Ref, class Ptr> //数据类型 引用(T&) 指针(T*)
class RBTreeIterator {
public:
	typedef RBTreeNode<T> Node;
	typedef RBTreeIterator<T, Ref, Ptr> Self; //迭代器类型
	Node* _node; //当前迭代器存放结点
	Node* _root; //存放根节点

	RBTreeIterator(Node* node, Node* root)
		:_node(node)
		,_root(root)
	{} //初始化

	Self operator++() {
		//左 根 右
		if (_node->_right) {
			//右不为空
			//找右的最左
			Node* min = _node->_right;
			while (min->_left) {
				min = min->_left;
			}
			_node = min;
		}
		else {
			//右为空 向上找 直到找到的当前节点是父节点的左子树
			Node* cur = _node;
			Node* parent =cur->_parent;
			while (parent && cur == parent->_right) {
				cur = parent;
				parent = cur->_parent;
			}
			_node = parent;
		}
		return *this;
	}
	Self operator--() {
		//右 根 左
		if (_node == nullptr) {
			//特殊处理 --end() 走到中序最后一个结点
			Node* rightmost = _root;
			while (rightmost && rightmost->_right) {
				rightmost = rightmost->_right;
			}
			_node = rightmost;

		}else if(_node->_left){
			//左不为空
			//找左的最右
			Node* min = _node->_left;
			while (min->_right) {
				min = min->_right;
			}
			_node = min;
		}
		else {
			//左为空 前驱是它的某个祖先。向上回溯。
			// 持续向上，直到“cur”是它“parent”的右孩子。
			Node* cur = _node;
			Node* parent = cur->_parent;
			while (parent && cur == parent->_left) {
				cur = parent;
				parent = cur->_parent;
			}
			_node = parent;
		}
		return *this;
	}
	Ref operator*() {
		return _node->_data;
	}
	Ptr operator->() {
		return &(_node->_data);
	}

	bool operator!= (const Self& s) const
	{
		return _node != s._node;
	}

	bool operator== (const Self& s) const
	{
		return _node == s._node;
	}
};

template<class K, class T,class KeyofT>
class RBTree {
	//typedef RBTreeNode<T> Node;
	using Node = RBTreeNode<T>;
public:
	typedef RBTreeIterator<T, T&, T*> Iterator;
	typedef RBTreeIterator<T,const T&,const T*> ConstIterator;
	RBTree() = default;
	Iterator Begin()
	{
		Node* cur = _root;
		while (cur && cur->_left)
		{
			cur = cur->_left;
		}

		return Iterator(cur, _root);
	}
	Iterator End()
	{
		return Iterator(nullptr, _root);
	}
	ConstIterator Begin() const
	{
		Node* cur = _root;
		while (cur && cur->_left)
		{
			cur = cur->_left;
		}

		return ConstIterator(cur, _root);
	}
	ConstIterator End() const
	{
		return ConstIterator(nullptr, _root);
	}
	pair<Iterator,bool> Insert(const T& data) {
		//树为空 该节点直接为根节点
		if (_root == nullptr)
		{
			_root = new Node(data);
			_root->_cor = BLACK;

			return { Iterator(_root, _root), true };
		}
		//树不为空 找空结点
		KeyofT kot; //取结点里date(pair类型)的first
		Node* parent = nullptr;
		Node* cur = _root;
		while (cur)
		{
			if (kot(cur->_data) < kot(data))
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (kot(cur->_data) > kot(data))
			{
				parent = cur;
				cur = cur->_left;
			}
			else
			{
				return { Iterator(cur,_root), false };
			}
		}
		//找到空结点 开辟新空间并初始化
		cur = new Node(data);
		Node* newnode = cur;
		cur->_cor = RED;
		//新节点与父节点链接起来
		if (kot(parent->_data) < kot(data))
		{
			parent->_right = cur;
		}
		else
		{
			parent->_left = cur;
		}
		cur->_parent = parent;

		//父亲是红色，出现连续的红色结点，需要处理
		while (parent && parent->_cor == RED) {
			Node* gf = parent->_parent;
			if (parent == gf->_left) {
				//   g
				// p   u
				Node* uncle = gf->_right;
				//1.叔叔存在且为红色 变色
				if (uncle && uncle->_cor == RED) {
					//变色了需继续向上
					parent->_cor = uncle->_cor = BLACK;
					gf->_cor = RED;

					cur = gf;
					parent = cur->_parent;
				}
				else {
					//2.叔叔不存在或存在为黑色 旋转加变色
					//情况a 单旋加变色 g为旋转点
					//      g 黑				     p
					//  红 p   u(不存在/黑) ->     c   g
					//红 c								 u
					if (cur == parent->_left) {
						RotateR(gf);
						parent->_cor = BLACK;
						gf->_cor = RED;
					 }
					//情况b 双旋加变色
					//  黑 g		 c
					// 红 p   u -> p   g
					//  红 c			 u
					else {
						RotateL(parent);
						RotateR(gf);
						cur->_cor = BLACK;
						gf->_cor = RED;
					}
					break;
				}
			}
			//parent == gf->_right
			else { 
				//   g
				// u   p
				Node* uncle = gf->_left;
				if (uncle && uncle->_cor == RED) {
					//变色了需继续向上
					parent->_cor = uncle->_cor = BLACK;
					gf->_cor = RED;

					cur = gf;
					parent = cur->_parent;
				}else{
					//2.叔叔不存在或存在为黑色 旋转加变色
					//情况a 单旋加变色 g为旋转点
					if (cur == parent->_right) {
						//   g			  p
						// u   p        g   c
						//       c	   u
						RotateL(gf);
						parent->_cor = BLACK;
						gf->_cor = RED;
					}
					//情况b 双旋加变色
					else { 
						//   g			c
						// u   p      g   p
						//    c      u
						RotateR(parent);
						RotateL(gf);
						cur->_cor = BLACK;
						gf->_cor = RED;
					}
					break;
				}
			}
		}
		_root->_cor = BLACK; //最后将根置为黑色
		return {Iterator(newnode,_root),true};
	}
	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		parent->_left = subLR;
		if (subLR)
			subLR->_parent = parent;

		Node* pParent = parent->_parent;

		subL->_right = parent;
		parent->_parent = subL;

		if (parent == _root)
		{
			_root = subL;
			subL->_parent = nullptr;
		}
		else
		{
			if (pParent->_left == parent)
			{
				pParent->_left = subL;
			}
			else
			{
				pParent->_right = subL;
			}

			subL->_parent = pParent;
		}
	}
	void RotateL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		parent->_right = subRL;
		if (subRL)
			subRL->_parent = parent;

		Node* parentParent = parent->_parent;
		subR->_left = parent;
		parent->_parent = subR;
		if (parentParent == nullptr)
		{
			_root = subR;
			subR->_parent = nullptr;
		}
		else
		{
			if (parent == parentParent->_left)
			{
				parentParent->_left = subR;
			}
			else
			{
				parentParent->_right = subR;
			}
			subR->_parent = parentParent;
		}
	}
	Node* Find(const K& key)
	{
		KeyofT kot;
		Node* cur = _root;
		while (cur)
		{
			if (kot(cur->_data)<key)
			{
				cur = cur->_right;
			}
			else if (kot(cur->_data) > key)
			{
				cur = cur->_left;
			}
			else
			{
				return cur;
			}
		}
		return nullptr;
	} //与二叉搜索树逻辑相同
	bool IsBalance()
	{
		if (_root == nullptr)
			return true;
		if (_root->_cor == RED)
			return false;
		// 参考值
		int refNum = 0;
		Node* cur = _root;
		while (cur)
		{
			if (cur->_cor == BLACK)
			{
				++refNum;
			}
			cur = cur->_left;
		}
		return Check(_root, 0, refNum);
	}
	int Height()
	{
		return _Height(_root);
	}
	int Size()
	{
		return _Size(_root);
	}

private:
	bool Check(Node* root, int blackNum, const int refNum)
	{
		if (root == nullptr)
		{
			// 前序遍历走到空时，意味着⼀条路径走完了
			//cout << blackNum << endl;
			if (refNum != blackNum)
			{
				cout << "存在黑色结点的数量不相等的路径" << endl;
				return false;
			}
			return true;
		}
		// 检查孩⼦不太方便，因为孩子有两个，且不⼀定存在，反过来检查父亲就方便多了
		if (root->_cor == RED && root->_parent->_cor == RED)
		{
			cout << root->_kv.first << "存在连续的红色结点" << endl;
			return false;
		}
		if (root->_cor == BLACK)
		{
			blackNum++;
		}
		return Check(root->_left, blackNum, refNum)
			&& Check(root->_right, blackNum, refNum);
	}
	int _Height(Node* root)
	{
		if (root == nullptr)
			return 0;
		int leftHeight = _Height(root->_left);
		int rightHeight = _Height(root->_right);
		return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
	}
	int _Size(Node* root)
	{
		if (root == nullptr)
			return 0;

		return _Size(root->_left) + _Size(root->_right) + 1;
	}

	Node* _root = nullptr;
};