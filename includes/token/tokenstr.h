#ifndef TOKENSTR_H
#define TOKENSTR_H
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "token.h"

using namespace std;

class TokenStr : public Token
{
public:
    TokenStr() : Token("", TokenType::TokenStr)
    {} 
    TokenStr(string thing) : Token(thing, TokenType::TokenStr), _token(thing)
    {
        // cout << "in tokenstr ctor, string: " << thing << endl;
    } 
    friend ostream& operator<<(ostream& outs, const TokenStr& print_tokenstr)
    {
        return outs;
    }
private:
    string _token;
};

#endif