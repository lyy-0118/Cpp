#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

enum State {
    EXIST,
    EMPTY,
    DELETE
};

template <class K,class V>
struct HashData {
    pair<K, V> _kv;
    State _state = EMPTY;
};

template<class K>
struct HashFunc
{
    size_t operator()(const K& key)
    {
        return (size_t)key;
    }
};

template<>  //特化字符串HashFunc
struct HashFunc<string>
{
    size_t operator()(const string& s)
    {
        // BKDR
        size_t hash = 0;
        for (auto ch : s)
        {
			hash += ch;  // 累加字符的ASCII值
            hash *= 131; // 乘以一个质数
        }

        return hash;
    }
};

inline unsigned long __stl_next_prime(unsigned long n)  
{
    // Note: assumes long is at least 32 bits.
    static const int __stl_num_primes = 28;
    static const unsigned long __stl_prime_list[__stl_num_primes] = {
        53, 97, 193, 389, 769,
        1543, 3079, 6151, 12289, 24593,
        49157, 98317, 196613, 393241, 786433,
        1572869, 3145739, 6291469, 12582917, 25165843,
        50331653, 100663319, 201326611, 402653189, 805306457,
        1610612741, 3221225473, 4294967291
    };
    const unsigned long* first = __stl_prime_list;
    const unsigned long* last = __stl_prime_list + __stl_num_primes;
    const unsigned long* pos = lower_bound(first, last, n);
    return pos == last ? *(last - 1) : *pos;
}
namespace open_address
{
    template<class K, class V, class Hash = HashFunc<K>>
    class HashTable
    {
    public:
        HashTable()
            : _tables(__stl_next_prime(0))
            , _n(0)
        {}

        ~HashTable() {}

        bool Insert(const pair<K, V>& kv) {
            // 插入逻辑
            if (Find(kv.first)) return false; // 如果已经存在，则不插入

            // 负载因子超过0.7时，扩容
            if (_n * 10 / _tables.size() >= 7) {
                // 扩容逻辑 这⾥利⽤类似深拷⻉现代写法的思想插⼊后交换解决
                HashTable<K, V, Hash> newT;
                newT._tables.resize(__stl_next_prime(_tables.size() + 1));
                for (const auto& data : _tables) {
                    if (data._state == EXIST) {
                        newT.Insert(data._kv);
                    }
                }
                _tables.swap(newT._tables);
            }
            //简单的线性探测
            /*size_t hash0 = kv.first % _tables.size();
            size_t hashi = hash0;
            size_t i = 1;*/
            Hash hash;
            size_t hash0 = hash(kv.first) % _tables.size();
            size_t hashi = hash0;
            size_t i = 1;
            while (_tables[hashi]._state == EXIST) {
                //冲突，继续探测 直到找到空位
                hashi = (hash0 + i) % _tables.size();
                ++i;
                //如果探测回到原点，说明找了一圈，没找到
                if (hashi == hash0) {
                    // 理论上因为有负载因子控制，不应该到达这里
                    // 但作为健壮性代码是好的
                    return false;
                }
            }
            _tables[hashi]._kv = kv;
            _tables[hashi]._state = EXIST;
            ++_n;
            return true;
        }

        HashData<K, V>* Find(const K& key) {
            // 查找逻辑
            /*size_t hash0 = key % _tables.size();
            size_t hashi = hash0;
            size_t i = 1;*/
            Hash hash;
            size_t hash0 = hash(key) % _tables.size();
            size_t hashi = hash0;
            size_t i = 1;
            while (_tables[hashi]._state != EMPTY) {
                if (_tables[hashi]._state == EXIST && _tables[hashi]._kv.first == key) {
                    return &_tables[hashi];
                }
                //冲突，继续探测
                hashi = (hash0 + i) % _tables.size();
                ++i;

                // 关键修正: 如果探测回到起点，说明找了一圈也没找到
                if (hashi == hash0) {
                    break;
                }
            }
            return nullptr;
        }

        bool Erase(const K& key) {
            // 删除逻辑
            HashData<K, V>* data = Find(key);
            if (data) {
                data->_state = DELETE;
                --_n;
                return true;
            }
            else {
                return false; // 如果不存在，则不删除
            }
        }

    private:
        vector<HashData<K, V>> _tables;
        size_t _n = 0;  // 表中存储数据个数
    };
}

namespace hash_bucket
{
    template<class K, class V>
    struct HashNode
    {
        pair<K, V> _kv;
        HashNode<K, V>* _next;

        HashNode(const pair<K, V>& kv)
            :_kv(kv)
            , _next(nullptr)
        { }
    };

    template<class K, class V, class Hash = HashFunc<K>>
    class HashTable
    {
        typedef HashNode<K, V> Node;
    public:
        HashTable()
            :_tables(__stl_next_prime(0))
            , _n(0)
        { }

        // 拷贝构造和赋值重载也需要

        ~HashTable()
        {
            for (size_t i = 0; i < _tables.size(); i++)
            {
                Node* cur = _tables[i];
                while (cur)
                {
                    Node* next = cur->_next;
                    delete cur;

                    cur = next;
                }

                _tables[i] = nullptr;
            }
        }

        bool Insert(const pair<K, V>& kv)
        {
            if (Find(kv.first))
                return false;

            // 负载因子 == 1时扩容
            if (_n == _tables.size())
            {
                /*HashTable<K, V> newht;
                newht._tables.resize(__stl_next_prime(_tables.size() + 1));
                for (size_t i = 0; i < _tables.size(); i++)
                {
                    Node* cur = _tables[i];
                    while (cur)
                    {
                        newht.Insert(cur->_kv);
                        cur = cur->_next;
                    }
                }

                _tables.swap(newht._tables);*/
                vector<Node*> newTable(__stl_next_prime(_tables.size() + 1));
                for (size_t i = 0; i < _tables.size(); i++)
                {
                    Node* cur = _tables[i];
                    while (cur)
                    {
                        Node* next = cur->_next;
                        // 头插到新表
                        size_t hashi = hash(cur->_kv.first) % newTable.size();
                        cur->_next = newTable[hashi];
                        newTable[hashi] = cur;

                        cur = next;
                    }

                    _tables[i] = nullptr;

                }

                _tables.swap(newTable);
            }
            Hash hash;
            size_t hashi = hash(kv.first) % _tables.size();
            // 头插
            Node* newnode = new Node(kv);
            newnode->_next = _tables[hashi];
            _tables[hashi] = newnode;
            ++_n;

            return true;
        }

        Node* Find(const K& key)
        {
            Hash hash;
            size_t hashi = hash(key) % _tables.size();
            Node* cur = _tables[hashi];
            while (cur)
            {
                if (cur->_kv.first == key)
                {
                    return cur;
                }

                cur = cur->_next;
            }

            return nullptr;
        }

        bool Erase(const K& key)
        {
            size_t hashi = key % _tables.size();
            Node* prev = nullptr;
            Node* cur = _tables[hashi];
            while (cur)
            {
                if (cur->_kv.first == key)
                {
                    if (prev == nullptr)
                    {
                        // 头结点
                        _tables[hashi] = cur->_next;
                    }
                    else
                    {
                        // 中间节点
                        prev->_next = cur->_next;
                    }

                    delete cur;
                    --_n;

                    return true;
                }
                else
                {
                    prev = cur;
                    cur = cur->_next;
                }
            }

            return false;
        }
    private:
        vector<Node*> _tables; // 指针数组
        size_t _n = 0;		   // 表中存储数据个数
    };
}