#include<iostream>
#include<assert.h>
using namespace std;

template<class K,class V>
struct AVLTreeNode{
	pair<K, V> _kv;
	AVLTreeNode<K, V>* _parent;
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	int _bf; //平衡因子

	AVLTreeNode(const pair<K,V>& kv)
		:_kv(kv),
		_parent(nullptr),
		_left(nullptr),
		_right(nullptr),
		_bf(0)
	{}
};

template<class K,class V>
class  AVLTree {
	typedef AVLTreeNode<K, V> Node;
	//using Node=AVLTreeNode<K, V>
public:
	bool Insert(const pair<K, V>& kv) {
		//树为空 直接插入
		if (!_root) {
			_root = new Node(kv); //生成新节点
			return true;
		}
		//树不为空
		//先找到插入的空位置
		Node* parent = nullptr;
		Node* cur = _root;
		while (cur) {
			if (cur->_kv.first < kv.first) {
				parent = cur;
				cur = cur->_right; //往大结点移动;
			}
			else if (cur->_kv.first > kv.first) {
				parent = cur;
				cur = cur->_left; //往小结点移动
			}
			else {
				return false; //此处不考虑相等的情况
			}
		}
		//找到位置后 开辟新空间
		cur = new Node(kv);
		if (parent->_kv.first < kv.first)
		{ //父小 放在父节点的右边
			parent->_right = cur;
		}
		else
		{ //父大 放在父节点的左边
			parent->_left = cur;
		}
		//链接父亲
		cur->_parent = parent;
		//控制平衡
		while (parent) {
			if (parent->_left == cur) {
				parent->_bf--;
			}
			else if (parent->_right == cur) {
				parent->_bf++;
			}
			else assert("balance false");

			if (parent->_bf == 0) {
				break; //此时已经达到平衡 不影响上面父节点
			}
			else if (parent->_bf == 1 || parent->_bf == -1) {
				//0-> 1 |-1 需要继续向上
				cur = parent;
				parent = parent->_parent;
			}
			else if (parent->_bf == 2 || parent->_bf == -2) {
				//此时已经不符合平衡二叉搜索树的条件 需要旋转
				if (parent->_bf == -2&&cur->_bf== -1) { //最左侧树不平衡 需要右旋
					RotateR(parent);
				}
				else if (parent->_bf == 2 && cur->_bf == 1) { //最右侧树不平衡 需要左旋
					RotateL(parent);
				}
				else if (parent->_bf == -2 && cur->_bf == 1) { //需要左右旋
					RotateLR(parent);
				}
				else if (parent->_bf == 2 && cur->_bf == -1) { //需要右左旋
					RotateRL(parent);
				}
				else {
					assert("false");
				}
				break;
			}
			else {
				assert("balance false!");
			}

		}
		return true;
	}
	Node* Find(const K& key) {
		/*if (!_root) {
			return false;
		}*/
		Node* cur = _root; //树为空不进入循环直接返回False
		while (cur) {
			if (cur->_kv.first < key) {
				//此时值小 往大的右走
				cur = cur->_right;
			}
			else if (cur->_kv.first > key) {
				//此时值大 往左
				cur = cur->_left;
			}
			else {
				return cur;
			}
		}
		return nullptr;
	}
	void RotateR(Node* parent) {
		Node* pp = parent->_parent;
		Node* subl = parent->_left;
		Node* sublr = subl->_right;

		//链接过程 发生变化的可能有6个点
		parent->_left = sublr;
		parent->_parent = subl;
		subl->_right = parent;

		if (sublr) { //如果sublr存在 那么父指向p
			sublr->_parent = parent;
		}

		//改变pp以及subl的父指向
		if (pp) {
			if (pp->_left == parent) {
				pp->_left = subl;
			}
			else {
				pp->_right = subl;
			}
			subl->_parent = pp;
		}
		else { //pp为空 parent为根结点 此时subl为新根节点
			_root = subl;
			subl->_parent = nullptr;
		}
		parent->_bf = subl->_bf = 0; //旋转后 已经平衡 改变平衡因子
	}
	void RotateL(Node* parent) {
		Node* pp = parent->_parent;
		Node* subr = parent->_right;
		Node* subrl = subr->_left;

		parent->_right = subrl;
		parent->_parent = subr;
		subr->_left = parent;

		if (subrl) { //如果subrl存在 那么父指向p
			subrl->_parent = parent;
		}

		//改变pp以及subr的父指向
		if (pp) {
			if (pp->_left == parent) {
				pp->_left = subr;
			}
			else pp->_right = subr;
			subr->_parent = pp;
		}
		else { //pp为空 parent为根结点 此时subr为新根节点
			_root = subr;
			subr->_parent = nullptr;
		}

		parent->_bf = subr->_bf = 0; //旋转后 已经平衡 改变平衡因子
	}
	void RotateLR(Node* parent) {
		Node* subl = parent->_left;
		Node* sublr = subl->_right;
		int bf = sublr->_bf;
		RotateL(parent->_left);
		RotateR(parent);
		if (bf == 0) {
			subl->_bf = 0;
			parent->_bf = 0;
			sublr->_bf = 0;
		}
		else if (bf == -1) {
			subl->_bf = 0;
			parent->_bf = 1;
			sublr->_bf = 0;
		}
		else if (bf == 1) {
			subl->_bf = -1;
			parent->_bf = 0;
			sublr->_bf = 0;
		}
		else {
			assert("RotateLR false");
		}
	}
	void RotateRL(Node* parent) {
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int bf = subRL->_bf;
		RotateR(parent->_right);
		RotateL(parent);
		if (bf == 0)
		{
			subR->_bf = 0;
			subRL->_bf = 0;
			parent->_bf = 0;
		}
		else if (bf == 1)
		{
			subR->_bf = 0;
			subRL->_bf = 0;
			parent->_bf = -1;
		}
		else if (bf == -1)
		{
			subR->_bf = 1;
			subRL->_bf = 0;
			parent->_bf = 0;
		}
		else
		{
			assert("RotateRL false");
		}
	}

	void InOrder() {
		//前序遍历 左 根 右
		_InOrder(_root);
		cout << endl;
	}
	bool IsBalanceTree()
	{
		return _IsBalanceTree(_root);
	}
	int Height() {
		return _Height(_root);
	}
	int Size() {
		return _Size(_root);
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
		cout << root->_kv.first << ":" << root->_kv.second << " ";
		_InOrder(root->_right);
	}
	//树高度=左子树高度 与 右子树高度 大的那个+ 1(自身)
	int _Height(Node* root)
	{
		if (root == nullptr)
			return 0;
		int leftHeight = _Height(root->_left);
		int rightHeight = _Height(root->_right);
		return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
	}

	//计算左右子树高度差与自身平衡因子比较 判断是否为AVL树
	bool _IsBalanceTree(Node* root)
	{
		// 空树也是AVL树
		if (nullptr == root)
			return true;
		// 计算pRoot结点的平衡因⼦：即pRoot左右⼦树的高度差
		int leftHeight = _Height(root->_left);
		int rightHeight = _Height(root->_right);
		int diff = rightHeight - leftHeight;
		// 如果计算出的平衡因⼦与pRoot的平衡因子不相等，或者
		// pRoot平衡因⼦的绝对值超过1，则⼀定不是AVL树
		if (abs(diff) >= 2)
		{
			cout << root->_kv.first << "高度差异常" << endl;
			return false;
		}
		if (root->_bf != diff)
		{
			cout << root->_kv.first << "平衡因子异常" << endl;
			return false;
		}
		// pRoot的左和右如果都是AVL树，则该树⼀定是AVL树
		return _IsBalanceTree(root->_left) && _IsBalanceTree(root->_right);
	}
	int _Size(Node* root)
	{
		if (root == nullptr)
			return 0;

		return _Size(root->_left) + _Size(root->_right) + 1;
	}
private:
	Node* _root = nullptr;
};