#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;
typedef vector<pll> vpll;

//#define cin fin
ifstream fin("input.txt");

vll compress(vll v, ll tar) {
	//i+2j=tar
	vll ans(tar + 1, INT64_MAX);
	for (ll i = 0; i <= tar; i++) {
		//i번압축
		vll c = v; ll ptr = 0, loss = 0;
		for (ll j = 0; j < i; j++) {
			loss += (c[ptr] + c[ptr + 1]);
			c[ptr + 1] ^= c[ptr];
			ptr++;
			loss -= c[ptr];
		}
		ans[i] = min(ans[i], loss);
		for (ll j = 1; true; j++) {
			loss += (c[ptr] + c[ptr + 1] + c[ptr + 2] + c[ptr + 3]);
			c[ptr + 2] ^= c[ptr + 1];
			c[ptr + 3] ^= c[ptr];
			ptr += 2;
			loss -= c[ptr] + c[ptr + 1];

			if (i + 2 * j <= tar)
				ans[i + 2 * j] = min(ans[i + 2 * j], loss);
			else break;
		}
	}
	return ans;
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	ll n, m; cin >> n >> m;
	vll v; ll sum = 0;
	for (ll i = 0; i < n; i++) {
		ll x; cin >> x;
		v.push_back(x);
		sum += x;
	}
	ll tar = n - m;
	vll left = compress(v, tar);
	reverse(v.begin(), v.end());
	vll right = compress(v, tar);
	ll loss = INT64_MAX;
	for (ll i = 0; i <= tar; i++) {
		loss = min(loss, left[i] + right[tar - i]);
	}
	cout << sum - loss;
}