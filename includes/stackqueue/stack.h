#ifndef MYSTACK_H
#define MYSTACK_H

#include "node.h"
#include "linked_list_functions.h"

using namespace std;

template <typename ITEM_TYPE>
class Stack{
public:
    class Iterator{
    public:
        friend class Stack;                 //give access to list to access _ptr

        Iterator(){_ptr = NULL;}            //default ctor

        Iterator(node<ITEM_TYPE>* p){ _ptr = p; }      //Point Iterator to where p is pointing to

        ITEM_TYPE operator *(){ return _ptr->_item; }    //dereference operator

        bool is_null(){return _ptr == NULL;}            //true if _ptr is NULL

        friend bool operator !=(const Iterator& left, const Iterator& right)  //true if left != right
        { return left._ptr != right._ptr; }

        friend bool operator ==(const Iterator& left, const Iterator& right)  //true if left == right
        { return left._ptr == right._ptr; }

        Iterator& operator++()                        //member operator:++it or ++it = new_value
        {                                                      
            if (_ptr) 
            {
                _ptr = _ptr->_next;
            }
            return *this;
        }

        friend Iterator operator++(Iterator& it, int unused)        //friend operator: it++
        {    
            assert(it._ptr != NULL);
            Iterator hi = it;
            if (it._ptr) 
            {
                it._ptr = it._ptr->_next;
            }
            return hi;
        }

    private:
        node<ITEM_TYPE>* _ptr;    //pointer being encapsulated
    };

    Stack() : _top(nullptr), _size(0) {}

    Stack(const Stack<ITEM_TYPE>& copyMe) : _top(nullptr), _size(0)
    { 
        _copy_list(_top, copyMe._top);
        _size = copyMe._size;
    }

    ~Stack()
    { 
        _clear_list(_top);
        _top = nullptr;
        _size = 0;
    }

    Stack<ITEM_TYPE>& operator=(const Stack<ITEM_TYPE>& RHS)
    {
       _copy_list(_top, RHS._top); 
       _size = RHS._size;
       return *this;
    }

    ITEM_TYPE top()
    { return _top->_item; }

    bool empty()
    { return _size == 0; }

    void push(ITEM_TYPE item)
    {
        _insert_head(_top, item);
        ++_size;
    }

    ITEM_TYPE pop()
    {
        if (_size < 1) 
        {
            _top = _remove_head(_top);
            return ITEM_TYPE();
        }

        ITEM_TYPE hi = _top->_item; // storing the item to delete
        _top = _remove_head(_top);
        --_size;
        return hi;
    }

    template<typename T>
    friend ostream& operator << (ostream& outs, const Stack<T>& printMe) 
    {
        node<T> *current = printMe._top;
        // outs << "Head->";
        outs << '{';
        while (current) 
        {
            outs << " " << current->_item;
            current = current->_next;
        }
        outs << '}' << endl;
        return outs;
    }

    Iterator begin() const //Iterator to the head node
    { 
        return _top; 
    }

    Iterator end() const  //Iterator to NULL
    {
        return nullptr;
    }

    int size() const 
    { return _size; }

private:
    node<ITEM_TYPE>* _top;
    int _size;
};


#endif