/* 区间加减修改，单点查询 K-D tree，维护了历史最小值
 * usage:
 * kdt::kd_tree<2> tree;
 * kdt::kd_tree<2>::kd_val x[MAXN];
 * for (int i = 1; i <= n; i++) x[i].pos = {%d, %d}, x[i].val = %d;
 * tree.init(x + 1, x + n + 1);
 */

namespace kdt {

constexpr int inf = 0x3f3f3f3f;

using array = initializer_list<int>;

template<class T> bool chkmax(T &a, const T b) { return b > a ? (a = b, 1) : 0; }
template<class T> bool chkmin(T &a, const T b) { return b < a ? (a = b, 1) : 0; }

template <const int d_size>
struct kd_tree {

    struct kd_pos {
        int pos[d_size];
        int  operator [] (const int x) const { return pos[x]; }
        int &operator [] (const int x) { return pos[x]; }
        inline bool operator == (const kd_pos &v) const {
            for (int i = 0; i < d_size; i++) if (v.pos[i] != pos[i]) return 0;
            return 1;
        }
        inline bool operator <= (const kd_pos &v) const {
            for (int i = 0; i < d_size; i++) if (pos[i] > v.pos[i]) return 0;
            return 1;
        }
        kd_pos(array a) { int j = 0; for (auto i : a) { pos[j++] = i; } }
        kd_pos(int x) { for (int i = 0; i < d_size; i++) pos[i] = x; }
        kd_pos() : kd_pos(0) {}
        void print() { debugf("{%lld, %lld}", pos[0], pos[1]); }
    };
    struct kd_val {
        kd_pos pos;
        int val;
        int &operator [] (const int x) { return pos[x]; }
        kd_val() : pos(), val(0) {}
        kd_val(array pos, int val) : pos(pos), val(val) {}
    };
    int tot_id; // for debug
    struct kd_node {
        int id; // for debug
        kd_pos min, max, pos;
        int tag, val;
        int mintag, minv;
        kd_node *ls, *rs;
        kd_node() : min(inf), max(-inf), tag(0) {}
    } empty, *null;
    void info(kd_node* p) {
        debugf("node #%lld (%lld, %lld) {\n"
                "    range (%lld, %lld) ==> (%lld, %lld)\n"
                "    son: #%lld, #%lld\n"
                "    minv = %lld, mintag = %lld\n"
                "    curval = %lld, curtag = %lld\n"
                "}\n",
                p->id, p->pos[0], p->pos[1],
                p->min[0], p->min[1], p->max[0], p->max[1],
                p->ls->id, p->rs->id,
                p->minv, p->mintag, p->val, p->tag
        );
    }
    inline kd_node* new_node() {
        kd_node* cur = new kd_node;
        cur->ls = cur->rs = null;
        cur->id = ++tot_id;
        return cur;
    }
    inline void add(kd_node *p, int tag, int mintag) {
        if (p == null) return;
        chkmin(p->minv, p->val + mintag);
        chkmin(p->mintag, p->tag + mintag);
        p->tag += tag;
        p->val += tag;
    }
    inline void pushup(kd_node *p) {
        for (int i = 0; i < d_size; i++) {
            chkmin(p->min[i], min(p->ls->min[i], p->rs->min[i]));
            chkmax(p->max[i], max(p->ls->max[i], p->rs->max[i]));
        }
    }
    inline void pushdown(kd_node *p) {
        add(p->ls, p->tag, p->mintag), add(p->rs, p->tag, p->mintag);
        p->mintag = p->tag = 0;
    }
    kd_node* build(kd_val a[], int l, int r, int d) {
        int mid = (l + r) >> 1;
        nth_element(a + l, a + mid, a + r + 1, [d] (kd_val a, kd_val b) {return a[d] < b[d];});
        kd_node* cur = new_node();
        cur->pos = cur->min = cur->max = a[mid].pos;
        cur->val = cur->minv = a[mid].val;
        if (l < mid) cur->ls = build(a, l, mid - 1, (d + 1) % d_size);
        if (mid < r) cur->rs = build(a, mid + 1, r, (d + 1) % d_size);
        pushup(cur);
        return cur;
    }
    void modify(kd_node *p, const kd_pos &min, const kd_pos &max, int v) {
        if (((int)(p->min <= max) + (int)(min <= p->max)) != 2) return;
        if (min <= p->min && p->max <= max) { add(p, v, v); return; }
        pushdown(p);
        if (min <= p->pos && p->pos <= max) { p->val += v, chkmin(p->minv, p->val);}
        modify(p->ls, min, max, v), modify(p->rs, min, max, v);
    }
    int query(kd_node *p, const kd_pos &pos, int d = 0) {
        if ((p->min <= pos || pos <= p->max) == 0) return 0;
        if (p->pos == pos) return p->minv;
        pushdown(p);
        int res = 0;
        if (pos <= p->ls->max && pos[d] <= p->pos[d])
            res = query(p->ls, pos, (d + 1) % d_size);
        if (res == 0)
            res = query(p->rs, pos, (d + 1) % d_size);
        return res;
    }
    void print(kd_node* p, int depth = 0) {
        info(p);
        if (p->ls != null) print(p->ls);
        if (p->rs != null) print(p->rs);
    }
    kd_node* init(kd_val* st, kd_val* ed) {
        return build(st, 0, ed - st - 1, 0);
    }
    kd_tree() { null = &empty; null->ls = null->rs = null; }
};

}

