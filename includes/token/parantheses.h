#ifndef PARANTHESES_H
#define PARANTHESES_H
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include "token.h"

using namespace std;

class RightParantheses : public Token
{
public:
    RightParantheses() : Token("", TokenType::RightParantheses)
    {}
    RightParantheses(string symbol) : Token(symbol, TokenType::RightParantheses), _token(symbol)
    {
        
    }
    // int get_precedence()
    // {
    //     return precedence;
    // }
    // void set_precedence(string symbol)
    // {
    //     precedence = 0; 
    // }
    friend ostream& operator<<(ostream& outs, const RightParantheses& rightparantheses)
    {
        return outs;
    }
private:
    string _token;
};

class LeftParantheses : public Token
{
public:
    LeftParantheses() : Token("", TokenType::LeftParantheses)
    {}
    LeftParantheses(string symbol) : Token(symbol, TokenType::LeftParantheses), _token(symbol)
    {
        // set_precedence(symbol);
    }
    // int get_precedence()
    // {
    //     return precedence;
    // }
    // void set_precedence(string symbol)
    // {
    //     precedence = 0; 
    // }
    friend ostream& operator<<(ostream& outs, const LeftParantheses& leftparantheses)
    {
        return outs;
    }
private:
    string _token;
    // int precedence;
};

#endif