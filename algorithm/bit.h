template <typename T, int n>
class binary_indexed_tree {
    T c[n + 10]; public:
    void add(int p, T v) { p++; for (; p <= n; p += p & -p) c[p] += v; }
    T query(int p) { p++; T v = 0; for (; p; p -= p & -p) v += c[p]; return v; }
    T range(int l, int r) { return query(r) - query(l - 1); }
};
