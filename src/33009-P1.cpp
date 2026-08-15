#include <bits/stdc++.h>
#define MAXIZ(a, b) a = max(a, b)
using namespace std;
using ll = long long;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

pair<ll, ll> dp[1ll << 20]; //{bitc, -i}

int main() {
	cin.tie(0)->sync_with_stdio(0); cout.tie(0);

	ll n, m; cin >> n >> m;

	vector<ll> v, cntv;
	vector<pair<ll, ll>> max_v;
	pair<ll, ll> max_s = { -1, -1 };
	for (ll j = 0; j < (1ll << m); j++) {
		dp[j] = { -1, 1 };
	}
	for (ll i = 0; i < n; i++) {
		string s; cin >> s;
		ll bit = 0, cnt = 0;
		for (char c : s)
			bit = bit * 2 + (c == 'Y'), cnt += (c == 'Y');
		v.push_back(bit);
		cntv.push_back(cnt);
		MAXIZ(dp[((1ll << m) - 1) ^ bit], make_pair(cnt, -i));
		max_v.push_back(max_s);
		MAXIZ(max_s, make_pair(cnt, -i));
	}
	for (ll i = 0; i < m; i++) {
		for (ll j = 0; j < (1ll << m); j++) {
			if (j & (1ll << i))
				MAXIZ(dp[j], dp[j ^ (1ll << i)]);
		}
	}
	ll ans_i = n + 2, ans_j, multiC = -1;
	for (ll j = 1; j < n; j++) {
		ll bits = dp[v[j]].first + cntv[j], i = -dp[v[j]].second;

		if (cntv[j] == m) {
			i = -max_v[j].second;
			bits = max_v[j].first + cntv[j];
		}

		if (i >= j || i == -1) continue;

		if (multiC < bits || (multiC == bits && i < ans_i)) {
			ans_i = i, ans_j = j;
			multiC = bits;
		}
	}

	if (ans_i == n + 2) cout << "No";
	else cout << (ans_i + 1) << ' ' << (ans_j + 1);
}