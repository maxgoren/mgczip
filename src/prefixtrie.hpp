#ifndef prefixtrie_hpp
#define prefixtrie_hpp
#include "hashmap.hpp"
using namespace std;

template <class T>
class PrefixTrie {
    private:
        struct trienode {
            bool eos;
            T value;
            HashMap<char, trienode*> next;
        };
        trienode* root;
        int count;
        T nilInfo;
    public:
        PrefixTrie() {
            count = 0;
            root = new trienode;
            root->eos = false;
        }
        ~PrefixTrie() {

        }
        void insert(string key, T value) {
            trienode* x = root;
            for (char c : key) {
                if (x->next.find(c).empty()) {
                    x->next[c] = new trienode;
                }
                x = x->next[c];
            }
            x->eos = true;
            x->value = value;
            count++;
        }
        T& get(string key) {
            trienode* x = root;
            for (char c : key) {
                if ((x->next).find(c).empty()) 
                    return nilInfo;
                x = x->next[c];
            }
            return x->eos ? x->value:nilInfo;
        }
        string longestPrefix(string key) {
            int d = 0, length = 0;
            trienode* x = root;
            string prefix;
            for (char c : key) {
                if (x->next.find(c).empty()) {
                    return prefix;
                }
                prefix.push_back(c);
                x = x->next[c];
            }
            return prefix;
        }
};

#endif