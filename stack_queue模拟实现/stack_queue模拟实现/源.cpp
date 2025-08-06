#include<iostream>
#include<vector>
#include<list>
#include<queue>
using namespace std;
#include"Stack.h"
#include"Queue.h"
#include"priority.h"

void test1() {
	lyy::Stack<int, vector<int>> s;
	s.push(1);
	s.push(2);
	s.pop();
	//s.pop();
	printf("%zu ", s.size());
	if (s.empty()) {
		printf("¿Õ");
	}
	else {
		printf("·Ç¿Õ");
	}
}
void test2() {
	lyy::Queue<int, list<int>> q;
	q.push(10);
	q.push(15);
	cout << q.back() << " ";
	cout << q.front();
	//s.pop();
	printf(" %zu ", q.size());
	if (q.empty()) {
		printf("¿Õ");
	}
	else {
		printf("·Ç¿Õ");
	}
}
void test3() {
	priority_queue<string> s;
	s.push("abc");
	s.push("abcd");
	s.push("bcd");
	while (!s.empty()) {
		cout << s.top()<<" ";
		s.pop();
	}
}
void test4() {
	vector<int> v(10, 1);
	lyy::priority_queue<int, vector<int>, lyy::Greater<int>> pq(v.begin(), v.end());
	cout << pq.size() <<endl;
	while (!pq.empty())
	{
		cout << pq.top() << " ";
		pq.pop();
	}
	cout << endl;

}
int main(){
	test4();
	return 0;
}