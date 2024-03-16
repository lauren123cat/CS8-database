#include "token.h"
#include "constants.h"
#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std;

SToken::SToken() : _token(""), _type(0)
{
    //cout << "in token constructor, " << "this is _token: " << _token << " and type: " << _type << endl;
}

SToken::SToken(string str, int type) : _token(str), _type(type)
{
    // cout << "in token constructor: " << endl << endl;
    // cout << "token: " << _token << endl;
    // cout << "type: " << type_string() << endl << endl;
}

ostream& operator <<(ostream& outs, const SToken& t)
{
    outs << '[' << t._token << "] " << endl;
    
    return outs;
}

int SToken::type() const 
{
    return _type;
}

string SToken::type_string() const
{ 
    /*  token types:
        const int TOKEN_NUMBER = 1;
        const int TOKEN_NUMBER2 = 3;
        const int TOKEN_ALPHA = 4;
        const int TOKEN_SPACE = 6;
        const int TOKEN_OPERATOR = 4;
        const int TOKEN_PUNC = 5;
        const int TOKEN_GOOD = 10;

        const int TOKEN_NUMBER = 1;
        const int TOKEN_ALPHA = 2;
        const int TOKEN_SPACE = 3;
        const int TOKEN_OPERATOR = 4;
        const int TOKEN_PUNC = 5;
    */
    string stringtype1 = "PUNC";
    string stringtype2 = "SPACE";
    string stringtype3 = "ALPHA";
    string stringtype4 = "NUMBER";
    if (_type == 1 || _type == 3)
    {
        return stringtype4;
    }
    else if (_type == 4)
    {
        return stringtype3;
    }
    else if (_type == 5)
    {
        return stringtype1;

    }
    else if (_type == 6)
    {
        return stringtype2;
    }
    else if (_type == 10)
    {
        return "GOOD";
    }
    else if (_type == 8)
    {
        return stringtype3;
    }
    return stringtype1;
}

string SToken::token_str() const
{
    return _token;
}