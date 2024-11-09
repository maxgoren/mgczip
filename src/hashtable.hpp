#ifndef hashtable_hpp
#define hashtable_hpp
#include "hash.hpp"
#include "kvpair.hpp"

template <class T>
struct Item {
    T info;
    bool inuse;
    Item(T i) : info(i), inuse(true) { }
    Item() : inuse(false) { }
    void clear() {
        info = T();
        inuse = false;
    }
};

template <class T>
class HTIterator {
    private:
        Item<T>* curr;
        int offset;
        int maxn;
    public:
        HTIterator(Item<T>* ptr, int max) : curr(ptr), offset(0), maxn(max) { 
            while (offset < maxn && !curr[offset].inuse) { offset++; }
        }
        T& get() {
            return curr[offset].info;
        }
        void next() {
            do {
                offset++;
            } while (offset < maxn && !curr[offset].inuse);
        }
        bool done() {
            return offset == maxn;
        }
};


template <class T>
class HashTable {
    private:
        Item<T>* table;
        int n;
        int maxn;
        void grow() {
            HashTable<T> tmp(2*maxn);
            for (int i = 0; i < maxn; i++)
                if (table[i].inuse)
                    tmp.insert(table[i].info);
            table = tmp.table;
            maxn *= 2;
        }
    public:
        HashTable(int sz = 5) {
            n = 0;
            maxn = sz;
            table = new Item<T>[maxn];
        }
        HashTable(const HashTable& ht) {
            n = 0;
            maxn = ht.maxn;
            table = new Item<T>[maxn];
            for (auto it = ht.iterator(); !it.done(); it.next()) {
                insert(it.get());
            }
        }
        ~HashTable() {

        }
        int size() const {
            return n;
        }
        bool empty() const {
            return n == 0;
        }
        void insert(T info) {
            unsigned int idx = Hash<T>()(info) % maxn;
            while (table[idx].inuse && table[idx].info != info) idx = (idx + 1) % maxn;
            table[idx] = info;
            n++;
            if (n >= maxn/2) { grow(); }
        }
        T& search(T info) {
            unsigned int idx = Hash<T>()(info) % maxn;
            while (table[idx].inuse && table[idx].info != info) idx = (idx + 1) % maxn;
            return table[idx].info;
        }
        bool contains(T info) {
            return search(info) == info;
        }
        void erase(T info) {
            if (!contains(info))
                return;
            unsigned int idx = Hash<T>()(info) % maxn;
            while (table[idx].inuse && table[idx].info != info) idx = (idx + 1) % maxn;
            table[idx].clear();
            idx = (idx+1) % maxn;
            while (table[idx].inuse) {
                T toreplace = table[idx].info;
                table[idx].clear();
                n--;
                insert(toreplace);
                idx = (idx+1) % maxn;
            }
            n--;
        }
        HTIterator<T> iterator() {
            return HTIterator<T>(table, maxn);
        }
        HashTable& operator=(const HashTable& ht) {
            if (this != &ht) {
                n = 0;
                maxn = ht.maxn;
                table = new Item<T>[maxn];
                for (auto it = ht.iterator(); !it.done(); it.next()) {
                    insert(it.get());
                }
            }
            return ht;
        }
};

#endif