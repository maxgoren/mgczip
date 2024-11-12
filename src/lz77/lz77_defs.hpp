#ifndef lz77_defs_hpp
#define lz77_defs_hpp
#include <iostream>
#include <vector>
using namespace std;


int window_size = 4096;
int lookahead_size = 256;

struct LZTriple {
    int offset;
    int length;
    char symbol;
    LZTriple(int o = 0, int l = 0, char c = '\0') : offset(o), length(l), symbol(c) { }
};

void print(LZTriple lzt) {
    cout<<"("<<lzt.offset<<","<<lzt.length<<","<<lzt.symbol<<") ";
}

#endif