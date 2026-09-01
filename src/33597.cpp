#include <bits/stdc++.h>
using ll = long long;
using namespace std;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;
typedef vector<pll> vpll;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

ll nxt[55][55];

void pr(vll v) {
	for (ll i = 0; i < v.size(); i++)
		cout << v[i] << ' ';
	cout << '\n';
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	ll n, m; cin >> n >> m;
	vpll v;
	for (ll i = 0; i < m; i++) {
		ll x, y; cin >> x >> y;
		v.push_back({ x, y });
		if (nxt[y][x]) {
			cout << "NO"; return 0;
		}
		nxt[x][y] = 1;
	}
	cout << "YES\n" << (m * 2) << '\n';
	for (ll i = 0; i < m; i++) {
		vll res(n);
		res[0] = v[i].first;
		res[1] = v[i].second;
		ll use = 0;
		for (ll j = 2; j < n; j++) {
			use++;
			if (use == res[0]) use++;
			if (use == res[1]) use++;
			if (use == res[0]) use++;
			res[j] = use;
		}
		pr(res);
		swap(res[0], res[1]);
		reverse(res.begin(), res.end());
		pr(res);
	}
}