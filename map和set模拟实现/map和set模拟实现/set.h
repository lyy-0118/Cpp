#pragma once
#include"RBT.h"
namespace lyy {
	template<class K>
	class set {
		struct SetKeyofT {
			const K& operator()(const K& key) {
				return key;
			}
		};
	public:
		typedef typename RBTree<K, const K, SetKeyofT>::Iterator iterator;
		typedef typename RBTree<K, const K, SetKeyofT>::ConstIterator const_iterator;
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
		pair<iterator, bool> insert(const K& key)
		{
			return _t.Insert(key);
		}
	private:
		RBTree<K, const K, SetKeyofT> _t;
	};
}