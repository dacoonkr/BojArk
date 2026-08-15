#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef pair<ll, ll> pll;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

const ll mod = 998244353;

ll modpow(ll a, ll b) {
	ll ans = 1, cur = a;
	while (b > 0) {
		if (b & 1) ans *= cur;
		b >>= 1;
		cur *= cur;
		cur %= mod;
		ans %= mod;
	}
	return ans;
}

struct MAX {
	vector<pll> seg;
	ll st = 1, sz = -1;

	MAX() {};

	void init(ll n, vector<ll> v) {
		sz = n;
		while (st < n) st *= 2;
		seg.resize(2 * st, { -1, -1 });
		for (ll i = 0; i < n; i++) {
			update(i, { v[i], i });
		}
	}

	void update(ll i, pll v) {
		ll p = st + i;
		seg[p] = v;
		while (p > 1) {
			p >>= 1;
			seg[p] = max(seg[p * 2], seg[p * 2 + 1]);
		}
	}

	pair<ll, ll> query(ll l, ll r, ll cl, ll cr, ll p) {
		if (l <= cl && cr <= r) return seg[p];
		if (r < cl || cr < l) return { -1, -1 };
		ll mid = (cl + cr) / 2;
		return max(query(l, r, cl, mid, p * 2), query(l, r, mid + 1, cr, p * 2 + 1));
	}

	pll maxof(ll l, ll r) { //0base
		return query(l + 1, r + 1, 1, st, 1);
	}

	bool greater(pll a, pll b, bool strict) {
		if (strict)
			return a.first > b.first;
		return a > b;
	}

	ll find_left(ll l, ll r, ll cl, ll cr, ll p, pll v, bool strict) {
		if (r < cl || cr < l) return -1;
		if (!greater(seg[p], v, strict)) return -1;
		if (cl == cr) return cl - 1;
		ll mid = (cl + cr) / 2;
		ll res = find_left(l, r, mid + 1, cr, p * 2 + 1, v, strict);
		if (res != -1) return res;
		return find_left(l, r, cl, mid, p * 2, v, strict);
	}

	ll find_right(ll l, ll r, ll cl, ll cr, ll p, pll v, bool strict) {
		if (r < cl || cr < l) return sz;
		if (!greater(seg[p], v, strict)) return sz;
		if (cl == cr) return cl - 1;
		ll mid = (cl + cr) / 2;
		ll res = find_right(l, r, cl, mid, p * 2, v, strict);
		if (res != sz) return res;
		return find_right(l, r, mid + 1, cr, p * 2 + 1, v, strict); 
	}

	//i보다큰수를 포함한 가장작은범위
	pll expand(ll i, pll v, bool strict) {
		ll left_idx = find_left(1, i + 1, 1, st, 1, v, strict);
		ll right_idx = find_right(i + 1, sz, 1, st, 1, v, strict);
		return { left_idx, right_idx };
	}
};

vector<ll> cnts(2, 1), psum = { 0, 1, 2 };

ll frequency(bool front, ll l, ll r, ll n) {
	if (l > r) return 0;
	if (!front) {
		//flip
		ll x1 = n - 1 - l;
		ll x2 = n - 1 - r;
		l = x2, r = x1;
	}
	return (psum[r + 1] - psum[l] + mod) % mod;
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	ll n; cin >> n;

	vector<ll> v[2];
	for (ll i = 0; i < n; i++) {
		cnts.push_back(cnts.back() * 2);
		cnts.back() %= mod;
		psum.push_back(psum.back() + cnts.back());
		psum.back() %= mod;
	}
	for (ll i = 0; i < 2; i++) {
		for (ll j = 0; j < n; j++) {
			ll x; cin >> x;
			v[i].push_back(x);
		}
	}

	MAX seg[2];
	seg[0].init(n, v[0]);
	seg[1].init(n, v[1]);

	ll sum = 0;

	for (ll row : {0, 1}) {
		ll nrow = 1 - row;
		for (ll i = 0; i < n; i++) {
			pair<ll, ll> f = seg[row].expand(i, { v[row][i], i }, false);
			pair<ll, ll> s = seg[nrow].expand(i, { v[row][i], i }, true);
			f.first++, f.second--;

			ll cur = 0;
			cur += frequency(true, max(f.first, s.first + 1), i, n) * frequency(false, s.second, f.second, n) % mod;
			cur += frequency(true, f.first, s.first, n) * frequency(false, i, min(f.second, s.second - 1), n) % mod;
			cur += frequency(true, f.first, s.first, n) * frequency(false, s.second, f.second, n) % mod;

			cur %= mod;
			sum += (cur * v[row][i]) % mod;
			sum %= mod;
		}
	}

	ll counts = modpow(2, n);
	ll rev = modpow(counts, mod - 2);

	//cout << "YES: " << sum << '\n';
	cout << (sum * rev) % mod;
}