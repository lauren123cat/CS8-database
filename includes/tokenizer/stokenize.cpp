#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>

#include "constants.h"
#include "stokenize.h"
#include "token.h"
#include "state_machine_functions.h"

using namespace std;

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

STokenizer::STokenizer() : _buffer(""), _pos(0)
{
    init_table(_table);
    make_table(_table);
    _done = false;
}

STokenizer::STokenizer(char str[]) : _pos(0)
{
    // cout << "in stokenizer ctor: " << str << endl;
    strcpy(_buffer, str);
    init_table(_table);
    make_table(_table);
    _done = false;
    // cout << "in stokenizer constructor, this is _buffer: " << _buffer << endl;
}

bool STokenizer::done()            //true: there are no more tokens
{
    return _done;
}   

bool STokenizer::more()            //true: there are more tokens
{
    return !_done;
}

//extract one token (very similar to the way cin >> works)
STokenizer & operator >> (STokenizer& s, SToken& t)
{
    int state = 0;
    string tokenn = "";

    if (s._buffer[s._pos] == '\0')
    {
        s._done = true;
        // cout << "the letter is null so now it stops" << endl;
    }

    s.get_token(state, tokenn);

    int type = -69; // <- random #
    // cout << "state: " << state << endl;
    t = SToken(tokenn, state);

    return s;
}

//set a new string as the input string
void STokenizer::set_string(char str[])
{
    strcpy(_buffer, str);
}

    //create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
void STokenizer::make_table(int table[][MAX_COLUMNS])
{
    // cout << "in make_table()" << endl;
    // digits
    mark_cells(0, table, DIGITS, 1); // if it's a digit then go to state 1
    mark_success(table, 1);          // row 1 is a success state
    mark_cells(1, table, DIGITS, 1); // loop through state 1 if it's still a digit

    mark_cell(1, table, '.', 2);      // if there's a denimal point then go to state 2
    mark_fail(table, 2);             // row 2 is a fail state

    mark_cells(2, table, DIGITS, 3); // if there's more digits then go to state 3
    mark_success(table, 3);          // row 3 is a success state
    mark_cells(3, table, DIGITS, 3); // continue looping through state 3 if it's still a digit

    // letters
    mark_cells(0, table, ALFA, 4); // if it's a letter then go to state 4
    mark_success(table, 4);        // row 4 is a success state
    mark_cells(4, table, ALFA, 4);  // loop through state 4 if it's still a letter

    // punctuation marks 
    mark_cells(0, table, PUNC, 5); // if it's a punctuation mark then go to state 5
    mark_success(table, 5);        // row 5 is a success state
    mark_cells(5, table, PUNC, 5);  // loop through state 5 if it's still a punctuation mark

    // spaces
    mark_cells(0, table, SPACES, 6); // if it's a space then go to state 6
    mark_success(table, 6);          // row 6 is a success state
    mark_cells(6, table, SPACES, 6);  // loop through state 6 if it's still a space

    // quotation mark
    mark_cell(0, table, QUOTATION, 7); // if it's a quotation mark, go to state 7
    mark_success(table, 7); 
    mark_cell(7, table, QUOTATION, 7); 

    // relationals and logicals
    mark_cells(0, table, GOOD, 10); 
    mark_success(table, 10);       
    mark_cells(10, table, GOOD, 10); 

    // parantheses
    mark_cells(0, table, PARAN, 11);
    mark_success(table, 11);
}

bool STokenizer::get_token(int& start_state, string& token)
{
    token = "";
    int curr_state = 0; 
    string tempoutside = ""; // to store temp value outside of the while loop

    while (curr_state != TOKEN_UNKNOWN && (_pos < strlen(_buffer)))
    // loops until they reach an invalid character or reaches the end of the buffer
    {
        string temp = ""; 
        curr_state = _table[curr_state][_buffer[_pos]]; // set state
        
        if (is_success(_table, curr_state)) // success state
        {
            token += tempoutside;
            tempoutside = "";
            token += _buffer[_pos]; 
            start_state = curr_state;
            _pos++;
        }
        else if (!is_success(_table, curr_state) && curr_state != TOKEN_UNKNOWN) // fail state but not because it reached an unknown value
        {
            if (_pos + 1 < strlen(_buffer)) // if it's not the last character in the buffer 
            {
                temp += _buffer[_pos]; 
                // cout << "fail state! temp is currently: " << temp << endl;
                tempoutside = temp;
                _pos++;   
            }
        }
    }

    return token != "";
}