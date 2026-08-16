#include <bits/stdc++.h>
using ll = long long;
using namespace std;
typedef pair<ll, ll> pll;

bool cmp(pll a, pll b) {
    return a.first * b.second < a.second * b.first;
}

pll add(pll a, pll b) {
    return { a.first + b.first, a.second + b.second };
}

bool below(pll a, pll b, pll p) {
    if (a.second < p.second && p.second < b.second) {
        return (p.first - a.first) * (b.second - p.second)
            < (b.first - p.first) * (p.second - a.second);
    }
    else if (a.second == p.second && a.first > p.first) return true;
    else if (b.second == p.second && b.first > p.first) return true;
    return false;
}

int main() {
    cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

    ll n, m; cin >> n >> m;
    vector<pll> src(n), tar(m);

    for (ll i = 0; i < n; i++) cin >> src[i].first;
    for (ll i = 0; i < n; i++) cin >> src[i].second;
    for (ll i = 0; i < m; i++) cin >> tar[i].first;
    for (ll i = 0; i < m; i++) cin >> tar[i].second;

    sort(src.begin(), src.end(), cmp);
    sort(tar.begin(), tar.end(), cmp);

    pll tar_ps = { 0, 0 };
    bool fail = false;
    for (ll i = 0; i < m; i++) {
        tar_ps = add(tar_ps, tar[i]);
        pll src_f = { 0, 0 }, src_s = { 0, 0 };
        for (ll j = 0; j < n; j++) {
            pll src_t = add(src_s, src[j]);
            src_f = src_s, src_s = src_t;
            if (below(src_f, src_s, tar_ps))
                fail = true;
        }
    }

    cout << (fail ? "No" : "Yes");
}