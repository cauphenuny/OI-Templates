//author: ycp | https://ycpedef.github.io
//type: generator
#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<sys/time.h>
using namespace std;

#define pr(x) cout << (x) << " "
#define prl(x) cout << (x) << endl
#define el() cout << endl
unsigned _seed() { timeval t; gettimeofday(&t, 0); return t.tv_sec + t.tv_usec; }
int ri(int l, int r) { return (l > r) ? -1 : rand() % (r - l + 1) + l; }
int rip(int l, int r) { int x = ri(l, r); pr(x); return x; }
int ripl(int l, int r) { int x = ri(l, r); prl(x); return x; }

int n, m, q;

int main() {
    srand(_seed());
    /*
     * put something
     */
    return 0;
}
