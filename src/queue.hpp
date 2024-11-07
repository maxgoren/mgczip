#ifndef queue_hpp
#define queue_hpp

template <class T>
class queue {
    private:
        struct qnode {
            T info;
            qnode* next;
            qnode(T i) : info(i), next(nullptr) { }
        };
        using link = qnode*;
        link head;
        link tail;
        int count;
        inline void init() {
            head = nullptr;
            tail = nullptr;
            count = 0;
        }
    public:
        queue() {
            init();
        }
        queue(const queue& q) {
            init();
            for (link it = q.head; it != nullptr; it = it->next)
                push(it->info);
        }
        ~queue() {
            clear();
        }
        int size() {
            return count;
        }
        bool empty() {
            return head == nullptr;
        }
        void push(T info) {
            link x = new qnode(info);
            if (empty()) {
                head = x;
            } else {
                tail->next = x;
            }
            tail = x;
            count++;
        }
        T& front() {
            return head->info;
        }
        T pop() {
            T ret = head->info;
            link x = head;
            head = head->next;
            delete x;
            count--;
            return ret;
        }
        void clear() {
            while (!empty()) pop();
        }
        queue& operator=(const queue& q) {
            if (this != q) {
                init();
                for (link it = q.head; it != nullptr; it = it->next)
                    push(it->info);
            }
            return *this;
        }
};


#endif