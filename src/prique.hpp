#ifndef prique_hpp
#define prique_hpp
#include <iostream>
using namespace std;

template <class T, typename cmp = std::greater<T>>
class MinHeap {
    private:
        T* data;
        int n;
        int maxn;
        void exch(int l, int r) {
            T tmp = data[l];
            data[l] = data[r];
            data[r] = tmp;
        }
    public:
        MinHeap() {
            n = 0;
            maxn = 1024;
            data = new T[maxn];
        }
        ~MinHeap() {
            delete [] data;
        }
        bool empty() {
            return n == 0;
        }
        int size() {
            return n;
        }
        void push(T x) {
            data[++n] = x;
            int m = n;
            while (m > 1 && cmp()(data[m/2], data[m])) {
                exch(m/2, m);
                m /= 2;
            }
        }
        T& peek() { 
            return data[1];
        }      
        T pop() {
            T ret = data[1];
            exch(1, n--);
            int x = 1;
            while (2*x <= n) {
                int child = 2*x;
                if (child < n && cmp()(data[child], data[child+1])) child++;
                if (!cmp()(data[x], data[child])) break;
                exch(x, child);
                x = child;
            }
            return ret;
        }
};

#endif