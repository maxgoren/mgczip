#ifndef hashtable_hpp
#define hashtable_hpp
#include "kvpair.hpp"

unsigned long stringhash(const char* str) {
    unsigned long h = 1337;
    for (int i = 0; str[i]; i++) {
        h = (h << 27) | (h >> 5);
        h ^= h*(long)str[i];
    }
    return h;
}

template <class T>
struct Hash {
    virtual std::size_t operator()(T);
};


template <> struct Hash<int> {
    std::size_t operator()(int data) {
        return std::size_t(data);
    }
};

template <> struct Hash<unsigned int> {
    std::size_t operator()(unsigned int data) {
        return std::size_t(data);
    }
};

template <> struct Hash<long> {
    std::size_t operator()(long data) {
        return std::size_t(data);
    }
};

template <> struct Hash<unsigned long> {
    std::size_t operator()(unsigned long data) {
        return std::size_t(data);
    }
};

template <> struct Hash<char> {
    std::size_t operator()(char data) {
        return std::size_t(data);
    }
};

template <> struct Hash<unsigned char> {
    std::size_t operator()(unsigned char data) {
        return std::size_t(data);
    }
};

template <> struct Hash<char*> {
    std::size_t operator()(char* data) {
        return std::size_t(stringhash(data));
    }
};

template <> struct Hash<std::string> {
    std::size_t operator()(std::string str) {
        return std::size_t(stringhash(str.c_str()));
    }
};

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
        HTIterator(Item<T>* ptr, int sp, int max) : curr(ptr), offset(sp), maxn(max) { 
                while (offset < maxn && !curr[offset].inuse) { 
                    offset++; 
                }
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
        T& operator*() {
            return get();
        }
        HTIterator operator++() {
            next();
            return *this;
        }
        HTIterator operator++(int) {
            HTIterator tmp = *this;
            ++*this;
            return tmp;
        }
        bool operator==(const HTIterator& it) const {
            return curr == it.curr && offset == it.offset;
        }
        bool operator!=(const HTIterator& it) const {
            return !(*this==it);
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
            //delete table;
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
            return HTIterator<T>(table, 0, maxn);
        }
        HTIterator<T> iterator() const {
            return HTIterator<T>(table, 0, maxn);
        }
        HTIterator<T> begin() {
            return HTIterator<T>(table, 0, maxn);
        }
        HTIterator<T> end() {
            return HTIterator<T>(table, maxn, maxn);
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