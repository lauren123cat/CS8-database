#ifndef MAP_H
#define MAP_H
#include <iostream>
#include <iomanip>

#include "bplustree.h"
#include "btree_array_funcs.h"

using namespace std;

template <typename K, typename V>
struct Pair
{
    K key;
    V value;

    Pair(const K& k = K(), const V& v = V()) : key(k), value(v)
    { }

    friend ostream& operator <<(ostream& outs, const Pair<K, V>& print_me)
    {
        //outs << print_me.key << ", " << print_me.value << endl;
        outs << print_me.key;
        outs << ", ";
        outs << print_me.value;
        return outs;
    }
    friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return (lhs.key == rhs.key);
    }
    friend bool operator !=(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return (lhs.key != rhs.key);
    }
    friend bool operator < (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return (lhs.key < rhs.key);
    }
    friend bool operator > (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return (lhs.key > rhs.key);
    }
    friend bool operator <= (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return (lhs.key <= rhs.key);
    }
    friend bool operator >= (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return (lhs.key >= rhs.key);
    }
    friend Pair<K, V> operator + (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        // Pair<K, V> temp = lhs;
        // temp.key += rhs.key;
        // return temp;
        Pair<K, V> temp = rhs;
        return temp;
    }
};

template <typename K, typename V>
class Map
{
public:
    typedef BPlusTree<Pair<K, V>> map_base; // map_base -> BPlusTree<Pair<K, V> >
    class Iterator
    {
    public:
        friend class Map;

        // default constructor:
        Iterator()
        {}

        // constructor:
        Iterator(typename map_base::Iterator it) : _it(it)
        {}

        Iterator operator ++(int unused)
        {
            return _it++;
        }
        Iterator operator ++()
        {
            return ++_it;
        }
        Pair<K, V> operator *()
        {
            return *_it;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs)
        {
            return lhs._it == rhs._it;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs)
        {
            return lhs._it != rhs._it;
        }
    private:
        typename map_base::Iterator _it;
    };

//  constructor:
    Map() : key_count(0)
    {}

//  Iterators
    Iterator begin()
    {
        return map.begin();
    }

    Iterator end()
    {
        return map.end();
    }

    Iterator lower_bound(K i) 
    {
        return map.lower_bound(i);
    }

    Iterator upper_bound(K i)
    {
        return map.upper_bound(i);
    }

//  Capacity
    int size() const;
    bool empty() const;

//  Element Access
    V& operator[](const K& key);
    V& at(const K& key);
    const V& at(const K& key) const;


//  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();
    V get(const K& key);

//  Operations:
    Iterator find(const K& key)
    {
        Pair <K, V> pair(key); // construct a pair
        return Iterator(map.find(pair)); // call btree find
    }

    bool contains(const Pair<K, V>& target) const;

    bool is_valid();

    friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me)
    {
        outs << print_me.map << endl;
        return outs;
    }
private:
    int key_count;
    BPlusTree<Pair<K, V>> map;
};

//------------------------------------------------------------------------------------------------
//   ⊹˚. ♡.𖥔 ݁ ˖  ♡⸜(˃ ᵕ ˂ )⸝   MAP FUNCTIONS    ⊹˚. ♡.𖥔 ݁ ˖  ≽^•⩊•^≼
//------------------------------------------------------------------------------------------------

    template <typename K, typename V>
    int Map<K,V>::size() const
    {
        return map.size();
    }

    template <typename K, typename V>
    bool Map<K,V>::empty() const
    {
        return map.size() == 0;
    }

    template <typename K, typename V>
    V& Map<K,V>::operator[](const K& key)
    {
        return map.get(Pair<K, V>(key)).value;
    }

    template <typename K, typename V>
    V& Map<K,V>::at(const K& key)
    {
        return map.get(Pair<K, V>(key)).value;
    }

    template <typename K, typename V>
    const V& Map<K,V>::at(const K& key) const
    {
        return map.get(Pair<K, V>(key)).value;
    }

    // insert key:
    template <typename K, typename V>
    void Map<K,V>::insert(const K& k, const V& v)
    {
        map.insert(Pair<K, V>(k, v));
    }

    // delete key:
    template <typename K, typename V>
    void Map<K,V>::erase(const K& key)
    {
        map.remove(Pair<K, V>(key));
    }

    // clear map
    template <typename K, typename V>
    void Map<K,V>::clear()
    {
        map.clear_tree();
    }

    template <typename K, typename V>
    V Map<K,V>::get(const K& key)
    {
        return map.get(Pair<K, V>(key)).value;
    }

    template <typename K, typename V>
    bool Map<K,V>::contains(const Pair<K, V>& target) const
    {
        return map.contains(target);
    }

    template <typename K, typename V>
    bool Map<K,V>::is_valid()
    {
        return map.is_valid();
    }

#endif

/* testing:
- array of 10,000 elements
- insert random elements from 100-9999 to array and btree
- shuffle array (mixes up array)
- remove based on the array shuffle
*/