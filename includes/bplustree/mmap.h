#ifndef MULTIMAP_H
#define MULTIMAP_H
#include <iostream>
#include <iomanip>
#include <vector>
#include "bplustree.h"

using namespace std;

template <typename K, typename V>
struct MPair
{
    K key;
    vector<V> value_list;

    // default constructor: key and no value -> empty vector
    MPair(const K& k = K()) : key(k)
    { 
        // cout << "in mpair ctor, ";
        // cout << "key: " << key << endl;
    }

    // constructor with a key and a value (push value into value_list)
    MPair(const K& k, const V& v) : key(k)
    {
        value_list.push_back(v);
    }

    // constructor with a key and a vector (to replace value_list)
    MPair(const K& k, const vector<V>& vlist) : key(k), value_list(vlist)
    { }

    // operator cout for vector
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me)
    {
        outs << "key: " << print_me.key << " [";
        for (int i = 0; i < print_me.value_list.size(); i++)
        {
            outs << print_me.value_list.at(i) << ' ';
        }
        outs << "]" << endl;
        return outs;
    }
    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key == rhs.key;
    }
    friend bool operator !=(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key != rhs.key;
    }
    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key < rhs.key;
    }
    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key <= rhs.key;
    }
    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key > rhs.key;
    }
    friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key >= rhs.key;
    }
    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs) 
    {
        MPair<K, V> temp(lhs.key, lhs.value_list); // the result 
        for (int i = 0; i < rhs.value_list.size(); i++)
        {
            temp.value_list.push_back(rhs.value_list[i]);
        }
        return temp;
    }
    friend MPair<K, V> operator += (MPair<K, V>& lhs, const MPair<K, V>& rhs) 
    {
        for (int i = 0; i < rhs.value_list.size(); i++)
        {
            // cout << "pushing: " << rhs.value_list[i] << endl;
            lhs.value_list.push_back(rhs.value_list[i]);
        }
        return lhs;
    }
};

template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K, V> > map_base;
    class Iterator{
    public:
        friend class MMap;
        // default iterator constructor:
        Iterator()
        { }

        // the real iterator constructor:
        Iterator(typename map_base::Iterator it) : _it(it)
        { }

        Iterator operator ++(int unused)
        {
            return (_it)++;
        }

        Iterator operator ++()
        {
            return ++(_it);
        }

        MPair<K, V> operator *()
        {
            return *(_it);
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

    // multi map constructor:
    MMap() 
    {
        // cout << "in default mmap constructor" << endl;
    }

//  Iterators
    Iterator begin()
    {
        return mmap.begin();
    }
    Iterator end()
    {
        return mmap.end();
    }
    Iterator lower_bound(K i)
    {
        return mmap.lower_bound(i);
    }
    Iterator upper_bound(K i)
    {
        return mmap.upper_bound(i);
    }

//  Capacity
    int size() const;
    bool empty() const;

//  Element Access
    const vector<V>& operator[](const K& key) const;
    vector<V>& operator[](const K& key);

//  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();

    bool contains(const K& key) const;
    vector<V> at(const K &i) const;
    vector<V> at(const K& i);

    vector<V>& get(const K& key);

    Iterator find(const K& key)
    {
        Iterator it(mmap.find(key));
        return it;
    }

    int count(const K& key);

    bool is_valid();

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me)
    {
        outs << print_me.mmap << endl;
        return outs;
    }

    void print_lookup(); 

private:
    BPlusTree<MPair<K, V>> mmap;
};

//------------------------------------------------------------------------------------------------
//   ⊹˚. ♡.𖥔 ݁ ˖  ♡⸜(˃ ᵕ ˂ )⸝    MULTI MAP FUNCTIONS    ⊹˚. ♡.𖥔 ݁ ˖  ≽^•⩊•^≼
//------------------------------------------------------------------------------------------------

    template <typename K, typename V>
    int MMap<K,V>::size() const
    {
        return mmap.size();
    }

    template <typename K, typename V>
    bool MMap<K,V>::empty() const
    {
        return mmap.empty();
    }

    template <typename K, typename V>
    const vector<V>& MMap<K,V>::operator[](const K& key) const
    {
        return mmap.get(MPair<K, V>(key)).value_list;
    }

    template <typename K, typename V>
    vector<V>& MMap<K,V>::operator[](const K& key)
    {
        return mmap.get(MPair<K, V>(key)).value_list;
    }

    template <typename K, typename V>
    void MMap<K,V>::insert(const K& k, const V& v)
    {
        mmap.insert(MPair<K, V>(k, v));
    }

    template <typename K, typename V>
    void MMap<K,V>::erase(const K& key)
    {
        mmap.remove(MPair<K, V>(key));
    }

    template <typename K, typename V>
    void MMap<K,V>::clear()
    {
        mmap.clear_tree();
    }

    template <typename K, typename V>
    bool MMap<K,V>::contains(const K& key) const
    {
        return mmap.contains(MPair<K, V>(key));
    }

    template <typename K, typename V>
    vector<V> MMap<K,V>::at(const K &i) const
    {
        MPair<K, V> temp(i);
        return mmap.get(temp).value_list;
    }

    template <typename K, typename V>
    vector<V> MMap<K,V>::at(const K &i) 
    {
        MPair<K, V> temp(i);
        return mmap.get(temp).value_list;
    }

    template <typename K, typename V>
    vector<V>& MMap<K,V>::get(const K& key)
    {
        return mmap.get(MPair<K, V>(key)).value_list;
    }

    template <typename K, typename V>
    int MMap<K,V>::count(const K& key) 
    {
        int count = 0;
        for (typename BPlusTree<MPair<K, V>>::Iterator it = mmap.begin(); it != mmap.end(); ++it) // ew this is so ugly 
        {
            if (it->key == key) 
            {
                count += it->value_list.size();
            }
        }
        return count;
    }

    template <typename K, typename V>
    bool MMap<K,V>::is_valid()
    {
        return mmap.is_valid();
    }

    // this makes the output for parser look like basic test
    template <typename K, typename V>
    void MMap<K,V>::print_lookup()
    {
        // i am making a vector of all of the keys. hard coding it. 
        vector<string> keys = {"cols", "command", "condition", "fields", "table_name", "values", "where"}; 

        for (vector<string>::const_iterator key = keys.begin(); key != keys.end(); ++key) 
        {
            MMap<K, V>::Iterator it = mmap.find_btree(*key); // this will give me the index location by using iterator
            if (it != mmap.end()) 
            {
                cout << *it << endl; // dereference it.
            }
        }
    }

#endif