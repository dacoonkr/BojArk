#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct frac {
	ll a, b; // a/b
};

ll divideRound(ll p, frac f) {
	return p * f.b / f.a;
}

ll multi(ll a, ll b, ll k) {
	ll ans = 0, cur = a;
	while (b > 0) {
		ans += cur * (b & 1);
		if (ans >= k) return -1;
		b >>= 1; cur *= 2;
	}
	return ans;
}

bool cutGetMoreThanK(ll x, ll y, ll z, frac side, ll k) {
	ll xS = divideRound(x, side);
	ll yS = divideRound(y, side);
	ll zS = divideRound(z, side);
	if (xS == 0 || yS == 0 || zS == 0) return false;

	if (xS >= k) return true;

	ll mul = multi(xS, yS, k);
	if (mul == -1) return true;

	mul = multi(mul, zS, k);
	if (mul == -1)
		return true;

	else return mul >= k;
}

ll cutBinary(ll x, ll y, ll z, ll side, ll k) {
	ll left = 1, right = 1e9;
	ll ans = 0;
	while (left <= right) {
		ll mid = (left + right) / 2;

		bool success = cutGetMoreThanK(x, y, z, { side, mid }, k);

		if (success) ans = mid, right = mid - 1;
		else left = mid + 1;
	}
	return ans;
}

frac fracMax(frac a, frac b) {
	if (a.a * b.b > a.b * b.a) return a;
	return b;
}

frac fracSimp(frac a) {
	ll gcdv = gcd(a.a, a.b);
	a.a /= gcdv;
	a.b /= gcdv;
	return a;
}

void solve() {
	ll x, y, z; cin >> x >> y >> z;
	ll k; cin >> k;

	frac res = { 0, 1 };
	res = fracMax(res, { x, cutBinary(x, y, z, x, k) });
	res = fracMax(res, { y, cutBinary(x, y, z, y, k) });
	res = fracMax(res, { z, cutBinary(x, y, z, z, k) });
	res = fracSimp(res);

	cout << res.a << ' ' << res.b << '\n';
}

int main() {
	cin.tie(0)->sync_with_stdio(0); cout.tie(0);

	ll t; cin >> t;
	while (t--) solve();
}