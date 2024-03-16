#ifndef UTILITIES_H
#define UTILITIES_H
#include <iostream>
#include <iomanip>

using namespace std;

bool file_exists(const char filename[]);
void open_fileRW(fstream& f, const char filename[]) throw(char*);
void open_fileW(fstream& f, const char filename[]);

void confuse_me();
void simple_test();

#endif

