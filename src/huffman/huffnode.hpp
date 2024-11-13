#ifndef huffnode_hpp
#define huffnode_hpp

struct HuffmanNode {
    char symbol;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;
    HuffmanNode(char s, int p, HuffmanNode* l, HuffmanNode* r) : symbol(s), frequency(p), left(l), right(r) { }
};

using link = HuffmanNode*;

bool isLeaf(link h) {
    if (h == nullptr)
        return false;
    return (h->left == nullptr && h->right == nullptr);
}

struct HuffCmp {
    bool operator()(HuffmanNode* hna, HuffmanNode* hnb) {
        return (hna->frequency > hnb->frequency);
    }
};

#endif