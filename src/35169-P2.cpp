#include <bits/stdc++.h>
using namespace std;
using ll = long long;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

ll dp[305][305];
const ll mod = 998244353;

int main() {
	cin.tie(0)->sync_with_stdio(0); cout.tie(0);

	ll n; cin >> n;
	vector<ll> v;

	for (ll i = 0; i < n; i++) {
		ll x; cin >> x;
		v.push_back(x);
	}

	for (ll l = 0; l < n; l++) {
		for (ll i = 0; i < n; i++) {
			ll j = i + l; //[i,j]
			if (j >= n) continue;

			if (i == j) {
				dp[i][j] = 1;
				continue;
			}
		
			ll ans = 0;
			ll multi = 1;
			for (ll k = i; k <= j; k++) {
				//cut [i,k] [k+1,j]
				if (v[i] < v[k]) {
					multi *= 2;
					multi %= mod;
				}
				if (k == j || v[i] < v[k + 1]) {
					ll left = (k == i) ? 1 : dp[i + 1][k];
					ll right = (k == j) ? 1 : dp[k + 1][j];
					ans += (left * right % mod) * multi % mod;
					ans %= mod;
				}
			}
			dp[i][j] = ans;
		}
	}

	cout << dp[1][n - 1];
}