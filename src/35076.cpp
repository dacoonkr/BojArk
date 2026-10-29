#include <bits/stdc++.h>
using namespace std;
using ll = long long;

double dp[105][105][105];

double dfs(ll a, ll b, ll c) { //a<=b<=c
    if (a > b) swap(a, b);
    if (b > c) swap(b, c);
    if (a > b) swap(a, b);

    if (dp[a][b][c] > 0) return dp[a][b][c];
    // cout << a << ' ' << b << ' ' << c << '\n';

    double ans = INT32_MAX;
    if (a > 0) {
        ans = min(ans, 1.5 + (dfs(a - 1, b + 1, c) + dfs(a - 1, b, c + 1)) / 2);
        ans = min(ans, 3.0 + dfs(a - 1, b, c + 1));
        ans = min(ans, 3.0 + dfs(a - 1, b + 1, c));
        return dp[a][b][c] = ans;
    }
    return 3.0 * b;
}

int main() {
    cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

    ll n; cin >> n;
    string s; cin >> s;

    ll r = 0, g = 0, b = 0;
    for (char c : s) {
        if (c == 'r') r++;
        if (c == 'g') g++;
        if (c == 'b') b++;
    }
    vector<ll> cnt = { r, b, g };
    sort(cnt.begin(), cnt.end());

    cout << fixed << setprecision(10);
    cout << dfs(cnt[0], cnt[1], cnt[2]);
}