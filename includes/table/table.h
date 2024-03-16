#ifndef TABLE_H
#define TABLE_H
#include <iostream>
#include <fstream>
#include <iomanip>
#include <istream>
#include <vector>
#include <cstring>
#include <string>
#include <algorithm> // sort?

#include "typedefs.h"
// #include "rpn/rpn.h"
#include "../bplustree/mmap.h"
#include "../bplustree/map.h"
#include "../bplustree/btree_array_funcs.h"
#include "../stackqueue/stack.h"
#include "../stackqueue/queue.h"
#include "../token/relational.h"
#include "../token/logical.h"
#include "../token/token.h"
#include "../token/tokenstr.h"
#include "../token/parantheses.h"
#include "../token/solution.h"
#include "../binary_files/utilities.h"
#include "../binary_files/file_record.h"

using namespace std;

class Table
{
/* 
SUMMARY:
- table is a collection of records
- records is rows (information of each person)
    - each person has a unique id # / recno
- fields is columns (ex. first, last, dept, phone)

TABLE CLASS:
1. creates a new table / file
2. opens an existing table / file
3. inserts records into the table by writing them into the file
4. "select all" returns a table object with all the records
5. "select where" returns a table object with records matching certain conditions
*/

public:
    Table();
    Table(const string &name, const vectorstr &fieldnames);
    Table(const string &name);

    void insert_into(const vectorstr &values);
    Table select_all();
    Table select(const vectorstr &fields, const string &field, const string &symbol, const string &condition); // done
    Table select(const vectorstr &fields, const Queue<Token*> &post); // done
    Table select(const vectorstr &fields, const vectorstr &strings);
    Table select_columns(const vectorstr &fields);
    vectorl select_recnos();
    vectorl select_options(const string &field, const string &symbol, const string &condition);
    vector<Token*> tokenize(const vectorstr &conditions); // tokenizes the strings
    Queue<Token*> shuntingyard(const vector<Token*> &tokens); // orders the tokens (infix -> postfix)

    ~Table(){}

    void write_info(FileRecord &record);
    void read_info();

    void set_fields(const vectorstr &fields); 
    vectorstr get_fields(); // returns vector of field names
    void reindex();

    friend ostream& operator <<(ostream& outs, const Table& to_print);
    void print_index(); // prints index structure
    vectorl _recnos;
    static int _serial; // better to be static so u can ++ it across all classes
    
private:
    vectorstr _fieldnames;
    vector<MMap<string, long>> _index; 
    Map <string, int> _fieldmap; 
    bool empty(); // no records have been added
    // int _serial = 0; // # of subset tables you have
    int _recordcount = 0;
    string _name; // table name
    vectorl get_select_recnos(); 
    // vectorl _recnos;
    void vectorstr_outputter(const vectorstr& vec); // just for me!
    void vectorl_outputter(const vectorl& vec); 
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class RPN
{
public:
    // ctor:
    RPN(Queue<Token*> queue) : postfix(queue)
    {
        // cout << "in RPN ctor, queue size: " << queue.size() << ", postfix size: " << postfix.size() << endl; 
    }
    vectorl make_solution(Table& table);
    friend ostream& operator <<(ostream& outs, const RPN& t)
    {
        return outs;
    }
    vectorl _intersect(vectorl vec1, vectorl vec2);
    vectorl _union(vectorl vec1, vectorl vec2);
private:
    Stack<Token*> stack;
    void print_stack()
    {
        cout << stack << endl;
    }
    Solution solution;
    Queue<Token*> postfix; // the buffer of tokens (organized)
};

#endif

