#include <bits/stdc++.h>
using ll = long long;
using namespace std;

const ll mod = 998244353;

ll modpow(ll a, ll b) {
    ll ans = 1;
    while (b > 0) {
        if (b & 1)  ans *= a;
        a *= a, b >>= 1;
        ans %= mod, a %= mod;
    }
    return ans;
}
ll inv[7];

vector<ll> keys(300005, -1);
vector<vector<ll>> probs;

ll killCnt(ll n, ll eye) {
    if (n < eye) return n;
    return n - ((n - eye) / 6 + 1);
}

void dfs(ll n) {
    if (keys[n] != -1) return;
    keys[n] = probs.size();
    probs.push_back({});

    if (n == 1) {
        probs[keys[n]] = { 0, 1 };
        return;
    }
    probs[keys[n]].resize(n + 1, 0);

    ll branches = min(6ll, n);
    for (ll eye = 1; eye <= branches; eye++) {
        ll die = killCnt(n, eye);
        dfs(die);
        for (ll i = 1; i <= n; i++) {
            if ((i - 1) % 6 + 1 == eye) continue;
            ll idx = killCnt(i, eye);
            probs[keys[n]][i] += (probs[keys[die]][idx]) * inv[branches] % mod;
            probs[keys[n]][i] %= mod;
        }
    }
}

int main() {
    cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

    ll n; cin >> n;

    for (ll i = 1; i <= 6; i++)
        inv[i] = modpow(i, mod - 2);
    dfs(n);

    vector<ll>& cur = probs[keys[n]];
    for (ll i = 1; i < cur.size(); i++) {
        cout << cur[i] << '\n';
    }
}