// /* notes!
// RPN
// assumes what we hand it is already tokenized
//     mmap vector
//     map indices
//     queue<token*> tokens could be relationals, logicals, etc
//     postfix = "age 12 = bruh moment < or"
//             ts  ts r  ts   ts    r l
//     stack<token*> 
//         [age 12] once we hit "=", we pop 12 and age -> solution1(12, age, =) SOLUTION IS A NEW CLASS INSIDE CHILD TOKENS
//         [solution1 bruh moment] once we hit "<" we pop bruh and moment -> solution2(bruh, moment, <) 
//         [solution1 solution2] once we hit "or" we pop solution1 and solution2 -> solution3() call union function  
//         [solution3] pop solution 3 and that's the vector of longs
//     vectorl solutionset contains (the recnos)

//     how i'm going to call it in select:
//     - RPN rpn1(queue);
//     - rpn1.makesolution()

// shunting yard:
// takes strings and tokenizes it for the RPN
//     post.push(new TokenStr("lname")) 
//     vectorstr = {lname, Liu, =, fname, Lauren, =, and} 
//         these are ALL STRINGS 
//         we look for relational, logical, and parantheses
//     prefix - tokens that aren't in order
//     postfix - tokens that are in order

// */

// #ifndef RPN_H
// #define RPN_H
// #include <iostream>
// #include <iomanip>

// #include "../table.h"
// #include "../typedefs.h"
// #include "../../token/token.h"
// #include "../../stackqueue/queue.h"
// #include "../../stackqueue/stack.h"
// #include "../../token/solution.h"
// #include "../../bplustree/mmap.h"
// #include "../../bplustree/map.h"
// #include "../../bplustree/btree_array_funcs.h"

// using namespace std;

// class RPN
// {
// public:
//     // ctor:
//     RPN(Queue<Token*> queue) : postfix(queue){}
    
//     void make_solution(Table& table); // takes in arguments, brings back vector of recnos
//     friend ostream& operator <<(ostream& outs, const RPN& t)
//     {
//         return outs;
//     }
//     Solution* _intersect(Solution* sol1, Solution* sol2);
//     Solution* _union(Solution* sol1, Solution* sol2);
//     // void testing (Table& t);
// private:
//     Stack<Token*> stack;
//     void print_stack()
//     {
//         cout << stack << endl;
//     }
//     Solution solution;
//     Queue<Token*> postfix; // the buffer of tokens (organized)
// };

// void RPN::make_solution(Table& table)
// {
//     // there's always gonna be 2 tokenstr's below a relational or logical because he's not gonna break our code
//     // the queue is already ordered by using shunting yard
//     // popping from postfix queue (the buffer basically) and pushing the results to stack
//     Token* current;
//     Token* token1;
//     Token* token2;
//     vectorl solutionholder;
//     int size = postfix.size();

//     for (int i = 0; i < size; i++) 
//     {
//         current = postfix.pop(); 
//         if (current->type() == TokenType::Relational) // relational 
//         {
//             // I AM ASSUMING THERE WILL BE 2 TOKENSTR tokens in the stack once i see relational
//             token1 = stack.pop(); // condition
//             token2 = stack.pop(); // fieldname
//             cout << "condition : " <<token1->token_str() << endl;
//             cout << "fieldname : " << token2->token_str() << endl;
//             solutionholder = table.select_options(token2->token_str(), current->token_str(), token1->token_str()); // change all to strings
//         }
//         else if (current->type() == TokenType::Logical) // logical
//         {
//             // I am assuming there will be 2 solution tokens in the stack once i see logical
//             token1 = stack.pop(); 
//             token2 = stack.pop(); 
//             Solution* temp;
//             Solution* sol1 = dynamic_cast<Solution*>(token1); // dynamically casting it because i don't know any other way to make the token -> solution
//             Solution* sol2 = dynamic_cast<Solution*>(token2);

//             if (current->token_str() == "and") 
//             {
//                 temp = _intersect(sol1, sol2);
//             }
//             else if (current->token_str() == "or")
//             {
//                 temp = _union(sol1, sol2);
//             }
//         }
//         else if (current->type() == TokenType::TokenStr) // tokenstr
//         {
//             stack.push(current);
//         }
//     }
//     cout << "this is the recnos: " << endl;
//     for (int i = 0; i < solutionholder.size(); i++)
//     {
//         cout << solutionholder[i] << " ";

//     }
//     cout << endl;
//     // return Solution(solutionholder); 
// }  

// Solution* RPN::_intersect(Solution* sol1, Solution* sol2)
// {
//     vectorl vec1 = sol1->recnos;
//     vectorl vec2 = sol2->recnos;
//     vectorl vec3;
//     vectorl smallerone = which_one_is_smaller(vec1, vec2); // this is so unefficient! 
//     for (int i = 0; i < smallerone.size(); i++) 
//     {
//         if (vec1.at(i) == vec2.at(i)) 
//         {
//             vec3.push_back(vec1.at(i));
//         }
//     }
//     Solution thesolution(vec3);
//     return &thesolution;
// }

// Solution* RPN::_union(Solution* sol1, Solution* sol2)
// {
//     // adds the recnos of the two solutions 
//     vectorl vec1 = sol1->recnos;
//     vectorl vec2 = sol2->recnos;
//     for (int i = 0; i < vec2.size(); i++) 
//     {
//         if (vec1.at(i) != vec2.at(i)) // no duplicates
//         {
//             vec1.push_back(vec2.at(i));
//         }
//     }
//     Solution thesolution(vec1);
//     return &thesolution;
// }

// #endif