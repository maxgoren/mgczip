#ifndef avlmap_hpp
#define avlmap_hpp
#include "avl.hpp"
#include "kvpair.hpp"


template <class K, class V>
class AVLMap {
    private:
        using info_type = KVPair<K,V>;
        using Iterator = AVLRankIterator<info_type>;
        AVL<info_type> tree;
    public:
        AVLMap();
        bool empty() const;
        int size() const;
        void insert(K key, V value);
        void erase(K key);
        info_type& find(K key);
        info_type& get(int rank);
        Iterator iterator();
        bool isAVL();
        V& operator[](K key);
};

template <class K, class V>
AVLMap<K,V>::AVLMap() {

}

template <class K, class V>
int AVLMap<K,V>::size() const {
    return tree.size();
}

template <class K, class V>
bool AVLMap<K,V>::empty() const {
    return tree.empty();
}

template <class K, class V>
void AVLMap<K,V>::insert(K key, V value) {
    KVPair<K,V> kvp(key, value);
    tree.insert(kvp);
}

template <class K, class V>
void AVLMap<K,V>::erase(K key) {
    KVPair<K,V> kvp(key, V());
    tree.erase(kvp);
}

template <class K, class V>
KVPair<K,V>& AVLMap<K,V>::find(K key) {
    KVPair<K,V> kvp(key, V());
    return tree.search(kvp);
}

template <class K, class V>
KVPair<K,V>& AVLMap<K,V>::get(int rank) {
    return tree.get(rank);
}

template <class K, class V>
bool AVLMap<K,V>::isAVL() {
    return tree.isAVL();
}

template <class K, class V>
AVLRankIterator<KVPair<K,V>> AVLMap<K,V>::iterator() {
    return tree.iterator();
}

template <class K, class V>
V& AVLMap<K,V>::operator[](K key) {
    if (find(key).empty())
        tree.insert(KVPair<K,V>(key,V()));
    return find(key).value();
}

#endif