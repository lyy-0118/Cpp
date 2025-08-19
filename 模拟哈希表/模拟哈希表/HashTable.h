#pragma once
#include <iostream>
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
    State _state;
};

template<class K, class V>
class HashTable
{
public:
    HashTable() :
        _tables(11),
        _n(0) 
    {
        for (size_t i = 0; i < 11; ++i) {
            _tables[i]._state = EMPTY;
        }
    }

    ~HashTable() {}
    bool Insert(const pair<K, V>& kv) {
        // 插入逻辑
		if (Find(kv.first)) return false; // 如果已经存在，则不插入
		// 负载因子超过0.7时，扩容
        if(_n*10/_tables.size() >= 7) {
            // 扩容逻辑 这⾥利⽤类似深拷⻉现代写法的思想插⼊后交换解决
            HashTable<K, V> newT;
			newT._tables.resize(_tables.size() * 2 + 1);
            for (const auto& data : _tables) {
                if (data._state == EXIST) {
                    newT.Insert(data._kv);
                }
            }
			_tables.swap(newT._tables);
		}
        //简单的线性探测
		size_t hash0 = kv.first % _tables.size();
		size_t hashi = hash0;
        size_t i = 1;
        while (_tables[hashi]._state == EXIST) {
            //冲突，继续探测 直到找到空位
            hashi = (hash0 + i) % _tables.size();
            ++i;
        }
        _tables[hashi]._kv = kv;
        _tables[hashi]._state = EXIST;
        ++_n;
        return true;
    }
    bool Remove(const K& key) {
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
    HashData<K,V>* Find(const K& key){
        // 查找逻辑
        size_t hash0 = key % _tables.size();
        size_t hashi = hash0;
        size_t i = 1;
        while(_tables[hashi]._state != EMPTY) {
            if (_tables[hashi]._state == EXIST && _tables[hashi]._kv.first == key) {
                return &_tables[hashi];
            }
			//冲突，继续探测
            hashi = (hash0 + i) % _tables.size();
            ++i;
		}
        return nullptr;
	}

private:
    vector<HashData<K, V>> _tables;
    size_t _n = 0;  // 表中存储数据个数
};