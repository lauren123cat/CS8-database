#ifndef MYQUEUE_H
#define MYQUEUE_H
#include "node.h"
#include "linked_list_functions.h"
#include "../token/relational.h"
#include "../token/token.h"
#include <iostream>
#include <iomanip>
using namespace std;

template <typename T>
class Queue
{
public:
    class Iterator{
    public:
        friend class Queue; //give access to list to access _ptr
        Iterator()
        {
            _ptr = nullptr; //default ctor
        } 
        
        Iterator(node<T>* p){ _ptr = p; }  //Point Iterator to where p is pointing to

        T operator *(){ return _ptr->_item; }  //dereference operator

        bool is_null(){ return _ptr == nullptr; }  //true if _ptr is NULL

        friend bool operator !=(const Iterator& left, const Iterator& right)  //true if left != right
        { return left._ptr != right._ptr; }

        friend bool operator ==(const Iterator& left, const Iterator& right)  //true if left == right
        { return left._ptr == right._ptr; }


        Iterator& operator++() //member operator:++it or ++it = new_value
        {                                                      
            if (_ptr) 
            {
                _ptr = _ptr->_next;
            }
            return *this;
        }

        friend Iterator operator++(Iterator& it, int unused)  //friend operator: it++
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
        node<T>* _ptr; //pointer being encapsulated
    };

    Queue() : _front(nullptr), _rear(nullptr), _size(0) 
    {}

    Queue(const Queue<T>& copyMe) : _front(nullptr), _rear(nullptr), _size(0)
    { 
        _copy_list(_front, copyMe._front); 
        _size = copyMe.size();
        // cout << "front: " << _front->_item << " | rear: " << _rear->_item << " | size: " << _size << endl;
    }

    ~Queue() 
    { 
        _clear_list(_front); 
        _front = nullptr;
        _rear = nullptr;
        _size = 0;
    }

    Queue& operator=(const Queue<T>& RHS) 
    { 
        if (this == &RHS) 
        {
            return *this;
        }
        _clear_list(_front); // clear the 2nd list before copyingg
        _copy_list(_front, RHS._front); 
        // cout << "front: " << _front->_item << " | rear: " << _rear->_item << " | size: " << _size << endl;
        _size = RHS.size();
        return *this;
    }

    bool empty() 
    { return _size == 0; }

    T front()
    { return _front->_item; }

    T back() 
    { return _rear->_item; }

    void push(T item) 
    {
        _rear = _insert_after(_front, _rear, item); 
        _size++;
        // cout << "front: " << *(_front->_item) << " | rear: " << *(_rear->_item) << " | size: " << _size << endl;
    }

    T pop() 
    {
        if (!_front) // if it's an empty list
        {
            // cout << "empty list :( size: " << _size << endl;
            return T();
        }

        T todelete= _front->_item;
        _remove_head(_front);
        
        // cout << "deleting: " << todelete << endl;
        --_size;

        if (!_front) // if it's now empty
        {
            _rear = nullptr;
            _front = nullptr;
            _size = 0;
        }
        else 
        {
            // cout << "front: " << _front->_item << " | rear: " << _rear->_item << " | size: " << _size << endl;
        }

        return todelete;
    }

    Iterator begin() const   //Iterator to the head node
    { 
        return _front;
    }

    Iterator end() const  //Iterator to NULL
    { 
        return nullptr;
    }

    void print_pointers() // ignore for now
    { 
        _print_list(_front); 
    }
    
    int size() const 
    { 
        return _size; 
    }
    
    template<typename TT>
    friend ostream& operator << (ostream& outs, const Queue<TT>& printMe)
    {
        node<T> *current = printMe._front;
        outs << '{';
        while (current) 
        {
            outs << " " << current->_item;
            current = current->_next;
        }
        outs << '}' << endl;
        return outs;
    }

private:
    node<T>* _front;
    node<T>* _rear;
    int _size;
};

#endif // MYQUEUE.H
