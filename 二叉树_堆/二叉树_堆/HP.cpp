#include"HP.h"

void HPInit(HP* php) {
	assert(php);
	php->n = NULL;
	php->size =php->capacity= 0;
}
void HPDestroy(HP* php) {
	assert(php);
	free(php->n);
	php->n = NULL;
	php->size = php->capacity = 0;
}
void HPCheck(HP* ps) {   //检查堆空间是否足够
	assert(ps);
	if (ps->size == ps->capacity) {
		//三目表达式
		int newcapacity = ps->capacity == 0 ? 4 : ps->capacity * 2;
		//扩容
		HPdataType* ans = (HPdataType*)realloc(ps->n, sizeof(HPdataType) * newcapacity);
		//扩容失败
		if (!ans) {
			perror("realloc fail");
			exit(1);
		}
		//申请成功
		ps->n = ans;
		ps->capacity = newcapacity;
	}
}
void Swap(HPdataType* x1, HPdataType* x2) {
	HPdataType tmp = *x1;
	*x1 = *x2;
	*x2 = tmp;
}

void AdjustUp(HPdataType* a, int child)
{
	// 初始条件
	// 中间过程
	// 结束条件
	int parent = (child - 1) / 2;
	while (child > 0) //当子节点调整到根结点时 就不需要再向上调整了
	{
		if (a[child] < a[parent])
		{
			Swap(&a[child], &a[parent]);
			child = parent;
			parent = (child - 1) / 2;
		}
		else
		{
			break;
		}
	}
}

void HPPush(HP* php,HPdataType x) {
	assert(php);
	//判断空间是否足够
	HPCheck(php);
	//1.将元素放到最后
	//2.向上调正
	int child = php->size - 1;
	php->n[php->size] = x;
	php->size++;
	//AdjustUp(php->n, php->size - 1);
	int parent = (child - 1) / 2;
	while (child > 0) { //parent>=0时循环继续
		if (php->n[child] < php->n[parent]) {
			//孩子比父亲小则交换
			Swap(&(php->n[child]), &(php->n[parent]));
			child = parent;
			parent= (child - 1) / 2;
		}
		else {
			break;
		}
	}
}

void AdjustDown(HPdataType* a, int n,int parent) {  //n表示堆里面的元素个数
	assert(a);
	//int parent = 0;
	//假设左孩子比较小 与右孩子相比
	int child = parent * 2 + 1;
	while (child <n) {  //child>=n 孩子不存在 跳出循环
		if (child + 1 < n && a[child] >a[child + 1]) {
			//若右孩子比较小 则
			child++;
		}
		if (a[child] <a[parent]) {
			//孩子比父亲小 则交换数值
			Swap(&(a[child]), &(a[parent]));
			//继续往下
			parent = child;
			child = parent * 2 + 1;
		}
		else {
			break;
		}
	}

}
void HPPop(HP* php) {
	assert(php);
	assert(php->size>0);
	//删除堆顶元素
	//1.将堆顶元素与堆尾元素交换
	//2.size--;
	//3.向下调整
	Swap(&(php->n[0]), &(php->n[php->size - 1]));
	php->size--;
	AdjustDown(php->n,php->size-1,0);
}
bool HPEmpty(HP* php)
{
	assert(php);

	return php->size == 0;
}

HPdataType HPTop(HP* php)
{
	assert(php);
	assert(php->size > 0);

	return php->n[0];
}
int Top(int* php)
{
	return php[0];
}