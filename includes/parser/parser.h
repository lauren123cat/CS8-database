#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <istream>
#include <vector>
#include <cstring>
#include <string>
#include <algorithm>

#include "ptypedefs.h"
#include "../table/table.h"
#include "../table/typedefs.h"
#include "../bplustree/bplustree.h"
#include "../bplustree/btree_array_funcs.h"
#include "../bplustree/mmap.h"
#include "../bplustree/map.h"
#include "../tokenizer/stokenize.h"
#include "../tokenizer/constants.h"
#include "../tokenizer/token.h"

/* ౨ৎ NOTES ౨ৎ
- purpose
    - parser is a MMap of strings & vector of strings
        ex.
        command - insert
        tablename - student
        col - last, first, age, major, class
    - things to identify (string key) differs depending on the command
        - if command == insert into, identify tablename & values 
        - if command == make, identify tablename & columns
        - if command == select, identify conditions, fields, tablename, and wherejnbk lm
- they give you a string / buffer (ie. str = "make table student fields last, first, age, major, class") which is the COMMAND LINE
- we tokenize the string into separate tokens using the stokenizer (but we want them to be strings so we can put them in the map and mmap)
    - only take alphas, don't take puncs or anything else
- types of strings
    - commands -- "make" / "create", "insert into", "select" -- tell you which function to use
    - conditions -- fields -- are always after select or where
- state machine determines what is what, and it has to be unique 
*/

class Parser
{
    public:
        Parser();
        Parser(char* s);

        mmap_ss parse_tree(); // returns ptree
        void set_string(char* s); // idk what this does yet
        void make_map();
        void make_table();
        void insert_ptree(); 
        int size();
        vectorstr tokenize(char* s); // tokenizes the strings

    private:
        map_si _keywords;
        mmap_ss _ptree;
        vectorstr _tokenized; // turn tokens to strings
        static int _table[MAX_ROWS][MAX_COLUMNS]; // new state machine (not the stokenizer one)
        int get_enum(string word);
};

#endif