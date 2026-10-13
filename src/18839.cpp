#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;
typedef vector<pll> vpll;

//#define cin fin
ifstream fin("input.txt");

ll bld[1000005][2];
pll dp[1000005][2]; //i번빌딩 j번계획 -> B를 몇개나 골랐는가? [L, R]

pll mergeintv(pll a, pll b) {
	if (a.first <= -1 && b.first <= -1) return { -2, -2 };
	if (b.first <= -1) return a;
	if (a.first <= -1) return b;
	return { min(a.first, b.first), max(a.second, b.second) };
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	ll n; cin >> n;
	for (ll j = 0; j < 2; j++) {
		for (ll i = 0; i < 2 * n; i++) {
			cin >> bld[i][j];
			dp[i][j] = { -2, -2 };
		}
	}

	dp[0][0] = { 0, 0 };
	dp[0][1] = { 1, 1 };
	for (ll i = 1; i < 2 * n; i++) {
		for (ll b = 0; b < 2; b++) {
			for (ll a = 0; a < 2; a++) {
				pll b_r = dp[i - 1][b], a_r = dp[i][a];
				ll b_v = bld[i - 1][b], a_v = bld[i][a];
				if (b_v <= a_v) { //전파 가능
					dp[i][a] = mergeintv(a_r, { b_r.first + a, b_r.second + a });
				}
			}
		}
	}
	ll req = n;
	string tr; bool suc = true;
	ll lSel = INT32_MAX;
	for (ll i = 2 * n - 1; i >= 0; i--) {
		ll sel = -1;
		if (dp[i][0].first <= req && req <= dp[i][0].second && bld[i][0] <= lSel)
			sel = 0;
		if (dp[i][1].first <= req && req <= dp[i][1].second && bld[i][1] <= lSel)
			sel = 1;

		if (sel == -1) {
			suc = false; break;
		}
		tr += (sel ? 'B' : 'A');
		req -= sel, lSel = bld[i][sel];
	}
	reverse(tr.begin(), tr.end());
	if (suc) cout << tr;
	else cout << -1;
}