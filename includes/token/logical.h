#ifndef LOGICAL_H
#define LOGICAL_H
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "token.h"

using namespace std;

/*
- 0 is more important than 1
*/

class LogicalAnd : public Token
{
public:
    LogicalAnd() : Token("", TokenType::LogicalAnd)
    {}
    LogicalAnd(string symbol) : Token(symbol, TokenType::LogicalAnd), _token(symbol)
    {
        set_precedence(symbol);
    }
    int get_precedence()
    {
        return precedence;
    }
    void set_precedence(string symbol)
    {
        if (symbol == "and")
        {
            precedence = 1;
        }
        else if (symbol == "or")
        {
            precedence = 0;
        }
    }
    friend ostream& operator<<(ostream& outs, const LogicalAnd& print_logical)
    {
        return outs;
    }
private:
    int precedence;
    string _token;
};

class LogicalOr : public Token
{
public:
    LogicalOr() : Token("", TokenType::LogicalOr)
    {}
    LogicalOr(string symbol) : Token(symbol, TokenType::LogicalOr), _token(symbol)
    {
        set_precedence(symbol);
    }
    int get_precedence()
    {
        return precedence;
    }
    void set_precedence(string symbol)
    {
        if (symbol == "and")
        {
            precedence = 1;
        }
        else if (symbol == "or")
        {
            precedence = 0;
        }
    }
    friend ostream& operator<<(ostream& outs, const LogicalOr& print_logical)
    {
        return outs;
    }
private:
    int precedence;
    string _token;
};

#endif