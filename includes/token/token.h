#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>

#include "../table/typedefs.h"

using namespace std;

/* notes:
- shunting yard turns infix notation -> RPN (so computers can understand precedence)
- instead of (4 + 3) * (7 - 4) it's (lname = barkeshli) and (age > 20)
- tokenstr - lname, barkeshli, age, 20
- relational - =, >, etc
- logical - and
- parantheses are different because when you pop the parantheses you don't add it to your postfix

my precedence:
0 - relational operators
1 - logical "and"
2 - logical "or"
3 - left parantheses
4 - right parantheses
jk i commented all of that out because i realized that using enums makes more sense to me
*/

enum class TokenType 
{
    Solution, 
    TokenStr, 
    RightParantheses,
    LeftParantheses, 
    LogicalOr,
    LogicalAnd,
    Relational,
};

class Token
{
public:
    Token(string thing) : _token(thing)
    {
        // i find the type myself
        // cout << "in token (string) ctor, string: " << thing << endl;
        if (relational_type())
        {
            _type = TokenType::Relational;
            // cout << "assigning type as relational" << endl;
        }
        else if (logical_and())
        {
            _type = TokenType::LogicalAnd;
            // cout << "assigning type as logicaland" << endl;
        }
        else if (logical_or())
        {
            _type = TokenType::LogicalOr;
            // cout << "assigning type as logicalor" << endl;
        }
        else if (left_parantheses())
        {
            _type = TokenType::LeftParantheses;
            // cout << "assigning type as left parantheses" << endl;
        }
        else if (right_parantheses())
        {
            _type = TokenType::RightParantheses;
            // cout << "assigning type as right parantheses" << endl;
        }
        else 
        {
            _type = TokenType::TokenStr; 
            // cout << "assigning type as tokenstr" << endl;
        }
    }
    Token(string thing, TokenType type): _token(thing), _type(type)
    {
        // cout << "in token (string,type) ctor, string: " << thing << endl;
    }
    virtual int get_precedence()
    {
        cout << "precedence should only be called for logicals" << endl;
        return -1;
    }
    virtual void set_precedence()
    {
        // cout << "precedence should only be called for logicals" << endl;
        return;
    }
    TokenType type()
    {
        return _type;
    }
    string type_string()
    {
        return _token;
    }
    virtual string token_str()
    {
        return _token;
    }

    bool relational_type()
    {
        return (_token == "<" || _token == ">" || _token == "=" || _token == ">=" || _token == "<=");
    }
    bool logical_and()
    {
        return (_token == "and");
    }
    bool logical_or()
    {
        return (_token == "or");
    }
    bool left_parantheses()
    {
        return (_token == "(");
    }
    bool right_parantheses()
    {
        return (_token == ")");
    }

    vectorl results()
    {
        vectorl stub;
        return stub;
    }
    friend ostream& operator<<(ostream& outs, const Token& print_token)
    {
        outs << "string token: " << print_token._token << endl;
        return outs;
    }

    friend ostream& operator<<(ostream &outs, Token* t) // cout the token pointer
    {
        outs << t->token_str() << " ";
        return outs;
    }

private:
    string _token; // the actual string
    TokenType _type; // the token type
};

#endif