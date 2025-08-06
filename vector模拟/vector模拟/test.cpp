#include"vector.h"
using namespace std;
void test1() {
    lyy::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    lyy::print_vector(v);
    v.insert(v.begin()+1, 3);
    lyy::print_vector(v);
}
int main() {
    test1();

    return 0;
}