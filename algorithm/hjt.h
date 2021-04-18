class remainable_segment_tree {
    struct Node {
        size_t siz;
        Node *ls, *rs;
        int id;
        void update() { siz = ls->siz + rs->siz; }
        Node(size_t s, Node *ls, Node *rs, int id): siz(s), ls(ls), rs(rs), id(id) {}
    } *null;
    vector <Node*> root;
    int minv, maxv, p, tot_id;
    size_t v, rk;
    #define mid ((l + r) >> 1)
    Node* refresh(Node *pre, int l, int r) {
        Node *cur = new Node(pre->siz + v, pre->ls, pre->rs, ++tot_id);
        if (l < r) {
            if (p <= mid) cur->ls = refresh(pre->ls, l, mid);
            else          cur->rs = refresh(pre->rs, mid + 1, r);
        }
        return cur;
    }
    int query(Node *pre, Node *cur, int l, int r) {
        if (l == r) { return l; }
        size_t siz = cur->ls->siz - pre->ls->siz;
        if (siz >= rk) { return query(pre->ls, cur->ls, l, mid); }
        else { rk -= siz; return query(pre->rs, cur->rs, mid + 1, r); }
    }
    #undef mid
    public:
    remainable_segment_tree() {
        null = new Node(0, 0, 0, 0);
        null->ls = null->rs = null, null->siz = 0;
        root.push_back(null);
        minv = maxv = tot_id = 0;
    }
    int getpos(size_t pre, size_t cur, size_t rank) {
        rk = rank;
        return query(root[pre], root[cur], minv, maxv);
    }
    void insert(size_t pre, size_t cur, int pos, size_t val) { // create a new version
        if (cur == root.size()) root.push_back(null);
        v = val, p = pos;
        root[cur] = refresh(root[pre], minv, maxv);
    }
    void set_limit(int l, int r) { // remember to set it !!!!!!
        if (root.size() <= 1) minv = l, maxv = r;
    }
    void move(size_t dest, size_t source) {
        root[dest] = root[source];
    }
} tree;

