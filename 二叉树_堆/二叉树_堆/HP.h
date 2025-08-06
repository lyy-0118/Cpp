#pragma once
#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
typedef int HPdataType;

//本质上是顺序表（数组）
typedef struct HP{
	HPdataType * n;
    int size;
	int capacity;
}HP;

void HPInit(HP* php);
void HPPop(HP* php);
void HPCheck(HP* ps);
void HPPush(HP* php, HPdataType x);
void Swap(HPdataType* x1,HPdataType* x2);

void AdjustUp(HPdataType* a, int child); 

//n表示堆里面的元素个数
void AdjustDown(HPdataType* a, int n,int parent);

bool HPEmpty(HP* php);
HPdataType HPTop(HP* php);
void HPDestroy(HP* php);