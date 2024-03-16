#ifndef SOLUTION_H
#define SOLUTION_H
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "token.h"
#include "../table/table.h"
#include "../table/typedefs.h"

using namespace std;


class Solution : public Token
{
public:
    Solution() : Token("", TokenType::Solution), recnos()
    {}

    Solution(vectorl recordnums) : Token("", TokenType::Solution), recnos(recordnums) 
    {
        // cout << "in solution ctor, printing vector: " << endl;
        for (int i = 0; i < recnos.size(); i++)
        {
            cout << recnos.at(i) << ", ";
        }
    }
    friend ostream& operator<<(ostream& outs, const Solution& print_solution)
    {
        for (int i = 0; i < print_solution.recnos.size(); i++)
        {
            outs << print_solution.recnos.at(i) << endl;
        }
        return outs;
    }
    vectorl get_vectorl()
    {
        return recnos;
    }
private:
    vectorl recnos;
};

#endif