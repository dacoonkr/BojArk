#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

ll dp[5005][5005];

ll ceilDivide(ll a, ll b) {
    return (a + b - 1) / b;
}

int main() {
    cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

    ll n, q; cin >> n >> q;
    vector<pll> segs;
    vll space_ps(1);

    for (ll i = 0; i < n; i++) {
        ll x, y; cin >> x >> y;
        if (i > 0) {
            ll space = x - segs.back().second;
            space_ps.push_back(space_ps.back() + space);
        }
        segs.push_back({ x, y });
    }

    for (ll l = 0; l < n; l++) {
        for (ll i = 1; i <= n - 1; i++) {
            ll j = i + l;
            if (j > n - 1) continue;

            if (i == j) {
                dp[i][j] = ceilDivide(space_ps[i] - space_ps[i - 1], 2);
            }
            else {
                dp[i][j] = dp[i][j - 1];
                dp[i][j] = max(dp[i][j], dp[i + 1][j]);
                dp[i][j] = max(dp[i][j], ceilDivide(space_ps[j] - space_ps[i - 1], (j - i + 2)));
            }
        }
    }

    for (ll i = 0; i < q; i++) {
        ll x, y; cin >> x >> y;
        cout << dp[x][y - 1] << '\n';
    }
}