#include<iostream>
using namespace std;
#include"set.h"
#include"map.h"


void Print(const lyy::set<int>& s)
{
	lyy::set<int>::const_iterator it = s.end();
	while (it != s.begin())
	{
		--it;
		cout << *it << " ";
	}
	cout << endl;
}

int main()
{
	//lyy::set<int> s;
	//s.insert(5);
	//s.insert(1);
	//s.insert(3);
	//s.insert(2);
	//s.insert(6);

	//lyy::set<int>::iterator sit = s.begin();
	////*sit += 10;
	//while (sit != s.end())
	//{
	//	cout << *sit << " ";
	//	++sit;
	//}
	//cout << endl;

	//for (auto& e : s)
	//{
	//	cout << e << " ";
	//}
	//cout << endl;

	//Print(s);

	lyy::map<string, string> dict;
	dict.insert({ "sort", "排序" });
	dict.insert({ "left", "左边" });
	dict.insert({ "right", "右边" });

	lyy::map<string, string>::iterator it = dict.begin();
	while (it != dict.end())
	{
		// 不能修改first，可以修改second
		//it->first += 'x';
		it->second += 'x';

		cout << it->first << ":" << it->second << endl;
		++it;
	}
	cout << endl;

	for (auto& kv : dict)
	{
		cout << kv.first << ":" << kv.second << endl;
	}
	dict["abcd"] = "1";
	cout << dict["abcd"] << endl;

	return 0;
}