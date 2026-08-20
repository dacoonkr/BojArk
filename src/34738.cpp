#include <bits/stdc++.h>
using ll = long long;
using namespace std;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

vector<ll> pos;
ll get_comp(ll x) {
	return lower_bound(pos.begin(), pos.end(), x) - pos.begin() + 1;
}

struct SEG { //구간더하기, 구간같은지 쿼리
	vector<ll> diff;
	vector<ll> seg;
	ll st = 1;
	SEG(ll n) {
		while (st < n) st *= 2;
		seg.resize(2 * st);
		diff.resize(n + 1);
		for (ll i = st; i < st * 2; i++)
			seg[i] = 1;
		for (ll i = st - 1; i > 0; i--)
			seg[i] = seg[i * 2] + seg[i * 2 + 1];
	}
	void apply(ll i) {
		if (i == 0) return;
		ll p = st + i - 1;
		seg[p] = (diff[i] == 0);
		while (p > 1) {
			p >>= 1;
			seg[p] = seg[p * 2] + seg[p * 2 + 1];
		}
	}
	ll query(ll l, ll r, ll cl, ll cr, ll p) {
		if (l <= cl && cr <= r) return seg[p];
		if (cr < l || r < cl) return 0;
		ll mid = (cl + cr) / 2;
		return query(l, r, cl, mid, p * 2) + query(l, r, mid + 1, cr, p * 2 + 1);
	}
	//1base
	void add(ll l, ll r, ll v) {
		diff[l - 1] += v; apply(l - 1);
		diff[r] -= v; apply(r);
	}
	bool same(ll l, ll r) {
		if (l >= r) return true;
		return query(l, r - 1, 1, st, 1) == (r - l);
	}
};

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	ll n; cin >> n;
	vector<tuple<char, ll, ll>> query;
	for (ll i = 0; i < n; i++) {
		char c; ll x, u;
		cin >> c >> x >> u;
		query.push_back({ c, x, u });
		if (c == '|') {
			pos.push_back(u - 1);
			pos.push_back(u);
			pos.push_back(u + 1);
		}
		else if (c == '-') {
			pos.push_back(u - 1);
			pos.push_back(u);
			pos.push_back(u + x - 1);
			pos.push_back(u + x);
		}
	}
	sort(pos.begin(), pos.end());
	pos.erase(unique(pos.begin(), pos.end()), pos.end());

	SEG seg(pos.size());
	for (auto& [c, x, u] : query) {
		ll ql, qr, qx;
		if (c == '|') ql = u, qr = u, qx = x;
		else if (c == '-') ql = u, qr = u + x - 1, qx = 1;
		ql = get_comp(ql), qr = get_comp(qr);

		bool suc = seg.same(ql, qr);
		cout << (suc ? "S" : "U");
		if (suc) seg.add(ql, qr, qx);
	}
}