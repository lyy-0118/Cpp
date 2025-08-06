#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:4996)
#include"BT.h"
#include<string.h>
BTnode* CreateBT() {
    BTnode* n1=CreateNode(1);
    BTnode* n2 = CreateNode(2);
    BTnode* n7 = CreateNode(7);
    BTnode* n3 = CreateNode(3);
    BTnode* n4 = CreateNode(4);
    BTnode* n5 = CreateNode(5);
    BTnode* n6 = CreateNode(6);
    BTnode* n8 = CreateNode(8);
    n1->left = n2;
    n1->right = n4;
    n2->left = n3;
    n2->right = n7;
    n4->left = n5;
    n4->right = n6;
    n6->right = n8;
    return n1;
}
void test01() {
    BTnode* n = CreateBT();
    printf("总节点个数%d\n", BTnodeSize(n));
    printf("叶子节点个数%d\n", BTtreeSize(n));
    printf("树高度%d\n", BTHigh(n));

    printf("第1层节点个数%d\n", BTknodeSize(n, 1));
    printf("第2层节点个数%d\n", BTknodeSize(n, 2));
    printf("第3层节点个数%d\n", BTknodeSize(n, 3));
    printf("第4层节点个数%d\n", BTknodeSize(n, 4));

    preorder(n);
    printf("\n");
    inorder(n);
    printf("\n");
    postorder(n);
}
void test02() {
    BTnode* n = CreateBT();
    BinaryTreeLevelOrder(n);
    if (BinaryTreeComplete(n)) {
        printf("是完全二叉树");
    }
    else {
        printf("不是完全二叉树！\n");
    }
    BTDestroy(n);
    n = NULL;  //传一级指针需要在销毁函数体外置为空 防止野指针
}
void test03() {
    char a[100] = {};
    scanf("%s", a);
    int n = strlen(a);
    int pi = 0;
    BTnode* btroot = BinaryTreeCreate(a, n, &pi);
    inorder(btroot);
}
int main(){
    //test01();
    test03();
    return 0;
}