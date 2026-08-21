#include <bits/stdc++.h>
using ll = long long;
using namespace std;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

ll base[64];

void push(ll x) {
	for (ll i = 63; i >= 0; i--) {
		if ((x >> i) & 1) {
			if (base[i]) x ^= base[i];
			else {
				base[i] = x;
				break;
			}
		}
	}
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	ll n; cin >> n;
	for (ll i = 0; i < n; i++) {
		ll x; cin >> x;
		push(x);
	}
	ll ans = 0;
	for (ll i = 63; i >= 0; i--)
		ans = max(ans, ans ^ base[i]);
	cout << ans;
}