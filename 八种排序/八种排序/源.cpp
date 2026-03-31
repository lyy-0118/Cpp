#include<vector>
#include<iostream>
#include "MySort.h"
using namespace std;

template<typename T>
void print_array(const vector<T>& arr) {
    for (auto x : arr)
        cout << x << " ";
    cout << endl;
}

int main() {

    vector<int> a = { 8, 9, 1, 7, 2, 3, 5, 4, 6, 0 };
    cout << "ÅÅÐòÇ°£º";
    print_array(a);
    Selection_Sort(a);
    cout << "ÅÅÐòºó£º";
    print_array(a);

    return 0;
}