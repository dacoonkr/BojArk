#include <bits/stdc++.h>
using ll = long long;
using namespace std;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

ll P[1000006];
ll nxt[1000006], bef[1000006];

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	ll n; cin >> n;
	vector<ll> v;
	for (ll i = 0; i < n; i++) {
		ll x; cin >> x;
		v.push_back(x);
	}
	v.push_back(v[0]);
	for (ll i = 0; i < n; i++) {
		P[v[i]] = 1;
		nxt[v[i]] = v[i + 1];
		bef[v[i + 1]] = v[i];
	}
	ll q; cin >> q;
	while (q--) {
		char c; cin >> c;
		if (c == '-') {
			ll x; cin >> x;
			P[x] = 0;
			nxt[bef[x]] = nxt[x];
			bef[nxt[x]] = bef[x];
		}
		else if (c == '+') {
			ll x, p; cin >> x >> p;
			P[x] = 1;
			bef[x] = bef[p], nxt[x] = p;
			nxt[bef[p]] = x;
			bef[p] = x;
		}
		else if (c == '?') {
			ll n; cin >> n;
			vll v, cont;
			ll maxv = 0;
			for (ll i = 0; i < 2 * n; i++) {
				ll x;
				if (i < n) cin >> x;
				else x = v[i - n];

				if (P[x]) {
					if (v.empty() || v.back() == 0)
						cont.push_back(1);
					else if (nxt[v.back()] == x)
						cont.push_back(cont.back() + 1);
					else cont.push_back(1);
				}
				else cont.push_back(0);
				v.push_back(x);
				maxv = max(maxv, cont.back());
			}
			if (maxv > n) cout << "*\n";
			else cout << maxv << '\n';
		}
	}
}