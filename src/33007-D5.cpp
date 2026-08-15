#include <bits/stdc++.h>
using ll = long long;
using namespace std;

ll queries[100005];
vector<pair<ll, ll>> endswith[100005]; //[R], {L, idx}

// rightmost appear
ll f[100005], s[100005];

struct rmq {
	vector<ll> seg;
	ll st = -1;

	void init(ll n) {
		st = 1;
		while (st < n) st *= 2;
		seg.resize(st * 2);
	}

	void update(ll i, ll value) { //i: 1-based
		ll p = i + st - 1;
		seg[p] = value;
		while (p > 1) {
			p >>= 1;
			seg[p] = max(seg[p * 2], seg[p * 2 + 1]);
		}
	}

	ll findLatestBiggerThanV(ll v) {
		ll p = 1;
		while (p < st) {
			ll next = p * 2;
			if (seg[next + 1] >= v) next++;
			p = next;
		}
		return p - st + 1;
	}
};

rmq left_idx;

void update(ll idx, ll value) {
	s[value] = f[value];
	f[value] = idx;
	left_idx.update(value, s[value]);
}

int main() {
	cin.tie(0)->sync_with_stdio(0); cout.tie(0);

	ll n; cin >> n;
	vector<ll> v(1); //1-index
	for (ll i = 0; i < n; i++) {
		ll x; cin >> x;
		v.push_back(x);
	}
	ll q; cin >> q;
	for (ll i = 0; i < q; i++) {
		ll l, r; cin >> l >> r;
		endswith[r].push_back({ l, i });
	}

	left_idx.init(100000);
	for (ll i = 1; i <= n; i++) {
		ll j;
		for (j = 1; j * j < v[i]; j++) {
			if (v[i] % j == 0) {
				update(i, j);
				update(i, v[i] / j);
			}
		}
		if (j * j == v[i]) update(i, j);
		if (i == 1) continue;
		for (auto qi : endswith[i]) {
			ll d = left_idx.findLatestBiggerThanV(qi.first);
			queries[qi.second] = d;
		}
	}
	for (ll i = 0; i < q; i++) {
		cout << queries[i] << '\n';
	}
}