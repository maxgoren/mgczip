#ifndef hash_hpp
#define hash_hpp
#include <iostream>

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

#endif