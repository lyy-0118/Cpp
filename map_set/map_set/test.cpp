// set
#ifndef __SGI_STL_INTERNAL_TREE_H
#include <stl_tree.h>
#endif
#include <stl_set.h>
#include <stl_multiset.h>

// map
#ifndef __SGI_STL_INTERNAL_TREE_H
#include <stl_tree.h>
#endif
#include <stl_map.h>
#include <stl_multimap.h>

// stl_set.h
template <class Key, class Compare = less<Key>, class Alloc = alloc>
class set {
public:
    // typedefs:
    typedef Key key_type;
    typedef Key value_type;

private:
    typedef rb_tree<key_type, value_type,
        identity<value_type>, Compare, Alloc> rep_type;
    rep_type t;  // red-black tree representing set
};

// stl_map.h
template <class Key, class T, class Compare = less<Key>, class Alloc = alloc>
class map {
public:
    // typedefs:
    typedef Key key_type;
    typedef T mapped_type;
    typedef pair<const Key, T> value_type;

private:
    typedef rb_tree<key_type, value_type,
        select1st<value_type>, Compare, Alloc> rep_type;
    rep_type t;  // red-black tree representing map
};

// stl_tree.h
struct __rb_tree_node_base
{
    typedef __rb_tree_color_type color_type;
    typedef __rb_tree_node_base* base_ptr;
    color_type color;
    base_ptr parent;
    base_ptr left;
    base_ptr right;
};

// stl_tree.h
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc = alloc>
class rb_tree {
protected:
    typedef void* void_pointer;
    typedef __rb_tree_node_base* base_ptr;
    typedef __rb_tree_node<Value> rb_tree_node;
    typedef rb_tree_node* link_type;
    typedef Key key_type;
    typedef Value value_type;

public:
    // insert
    pair<iterator, bool> insert_unique(const value_type& x);

    // erase and find
    size_type erase(const key_type& x);
    iterator find(const key_type& x);

protected:
    size_type node_count; // keeps track of size of tree
    link_type header;
};

template <class Value>
struct __rb_tree_node : public __rb_tree_node_base
{
    typedef __rb_tree_node<Value>* link_type;
    Value value_field;
};