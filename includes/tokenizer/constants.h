#ifndef CONSTANTS_H
#define CONSTANTS_H

const int MAX_COLUMNS = 256;
const int MAX_ROWS = 100;
const int MAX_BUFFER = 200;

const char ALFA[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
const char DIGITS[] = "0123456789";
const char QUOTATION = '\"'; 
const char GOOD[] = "*><="; // other things i wanna keep in the vectorstr
const char PARAN[] = "()"; // so they can be individual
//const char OPERATORS[] = "><=+-%&|"; 
const char SPACES[] = {' ', '\t', '\n', '\0'};
const char PUNC[] = "?.,:;'`~!+-%&|";
const int START_DOUBLE = 0;
const int START_SPACES = 4;
const int START_ALPHA = 6;
const int START_OPERATOR = 20;
const int START_PUNC = 10;

//token types:
const int TOKEN_NUMBER = 1;
const int TOKEN_ALPHA = 2;
const int TOKEN_SPACE = 3;
const int TOKEN_OPERATOR = 4;
const int TOKEN_PUNC = 5;

const int TOKEN_UNKNOWN = -1;
const int TOKEN_END = -2;

// these are the columns for the state machine matrix:
enum Keywords : int
{
    STATE,   // 0 we can't put anything on this column
    CREATE,  // 1 create or make
    MAKE,    // 2
    TABLE,   // 3 
    SYMBOL,  // 4
    FIELDS,   // 5
    WHERE,   // 6
    INSERT,  // 7
    INTO,    // 8
    VALUES,   // 9
    STAR,    // 10
    SELECT, // 11
    FROM // 12
};

#endif