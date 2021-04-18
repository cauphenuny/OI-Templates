#include <utility>
#include <cstring>
#include <cstdio>
#include <cctype>
#include <list>

namespace poly {

using u64 = uint64_t;
using u32 = uint32_t;
using std::forward;
struct poly_t;

const u64 mod = 998244353;
const u64 g = 3;

int *log2 = nullptr, *rev = nullptr, rev_limit = 1;
u32 **w;

inline u64 power(u64 a, int b);
inline void swap(u64 &a, u64 &b);
inline void reverse(u64* p1, u64* p2);
inline void read(u64 &a);
inline void init(int n);
inline void fill(poly_t &dest, const poly_t &src, int len);
inline void cut(poly_t &src, int len);

struct poly_t {
    u64 *f; int siz, deg;

    inline void read(int);
    inline void print(int) const;

    poly_t(int); poly_t(int, int);
    poly_t(const poly_t &); poly_t(poly_t &&);
    ~poly_t() { delete[] f; }
    void resize(int);
    void copy(const poly_t &);
    poly_t& operator = (const poly_t &);
    poly_t& operator = (poly_t &&);
    u64& operator [] (const int &x) { return f[x]; }
    u64 operator [] (const int &x) const { return f[x]; }

    inline void ntt(int);
    inline void dft(int);
    inline void idft(int);
    poly_t inv();
};

// -------------------------- poly_t start ----------------------------

#define cp(dest, f, cnt) memcpy(dest, f, cnt * sizeof(u64))
inline void poly_t::copy(const poly_t &p) {
    deg = p.deg, siz = p.siz, f = new u64[siz](); cp(f, p.f, siz);
}
void poly_t::resize(int c) {
    if (c <= siz) return;
    u64 *f0 = f; f = new u64[c](); cp(f, f0, siz);
    siz = c, delete[] f0;
}
#undef cp

poly_t::poly_t(int s) { wlog("new"); siz = s; f = new u64[siz](); }
poly_t::poly_t(int s, int n) { wlog("new"); siz = s; f = new u64[siz](); read(n); }
poly_t::poly_t(const poly_t& p) { wlog("copy"); copy(p); }
poly_t::poly_t(poly_t &&p) { wlog("move"); deg = p.deg, siz = p.siz, f = p.f, p.f = nullptr; }
poly_t& poly_t::operator = (const poly_t& p) { wlog("copy");
    if (this != &p) { delete[] f; copy(p); }
    return *this;
}
poly_t& poly_t::operator = (poly_t &&p) { wlog("move");
    if (this != &p) {
        delete[] f; siz = p.siz, f = p.f, p.f = nullptr;
    }
    return *this;
}

template<typename t1, typename t2>
poly_t operator + (t1 && a, t2 && b) {
    poly_t c(std::forward<t1>(a));
    for (int i = 0; i < c.siz; i++) c[i] = (c[i] + b[i]) % mod;
    return c;
}

template<typename t1, typename t2>
poly_t operator - (t1 && a, t2 && b) {
    poly_t c(std::forward<t1>(a));
    for (int i = 0; i < c.siz; i++) c[i] = (c[i] - b[i] + mod) % mod;
    return c;
}

template<typename t1, typename t2>
poly_t operator * (t1 &&p1, t2 &&p2) {
    poly_t a(std::forward<t1>(p1)), b(std::forward<t2>(p2));
    //wlog("dft");
    int n = std::max(p1.siz, p2.siz);
    a.dft(n), b.dft(n);
    poly_t c(a.siz);
    for (int i = 0; i < c.siz; i++) (c[i] = a[i] * b[i]) %= mod;
    //wlog("idft");
    c.idft(n);
    //wlog("complete");
    return c;
}

inline void poly_t::read(int n) {
    deg = n;
    for (int i = 0; i < n; i++) { poly::read(f[i]); }
}
inline void poly_t::print(int n) const {
    for (int i = 0; i < n; i++) { printf("%lu ", f[i]); } putchar('\n');
}

inline void poly_t::ntt(int n) {
    int *rev = new int[n];
    for (int i = 0; i < n; i++) {
        rev[i] = (rev[i >> 1] >> 1) | ((i & 1) * (n >> 1));
    }
    for (int i = 0; i < n; i++) {
        if (rev[i] < i) {
            poly::swap(f[i], f[rev[i]]);
        }
    }
    u64 t1; u32 t2, *g;
    for (int k = 1, i, j, l, x = 1; k < n; k <<= 1, x++) {
        l = k << 1;
        for (i = 0; i < n; i += l) {
            for (j = i, g = w[x]; j < i + k; j++, g++) {
                t1 = f[j], t2 = (((*g) * f[j + k])) % mod;
                f[j] = t1 + t2;
                f[j + k] = t1 - t2 + mod;
            }
        }
        if (k & (1 << 16)) {
            wlog("mod");
            for (int i = 0; i < n; i++) {
                f[i] %= mod;
            }
        }
    }
    delete[] rev;
}

inline void poly_t::dft(int n) {
    ntt(n);
    for (int i = 0; i < n; i++) f[i] %= mod;
}

inline void poly_t::idft(int n) {
    poly::reverse(f + 1, f + n);
    ntt(n);
    u64 inv = poly::power(n, mod - 2);
    for (int i = 0; i < n; i++) (f[i] *= inv) %= mod;
}

poly_t inv(const poly_t &p) {
    wlog("start inv");
    poly_t f(p), g(p.siz);
    g[0] = poly::power(f[0], mod - 2);
    std::list<int> len;
    int t = f.deg;
    while (t != 1) len.push_front(t), t = (t + 1) >> 1;
    for (auto l : len) {
        debug(l);
        g.print((l + 1) >> 1);
        int k = 1;
        while (k < l) k <<= 1;
        wlog("fill"); poly::fill(f, p, k); // copy
        wlog("dft"); g.dft(k), f.dft(k);
        wlog("calc"); for (int i = 0; i < k; i++) g[i] = g[i] * (2 - (f[i] * g[i] % mod) + mod) % mod;
        wlog("idft"); g.idft(k);
        wlog("cut"); poly::cut(g, l);
    }
    return g;
}

poly_t poly_t::inv() {
    return poly::inv(*this);
}

poly_t ln(poly_t p) {
    //todo
    return p;
}

poly_t exp(poly_t p) {
    //todo
    return p;
}

//------------------------- poly_t end -------------------------------

inline u64 power(u64 a, int b) {
    u64 res = 1;
    //debugf("power(%llu, %d) ", a, b);
    while (b) { if (b & 1) (res *= a) %= mod; b >>= 1, (a *= a) %= mod; }
    //debugf("= %llu\n", res);
    return res;
}
inline void swap(u64 &a, u64 &b) { u64 t = a; a = b, b = t; }

inline void reverse(u64* p1, u64* p2) { p2--; while (p1 < p2) { swap(*p1, *p2), p1++, p2--; } }

inline void read(u64 &a) {
    a = 0; char c = getchar(); int f = 0;
    while (!isdigit(c)) { f ^= c == '-',  c = getchar(); }
    while (isdigit(c)) { a = a * 10 + (c ^ 48),  c = getchar(); }
    a *= f ? -1 : 1;
}

inline void init(int n = 1e6) {
    delete[] rev, rev = new int[n + 10]();
    int log_limit = 1;
    while ((rev_limit << 1) <= n) rev_limit <<= 1, log_limit++;
    for (int i = 0; i < rev_limit; i++) {
        rev[i] = (rev[i >> 1] >> 1) | ((i & 1) * (n >> 1));
    }

    delete[] log2, log2 = new int[n + 10]();
    log2[0] = -1;
    for (int i = 1; i <= n; i++) log2[i] = log2[i >> 1] + 1;

    delete[] w, w = new u32*[log_limit + 10]();
    for (int i = 0; i < log_limit; i++) w[i] = new u32[1 << i]();
    for (int i = 0, l; i < log_limit; i++) {    
        l = (1 << i);
        u64 g0 = power(g, (mod - 1) / l);
        u32 *p = w[i];
        *p = 1;
        for (int j = 1; j < l; j++, p++) { *(p + 1) = *p * g0 % mod; }
    }
}

inline void cut(poly_t &src, int len) {
    //for (int i = len; i < src.siz; i++) src[i] = 0;
    memset(src.f + len, 0, sizeof(u64) * (src.siz - len));
}

inline void fill(poly_t &dest, const poly_t &src, int len) {
    //for (int i = 0; i < len; i++) dest[i] = src[i];
    wlog("copy"); memcpy(dest.f, src.f, sizeof(u64) * len);
    wlog("cut"); cut(dest, len);
}

} // namespace poly
