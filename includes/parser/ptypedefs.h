#ifndef PTYPEDEF_H
#define PTYPEDEF_H
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

#include "../bplustree/mmap.h"
#include "../bplustree/map.h"

using namespace std;

typedef MMap<string, string> mmap_ss;
typedef Map<string, int> map_si;

#endif