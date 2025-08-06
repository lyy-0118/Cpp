#include"list.h"
#include<list>
#include<vector>
using namespace std;
class A {
public:
	A(int a1,int a2):_a1(a1),_a2(a2) {
		cout << "默认" << endl;
	}
	A(const A& a) {
		_a1 = a._a1;
		_a2 = a._a2;
		cout << "拷贝" << endl;
	}
private:
	int _a1;
	int _a2;
};
void test1() {
	lyy::list<int> lt;
	lt.push_back(1);
	lt.push_back(2);
	lyy::print_container(lt);
}
void test2() {
	lyy::list<int> lt;
	lt.push_back(1);
	lt.push_back(2);
	lt.push_back(3);
	lyy::print_container(lt);

	//找对应位置的迭代器
	/*auto it = lt.begin();
	int k = 2;
	while (k--) {
		++it;
	}*/
	auto it1 = lt.end();  //相当于尾插
	lt.insert(it1, 4);
	lyy::print_container(lt);

	//auto it2 = lt.begin();  //相当于头插
	//lt.insert(it2, 5);
	//lyy::print_container(lt);
	lt.push_front(100);
	lyy::print_container(lt);

}
void test3() {
	lyy::list<int> lt={1,2};
	//std::list<int> lt2 = { 1,2 };
	lt.push_back(6);
	lt.push_back(7);
	lyy::print_container(lt);
	auto it = lt.begin();
	it = it++;

}
void test4() {
	//lyy::list<int> lt = { 1,2 };
	////std::list<int> lt2 = { 1,2 };
	//lt.push_back(6);
	//lt.push_back(7);
	//lyy::print_container(lt);
	std::vector<string> v({"lyy", "wy"});
	lyy::list<string> lt2(v.begin(), v.end());
	lyy::print_container(lt2);
	/*lt2.clear();
	lyy::print_container(lt2);*/
	lyy::list_iterator<string,string&,string*> it = lt2.begin();
	while (it != lt2.end()) {
		cout << *it << " ";
		it++;
	}

}

int main(){
	test3();
	return 0;
}