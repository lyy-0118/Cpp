#include<iostream>
#include<vector>
#include<map>
#include<queue>
using namespace std;
class Solution {
public:
    //实现一个比较逻辑
    struct Com {
        //重载运算符()
        bool operator()(const pair<string, int>& a, const pair<string, int>& b) {
            // 按照次数比较，如果次数相同，再按照单词字典序比较
            if (a.second != b.second) {
                return a.second > b.second; //次数多
            }
            else {
                return a.first < b.first;   //次数相等 字符串小
            }
        }
    };
    vector<string> topKFrequent(vector<string>& words, int k) {
        //top-k问题
        map<string, int> hash;
        //将字符串及其出现的次数存储到Map里
        for (auto word : words) {
            hash[word]++;
        }
        // 采用优先级队列，找出现次数最多的前K个IP地址
        priority_queue<pair<string, int>, vector<pair<string, int>>, Com> q;
        for (auto e : hash) {
            q.push(e); //将hash存储的键值对push进优先级队列
            if (q.size() > k) {
                q.pop(); //大于k个移除堆顶最小元素
            }
        }
        // 将优先级队列中元素（键值对）中的字符串部分放置到vector中
        vector<string> ret;
        while (!q.empty())
        {
            ret.push_back(q.top().first);
            q.pop();
        }
        // 因为是根据次数创建的小堆，因此需要逆置
        reverse(ret.begin(), ret.end());
        return ret;
    }
};
int main(){

	return 0;
}