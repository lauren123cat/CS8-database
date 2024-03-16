#include "parser.h"
int Parser::_table[MAX_ROWS][MAX_COLUMNS];

Parser::Parser()
{
    init_table(_table); // stokenizer's init_table
    make_table(); // parser's make_table
}

Parser::Parser(char* s)
{
    init_table(_table);
    _tokenized = tokenize(s);
    make_table();
    make_map();
    // cout << endl << "map: " << endl << _keywords << endl;
    insert_ptree();
    // _ptree.print_lookup();
}

mmap_ss Parser::parse_tree()
{
    // cout << "in parse_tree()" << endl;
    return _ptree;
}

void Parser::set_string(char* s)
{
    // oopsies i did everything in tokenize function :(
    _tokenized = tokenize(s);
    // cout << "ptree before clear: " << endl;
    _ptree.print_lookup();
    _ptree.clear();
    insert_ptree();
    // cout << "ptree after clear and insert: " << endl;
    _ptree.print_lookup();
}

void Parser::make_map()
{
    /* 
    - makes a new map<string, int>
    - stores enum # as the int so you can keep track of the column when going inside of the state machine
    */
    _keywords.insert("state", STATE);
    _keywords.insert("create", CREATE);
    _keywords.insert("make", MAKE);
    _keywords.insert("table", TABLE);
    _keywords.insert("symbol", SYMBOL);
    _keywords.insert("fields", FIELDS);
    _keywords.insert("where", WHERE); 
    _keywords.insert("insert", INSERT);
    _keywords.insert("into", INTO);
    _keywords.insert("values", VALUES);
    _keywords.insert("*", STAR);
    _keywords.insert("select", SELECT);
    _keywords.insert("from", FROM);

    // STATE,   // 0 we can't put anything on this column
    // CREATE,  // 1 create or make
    // MAKE,    // 2
    // TABLE,   // 3 table name
    // SYMBOL,  // 4
    // FIELDS,   // 5
    // WHERE,   // 6
    // INSERT,  // 7
    // INTO,    // 8
    // VALUES,   // 9
    // STAR,    // 10
    // SELECT, // 11
    // FROM // 12
}

void Parser::make_table()
{
    // use state machine functions to build a new table, based on table commands

    // "create" or "make"
    mark_cell(0, _table, CREATE, 1); // add to command
    mark_cell (0, _table, MAKE, 1); // add to command
    mark_cell(1, _table, TABLE, 2);
    mark_cell(2, _table, SYMBOL, 3); // add to table name
    mark_cell(3, _table, FIELDS, 4);
    mark_cell(4, _table, SYMBOL, 5); // add to fields
    mark_cell(5, _table, SYMBOL, 5); // loop symbols (in case there's multiple fields)
    mark_success(_table, 5);  

    // "select"
    mark_cell(0, _table, SELECT, 7); // add to command
    mark_cell(7, _table, SYMBOL, 8); // add to fields
    mark_cell(7, _table, STAR, 8); // add to fields
    mark_cell(8, _table, SYMBOL, 8); // loop symbols (in case there's multiple fields)
    mark_cell(8, _table, FROM, 9);
    mark_cell(9, _table, SYMBOL, 10); // add to table_name
    mark_success(_table, 10); 
    mark_cell(10, _table, WHERE, 11);   
    mark_cell(11, _table, SYMBOL, 12); // add to conditions
    mark_cell(12, _table, SYMBOL, 12); // loop symbols (in case there's multiple conditions)
    mark_success(_table, 12); 
    
    // "insert into"
    mark_cell(0, _table, INSERT, 14); // add to command
    mark_cell(14, _table, INTO, 15);
    mark_cell(15, _table, SYMBOL, 16); // add to table name
    mark_cell(16, _table, VALUES, 17);
    mark_cell(17, _table, SYMBOL, 18); // add to values
    mark_cell(18, _table, SYMBOL, 18); // loop symbols (in case there's multiple fields)
    mark_success(_table, 18);
}

void Parser::insert_ptree() // adds into mmap
{
    /*
    - adds tokenized strings to mmap _keywords
    - MPair<string, column> (for state machine)
    - figure out the column number (it was ascii numbers last time)
    - similar to stokenizer's get_token() because it goes through the state machine, but instead of using ascii values we use the enums
    */
    // cout << "in insert_ptree()" << endl << endl;
    int curr_state = 0;
    int curr_col;
    
    for (int i = 0; i < _tokenized.size(); i++)
    {
        // getting column
        if (get_enum(_tokenized.at(i)) == Keywords::STATE) // it is a symbol
        {
            curr_col = 4;
        }
        else 
        {
            curr_col = get_enum(_tokenized.at(i)); // it is a keyword
        }

        curr_state = _table[curr_state][curr_col];

        // cout << "curr string: " << _tokenized.at(i) << endl;
        // cout << "curr state: " << curr_state << endl;
        // cout << "curr col: " << curr_col << endl << endl;

        // hard code!
        switch (curr_state)
        {
        // make 
        case 1: // make or create
            _ptree.insert("command", _tokenized.at(i)); 
            break;
        case 3: // symbol (tablename)
            _ptree.insert("table_name", _tokenized.at(i)); 
            break;
        case 5: // symbols (fields)
            _ptree.insert("cols", _tokenized.at(i));
            break;

        // select
        case 7: // select
            _ptree.insert("command", _tokenized.at(i));
            break;
        case 8: // symbol or star
            _ptree.insert("fields", _tokenized.at(i));
            break;
        case 10:
            _ptree.insert("table_name", _tokenized.at(i));
            break;
        case 11:
            _ptree.insert("where", "yes");
            break;
        case 12:
            _ptree.insert("condition", _tokenized.at(i));
            break;

        // insert
        case 14: // insert
            _ptree.insert("command", _tokenized.at(i));
            break;
        case 16: 
            _ptree.insert("table_name", _tokenized.at(i));
            break;
        case 18:
            _ptree.insert("values", _tokenized.at(i));
            break;

        default: // useless words like from, where, etc. EW!!!!!!
            break;
        }
    }
}

vectorstr Parser::tokenize(char* s) 
{ 
    // cout << "in tokenize()" << endl;
    // cout << "input: " << s << endl;
    vectorstr tokenized;
    STokenizer stk(s);
    SToken t;
    stk >> t;
    while (stk.more())
    {
        // cout << "hi" << endl;
        if (t.token_str() == "\"")
        {
            vectorstr quote; // temp
            t = SToken();
            cout << t << endl;
            stk >> t;
            while (t.token_str() != "\"" && stk.more())
            {
                // cout << "curr: " << t.token_str() << endl;
                quote.push_back(t.token_str());
                t = SToken();
                stk >> t;
            }
            string temp = "";
            for (int i = 0; i < quote.size(); i++)
            {
                temp += quote.at(i);
            }
            tokenized += temp;
        }
        if (t.type_string() == "ALPHA" || t.type_string() == "NUMBER" || t.type_string() == "GOOD")
        {
            tokenized.push_back(t.token_str()); 
        }
        else 
        {
            if (t.token_str() == "(" || t.token_str() == ")" || t.token_str() == ">")
            {
                tokenized.push_back(t.token_str()); 
            }
        }
        t = SToken();
        stk >> t;
    }

    cout << "after tokenization: ";
    for (int i = 0; i < tokenized.size(); i++)
    {
        cout << tokenized[i] << "|";
    }
    cout << endl;

    return tokenized;
}

int Parser::get_enum(string word)
{
    if (word == "create") 
        return 1;
    else if (word == "make") 
        return 2;
    else if (word == "table") 
        return 3;
    else if (word == "symbol") 
        return 4;
    else if (word == "fields") 
        return 5;
    else if (word == "where") 
        return 6;
    else if (word == "insert") 
        return 7;
    else if (word == "into") 
        return 8;
    else if (word == "values") 
        return 9;
    else if (word == "*") 
        return 10;
    else if (word == "select") 
        return 11;
    else if (word == "from") 
        return 12;
    else 
        return 0;
}

int Parser::size()
{
    return _ptree.size();
}

// enum Keywords : int
// {
//     STATE,   // 0 we can't put anything on this column
//     CREATE,  // 1 create or make
//     MAKE,    // 2
//     TABLE,   // 3 table name
//     SYMBOL,  // 4
//     FIELDS,   // 5
//     WHERE,   // 6
//     INSERT,  // 7
//     INTO,    // 8
//     VALUES,   // 9
//     STAR,    // 10
//     SELECT, // 11
//     FROM // 12
// };