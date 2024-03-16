#ifndef TOKENSTOKENIZER_H
#define TOKENSTOKENIZER_H
#include <iostream>
#include <iomanip>
using namespace std;

class SToken
{
public:
    SToken();
    SToken(string str, int type);
    friend ostream& operator <<(ostream& outs, const SToken& t);
    int type() const;
    string type_string() const;
    string token_str() const;
private:
    string _token;
    int _type;
};

#endif