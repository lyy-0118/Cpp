#pragma once
#include <vector>
#include <utility>

//冒泡
template<typename T>
void Bubble_Sort(std::vector<T>& arr) {
    int n = static_cast<int>(arr.size());
    for (int i = 0; i < n - 1; i++) {
        bool flag = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                flag = true;
            }
        }
        if (!flag) break;
    }
}

//选择
template<typename T>
void Selection_Sort(std::vector<T>& a) {
    int left = 0, right = static_cast<int>(a.size()) - 1;
    while (left < right) {
        int mini = left, maxi = left;
        for (int i = left; i <= right; i++) {
            if (a[i] < a[mini]) mini = i;
            if (a[i] > a[maxi]) maxi = i;
        }

        std::swap(a[left], a[mini]);
        if (maxi == left) maxi = mini;
        std::swap(a[right], a[maxi]);

        left++;
        right--;
    }
}

//插入
template<typename T>
void Insertion_Sort(std::vector<T>& arr) {
    int n = static_cast<int>(arr.size());
    for (int i = 1; i < n; i++) {
        T key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// 希尔排序（插入排序的改进版）
// gap==1时候为插入排序，又叫递减增量排序算法
void ShellSort(int* a, int n)
{
    int gap = n;
    while (gap > 1)
    {
        gap = gap / 3 + 1;
        for (int i = gap; i < n; ++i)
        {
            int tmp = a[i];
            int j = i - gap;
            while (j >= 0 && a[j] > tmp)
            {
                a[j + gap] = a[j];
                j -= gap;
            }
            a[j + gap] = tmp;
        }
    }
}

//// 希尔排序带注释版
//void ShellSort(int* a, int n)
//{
//    // gap 表示分组时的间隔（增量）
//    int gap = n;
//
//    // 当 gap 缩小到 1 时，最后一轮其实就是直接插入排序
//    while (gap > 1)
//    {
//        // 缩小增量
//        // 这种写法可以保证 gap 最终一定变成 1
//        gap = gap / 3 + 1;
//
//        // 从第 gap 个元素开始，依次对每个“组”做插入排序
//        for (int i = gap; i < n; ++i)
//        {
//            // 保存当前待插入的元素
//            int tmp = a[i];
//
//            // j 指向当前组中，i 前面的那个元素
//            int j = i - gap;
//
//            // 在当前组内向前寻找插入位置
//            // 如果前面的元素比 tmp 大，就向后挪动
//            while (j >= 0 && a[j] > tmp)
//            {
//                a[j + gap] = a[j];
//                j -= gap; // 注意这里是减 gap，不是减 1
//            }
//
//            // 找到合适位置后插入 tmp
//            a[j + gap] = tmp;
//        }
//    }
//}