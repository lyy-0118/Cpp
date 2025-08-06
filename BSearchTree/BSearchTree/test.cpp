#include"BST.h"
using namespace std;
void test1() {
	key::BSTree<int> t;
	//≤Â»Î≤‚ ‘
	t.Insert(1);
	t.Insert(8);
	t.Insert(5);
	t.Insert(12);
	t.Insert(3);
	t.Insert(9);
	t.InOrder();

	//…æ≥˝≤‚ ‘
	t.Erase(5);
	t.InOrder();
	t.Erase(1);
	t.InOrder();
	t.Erase(8);
	t.InOrder();
	t.Erase(12);
	t.InOrder();
	t.Erase(3);
	t.InOrder();
	t.Erase(9);
	t.InOrder();
}
void test2() {
	key_value::BSTree<string, int> t;
	t.Insert("a", 1);
	t.Insert("c", 3);
	t.Insert("b", 2);
	auto e = t.Find("a");
	cout << e->_value << " ";
	t.InOrder();

	key_value::BSTree<string, int> t2(t);
	cout << "t2:";
	t2.InOrder();

	key_value::BSTree<string, int> t3(t);
	t3.Erase("b");
	t2 = t3;
	cout << "t2:";
	t2.InOrder();
}
void TestBSTree()
{
	/*key_value::BSTree<string, string> dict;
	dict.Insert("insert", "≤Â»Î");
	dict.Insert("erase", "…æ≥˝");
	dict.Insert("left", "◊Û±ﬂ");
	dict.Insert("string", "◊÷∑˚¥Æ");
	string str;
	while (cin >> str)
	{
		auto ret = dict.Find(str);
		if (ret)
		{
			cout << str << ":" << ret->_value << endl;
		}
		else
		{
			cout << "µ•¥ ∆¥–¥¥ÌŒÛ" << endl;
		}
	}*/

	string strs[] = { "∆ªπ˚", "Œ˜πœ", "∆ªπ˚", "”£Ã“", "∆ªπ˚", "”£Ã“", "∆ªπ˚", "”£Ã“", "∆ªπ˚" };
	// Õ≥º∆ÀÆπ˚≥ˆœ÷µƒ¥Œ
	key_value::BSTree<string, int> countTree;
	for (auto str : strs)
	{
		auto ret = countTree.Find(str);
		if (ret == NULL)
		{
			countTree.Insert(str, 1);
		}
		else
		{
			ret->_value++;
		}
	}
	countTree.InOrder();
}
int main(){
	//test1();
	//test2();
	TestBSTree();
	return 0;
}