#ifndef huffnode_hpp
#define huffnode_hpp
#include "../queue.hpp"

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

struct Levelorder {
    void operator()(link node) {
        bfs(node);
    }
    void bfs(link h) {
        queue<link> fq;
        fq.push(h);
        while (!fq.empty()) {
            int nc = fq.size();
            while (nc > 0) {
                link t = fq.front();
                fq.pop();
                nc--;
                if (t != nullptr) {
                    cout<<"["<<t->symbol<<":("<<t->frequency<<")]";
                    fq.push(t->left);
                    fq.push(t->right);
                }
            }
            cout<<endl;
        }
    }
};

#endif