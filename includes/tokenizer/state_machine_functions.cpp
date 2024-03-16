#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>

#include "state_machine_functions.h"
#include "constants.h"
#include "stokenize.h"
#include "token.h"

using namespace std;

void init_table(int _table[][MAX_COLUMNS]) 
{
    for (int i = 0; i < MAX_ROWS; i++) 
    {
        for (int j = 0; j < MAX_COLUMNS; j++)
        {
            _table[i][j] = -1;
        }
    }
}

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state)
{
    _table[state][0] = 1; 
}

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLUMNS], int state)
{
    _table[state][0] = 0;
}

//true if state is a success state
bool is_success(int _table[][MAX_COLUMNS], int state) 
{
    return _table[state][0] == 1;
}

//Mark a range of cells in the array. 
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state) 
{
    for (int i = from; i < to; i++) 
    {
        _table[row][i] = state;
    }
}

//Mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state) // this is the one we are using for the final project
{
    for (int i = 0; i < strlen(columns); i++) 
    {
        _table[row][columns[i]] = state;
    }
}

//Mark this row and column
void mark_cell(int row, int table[][MAX_COLUMNS], int column, int state) 
{
    table[row][column] = state;
}

//This can realistically be used on a small table
void print_table(int _table[][MAX_COLUMNS]) 
{
    int k = 0;
    for (int i = 0; i < 9; i++) 
    {
        cout << '[' << _table[0][k] << ']' << ' ';
        for (int j = '1'; j < '9' + 1; j++)
        {
            cout << '[' << _table[i][j] << ']' << ' ';
        }
        k++;
        cout << endl;
    }
}

//show string s and mark this position on the string:
//hello world   pos: 7
//       ^
void show_string(char s[], int _pos) 
{
    for (int i = 0; i < strlen(s); i++) 
    {
        cout << s[i];
    }
    cout << "   pos: " << _pos << endl;
    for (int j = 0; j < _pos - 1; j++) 
    {
        cout << ' ';
    }
    cout << '^' << endl;
}