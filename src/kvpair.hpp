#ifndef kvpair_hpp
#define kvpair_hpp

template <class K, class V>
class KVPair {
    private:
        K _key;
        V _value;
        bool occupied;
    public:
        KVPair(K k, V v) : _key(k), _value(v), occupied(true) { }
        KVPair() : occupied(false) { }
        K key() const {
            return _key;
        }
        V& value() {
            return _value;
        }
        bool empty() {
            return occupied == false;
        }
        void clear() {
            occupied = false;
        }
        bool operator==(const KVPair<K,V>& kvp) const {
            return _key == kvp._key;
        } 
        bool operator!=(const KVPair<K,V>& kvp) const {
            return !(_key == kvp._key);
        }
        bool operator<(const KVPair<K,V>& kvp) const {
            return _key < kvp._key;
        }
        bool operator>(const KVPair<K,V>& kvp) const {
            return kvp._key < _key;
        }
};

template <class K, class V>
bool operator<(const K v, const KVPair<K,V>& kvp) {
    return v < kvp.key();
}

template <class K, class V>
bool operator>(const K v, const KVPair<K,V>& kvp) {
    return v > kvp.key();
}

#endif