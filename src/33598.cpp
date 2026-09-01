#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

ll sz[2025][2025];
bool out[2025][2025];

vll select(vll v, ll tar) {
	vll sel;
	ll sumidx = 0;
	while (1) {
		ll idx = upper_bound(v.begin(), v.end(), tar) - v.begin() - 1;
		if (v[idx] == 0) break;
		if (sumidx + idx > 2025) break;
		sel.push_back(idx);
		tar -= v[idx];
		sumidx += idx + 1;
	}
	return sel;
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	ll k, r, c = 2025; //fix to 2025
	cin >> k;

	if (k == 0) {
		cout << "1 1\n.";
		return 0;
	}

	for (r = 1; r <= 2019; r++) {
		ll cnt = (r * (r - 1) / 2) * (c * (c - 1) / 2);
		sz[r][c] = cnt;
		if (cnt >= k && sz[r - 1][c] < k) {
			r--;
			cerr << "rec: " << r << '*' << c << '\n';
			break;
		}
	}

	ll cnt = (r * (r - 1) / 2) * (c * (c - 1) / 2);
	vll var1;
	for (ll i = 0; i < r; i++)
		for (ll j = 0; j < c; j++)
			out[i][j] = true;
	for (ll i = 0; i <= c; i++) {
		ll tmp = (i * (i - 1) / 2) * r;
		var1.push_back(tmp);
	}
	vll sel1 = select(var1, k - cnt);
	ll ptr = 0;
	for (ll t : sel1) {
		for (ll i = 0; i < t; i++) {
			out[r][ptr + i] = true;
		}
		cnt += var1[t];
		ptr += t + 1;
	}

	for (pll line : { make_pair(2, 3), { 5, 6 }, { 8, 9 } }) {
		vll var2;
		for (ll i = 0; i <= c; i++) {
			ll tmp = (i * (i - 1) / 2);
			var2.push_back(tmp);
		}
		vll sel2 = select(var2, k - cnt);
		ptr = 0;
		for (ll t : sel2) {
			for (ll i = 0; i < t; i++) {
				out[r + line.first][ptr + i] = true;
				out[r + line.second][ptr + i] = true;
			}
			cnt += var2[t];
			ptr += t + 1;
		}
	}

	//cout << (k - cnt) << '\n';
	cout << (r + 10) << ' ' << c << '\n';
	for (ll i = 0; i < r + 10; i++) {
		for (ll j = 0; j < c; j++)
			cout << (out[i][j] ? '#' : '.');
		cout << '\n';
	}
}