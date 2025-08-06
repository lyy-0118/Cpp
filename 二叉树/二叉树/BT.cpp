//二叉树相关代码实现的定义
#include"BT.h"
BTnode* CreateNode(BTdata x) {
	BTnode* newnode = (BTnode*)malloc(sizeof(BTnode));
	if (!newnode) {
		perror("malloc fail");
		return NULL;
	}
	else {
		newnode->val = x;
		newnode->left = NULL;
		newnode->right = NULL;
	}
	return newnode;
}
int BTnodeSize(BTnode* root) {
	if (root == NULL) return 0;
	else return BTnodeSize(root->left) + BTnodeSize(root->right) + 1;
}
int BTtreeSize(BTnode* root) {
	if (root == NULL) return 0;
	else if (root->left == NULL && root->right == NULL)
	{
		return 1;  //左右字树均为空则是叶子节点
	}
	else return BTtreeSize(root->left) + BTtreeSize(root->right);
}
int BTHigh(BTnode* root) {  //左子树 右子树高度大的那个加1(自身）
	if (root == NULL) return 0;
	int lefthigh = BTHigh(root->left);
	int righthigh = BTHigh(root->right);
	return lefthigh > righthigh ? lefthigh + 1 : righthigh + 1;
}

//求第k层节点个数
int BTknodeSize(BTnode* root,int k) {
    assert(k >0);
	if (root == NULL) return 0;    //空返回0
	else if (k == 1) return 1;     //第一层 返回1
	else if (k > 1) return BTknodeSize(root->left, k - 1) + BTknodeSize(root->right, k - 1); //K层(k>1) k-1层的左子树+k-1层的右子树
}

void preorder(BTnode* root) {
    //根 左子树 右子树
    if (root == NULL) {
        printf("N ");
        return;
    }
    printf("%d ", root->val);
    preorder(root->left);
    preorder(root->right);
}
void inorder(BTnode* root) {
    //中序 左子树 根 右子树
    if (root == NULL) {
        //printf("N ");
        return;
    }
    inorder(root->left);
    printf("%c ", root->val);
    inorder(root->right);
}
void postorder(BTnode* root) {
    //后序 左子树 右子树 根
    if (root == NULL) {
        printf("N ");
        return;
    }
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->val);
}
//层序遍历
void BinaryTreeLevelOrder(BTnode* root) {
    assert(root);
    //创建一个队列
    Queue q;
    QueueInit(&q);
    QueuePush(&q, root);  //push根节点
    while (!QueueEmpty(&q)) {  //当队列非空时
        BTnode* front = QueueHead(&q);  //取出队列首数据 
        QueuePop(&q);                   //头删
        if (front->left) { 
            QueuePush(&q, front->left);   //push左子树
        }
        if (front->right) {
            QueuePush(&q, front->right);  //push右子树
        }
        printf("%d ", front->val);
    }
    QueueDestroy(&q);

}

//通过前序遍历的数组"ABD##E#H##CF##G##"构建二叉树
BTnode* BinaryTreeCreate(BTdata* a, int n, int* pi) {
	if (*pi >= n || a[*pi] == '#') { //此时索引大于数组长度或读取到特殊位置
		(*pi)++;
		return NULL; //表示空值
	}
	BTnode* node = CreateNode(a[*pi]);
	(*pi)++;
	node->left = BinaryTreeCreate(a, n, pi);
	node->right = BinaryTreeCreate(a, n, pi);
	return node;
}

//判断是否是完全二叉树
bool BinaryTreeComplete(BTnode* root) {
	assert(root);
	//创建一个队列
	Queue q;
	QueueInit(&q);
	QueuePush(&q, root);

	//把二叉树数据push到队列里
	//1.层序遍历，空也进队列
	while (!QueueEmpty(&q)) {
		BTnode* front = QueueHead(&q);
		QueuePop(&q);
		//当遇到第一个空时 跳出循环 开始判断
		if (front == NULL) {
			break;
		}
		QueuePush(&q, front->left);
		QueuePush(&q, front->right);
	}
	//2.遇到第一个空节点时，开始判断，后面全空就是完全二叉树，后面有非空就不是完全二叉树
	while (!QueueEmpty(&q)) {
		BTnode* front = QueueHead(&q);
		QueuePop(&q);
		if (front != NULL) {
			return false;
		}
	}
	QueueDestroy(&q);
	return true;
}
void BTDestroy(BTnode* root){
	if (root == NULL) return;
	BTDestroy(root->left);
	BTDestroy(root->right);
	free(root);
}
void BinaryTreeDestory(BTnode** root){
	if (*root == NULL) {
		return;
	}
	BinaryTreeDestory(&((*root)->left));
	BinaryTreeDestory(&((*root)->right));
	free(*root);
	*root = NULL;
}
void QueueInit(Queue* pq) {
	assert(pq);
	pq->head = NULL;
	pq->tail = NULL;
	pq->size = 0;
}
//队尾插入
void QueuePush(Queue* pq, Type x) {
	assert(pq);
	QueueNode* newnode = (QueueNode*)malloc(sizeof(QueueNode));
	if (!newnode) {
		perror("malloc fail");
		return;
	}
	newnode->val = x;
	newnode->next = NULL;
	if (pq->size == 0) {
		pq->head = pq->tail = newnode;
	}
	else {
		pq->tail->next = newnode;
		pq->tail = newnode;
	}
	pq->size++;
}
//队头删除
void QueuePop(Queue* pq) {
	assert(pq);
	assert(pq->size > 0); //元素必须大于0 否则没数据无法删除

	if (pq->size == 1) { //1个节点
		free(pq->head);
		pq->head = pq->tail = NULL;
	}
	else { //多个节点 head往后走
		QueueNode* next = pq->head->next;
		free(pq->head);
		pq->head = next;
	}
	pq->size--;
}

void QueueDestroy(Queue* pq) {
	assert(pq);
	QueueNode* cur = pq->head;
	while (cur) {
		QueueNode* next = cur->next;
		free(cur);
		cur = next;
	}
	pq->head = pq->tail = NULL;
	pq->size = 0;
}

int QueueSize(Queue* pq) {
	assert(pq);
	return pq->size;
}

Type QueueHead(Queue* pq) {
	assert(pq);
	assert(pq->head);
	return pq->head->val;
}

Type QueueTail(Queue* pq) {
	assert(pq);
	assert(pq->tail);
	return pq->tail->val;
}
bool QueueEmpty(Queue* pq) {
	assert(pq);
	return pq->size == 0;
}