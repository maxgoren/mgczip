#ifndef hashmap_hpp
#define hashmap_hpp
#include "hashtable.hpp"
#include "kvpair.hpp"

template <class K, class V> struct Hash<KVPair<K,V>> {
    std::size_t operator()(const KVPair<K,V>& kvp) {
        return Hash<K>()(kvp.key());
    }
};

template <class K, class V>
class HashMap {
    private:
        using entry = KVPair<K,V>;
        HashTable<entry> table;
    public:
        HashMap() {

        }
        int size() const {
            return table.size();
        }
        bool empty() const {
            return table.empty();
        }
        void insert(K key, V value) {
            table.insert(entry(key, value));
        }
        void erase(K key) {
            entry tmp(key, V());
            table.erase(tmp);
        }
        KVPair<K,V>& find(K key) {
            entry tmp(key, V());
            return table.search(tmp);
        }
        bool contains(K key) {
            entry tmp(key, V());
            return table.contains(tmp);
        }
        HTIterator<entry> iterator() {
            return table.iterator();
        }
        HTIterator<entry> begin() {
            return table.begin();
        }
        HTIterator<entry> end() {
            return table.end();
        }
        V& operator[](K key) {
            if (!contains(key)) {
                entry kvp(key, V());
                table.insert(kvp);
            }
            return find(key).value();
        }
};


#endif