#include"HashTable.h"
int main() {
	int a[] = { 19,30,5,36,13,20,21,12 };
	HashTable<int, int> ht;
	for (auto e : a)
	{
		ht.Insert({ e, e });
	}
	ht.Insert({ 15, 15 }); // 测试插入已存在的键
	if (ht.Find(20)) {
		cout << "找到了" << endl;
	}
	else {
		cout << "没找到" << endl;
	}
	ht.Remove(20); // 测试删除键
	if (ht.Find(20)) {
		cout << "找到了" << endl;
	}
	else {
		cout << "没找到" << endl;
	}
	return 0;
}