#include <iostream>
using namespace std;

int main() {
    unsigned int x = 0x12345678;
    /*不再把这块内存当成一个 4 字节整数看，
    而是把它当成一个个字节来看。*/
    unsigned char* p = (unsigned char*)&x;

    if (p[0] == 0x78) {
        cout << "小端 little-endian\n";
    }
    else if (p[0] == 0x12) {
        cout << "大端 big-endian\n";
    }
    else {
        cout << "未知字节序\n";
    }

    return 0;
}