#include"vector.h"
using namespace std;
void test1() {
    
    lyy::vector<int> v(1,5);
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.reserve(5);
    v.insert(v.begin(), 3);
}
void test2() {
    lyy::vector<int> v1(2, 5);
    lyy::print_container(v1);
    lyy::vector<int> v2(2, 3);
    lyy::print_container(v2);
    v2 = v1;
    lyy:: print_container(v2);
}
int main(){
    test2();
    
    return 0;
}