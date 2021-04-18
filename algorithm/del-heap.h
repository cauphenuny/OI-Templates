template <typename T>
class deletable_priority_queue {
    priority_queue<T> _main, _del;
    int siz;
    void clear() {
        while (_main.size() && _del.size() && (_main.top() == _del.top())) _main.pop(), _del.pop();
    }
    public:
    void push(T x) { _main.push(x), siz++; }
    void del(T x) {
        _del.push(x), siz--;
    }
    void pop() { _del.push(_main.top()), siz--; }
    bool empty() {
        return siz == 0;
    }
    int size() {
        return siz;
    }
    T top() {
        clear();
        return _main.top();
    }
};

