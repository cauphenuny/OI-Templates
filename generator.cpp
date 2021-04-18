//author: ycp | https://ycpedef.github.io
//type: generator
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <random>
#include <chrono>
#include <functional>
#include <vector>
#include <cassert>

using namespace std;

using T = long long;

#define endline() putchar('\n')

void write(T x){
    if (x < 0) putchar('-'), x = -x;
	T y = 10, len = 1;
	while (y <= x) y *= 10, len++;
	while (len--)  y /= 10, putchar(x / y + 48), x %= y;
}

void print(const T &x) {
    write(x), putchar(' ');
}

void print(const vector<T> &a) {
    for (auto i : a) print(i);
    endline();
}

void println(const T &x) {
    write(x), endline();
}

struct range {
    T l, r;
    T size() const { return r - l + 1; }
    range(const T &x) { l = 0, r = x; }
    range(const T &_l, const T &_r) { l = _l, r = _r; }
    range(const initializer_list<T> &a) {
        l = *(a.begin());
        r = *(next(a.begin()));
    }
};

mt19937 rnd((random_device())());

T randint(const range &a) {
    uniform_int_distribution<T> distrib(a.l, a.r);
    return distrib(rnd);
}

// ======================================================== List =========================================================

vector<T> generate_list(const int &size, const range &a) {
    assert(size > 0);
    vector<T> res;
    uniform_int_distribution<T> distrib(a.l, a.r);
    for (int i = 0; i < size; i++) { res.push_back(distrib(rnd)); }
    return res;
}

vector<T> generate_list(const int &size, function<T(const vector<T>&)> generate) {
    assert(size > 0);
    vector<T> res;
    for (int i = 0; i < size; i++) { res.push_back(generate(res)); }
    return res;
}

vector<T> generate_increase_list(const int &size, const range &limit) {
    assert(limit.size() >= size);
    auto generate = [limit, size](const vector<T> &cur) -> T {
        T prev;
        if (cur.size()) prev = cur[cur.size() - 1];
        else            prev = limit.l - 1;
        T l = prev + 1;
        T r = max(prev + 1, limit.l + (T)(((cur.size() + 2) * 1.0 / size) * (limit.r - limit.l)));
        r = min(r, (T)(limit.r - (size - cur.size() - 1)));
        return randint({l, r});
    };
    vector<T> res = generate_list(size, generate);
    return res;
}

vector<T> generate_decrease_list(const int &size, const range &limit) {
    vector<T> res = generate_increase_list(size, limit);
    reverse(res.begin(), res.end());
    return res;
}

vector<T> generate_non_decrease_list(const int &size, const range &limit) {
    vector<T> res = generate_list(size, limit);
    sort(res.begin(), res.end());
    return res;
}

vector<T> generate_non_increase_list(const int &size, const range &limit) {
    vector<T> res = generate_non_decrease_list(size, limit);
    reverse(res.begin(), res.end());
    return res;
}

vector<T> generate_unique_list(const int &size, const range &limit) {
    vector<T> res = generate_increase_list(size, limit);
    shuffle(res.begin(), res.end(), default_random_engine((random_device())()));
    return res;
}

// ======================================================== Graph =======================================================

struct edge {
    int from, to;
    T val;
    edge(int x, int y, T v = 0): from(x), to(y), val(v) {}
};

struct tree {
    bool has_val;
    int n;
    vector<edge> edges;
    tree();
    tree(const int &size); 
    tree(const int &size, const range &); 
};

tree generate_random_tree(const int &size) {
    tree res;
    res.n = size;
    res.has_val = 0;
    for (int i = 2; i <= size; i++) {
        int j = randint({1, i - 1});
        res.edges.push_back(edge(j, i));
    }
    return res;
}

tree generate_random_tree_with_val(const int &size, const range &val_range) {
    tree res;
    res.n = size;
    res.has_val = 1;
    uniform_int_distribution<T> getval(val_range.l, val_range.r);
    for (int i = 2; i <= size; i++) {
        int j = randint({1, i - 1});
        res.edges.push_back(edge(j, i, getval(rnd)));
    }
    return res;
}

tree::tree() {
    has_val = n = 0;
}

tree::tree(const int &size) {
    auto t = generate_random_tree(size);
    has_val = t.has_val, n = t.n;
    edges = t.edges;
}

tree::tree(const int &size, const range &val_range) {
    auto t = generate_random_tree_with_val(size, val_range);
    has_val = t.has_val, n = t.n;
    edges = t.edges;
}

void print(tree t) {
    println(t.n);
    shuffle(t.edges.begin(), t.edges.end(), default_random_engine((random_device())()));
    for (auto e : t.edges) {
        if (randint({0, 1})) {
            swap(e.from, e.to);
        }
        if (t.has_val) print(e.from), print(e.to), println(e.val);
        else print(e.from), println(e.to);
    }
}

int n, m, q;

int main() {
    return 0;
}

