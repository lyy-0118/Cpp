//二叉树相关代码实现的声明
#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
typedef char BTdata;

typedef struct BTnode {
	BTdata val;
	struct BTnode* left;
	struct BTnode* right;
}BTnode;

BTnode* CreateNode(BTdata x);

//二叉树总节点个数
int BTnodeSize(BTnode* root);

//叶子节点个数
int BTtreeSize(BTnode* root);

//高度
int BTHigh(BTnode* root);

int BTknodeSize(BTnode* root, int k);

//前序遍历
void preorder(BTnode* root);
//中序遍历
void inorder(BTnode* root);
//后序遍历
void postorder(BTnode* root);
//层序遍历
void BinaryTreeLevelOrder(BTnode* root);

BTnode* BinaryTreeCreate(BTdata* a, int n, int* pi);
//判断是否是完全二叉树
bool BinaryTreeComplete(BTnode* root);
//二叉树的销毁
void BTDestroy(BTnode* root);   //传一级指针 需要在执行完函数体后将root指针置为空
void BinaryTreeDestory(BTnode** root); //传二级指针
//将队列里存放数据的类型改为二叉树节点
typedef struct BTnode* Type;

//类似于链表结点
typedef struct QueueNode {
	Type val;     //数据
	struct QueueNode* next; //指向下一个节点的指针
}QueueNode;

//一个队列结构体
typedef struct Queue {
	QueueNode* head;
	QueueNode* tail;
	int size;
}Queue;

void QueueInit(Queue* pq);
void QueuePush(Queue* pq, Type x); //尾入
void QueuePop(Queue* pq);         //头出
void QueueDestroy(Queue* pq);
int QueueSize(Queue* pq);
//取队头的数据
Type QueueHead(Queue* pq);
//取队尾的数据
Type QueueTail(Queue* pq);

bool QueueEmpty(Queue* pq);