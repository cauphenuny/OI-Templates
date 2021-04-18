namespace List {

template<typename T>
class list {
    private:
    struct Node {
        T val;
        Node *prev, *next;
    } *head, *tail;
    int siz;
    Node* newnode(T val) {
        Node* cur = new Node;
        cur->prev = cur->next = cur;
        cur->val = val;
        return cur;
    }

    public:
    typedef Node* iterator;
    int size() { return siz; }
    iterator begin() { return head->next; }
    iterator end() { return tail; }
    iterator insert(iterator pos, T val) {
        iterator cur = newnode(val);
        siz++;
        pos->next->prev = cur;
        cur->next = pos->next;
        pos->next = cur;
        cur->prev = pos;
        return cur;
    }
    iterator push_front(T val) {
        return insert(head, val);
    }
    iterator push_back(T val) {
        return insert(tail->prev, val);
    }
    iterator erase(iterator pos) {
        siz--;
        iterator nxt = pos->next;
        pos->prev->next = pos->next;
        pos->next->prev = pos->prev;
        delete pos;
        return nxt;
    }
    iterator erase(iterator begin, iterator end) {
        while (begin != end) {
            begin = erase(begin);
        }
    }
    void cat(list<T> l) {
        iterator pre = tail->prev;
        tail = l.begin();
        tail->prev = pre;
        pre->next = tail;
        tail = l.end();
    }
#ifdef LIST_DEBUG
    void print() {
        debugf("---- print ----\n");
        for (auto it = begin(); it != end(); it = it->next) {
            debugf("node(%d, %d): %d\n", it->val.x, it->val.y - 4, it->val.v);
        }
        debugf("---- done  ----\n");
    }
#endif
    list() {
        head = new Node;
        tail = new Node;
        head->prev = head;
        head->next = tail;
        tail->prev = head;
        tail->next = tail;
        siz = 0;
    }
};

}
