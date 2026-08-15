#include <bits/stdc++.h>
using namespace std;
using ll = long long;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

ll req[1 << 24]; //a,c리스트들을 넣으면 필요한 b를반환함
ll valid[1 << 24]; //a,c리스트에서 뽑앗을 때 b가 있으면 됨
ll dp[1 << 24];
const ll mod = 998244353;

int main() {
	cin.tie(0)->sync_with_stdio(0); cout.tie(0);

	ll n, m; 
	cin >> n >> m; //n:비트수

	for (ll i = 0; i < m; i++) {
		ll a, b, c; cin >> a >> b >> c;
		a--, b--, c--;

		req[(1 << a) | (1 << c)] |= (1ll << b);
	}
	for (ll i = 0; i < n; i++) {
		for (ll j = 0; j < (1ll << n); j++) {
			if (j & (1ll << i))
				req[j] |= req[j ^ (1ll << i)];
		}
	}
	ll all_mask = (1ll << n) - 1;
	for (ll i = 0; i < (1ll << n); i++) {
		ll rev = all_mask ^ i;
		if ((req[i] & rev) == 0 && (req[rev] & i) == 0) //a,c요구 리스트에서 b찾기
			valid[i] = true;
	}

	dp[0] = 1;
	for (ll i = 1; i < (1ll << n); i++) {
		if (!valid[i]) continue;
		for (ll j = 0; j < n; j++) {
			if (i & (1ll << j)) {
				ll bef = i ^ (1ll << j);
				if (valid[bef]) {
					dp[i] += dp[bef];
					dp[i] %= mod;
				}
			}
		}
	}

	cout << dp[all_mask];
}