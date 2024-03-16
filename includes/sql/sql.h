#ifndef SQL_H
#define SQL_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <istream>
#include <vector>
#include <cstring>
#include <string>
#include <algorithm>

#include "assert.h"
#include "../parser/parser.h"
#include "../parser/ptypedefs.h"
#include "../table/table.h"
#include "../table/typedefs.h"
#include "../bplustree/bplustree.h"
#include "../bplustree/btree_array_funcs.h"
#include "../bplustree/mmap.h"
#include "../bplustree/map.h"
#include "../tokenizer/stokenize.h"
#include "../tokenizer/constants.h"
#include "../tokenizer/token.h"

/* notes:
- merges table and parser together
*/

using namespace std;

class SQL
{
    public:
        SQL();
        Table command(const string& s);
        /*
        the big function. takes in the big big command line.
        sends string to parser and gets the parse tree (mmap)
        calls run_command to execute query
        */
        vectorl select_recnos();
        vectorl _srecnos;
    private:
        Table _table;
};
#endif


