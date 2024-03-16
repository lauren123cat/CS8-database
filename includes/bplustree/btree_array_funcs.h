#ifndef ARRAY_H
#define ARRAY_H
#include <iostream>
#include <iomanip>
#include <vector>

#include "../table/typedefs.h"

using namespace std;      
                                                        
//return the first element in data that is not less than entry
template <class T>
int first_ge(const T data[ ], int n, const T& entry);      

//insert entry at index i in data
template <class T>
void insert_item(T data[ ], int i, int& n, T entry);          

//delete item at index i and place it in entry
template <class T>
void delete_item(T data[ ], int i, int& n, T& entry);  

//print array data
template <class T>
void print_array(const T data[], int n, int pos = -1); 

//move n/2 elements from the right of data1 and move to data2
template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2);  


//copy src[] into dest[]
template <class T>
void copy_array(T dest[], const T src[], int& dest_size, int src_size); 

//swap the two items
template <class T>
void swap(T& a, T& b);  

//insert entry into the sorted array
//data with length n
template <class T>
void ordered_insert(T data[ ], int& n, T entry);  

//append entry to the right of data
template <class T>
void attach_item(T data[ ], int& n, const T& entry); 

//append data2 to the right of data1
template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2); 

//return the larger of the two items
template <class T>
T maximal(const T& a, const T& b);                      

//return index of the largest item in data
template <class T>
int index_of_maximal(T data[ ], int n);                            

//item > all data[i]
template <class T>
bool is_gt(const T data[], int n, const T& item);    

//item <= all data[i]
template <class T>
bool is_le(const T data[], int n, const T& item);  

//-------------- Vector Extra operators: ---------------------

//print vector list
template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list); 

//list.push_back addme
template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme); 

template <typename T>
vector<T>& operator +=(vector<T>& list1, vector<T>& list2);

template <typename T>
vector<T>& which_one_is_smaller(vector<T>& list1, vector<T>& list2);

// void vectorstr_outputter(const vectorstr& vec); // just for me!
// void vectorl_outputter(const vectorl& vec); 

// ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ ❤ // 

// return the first element in data that is not less than entry
template <class T>
int first_ge(const T data[], int n, const T& entry)
{
    for (int i = 0; i < n; i++) 
    {
        if (data[i] >= entry) // greater than or equal to 
        {
            return i; 
        }
    }
    return n;
}

//insert entry at index i in data
template <class T>
void insert_item(T data[], int i, int &n, T entry)
{
    if (i > n) // out of bounds
    {
        return;
    }
    
    n++; // increment size

    for (int index = n - 1; index > i; index--) // shift everything after i to the right
    {
        data[index] = data[index - 1];
    }

    data[i] = entry;
}

//delete item at index i and place it in entry
template <class T>
void delete_item(T data[], int i, int &n, T &entry)
{
    if (i >= n) // out of bounds
    {
        return;
    }

    entry = data[i]; // save deleted item in entry
    n--; // decrement size

    for (int index = i; index < n; index++) // shift everything after i to the left
    {
        data[index] = data[index + 1];
    }
}

//print array data
template <class T>
void print_array(const T data[], int n, int pos)
{
    for (int i = 0; i < n; i++) 
    {
        cout << data[i] << ' ';
    }
    cout << endl;
}

//move n/2 elements from the right of data1 and move to data2
template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2)
{
    int half = n1/2;
    if (n1 % 2 == 0) // even # size
    {
        half--;
    }

    int start = 0;
    T deleted;

    for (int i = half + 1; i < n1; i++) 
    {
        insert_item(data2, start, n2, data1[i]);
        start++;
    }
    n1 -= start;
}

//copy src[] into dest[]
template <class T>
void copy_array(T dest[], const T src[], int& dest_size, int src_size)
{
    dest_size = src_size;
    for (int i = 0; i < src_size; i++) 
    {
        dest[i] = src[i];
    }
}

//remove the last element in data and place it in entry
template <class T>
void detach_item(T data[ ], int& n, T& entry)
{
    n--;
    entry = data[n];
}

//swap the two items
template <class T>
void swap(T& a, T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

//insert entry into the sorted array
//data with length n
template <class T>
void ordered_insert(T data[ ], int& n, T entry)
{
    int index = first_ge(data, n, entry);
    insert_item(data, index, n, entry);
}

//append entry to the right of data
template <class T>
void attach_item(T data[ ], int& n, const T& entry)
{
    insert_item(data, n, n, entry);
}

//append data2 to the right of data1
template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2)
{
    int count = n2;
    for (int i = 0; i < count; i++) 
    {
        attach_item(data1, n1, data2[i]);
    }
    n2 = 0;
}

//return the larger of the two items
template <class T>
T maximal(const T& a, const T& b)
{
    return a; // stub
}                     

//return index of the largest item in data
template <class T>
int index_of_maximal(T data[ ], int n)  
{
    return -1; // stub
}                       

//item > all data[i]
template <class T>
bool is_gt(const T data[], int n, const T& item)
{
    return false; // stub
}

//item <= all data[i]
template <class T>
bool is_le(const T data[], int n, const T& item)
{
    return false; // stubs
}

//print vector list
template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list)
{
    for (int i = 0; i < list.size(); i++) 
    {
        outs << list.at(i) << ' ';
    }
    return outs; 
}

//list.push_back addme
template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme)
{
    list.push_back(addme);
    return list; 
}

template <typename T>
vector<T>& operator +=(vector<T>& list1, vector<T>& list2)
{
    for (int i = 0; i < list2.size(); i++) 
    {
        list1.push_back(list2.at(i));
    }
    return list1;
}

template <typename T>
vector<T>& which_one_is_smaller(vector<T>& list1, vector<T>& list2)
{
    if (list1.size() < list2.size())
    {
        return list1;
    }
    else 
    {
        return list2;
    }
}

// void vectorstr_outputter(const vectorstr& vec) 
// {
//     for (int i = 0; i < vec.size(); i++)
//     {
//         cout << vec.at(i) << ", ";
//     }
//     cout << endl;
// }

// void vectorl_outputter(const vectorl& vec)
// {
//     for (int i = 0; i < vec.size(); i++)
//     {
//         cout << vec.at(i) << ", ";
//     }
//     cout << endl;
// }

#endif