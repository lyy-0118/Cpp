#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
#include<set>
#include<unordered_set>

using namespace std;

void test_set1()
{
	unordered_set<int> s = { 3,1,6,7,8,2,1,1,5,6,7,6 };
	unordered_set<int>::iterator it = s.begin();
	while (it != s.end())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;
}

void test_set2()
{
	const size_t N = 10000000;
	unordered_set<int> us;
	set<int> s;
	vector<int> v;
	v.reserve(N);
	srand(time(0));
	for (size_t i = 0; i < N; ++i)
	{
		//v.push_back(rand()); // N比较大时，重复值比较多
		//v.push_back(rand() + i); // 重复值相对少
		v.push_back(i); // 没有重复，有序
	}

	// 21:15
	size_t begin1 = clock();
	for (auto e : v)
	{
		s.insert(e);
	}
	size_t end1 = clock();
	cout << "set insert:" << end1 - begin1 << endl;

	size_t begin2 = clock();
	us.reserve(N);
	for (auto e : v)
	{
		us.insert(e);
	}
	size_t end2 = clock();
	cout << "unordered_set insert:" << end2 - begin2 << endl;

	cout << "插入数据个数：" << s.size() << endl;
	cout << "插入数据个数：" << us.size() << endl << endl;

	int m1 = 0;
	size_t begin3 = clock();
	for (auto e : v)
	{
		auto ret = s.find(e);
		if (ret != s.end())
		{
			++m1;
		}
	}
	size_t end3 = clock();
	cout << "set find:" << end3 - begin3 << "->" << m1 << endl;

	int m2 = 0;
	size_t begin4 = clock();
	for (auto e : v)
	{
		auto ret = us.find(e);
		if (ret != us.end())
		{
			++m2;
		}
	}
	size_t end4 = clock();
	cout << "unorered_set find:" << end4 - begin4 << "->" << m2 << endl;

	size_t begin5 = clock();
	for (auto e : v)
	{
		s.erase(e);
	}
	size_t end5 = clock();
	cout << "set erase:" << end5 - begin5 << endl;
	size_t begin6 = clock();

	for (auto e : v)
	{
		us.erase(e);
	}
	size_t end6 = clock();
	cout << "unordered_set erase:" << end6 - begin6 << endl << endl;
}

#include"HashTable.h"

//int main()
//{
//	//int a[] = { 19,30,52,63,11,22 };
//	int a[] = { 19,30,5,36,13,20,21,12 };
//	HashTable<int, int> ht;
//	for (auto e : a)
//	{
//		ht.Insert({ e, e });
//	}
//
//	//ht.Insert({ 15, 15 });
//
//	ht.Erase(30);
//	if (ht.Find(20))
//	{
//		cout << "找到了" << endl;
//	}
//
//	if (ht.Find(30))
//	{
//		cout << "找到了" << endl;
//	}
//	else
//	{
//		cout << "没有找到" << endl;
//	}
//
//	return 0;
//}

//struct StringHashFunc
//{
//	size_t operator()(const string& s)
//	{
//		size_t hash = 0;
//		for (auto ch : s)
//		{
//			hash += ch;
//		}
//
//		return hash;
//	}
//};

struct Date
{
	int _year;
	int _month;
	int _day;

	Date(int year = 1, int month = 1, int day = 1)
		:_year(year)
		, _month(month)
		, _day(day)
	{
	}

	bool operator==(const Date& d)
	{
		return _year == d._year
			&& _month == d._month
			&& _day == d._day;
	}
};

struct DateHashFunc
{
	size_t operator()(const Date& d)
	{
		size_t hash = 0;
		hash += d._year;
		hash *= 131;

		hash += d._month;
		hash *= 131;

		hash += d._day;
		hash *= 131;

		return hash;
	}
};

//int main()
//{
//	//int a[] = { 19,30,52,63,11,22 };
//	
//	const char* a1[] = { "abcd", "sort", "insert" };
//	HashTable<string, string> ht1;
//	for (auto& e : a1)
//	{
//		ht1.Insert({ e, e });
//	}
//
//	cout << HashFunc<string>()("abcd") << endl;
//	cout << HashFunc<string>()("bcad") << endl;
//	cout << HashFunc<string>()("aadd") << endl;
//
//	int a2[] = { -19,-30,5,36,13,20,21,12 };
//	HashTable<int, int> ht2;
//	for (auto e : a2)
//	{
//		ht2.Insert({ e, e });
//	}
//
//	// 哈希冲突
//	HashTable<Date, int, DateHashFunc> ht;
//	ht.Insert({ { 2024, 10, 12 }, 1});
//	ht.Insert({ { 2024, 12, 10 }, 1 });
//
//	return 0;
//}

int main()
{
	int a2[] = { 19,30,5,36,13,20,21,12,24,96 };
	hash_bucket::HashTable<int, int> ht2;
	for (auto e : a2)
	{
		ht2.Insert({ e, e });
	}

	ht2.Insert({ 100, 100 });
	ht2.Insert({ 101, 101 });


	return 0;
}