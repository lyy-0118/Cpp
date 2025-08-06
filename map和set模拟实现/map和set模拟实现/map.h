#pragma once
#include"RBT.h"
namespace lyy {
	template<class K,class V>
	class map {
		struct MapKeyofV {
			const K& operator()(const pair<K, V>& kv)
			{
				return kv.first;
			}
		};
	public:
		typedef typename RBTree<K, pair<const K, V>, MapKeyofV>::Iterator iterator;
		typedef typename RBTree<K, pair<const K, V>, MapKeyofV>::ConstIterator const_iterator;
		iterator begin()
		{
			return _t.Begin();
		}
		iterator end()
		{
			return _t.End();
		}
		const_iterator begin() const
		{
			return _t.Begin();
		}
		const_iterator end()  const
		{
			return _t.End();
		}
		pair<iterator, bool> insert(const pair<K, V>& kv)
		{
			return _t.Insert(kv);
		}
		V& operator[](const K& key) {
			pair<iterator, bool> ret = insert({ key, V() }); //充当查找功能
			return ret.first->second; 
			//       iterator   V
		}
	private:
		RBTree<K, pair<const K, V>, MapKeyofV> _t;
	};
}