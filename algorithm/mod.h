template <typename T, T mod>
class modint {
    public:
    T val;
    void format() { if (val < 0) { val += ((-val) / (mod) + 1) * mod; } val %= mod; }
    modint(T x) { val = x; format(); }
    modint() { val = 0; }
    void operator = (T x)  { val = x; format(); }
    void operator = (modint x) { val = x.val; }
    friend modint operator * (modint a, modint b) { return (a.val * b.val) % mod; }
    friend modint operator + (modint a, modint b) { return (a.val + b.val) % mod; }
    friend modint operator - (modint a, modint b) { return (a.val - b.val + mod) % mod; }
    void operator *= (modint n) { val = ((*this) * n).val; }
    void operator += (modint n) { val = ((*this) + n).val; }
    void operator -= (modint n) { val = ((*this) - n).val; }
    modint operator ^ (T b) {
        if (b < 0) { b += ((-b) / (mod-1) + 1) * (mod-1); } b %= (mod-1);
        modint res(1), a(val);
        while (b) { if (b & 1) { res *= a; } b >>= 1, a *= a; }
        return res;
    }
    void operator ^= (T b) { val = ((*this) ^ b).val; }
    friend modint operator / (modint a, modint b) { return (modint){a * (b ^ -1)}; }
    void operator /= (modint n) { val = ((*this) * (n ^ -1)).val; }
    modint inv() { return (*this) ^ -1; }
    typedef basic_ostream<char>& otype;
    friend otype operator << (otype &out, modint x) { cout << x.val; return out; }
    typedef basic_istream<char>& itype;
    friend itype operator >> (itype &in, modint x) { cin >> x.val; return in; }
};
