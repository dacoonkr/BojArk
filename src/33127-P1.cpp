#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef pair<ll, ll> pll;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

ll last[300005];

struct MST {
	vector<vector<ll>> seg;
	ll st = -1;
	MST(ll n, vector<ll>& item) {
		st = 1;
		while (st < n) st *= 2;
		seg.resize(st * 2);

		for (ll i = 0; i < n; i++) {
			push(i, item[i]);
		}
		for (ll i = 0; i < st * 2; i++)
			sort(seg[i].begin(), seg[i].end());
	}
	void push(ll i, ll v) { //0base
		ll p = st + i;
		while (p > 0) {
			seg[p].push_back(v);
			p /= 2;
		}
	}
	ll query(ll p, ll l, ll r, ll cl, ll cr, ll v) {
		if (l <= cl && cr <= r) {
			return lower_bound(seg[p].begin(), seg[p].end(), v) - seg[p].begin();
		}
		if (cr < l || r < cl) return 0;
		ll mid = (cl + cr) / 2;
		ll sum = 0;
		sum += query(p * 2, l, r, cl, mid, v);
		sum += query(p * 2 + 1, l, r, mid + 1, cr, v);
		return sum;
	}
	ll countDistinct(ll l, ll r) {
		if (l > r) return 0;
		return query(1, l, r, 1, st, l);
	}
};

int main() {
	cin.tie(0)->sync_with_stdio(0); cout.tie(0);

	ll n, m; cin >> n >> m;
	vector<ll> link;
	vector<pair<ll, ll>> points;
	for (ll i = 1; i <= n; i++) {
		ll x; cin >> x;
		link.push_back(last[x]);
		points.push_back({ x, i });
		last[x] = i;
	}

	sort(points.begin(), points.end());
	MST seg(n, link);

	ll ptr = 0;
	ll ans = 0;
	for (ll tar = 1; tar <= m; tar++) {
		vector<ll> pos;
		while (ptr < n && points[ptr].first == tar) {
			pos.push_back(points[ptr].second);
			ptr++;
		}
		
		if (pos.size() == 0)
			ans += seg.countDistinct(1, n);
		else {
			ans += m - 1;
			ans += seg.countDistinct(1, pos.front() - 1);
			ans += seg.countDistinct(pos.back() + 1, n);
			for (ll i = 0; i + 1 < pos.size(); i++)
				ans += 2 * seg.countDistinct(pos[i] + 1, pos[i + 1] - 1);
		}
	}
	cout << ans / 2;
}