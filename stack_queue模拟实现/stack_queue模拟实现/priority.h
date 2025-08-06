#pragma once
namespace lyy
{   
    //仿函数
    template<class T>
    class Less {
    public:
        bool operator()(const T& x, const T& y) {
            return x < y;
        }
    };

    template<class T>
    class Greater
    {
    public:
        bool operator()(const T& x, const T& y)
        {
            return x > y;
        }
    };

    template <class T, class Container=vector<int>,class Compare = Less<T> >
    class priority_queue{
    public:
        priority_queue() {}
        // 默认是大堆
        template <class InputIterator>
        priority_queue(InputIterator first, InputIterator last) {
            while (first != last) {
                c.push_back(*first);
                ++first;
            }
        }

        bool empty() const {
            return c.empty();
        }

        size_t size() const {
            return c.size();
        }

        const T& top() const {
            return c.front(); //现在返回的是一个数字 
        }

        void push(const T& x) {
            c.push_back(x);
            AdjustUp(c.size() - 1);
        }

        void pop() {
            swap(c[0], c[c.size() - 1]);
            c.pop_back();
            AdjustDown(0);
        }
        void AdjustUp(int child)
        {
            int parent = (child - 1) / 2;
            while (child > 0) //当子节点调整到根结点时 就不需要再向上调整了
            {
                if (comp(c[parent],c[child]))
                {
                    swap(c[child],c[parent]);
                    child = parent;
                    parent = (child - 1) / 2;
                }
                else
                {
                    break;
                }
            }
        }
        void AdjustDown(int parent) {  //n表示堆里面的元素个数
            //int parent = 0;
            //假设左孩子比较小 与右孩子相比
            int child = parent * 2 + 1;
            while (child < c.size()) {  //child>=n 孩子不存在 跳出循环
                if (child + 1 < c.size()&& comp(c[child],c[child+1])) {
                    //若右孩子比较小 则
                    child++;
                }
                if (comp(c[parent], c[child])) {
                    //孩子比父亲小 则交换数值
                    swap(c[child], c[parent]);
                    //继续往下
                    parent = child;
                    child = parent * 2 + 1;
                }
                else {
                    break;
                }
            }

        }
    private:
        Container c;  //适配器
        Compare comp; //比较类仿函数
    };
};