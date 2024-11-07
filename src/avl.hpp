#ifndef avl_hpp
#define avl_hpp
#include <iostream>
using std::max;

template <class T>
struct avlnode {
    T key;
    int ht;
    int sz;
    avlnode* left;
    avlnode* right;
    avlnode(T k) : key(k), ht(0), sz(1), left(nullptr), right(nullptr) { }
};

template <class T>
class AVLRankIterator {
    private:
        using link = avlnode<T>*;
        link root;
        int rank;
        link select(avlnode<T>* h, int rank) {
            if (h == nullptr)
                return nullptr;
            int curr = h->left == nullptr ? 0:h->left->sz;
            if (curr > rank) return select(h->left, rank);
            if (curr < rank) return select(h->right, rank - curr - 1);
            return h;
        }
    public:
        AVLRankIterator(link rt, int rk) : root(rt), rank(rk) {
            
        }
        T& get() {
            return select(root, rank)->key;
        }
        bool done() {
            return select(root, rank) == nullptr;
        }
        void next() {
            rank++;
        }
};

template <class T>
class AVL {
    private:
        using node_ptr = avlnode<T>*;
        node_ptr root;
        int count;
        int setheight(node_ptr h);
        int height(node_ptr h);
        int balanceFactor(node_ptr h);
        int bf(node_ptr h);
        int setsize(node_ptr h);
        int size(node_ptr h);
        T& select(node_ptr h, int rank);
        int rank(node_ptr h, T info);
        node_ptr updateRankAndBalance(node_ptr h);
        node_ptr rotL(node_ptr h);
        node_ptr rotR(node_ptr h);
        node_ptr rotLR(node_ptr h);
        node_ptr rotRL(node_ptr h);
        node_ptr leftHeavy(node_ptr h);
        node_ptr rightHeavy(node_ptr h);
        node_ptr balance(node_ptr h);
        node_ptr min(node_ptr h);
        node_ptr eraseMin(node_ptr h);
        node_ptr put(node_ptr h, T info);
        node_ptr erase(node_ptr h, T info);
        node_ptr search(node_ptr h, T info);
        node_ptr clone(node_ptr h);
        void clean_up(node_ptr h);
        bool isBalanced(node_ptr h);
        T nilValue;
    public:
        AVL();
        AVL(const AVL& avl);
        ~AVL();
        int size() const;
        bool empty() const;
        void insert(T info);
        void erase(T info);
        T& search(T info);
        int rank(T info);
        T& get(int rank);
        bool isAVL();
        AVLRankIterator<T> iterator();
        AVL& operator=(const AVL& avl);
};

template <class T>
AVL<T>::AVL() {
    count = 0;
    root = nullptr;
}

template <class T>
AVL<T>::AVL(const AVL& avl) {
    root = nullptr;
    count = 0;
    root = clone(avl.root);
}

template <class T>
AVL<T>::~AVL() {
    clean_up(root);
}

template <class T>
AVL<T>& AVL<T>::operator=(const AVL& avl) {
    if (this != &avl) {
        clean_up(root);
        root = nullptr;
        count = 0;
        root = clone(avl.root);
    }
    return *this;
}

template <class T>
int AVL<T>::size() const {
    return count;
}

template <class T>
bool AVL<T>::empty() const {
    return root == nullptr;
}

template <class T>
void AVL<T>::insert(T info) {
    root = put(root, info);
}

template <class T>
void AVL<T>::erase(T info) {
    root = erase(root, info);
}

template <class T>
T& AVL<T>::search(T info) {
    avlnode<T>* resultnode = search(root, info);
    return resultnode == nullptr ? nilValue:resultnode->key;
}

template <class T>
int AVL<T>::rank(T info) {
    return rank(root, info);
}

template <class T>
T& AVL<T>::get(int rank) {
    return select(root, rank);
}

template <class T>
bool AVL<T>::isAVL() {
    return isBalanced(root);
}

template <class T>
AVLRankIterator<T> AVL<T>::iterator() {
    return AVLRankIterator<T>(root, 0);
}

template <class T>
void AVL<T>::clean_up(avlnode<T>* h) {
    if (h != nullptr) {
        clean_up(h->left);
        clean_up(h->right);
        delete h;
    }
}


template <class T>
avlnode<T>* AVL<T>::clone(avlnode<T>* h) {
    if (h == nullptr) return h;
    avlnode<T>* t = new avlnode<T>(h->key);
    t->ht = h->ht;
    t->sz = h->sz;
    t->left = clone(h->left);
    t->right = clone(h->right);
    return t;
}

template <class T>
int AVL<T>::setsize(avlnode<T>* h) {
    if (h == nullptr)
        return 0;
    return 1 + setsize(h->left) + setsize(h->right);
}

template <class T>
int AVL<T>::setheight(avlnode<T>* h) {
    if (h == nullptr) {
        return 0;
    }
    return 1 + max(setheight(h->left), setheight(h->right));
}

template <class T>
int AVL<T>::height(avlnode<T>* h) {
    return h == nullptr ? -1:h->ht;
}

template <class T>
int AVL<T>::size(avlnode<T>* h) {
    return h == nullptr ? 0:h->sz;
}

template <class T>
int AVL<T>::balanceFactor(avlnode<T>* h) {
    return height(h->left) - height(h->right);
}

template <class T>
avlnode<T>* AVL<T>::updateRankAndBalance(avlnode<T>* h) {
    h->sz = setsize(h);
    h->ht = setheight(h);
    return h;
}

template <class T>
int AVL<T>::bf(avlnode<T>* h) {
    return balanceFactor(h);
}

template <class T>
avlnode<T>* AVL<T>::rotR(avlnode<T>* h) {
    avlnode<T>* x = h->left;
    h->left = x->right;
    x->right = h;
    h = updateRankAndBalance(h);
    x = updateRankAndBalance(x);
    return x;
}

template <class T>
avlnode<T>* AVL<T>::rotL(avlnode<T>* h) {
    avlnode<T>* x = h->right;
    h->right = x->left;
    x->left = h;
    h = updateRankAndBalance(h);
    x = updateRankAndBalance(x);
    return x;
}

template <class T>
avlnode<T>* AVL<T>::rotRL(avlnode<T>* h) {
    h->right = rotR(h->right);
    return rotL(h);
}

template <class T>
avlnode<T>* AVL<T>::rotLR(avlnode<T>* h) {
    h->left = rotL(h->left);
    return rotR(h);
}

template <class T>
avlnode<T>* AVL<T>::rightHeavy(avlnode<T>* x) {
    if (bf(x->right) > 0) {
        x = rotRL(x);
    } else {
        x = rotL(x);
    }
    return x;
}

template <class T>
avlnode<T>* AVL<T>::leftHeavy(avlnode<T>* x) {
    if (bf(x->left) < 0) {
        x = rotLR(x);
    } else {
        x = rotR(x);
    }
    return x;
}

template <class T>
avlnode<T>* AVL<T>::balance(avlnode<T>* x) {
    switch (bf(x)) {
        case -2: return rightHeavy(x);
        case 2:  return leftHeavy(x);
        default: break;
    }
    return x;
}


template <class T>
avlnode<T>* AVL<T>::min(avlnode<T>* h) {
    avlnode<T>* x = h;
    while (x->left != nullptr) {
        x = x->left;
    }
    return x;
}

template <class T>
avlnode<T>* AVL<T>::eraseMin(avlnode<T>* h) {
    if (h == nullptr)
        return nullptr;
    if (h->left == nullptr)
        return h->right;
    h->left = eraseMin(h->left);
    return balance(h);
}

template <class T>
avlnode<T>* AVL<T>::erase(avlnode<T>* h, T v) {
    if (h == nullptr)
        return h;
    if (v < h->key) {
        h->left = erase(h->left, v);
    } else if (v > h->key) {
        h->right = erase(h->right, v);
    } else {
        avlnode<T>* t = h;
        if (h->right == nullptr) {
            h = h->left;
        } else {
            h = min(t->right);
            h->right = eraseMin(t->right);
            h->left = t->left;
        }
        delete t;
    }
    return balance(h);
}

template <class T>
avlnode<T>* AVL<T>::search(avlnode<T>* h, T key) {
    if (h == nullptr)
        return h;
    if (key == h->key)
        return h;
    if (key < h->key) 
         return search(h->left, key);
    else return search(h->right, key);
}

template <class T>
avlnode<T>* AVL<T>::put(avlnode<T>* h, T v) {
    if (h == nullptr) {
        count++;
        return new avlnode<T>(v);
    }
    if (v < h->key) h->left = put(h->left, v);
    else h->right = put(h->right, v);
    h = updateRankAndBalance(h);
    return balance(h);
}

template <class T>
bool AVL<T>::isBalanced(avlnode<T>* h) {
    if (h != nullptr) {
        if (bf(h) < -1) return false;
        if (bf(h) > 1) return false;
        return isBalanced(h->left) && isBalanced(h->right);
    }
    return true;
}

template <class T>
T& AVL<T>::select(avlnode<T>* h, int rank) {
    if (h == nullptr)
        return nilValue;
    int curr = size(h->left);
    if (curr > rank) return select(h->left, rank);
    if (curr < rank) return select(h->right, rank - curr - 1);
    return h->key;
}

template <class T>
int AVL<T>::rank(avlnode<T>* h, T info) {
    if (h == nullptr)
        return -1;
    if (info < h->key) return rank(h->left, info);
    else if (info > h->key) return 1 + rank(h->right, info) + size(h->left);
    else return size(h->left);
}

#endif