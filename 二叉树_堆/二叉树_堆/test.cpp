#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning(disable:4996)
#include"HP.h"
#include<time.h>
void test01()
{
	int a[] = { 232,66,222,33,7,1,66,3333,999 };
	HP hp;
	HPInit(&hp);
	for (size_t i = 0; i < sizeof(a) / sizeof(int); i++)
	{
		HPPush(&hp, a[i]);
	}

	int i = 0;
	while (!HPEmpty(&hp))
	{
		printf("%d ", HPTop(&hp));
		a[i++] = HPTop(&hp);
		HPPop(&hp);
	}
	printf("\n");

	
	/*找出最小的k个数
	int k = 0;
	scanf("%d",&k);
	while (k--)
	{
		printf("%d ", HPTop(&hp));
		HPPop(&hp);
	}
	printf("\n");*/

	HPDestroy(&hp);
}

//从大到小排序堆排序算法 
void HeapSort(HPdataType* a,int size) {
	int i = 0;
	//1.先建成一个堆 降序建小堆 升序建大堆 
	//向上调整 时间复杂度O（N*logN)
	/*for (i = size-1; i > 0; i--) {
		AdjustUp(a, i);
	}*/
	//从第一个非叶节点开始，依次向下调整
	//向下调整 时间复杂度O(N)
	for (i = (size - 1 - 1) / 2; i >= 0; i--) {
		AdjustDown(a, size, i);
	}
	int end = size-1;  //元素下标最大为size-1
	//1.把最小的与最后的数 交换位置 (end向前走）
	//2.再向下调整 选出次小的 重复第一
	while (end>0) {
		Swap(&a[0], &a[end]);
		AdjustDown(a, end, 0);
		end--;
	}
}
void test02() {
	int a[] = { 1,33,22,999,21,4,444,5 };
	HeapSort(a, sizeof(a) / sizeof(a[0]));
	int i = 0;
	for (i = 0; i < sizeof(a) / sizeof(a[0]); i++) {
		printf("%d ", a[i]);
	}
}
void PrintTopK(int* a, int k) {
	int i = 0;
	while (i < k) {
		printf("%d ", a[i]);
		i++;
	}
}
void test03() {
	//找TopK问题
	srand(time(0));
	int n = 10;
	int* a = (int*)malloc(sizeof(int) * n);
	int i = 0;
	for (i = 0; i < n; i++) {
		a[i] = rand() % 1000 + i;
		//printf("%d ", a[i]);
	}
	a[1] = 100000;
	a[5] = 2123213;
	a[9] = 9999999;

	/*法1：数据量小的时候
	for (i = (n - 2) / 2; i >= 0; i--) {
		AdjustDown(a, n, i);
	} 
	for (i = n - 1; i > 0; i--) {
		Swap(&a[0], &a[i]);
		AdjustDown(a, i, 0);
	}*/
	int* ans = (int*)malloc(sizeof(int) * 5); //内存有限 先创建一个小堆
	for (i = 0; i < 5; i++) {
		ans[i] = a[i];
	}
	for (i = (5 - 2) / 2; i >= 0; i--) {
		AdjustDown(ans, 5, i);
	}
	for (i = 5; i < n;i++) {   //剩余n-k个数据依次比较 若比堆顶大则成为新堆顶元素 再向下调整成为新堆
		if (a[i] > ans[0]) {
			ans[0] = a[i];
			AdjustDown(ans, 5, 0);
		}
	}
	HeapSort(ans, 5);
	int k = 3;
	PrintTopK(ans, k);
}
int main(){
    //test01();
	//test02();
	test03();

    return 0;
}