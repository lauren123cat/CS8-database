#ifndef RELATIONAL_H
#define RELATIONAL_H
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include "token.h"


using namespace std;

class Relational : public Token
{
public:
    Relational() : Token("", TokenType::Relational)
    {}
    Relational(string symbol) : Token(symbol, TokenType::Relational), _token(symbol) // in polymorphism, you have to call parent's ctor 
    {
        // cout << "in relational ctor, string: " << symbol << endl;
    }

    int get_precedence()
    {
        return 2;
    }

    friend ostream& operator<<(ostream& outs, const Relational& print_relational)
    {
        return outs;
    }
private:
    string _token;
};

#endif