#ifndef huffnode_hpp
#define huffnode_hpp
#include "queue.hpp"

struct HuffmanNode {
    char symbol;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;
    HuffmanNode(char s, int p, HuffmanNode* l, HuffmanNode* r) : symbol(s), frequency(p), left(l), right(r) { }
    
};

struct HuffCmp {
    bool operator()(HuffmanNode* hna, HuffmanNode* hnb) {
        return (hna->frequency > hnb->frequency);
    }
};

using link = HuffmanNode*;

#endif