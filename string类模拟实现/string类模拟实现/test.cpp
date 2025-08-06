#include"string.h"
void test1() {
    lyy::string s1("hello");
    lyy::string s2(s1);
    lyy::string s3=s1;
    cout << s1.c_str()<<endl;
    cout << s2.c_str()<<endl;
    cout << s3.c_str() << endl;
}
void test2() {
    string a("abcd");
    for (auto e : a) {
        cout << e << ' ';
    }
}
void test3() {
    lyy::string s1("hello");
    lyy::string s = s1.substr(1,3);
    cout<<s.c_str()<<endl;
    cout << s.size() <<' '<< s.capacity() << endl;
}
void test4(){
    string s1("hello");
    string s = s1.substr(0, 5);
    cout << s.c_str() << endl;
    cout << s.size() << ' ' << s.capacity() << endl;
}
void test5() {
    lyy::string s1("abcd");
    lyy::string s2("abcdef");
    s2.erase(0);
    cout << s2;
    s1 == s2 ? 1 : 2;
}
int main(){
    test5();
    //test4();
    //test2();
    return 0;
}