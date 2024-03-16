#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#include <iostream>
#include <iomanip>

#include "btree_array_funcs.h"
#include "assert.h"

using namespace std;

template <class T>
class BPlusTree
{
public:
    class Iterator{
    public:
        friend class BPlusTree;
        Iterator (BPlusTree<T>* _it = NULL, int _key_ptr = 0): it(_it), key_ptr(_key_ptr)
        {}

        T operator *()
        {
            return it->data[key_ptr];
        }

        const T operator *() const
        {
            return it->data[key_ptr];
        }

        Iterator operator++(int un_used) // post increment
        {
            Iterator hold = Iterator(it, key_ptr);
            key_ptr++;
            if (key_ptr >= it->data_count)
            {
                it = it->next;
                key_ptr = 0;
            }
            return hold;
        }

        Iterator operator++() // pre increment 
        {
            key_ptr++;
            if (key_ptr >= it->data_count)
            {
                it = it->next;
                key_ptr = 0;
            }
            return *this;
        }

        friend bool operator ==(const Iterator& lhs, const Iterator& rhs)
        {
            return (lhs.it == rhs.it) && (lhs.key_ptr == rhs.key_ptr);
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs)
        {
            return !(lhs.it == rhs.it);
        }

        void print_Iterator()
        {
            if (it)
            {
                cout<<"iterator: ";
                print_array(it->data, it->data_count, key_ptr);
            }
            else
            {
                cout<<"iterator: NULL, key_ptr: "<<key_ptr<<endl;
            }
        }

        bool is_null()
        {
            return !it;
        }

        void info()
        {
            cout << endl << "Iterator info:"<<endl;
            cout << "key_ptr: "<<key_ptr<<endl;
            cout << "it: "<<*it<<endl;
        }

    private:
        BPlusTree<T>* it;
        int key_ptr;
    };

    // constructors:
    BPlusTree(bool dups = false);
    BPlusTree(T *a, int size, bool dups = false);

    // copy constructor:
    BPlusTree(const BPlusTree<T>& other);

    // destructor:
    ~BPlusTree();

    // assignment operator:
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS);

    // copy other into *this
    void copy_tree(const BPlusTree<T>& other);  
    void copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& last_node);

    // insert entry into the tree
    void insert(const T& entry);  

    // remove entry from the tree
    void remove(const T& entry);   

    // clear this object (delete all nodes etc.)
    void clear_tree();             

    // true if entry can be found
    bool contains(const T& entry);
    bool contains(const T& entry) const; 

    // return a reference to entry
    T& get(const T& entry);             

    // return a reference to entry
    const T& get(const T& entry) const;

    // return a reference to entry
    T& get_existing(const T& entry);     

    // return an iterator to this key (NULL if not there)
    Iterator find(const T& key)
    {
        if (!contains(key))
        {
            return Iterator(nullptr); 
        }
        T* found = find_ptr(key);
        BPlusTree<T>* temp = new BPlusTree<T>;
        temp->insert(*found);

        return Iterator(temp);
    } 

    // return an iterator to this key (NULL if not there)
    Iterator find_btree(const T& key)
    {
        if (!contains(key))
        {
            return Iterator(nullptr); 
        }
        T* found = find_ptr_btree(key);
        BPlusTree<T>* temp = new BPlusTree<T>;
        temp->insert(*found);

        return Iterator(temp);
    } 
                                        
    //return first that goes NOT BEFORE
    // key entry or next if does not
    // exist: >= entry                                   
    Iterator lower_bound(const T& key)
    {
        /*
        - while loops
        - it starts at beginning and keep iterating until it finds the first value that is greater or equal to given key
        - will return the iterator with begin
        */
        Iterator greycat = begin();
        while (greycat != end()) // dereferencing iterator gives u the value
        {
            if (*greycat >= key)
            {
                return greycat;
            }
            greycat++;
        }
        return end();
    }

    //return first that goes AFTER key
    //exist or not, the next entry  >entry
    Iterator upper_bound(const T& key)
    {
        Iterator greycat = begin();
        while (greycat != end()) // dereferencing iterator gives u the value
        {
            if (*greycat > key)
            {
                return greycat;
            }
            greycat++;
        }
        return end();
    }

    //count the number of elements
    int size() const;                    

    //true if the tree is empty
    bool empty() const;                  

    // print tree
    void print_tree(int level = 0, ostream &outs = cout) const;

    // cout operator
    friend ostream& operator<<(ostream& outs, const BPlusTree<T>& print_me)
    {
        print_me.print_tree(0, outs);
        return outs;
    }

    bool is_valid();

    string in_order();

    string pre_order();

    string post_order();

    Iterator begin() // similar to getsmallestnode()
    {
        return Iterator(get_smallest_node());
    }

    Iterator end()
    {
        return Iterator(nullptr);
    }
    
    ostream& list_keys(Iterator from = NULL, Iterator to = NULL) // implementation was given
    {
        if (from == NULL) from = begin();
        if (to == NULL) to = end();
        for (Iterator it = from; it != to; it++)
            cout<<*it<<" ";
        return cout;
    }

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    //true if duplicate keys are allowed
    bool dups_ok;                        

    int data_count;                    

    T data[MAXIMUM + 1];  
               
    int child_count;                    

    BPlusTree* subset[MAXIMUM + 2];     

    BPlusTree* next;

    bool is_leaf() const 
    {
        return child_count == 0;
    }             

    // return a pointer to this key (NULL if not there)
    T* find_ptr_btree(const T& entry);     
    T* find_ptr(const T& entry);     
    const T* find_ptr(const T& entry) const;

    // insert element functions
    // allows MAXIMUM + 1 data elements in the root
    void loose_insert(const T& entry);   
    //fix excess in child i
    void fix_excess(int i);              

    // remove element functions:
    // allows MINIMUM - 1 data elements in the root
    void loose_remove(const T& entry);  

    // fix shortage in child i
    BPlusTree<T>* fix_shortage(int i);  

    // return the smallest key in this suBPlusTree
    BPlusTree<T>* get_smallest_node();

    // entry := leftmost leaf
    void get_smallest(T& entry);      

    // entry := rightmost leaf
    void get_biggest(T& entry);       

    // remove the biggest child of tree->entry
    void remove_biggest(T& entry);   

    // transfer one element LEFT from child i
    void transfer_left(int i);        

    // transfer one element RIGHT from child i
    void transfer_right(int i);      

    // merge subset i with  i+1
    BPlusTree<T> *merge_with_next_subset(int i); 

};

//------------------------------------------------------------------------------------------------
//   ⊹˚. ♡.𖥔 ݁ ˖  ♡⸜(˃ ᵕ ˂ )⸝   B+ TREE PUBLIC FUNCTIONS    ⊹˚. ♡.𖥔 ݁ ˖  ≽^•⩊•^≼
//------------------------------------------------------------------------------------------------

    // constructor:
    template <typename T>
    BPlusTree<T>::BPlusTree(bool dups) : dups_ok(dups), data_count(0), child_count(0), next(NULL)
    { 
        T temp;
        data[0] = temp;
    }

    // constructor from array:
    template <typename T>
    BPlusTree<T>::BPlusTree(T *a, int size, bool dups) : dups_ok(dups), data_count(0), child_count(0), next(NULL)
    {
        for (int i = 0; i < size; i++)
        {
            insert(a[i]);
        }
    }

    // copy constructor:
    template <typename T>
    BPlusTree<T>::BPlusTree(const BPlusTree<T>& other): data_count(0), child_count(0)
    { 
        copy_tree(other);
    }

    // destructor:
    template <typename T>
    BPlusTree<T>::~BPlusTree()
    {
        const bool debug = false;
        if (debug) cout<<"** destructor fired!"<<endl;
        clear_tree();
    }

    // assignment operator:
    template <typename T>
    BPlusTree<T>& BPlusTree<T>::operator =(const BPlusTree<T>& RHS)
    {
        const bool debug = false;
        if (debug) cout<<"** assignment operator fired!"<<endl;
        copy_tree(RHS);
        return *this; 
    }

    // copy other into *this
    template <typename T>
    void BPlusTree<T>::copy_tree(const BPlusTree<T>& other)
    {
        clear_tree();
        BPlusTree<T> *lastnode = nullptr; 
        copy_tree(other, lastnode);
    }

    template <typename T>
    void BPlusTree<T>::copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& last_node)
    {
        clear_tree();
        copy_array(data, other.data, data_count, other.data_count);
        if (!other.is_leaf())
        {
            // deep copy the pointer values:
            for (int i = 0; i < other.child_count; i++)
            {
                subset[i] = new BPlusTree<T>;
                subset[i]->copy_tree(*(other.subset[i]), last_node);
            }
            child_count = other.child_count;
        }
        // base case (leaf):
        else 
        {
            if (last_node)
            {
                last_node->next = this;
                last_node = this;
            }
            else 
            {
                last_node = this;
            }
        }
    }

    // insert entry into the tree
    template <typename T>
    void BPlusTree<T>::insert(const T& entry)
    {
            loose_insert(entry);
            if (data_count > MAXIMUM) // excess in root
            {
                /*
                    1. create a new BPlusTree object (dynamic)
                    2. shallow copy the my data array into this BPlusTree's data
                    3. shallow copy my subset array into this BPlusTree's subset array.
                    4. set my dc = 0; set my cc = 1; and point my subset[0] to this new BPlusTree.
                    5. call fix_excess(0)
                */
                BPlusTree<T>* tree = new BPlusTree<T>;
        
                copy_array(tree->data, data, tree->data_count, data_count);
                copy_array(tree->subset, subset, tree->child_count, child_count);

                data_count = 0;
                child_count = 1;
                subset[0] = tree; 
                fix_excess(0);
            }
        }     

    // remove entry from the tree
    template <typename T>
    void BPlusTree<T>::remove(const T& entry)
    {
        // in case the entry is not in the tree:
        if (!contains(entry)) 
        {
            cout << "the value that you want to delete isn't even here!!!" << endl;
            return;
        }

        loose_remove(entry);
        if (data_count < MINIMUM && child_count > 0) // shortage in root
        {
            cout << "shortage in root (dc, cc): " << data_count << " & " << child_count << endl;

            BPlusTree<T>* shrinkptr = subset[0]; 

            copy_array(data, shrinkptr->data, data_count, shrinkptr->data_count);
            copy_array(subset, shrinkptr->subset, child_count, shrinkptr->child_count);

            shrinkptr->data_count = 0;
            shrinkptr->child_count = 0;
            delete shrinkptr;
            shrinkptr = nullptr;
        }
    }           

    // clear this object (delete all nodes etc.)
    template <typename T>
    void BPlusTree<T>::clear_tree()
    {
        data_count = 0;
        for (int i = 0; i < child_count; i++) 
        {
            subset[i]->clear_tree(); 
            delete subset[i];
            subset[i] = nullptr;
        }
        child_count = 0;
    }           

    template <typename T>
    bool BPlusTree<T>::contains(const T& entry)
    {
        // return find_ptr(entry);
        int index = first_ge(data, data_count, entry);

        if (index < data_count && data[index] == entry) 
        {
            // cout << "entry: " << entry << " & curr data: " << data[index] << endl;
            return true;
        }
        else if (index <= data_count && !is_leaf()) 
        {
            // cout << "entry: " << entry << " & curr data: " << data[index] << endl;
            return subset[index]->contains(entry);
        }
        return false;
    }

    // true if entry can be found
    template <typename T>
    bool BPlusTree<T>::contains(const T& entry) const // copy pasted find_ptr
    {
        int index = first_ge(data, data_count, entry);

        if (index < data_count && data[index] == entry) 
        {
            // cout << "entry: " << entry << " & curr data: " << data[index] << endl;
            return true;
        }
        else if (index <= data_count && !is_leaf() && data[index] != entry) 
        {
            return subset[index]->contains(entry);
        }
        return false;
    }

    // return a reference to entry
    template <typename T>
    T& BPlusTree<T>::get(const T& entry) 
    {
        // cout << "in get()" << endl;
        //If entry is not in the tree, add it to the tree
        if (!contains(entry))
        {
            cout << "get() does not contain " << entry << ", not inserting new one" << endl;
            insert(entry);
        }
        // cout << "get() contains " << endl;
        // return *(find_ptr(entry));
        return get_existing(entry);
    }             

    // return a reference to entry
    template <typename T>
    const T& BPlusTree<T>::get(const T& entry) const // copy pasted find_ptr
    {
        cout << "in get() const" << endl;        
        cout << "entry: " << entry << endl;
        assert(!contains(entry));
        return *find_ptr(entry);
    }

    // return a reference to entry
    template <typename T>
    T& BPlusTree<T>::get_existing(const T& entry) 
    {
        //assert that entry is not in the tree.
        //assert(contains(entry));

        int i = first_ge(data, data_count, entry);

        // cout << "index: " << i  << " dc: " << data_count << endl;
        bool found = (i<data_count && data[i]==entry);
        if (is_leaf())
        {
            if (found)
            {
                return data[i];
            }
            else
            {
                cout<<"get_existing was called with nonexistent entry: "<< entry << endl;
                // assert(found);
            }
        }
        if (found) //inner node
            return subset[i+1]->get_existing(entry);
            //or just return true?
        else //not found yet...
            return subset[i]->get_existing(entry);
    }   

    //count the number of elements
    template <typename T>
    int BPlusTree<T>::size() const
    {
        int size = 0;
        if (is_leaf())
        {
            for (int i = 0; i < data_count; i++) 
            {
                size++;
            }
        }
        else 
        {
            size += subset[0]->size();
            for (int i = 0; i < data_count; i++) 
            {
                size += subset[i + 1]->size();
            }
        }
        return size;
    }                          

    //true if the tree is empty
    template <typename T>
    bool BPlusTree<T>::empty() const
    {
        return data_count == 0 && child_count == 0; 
    }               

    // print tree
    template <typename T>
    void BPlusTree<T>::print_tree(int level, ostream &outs) const
    {
        // base case: (printing)
        if (is_leaf()) 
        {
            for (int i = data_count - 1; i >= 0; i--) 
            {
                cout << data[i] << " " << endl;
            }
            return;
        }

        // recursion:
        subset[child_count - 1]->print_tree(level + 1, outs);

        // print the parent once you wake up in the parent's house
        for (int i = data_count - 1; i >= 0; i--) 
        {
            subset[i]->print_tree(level + 1, outs);
        }
    }

    template <typename T>
    bool BPlusTree<T>::is_valid()
    {
        // base case:
        if (is_leaf()) 
        {
            // do the leaves have excess / shortage?
            if (child_count != data_count + 1 || data_count < MINIMUM || data_count > MAXIMUM) 
            {
                return true;
            }
            else 
            {
                return false;
            }
        } 
        else 
        {
            if (child_count != data_count + 1 || data_count < MINIMUM || data_count > MAXIMUM) 
            {
                // cout << "not valid" << endl;
                return false;
            }
            for (int i = 0; i < child_count; i++) 
            {
                // recursion:
                if (!subset[i]->is_valid()) 
                {
                    return false;
                }
            }
            return true; // true if it got down this far
        }
    }

    template <typename T>
    string BPlusTree<T>::in_order()
    {
        string S = "";
        if (is_leaf())
        {
            for (int i = 0; i < data_count; i++) 
            {
                S += to_string(data[i]);
                S += '|';
            }
        }
        else 
        {
            for (int i = 0; i < data_count; i++) 
            {
                S += subset[i]->in_order();
                S += to_string(data[i]);
                S += '|';
            }
            S += subset[data_count]->in_order();
        }
        return S;
    }

    template <typename T>
    string BPlusTree<T>::pre_order()
    {
        string S = "";
        if (is_leaf())
        {
            for (int i = 0; i < data_count; i++) 
            {
                S += to_string(data[i]);
                S += '|';
            }
        }
        else 
        {
            for (int i = 0; i < data_count; i++) 
            {
                S += to_string(data[i]);
                S += '|';
                S += subset[i]->pre_order();
            }
            S += subset[data_count]->pre_order();
        }
        return S;
    }

    template <typename T>
    string BPlusTree<T>::post_order()
    {
        string S = "";
        if (is_leaf())
        {
            for (int i = 0; i < data_count; i++) 
            {
                S += to_string(data[i]);
                S += '|';
            }
        }
        else 
        {
            S += subset[0]->post_order();
            for (int i = 0; i < data_count; i++) 
            {
                // S += subset[i]->post_order();
                S += subset[i + 1]->post_order();
                S += to_string(data[i]);
                S += '|';
            }
        }
        return S;
    }

//------------------------------------------------------------------------------------------------
//   ⊹˚. ♡.𖥔 ݁ ˖  ♡⸜(˃ ᵕ ˂ )⸝   B+ TREE PRIVATE FUNCTIONS    ⊹˚. ♡.𖥔 ݁ ˖  ≽^•⩊•^≼
//------------------------------------------------------------------------------------------------

    // return a pointer to this key (NULL if not there)
    template <typename T>
    T* BPlusTree<T>::find_ptr_btree(const T& entry)
    {
        /* summary:
        - use first_ge(data, data_count, entry); that index will be the index that we go to next
        - short circuit if index reaches size 
        */

        int index = first_ge(data, data_count, entry);

        if (index < data_count && data[index] == entry) // found entry :D
        {
            return &data[index];
        }
        else if (index <= data_count && !is_leaf() && data[index] != entry) 
        {
            return subset[index]->find_ptr(entry);
        }
        return nullptr;
    }          

       // return a pointer to this key (NULL if not there)
    template <typename T>
    T* BPlusTree<T>::find_ptr(const T& entry)
    {
        /* summary:
        - use first_ge(data, data_count, entry); that index will be the index that we go to next
        - short circuit if index reaches size 
        */
        // cout << "findptr" << endl;
        // cout << "looking for entry: " << entry << endl; 
        int index = first_ge(data, data_count, entry);
        // cout << "index: " << index << ", dc: " << data_count << endl;
        if (is_leaf() && (index < data_count) && (data[index] == entry)) // found entry :D
        {
            // cout << "found entry: " << data[index] << endl;
            return &data[index];
        }
        else if (index <= data_count) 
        {
            return subset[index]->find_ptr(entry);
        }
        return nullptr;
    }              

    template <typename T>
    const T* BPlusTree<T>::find_ptr(const T& entry) const
    {
        // this is the const version of find_ptr so that i can call it in get() const >:(
        // cout << "const findptr" << endl;
        int index = first_ge(data, data_count, entry);

        if (index < data_count && data[index] == entry) // found entry :D
        {
            return &data[index];
        }
        else if (index <= data_count && !is_leaf() && data[index] != entry) 
        {
            return subset[index]->find_ptr(entry);
        }
        return nullptr;
    }               

    // insert element functions
    // allows MAXIMUM + 1 data elements in the root
    template <typename T>
    void BPlusTree<T>::loose_insert(const T& entry)
    {
        /* summary:
        - only insert in the leaves
        */
        int index = first_ge(data, data_count, entry);

        if (index < data_count && index >= 0 && data[index] == entry)
        {
            if (is_leaf()) // its a leaf
            {
                if (dups_ok) 
                {
                    insert_item(data, index, data_count, entry);
                    data[index] = data[index] + entry;
                }
                else 
                {
                    data[index] = data[index] + entry;
                    return;
                }
            }
            else // it's an inner node
            {
                subset[index + 1]->loose_insert(entry);
                fix_excess(index + 1);
            }
        }
        else if (index <= data_count && index >= 0) // not found
        {
            if (is_leaf()) // its a leaf
            {
                // inserting here:
                insert_item(data, index, data_count, entry);
            }
            else // it's an inner node
            {
                subset[index]->loose_insert(entry);
                fix_excess(index);
            }
        }
    }  

    //fix excess in child i
    template <typename T>
    void BPlusTree<T>::fix_excess(int i)
    {
        if (subset[i]->data_count <= MAXIMUM) 
        {
            // cout << data[i] << "'s children are ok!" << endl;
            return;
        }
    
        T middlevalue;

        BPlusTree<T>* tree = new BPlusTree<T>;
        insert_item(subset, i + 1, child_count, tree); // add to the pointer array and shifting the pointers
        split(subset[i]->data, subset[i]->data_count, tree->data, tree->data_count); // move half of the values in subset[i] -> tree
        split(subset[i]->subset, subset[i]->child_count, tree->subset, tree->child_count); // move half of the pointers too
        detach_item(subset[i]->data, subset[i]->data_count, middlevalue);
        int index = first_ge(data, data_count, middlevalue);
        insert_item(data, index, data_count, middlevalue);
    
        if (subset[i]->is_leaf())
        {
            insert_item(tree->data, 0, tree->data_count, middlevalue);
            tree->next = subset[i]->next;
            subset[i]->next = tree;
        }
    }          

    // remove element functions:
    // allows MINIMUM - 1 data elements in the root
    template <typename T>
    void BPlusTree<T>::loose_remove(const T& entry)
    {
        int index = first_ge(data, data_count, entry);

        // found the node:
        if (index < data_count && data[index] == entry) 
        {
            if (is_leaf()) // leaf node (easy!)
            {
                T dummyvariable;
                delete_item(data, index, data_count, dummyvariable);
            }
            else // inner node (replace!)
            {
                T max;
                subset[index]->remove_biggest(max); 
                data[index] = max;
                fix_shortage(index);
            }
        }
        // still finding the node (make sure it's not a leaf)
        if (!is_leaf()) 
        {
            subset[index]->loose_remove(entry); // recursion
            fix_shortage(index);
        }
    }  

    // fix shortage in child i
    template <typename T>
    BPlusTree<T>* BPlusTree<T>::fix_shortage(int i)
    {
        if (subset[i]->data_count >= MINIMUM) 
        {
            // cout << data[i] << " has enough children: " << subset[i]->data_count << endl;
            return nullptr;
        }
        // cout << data[i] << " has NO CHILDREN! SHE NEEDS HELP!!!" << subset[i]->data_count << endl;

        // CASE 1: borrow from left sibling
        if (i > 0 && (subset[i - 1]->data_count > MINIMUM)) 
        {
            cout << "borrowing from left sibling!" << endl;
            // cout << "i - 1: " << i - 1 << endl;
            // rotate_right(i - 1);

        }
        // CASE 2: borrow from right sibling
        else if (i < data_count && (subset[i + 1]->data_count > MINIMUM)) 
        {
            cout << "borrowing from right sibling!" << endl;
            // rotate_left(i + 1);
        }
        // CASE 3: merge with left sibling
        else if (i > 0) 
        {
            cout << "merge with left sibling!" << endl; 
            merge_with_next_subset(i - 1);
        }
        // CASE 4: merge with right sibling
        else 
        {
            cout << "merge with right sibling!" << endl;
            merge_with_next_subset(i);
        }
        BPlusTree<T>* stub = new BPlusTree<T>;
        return stub;
    } 

    // return the smallest key in this suBPlusTree
    template <typename T>
    BPlusTree<T>* BPlusTree<T>::get_smallest_node()
    {
        if (is_leaf())
        {
            return this;
        }
        else
        {
            return subset[0]->get_smallest_node();
        }
    }

    // entry := leftmost leaf
    template <typename T>
    void BPlusTree<T>::get_smallest(T& entry)
    {
        get_smallest_node();
    }    

    // entry := rightmost leaf
    template <typename T>
    void BPlusTree<T>::get_biggest(T& entry)
    {
        return; // stub
    }      

    // remove the biggest child of tree->entry
    template <typename T>
    void BPlusTree<T>::remove_biggest(T& entry)
    {
        if (is_leaf()) 
        {
            detach_item(data, data_count, entry);
        }
        else 
        {
            subset[data_count]->remove_biggest(entry);
        }
    }   
    

    // transfer one element LEFT from child i
    template <typename T>
    void BPlusTree<T>::transfer_left(int i)
    {
        return; // stub
    }        

    // transfer one element RIGHT from child i
    template <typename T>
    void BPlusTree<T>::transfer_right(int i)
    {
        return; // stub
    }

    // merge subset i with  i+1
    template <typename T>
    BPlusTree<T>* BPlusTree<T>::merge_with_next_subset(int i)
    {
        BPlusTree<T>* stub = new BPlusTree<T>;
        return stub;
    }

#endif