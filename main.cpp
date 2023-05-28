#include <iostream>
#include <vector>

using namespace std;
#define int long long
int INF = 2e9, P = 1e9 + 7, k = 1e5 + 1;
vector<int> tens(k, 0), ones(k), elevens(k), sumelv(k), ansones(k);

struct Node {
    int value, nonz, ans, pref, suff;
    short flag = -1;
    signed len;
    //bool needToRecalc = false;
    Node* l = nullptr, * r = nullptr;

    Node(int v) {
        value = v;
        nonz = !!v;
        pref = v;
        suff = v;
        ans = v;
        len = 1;
    }

    Node(Node* l, Node* r) {
        this->l = l;
        this->r = r;
        len = l->len + r->len;
        value = (l->value * tens[r->len] + r->value) % P;
        pref = (l->pref + r->pref + l->value * ones[r->len]) % P;
        suff = (r->suff + l->suff * tens[r->len] + r->value * l->nonz) % P;
        nonz = l->nonz + r->nonz;
        ans = (l->suff * ones[r->len] + r->pref * l->nonz + l->ans + r->ans) % P;
    }

    Node() {}
};


void setnode(Node*& n, int o) {
    n->value = (elevens[n->len] * o) % P;
    if (!o)
        n->nonz = 0;
    else
        n->nonz = n->len;
    n->pref = (sumelv[n->len] * o) % P;
    n->suff = (sumelv[n->len] * o) % P;
    n->ans = (ansones[n->len] * o) % P;
    n->flag = o;
}

Node* merge(Node* l, Node* r) {
    return new Node(l, r);
}

void merge(Node*& n, Node* l, Node* r)
{
    n->len = l->len + r->len;
    n->value = (l->value * tens[r->len] + r->value) % P;
    n->pref = (l->pref + r->pref + l->value * ones[r->len]) % P;
    n->suff = (r->suff + l->suff * tens[r->len] + r->value * l->nonz) % P;
    n->nonz = l->nonz + r->nonz;
    n->ans = (l->suff * ones[r->len] + r->pref * l->nonz + l->ans + r->ans) % P;
}


Node* build(int l, int r, vector<int>& v) {
    if (r == l)
        return new Node(v[l]);
    else {
        Node* lt = build(l, (r + l) / 2, v);
        Node* rt = build((r + l) / 2 + 1, r, v);
        return new Node(lt, rt);
    }
}

void creq(Node*& n, int l, int r, int tl, int tr, int o) {
    if (tl <= l && r <= tr) {
        setnode(n, o);
        return;
    }
    if (n->flag != -1) {
        setnode(n->l, n->flag);
        setnode(n->r, n->flag);
        //n->needToRecalc = true;
        n->flag = -1;
    }
    int mid = ((r + l) >> 1);
    if (tr <= mid) {
        creq(n->l, l, mid, tl, tr, o);
    }
    else if (mid + 1 <= tl)
        creq(n->r, mid + 1, r, tl, tr, o);
    else {
        creq(n->l, l, mid, tl, tr, o);
        creq(n->r, mid + 1, r, tl, tr, o);
    }
    //if(n->needToRecalc)
    merge(n, n->l, n->r);
}

Node* ireq(Node*& n, int l, int r, int tl, int tr) {
    if (tl <= l && r <= tr) {
        return n;
    }
    if (n->flag != -1) {
        setnode(n->l, n->flag);
        setnode(n->r, n->flag);
        //n->needToRecalc = true;
        n->flag = -1;
    }
    int mid = ((r + l) >> 1);
    Node* ans = new Node();
    if (tr <= mid) {
        ans = ireq(n->l, l, mid, tl, tr);
    }
    else if (mid + 1 <= tl)
        ans = ireq(n->r, mid + 1, r, tl, tr);
    else {
        ans = merge(ireq(n->l, l, mid, tl, tr), ireq(n->r, mid + 1, r, tl, tr));
    }
    //if(n->needToRecalc)
    merge(n, n->l, n->r);
    return ans;
}

signed main() {
    tens[0] = 1;
    for (int i = 1; i < k; i++) {
        tens[i] = (tens[i - 1] * 10) % P;
    }
    for (int i = 1; i < k; i++) {
        ones[i] = (ones[i - 1] + tens[i]) % P;
    }
    for (int i = 1; i < k; i++) {
        elevens[i] = (elevens[i - 1] * 10 + 1) % P;
    }
    for (int i = 1; i < k; i++) {
        sumelv[i] = (sumelv[i - 1] * 10 + i) % P;
    }
    for (int i = 1; i < k; i++) {
        ansones[i] = (ansones[i - 1] + sumelv[i]) % P;
    }
    //cout << tens[3] << ' ' << elevens[3] << ' ' << ones[3] << ' ' << sumelv[3] << ' ' << ansones[3] << "\n";
    int n, m, a, l, r, c;
    cin >> n >> m;
    vector<int> v(n);
    for (auto& el : v)
        cin >> el;
    Node* st = build(0, n - 1, v);

    for (int i = 0; i < m; i++) {
        cin >> a >> l >> r;
        r--;
        l--;
        if (a == 1) {
            cin >> c;
            creq(st, 0, n - 1, l, r, c);
        }
        else {
            cout << ireq(st, 0, n - 1, l, r)->ans << "\n";
        }
    }
}