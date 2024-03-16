#ifndef LINKEDLISTFUNCTIONSTEMPLATED_H
#define LINKEDLISTFUNCTIONSTEMPLATED_H
#include "node.h"
#include <iostream>
#include <iomanip>
using namespace std;

//Linked List General Functions:
template <typename T>
void _print_list(node<T>* head);

//recursive fun! :)
template <typename T>
void _print_list_backwards(node<T> *head);

//return ptr to key or NULL
template <typename T>
node<T>* _search_list(node<T>* head,
                            T key);

template <typename T>
node<T>* _insert_head(node<T> *&head, T insert_this);

template <typename T>
node<T>* _remove_head(node<T> *&head); // I ADDED THIS FUNCTION 

//insert after ptr: insert head if marker null
template <typename T>
node<T>* _insert_after(node<T>*& head, node<T> *after_this, T insert_this);

//insert before ptr: insert head if marker null
template <typename T>
node<T>* _insert_before(node<T>*& head, node<T>* before_this, T insert_this);

//ptr to previous node
template <typename T>
node<T>* _previous_node(node<T>* prev_to_this);

//delete, return item
template <typename T>
T _delete_node(node<T>*& head, node<T>* delete_this);

//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head);
//duplicate the list, return pointer to last node in dest... 
//     use this function in your queue big three 

template <typename T>
node<T> *_copy_list(node<T> *&dest, node<T> *src);

//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head);

//_item at this position
template <typename T>
T& _at(node<T>* head, int pos);

//----- DEFINITIONS -----------------------------------------------------------------------------------------------------------------------------


//Linked List General Functions:
template <typename T>
void _print_list(node<T>* head) 
{
    if (head) // not empty 
    {
        while (head->_next) 
        {
            cout << head->_item << ' ';
            head = head->_next;
        }
        cout << head->_item;
    }
}

//recursive fun! :)
template <typename T>
void _print_list_backwards(node<T> *head) 
{
    if (!head) // base case
    {
        return;
    }

    if (!head->_next) // base case 
    {
        cout << head->_item;
    }

    else // recursion
    {
        _print_list_backwards(head->_next);
        cout << ' ' << head->_item;
    }
}

//return ptr to key or NULL
template <typename T>
node<T>* _search_list(node<T>* head, T key) 
{
    node<T> *current = head;
    while (current)
    {
        if (current->_item == key) 
        {
            return current;
        }
        current = current->_next;
    }
    return nullptr; // if the key isn't in any of the nodes
}


template <typename T>
node<T>* _insert_head(node<T> *&head, T insert_this) 
{
    node<T> *newnode = new node<T>(insert_this);
    if (!head) // empty 
    {
        head = newnode;
    }
    else // not empty
    {
        newnode->_next = head;
        head->_prev = newnode;
        head = newnode;
    }
    return newnode;
}

template <typename T>
node<T>* _remove_head(node<T> *&head) 
{
    if (!head) // nothing
    {
        return nullptr;
    }
    if (!head->_next) // just 1
    {
        delete head;
        head = nullptr;
    }
    else // more than 1
    {
        // cout << "head: " << head->_item << endl;
        node<T>* temp = head;
        head = head->_next;
        head->_prev = nullptr; 
        // cout << "head: " << head->_item << endl;
        delete temp;
        temp = nullptr;
    }
    return head;
}

//insert after ptr: insert head if marker null
template <typename T>
node<T>* _insert_after(node<T>*& head, node<T> *after_this, T insert_this) 
{
    if (!head) // empty list
    {
        return _insert_head(head, insert_this);
    }

    node<T> *newnode = new node<T>(insert_this);

    if (!after_this->_next) // inserting at the end
    {
        after_this->_next = newnode;
        newnode->_prev = after_this;
    }
    else // inserting in the middle
    {
        newnode->_next = after_this->_next;
        after_this->_next = newnode;
        newnode->_prev = after_this;
    }
    return newnode;
}

//insert before ptr: insert head if marker null
template <typename T>
node<T>* _insert_before(node<T>*& head, node<T>* before_this, T insert_this) 
{
    if (!head) // empty list
    {
        return _insert_head(head, insert_this);
    }

    node<T> *newnode = new node<T>(insert_this);

    if (!before_this->_prev) // inserting in the front
    {
        before_this->_prev = newnode;
        newnode->_next = before_this;
        // does this break?
        head = newnode;
    }
    else // inserting in the middle
    {
        before_this->_prev->_next = newnode;
        newnode->_next = before_this;
        newnode->_prev = before_this->prev;
        before_this->_prev = newnode;
    }
    return newnode;
}

//ptr to previous node
template <typename T>
node<T>* _previous_node(node<T>* prev_to_this) 
{
    return prev_to_this->_prev;
}

//delete, return item
template <typename T>
T _delete_node(node<T>*& head, node<T>* delete_this) 
{
    if (!head) 
    {
        return T();
    }
    T ew = delete_this->_item;
    if (!delete_this->_prev) // deleting head
    {
        if (head->_next) 
        {
            head = delete_this->_next;
            head->_prev = nullptr;
            delete delete_this;
            delete_this = nullptr;
        }
        else 
        {
            delete delete_this;
            head = nullptr;
        }
    }
    else if (!delete_this->_next) // deleting tail
    {
        delete_this->_prev->_next = nullptr;
        delete delete_this;
    }
    else // deleting middle 
    {
        delete_this->_prev->_next = delete_this->_next;
        delete_this->_next->_prev = delete_this->_prev;
        delete delete_this;
    }
    return ew;
}

//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head) 
{
    if (!head) 
    {
        head = nullptr;
        return nullptr;
    }

    node<T> *current = head;
    node<T> *head2 = new node<T>(current->_item);
    node<T> *current2 = head2;

    while (current) 
    {
        current2 = _insert_after(head2, current2, current->_item);
        current = current->_next;
    }

    return _remove_head(head2);
}

//duplicate the list, return pointer to last node in dest... 
//     use this function in your queue big three 
template <typename T>
node<T> *_copy_list(node<T> *&dest, node<T> *src) 
{
    if (!src) // if the 1st list is empty 
    {
        _clear_list(dest); // make the 2nd list clear
        dest = nullptr;
        return nullptr;
    }

    node<T> *current = src; 
    node<T> *current2 = dest;

    //cout << current2->_item << endl;

    while (current) 
    {
        current2 = _insert_after(dest, current2, current->_item);
        //cout << current->_item << endl;
        //cout << current2->_item << endl;
        current = current->_next;
    }

    return current2;
}

//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head) 
{
    while (head) 
    {
        node<T> *temp = head;
        head = head->_next;
        delete temp;
    }
    head = nullptr;
}

//_item at this position
template <typename T>
T& _at(node<T>* head, int pos) 
{
    int i = 0;
    node<T> *current = head;
    while (i < pos && current) 
    {
        current = current->_next;
        ++i;
    }
    return current->_item;
}





#endif //LINKEDLISTFUNCTIONSTEMPLATED_H
