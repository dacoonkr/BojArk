#include <bits/stdc++.h>
using ll = long long;
using namespace std;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

ll star[200005], brb[200005][4];
const ll INF = 4e9;

struct SEG { //min
	vector<pair<ll, ll>> seg; ll st = 1;
	void init(ll n) {
		while (st < n) st *= 2;
		seg.assign(st * 2, { INF, -1 });
	}
	void update(ll i, ll v) { //0base
		ll p = st + i;
		seg[p] = { v, i };
		while (p > 1) { p >>= 1; seg[p] = min(seg[p * 2], seg[p * 2 + 1]); }
	}
	pll query(ll l, ll r, ll cl, ll cr, ll p) {
		if (r < cl || cr < l) return { INF, -1 };
		if (l <= cl && cr <= r) return seg[p];
		ll mid = (cl + cr) >> 1;
		return min(query(l, r, cl, mid, p * 2), query(l, r, mid + 1, cr, p * 2 + 1));
	}
	pll gmin(ll no) { //no번을 제외한 전체최소
		if (no == -1) return seg[1];
		if (no == 0) return query(1, st - 1, 0, st - 1, 1);
		if (no == st - 1) return query(0, st - 2, 0, st - 1, 1);
		return min(query(0, no - 1, 0, st - 1, 1),
			query(no + 1, st - 1, 0, st - 1, 1));
	}
};

SEG seg[7];

tuple<ll, vll, vll> test(vll d) { //값, 평점변경, 위치
	vll pos; ll cost = 0;
	for (ll i = 0; i < d.size(); i++) {
		ll exp = -1;
		if (i == 1) exp = pos[0]; //1번은 0번과 중복없음
		pll gv = seg[3 + d[i]].gmin(exp);
		if (gv.first == INF) return { INF, {}, {} };
		cost += gv.first;
		pos.push_back(gv.second);
	}
	return { cost, d, pos };
}

void apply(vll c, vll p) {
	for (ll i = 0; i < p.size(); i++) {
		for (ll j = 0; j < 7; j++)
			seg[j].update(p[i], INF);
		star[p[i]] += c[i];
		for (ll j = 0; j < 7; j++) {
			ll aft = star[p[i]] + (j - 3);
			if (0 <= aft && aft <= 3) {
				ll cost = brb[p[i]][aft] - brb[p[i]][star[p[i]]];
				seg[j].update(p[i], cost);
			}
		}
	}
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	ll n; cin >> n;
	for (ll i = 0; i < 7; i++)
		seg[i].init(n);

	for (ll i = 0; i < n; i++) {
		for (ll j = 1; j <= 3; j++)
			cin >> brb[i][j];
		seg[4].update(i, brb[i][1]);
		seg[5].update(i, brb[i][2]);
		seg[6].update(i, brb[i][3]);
	}
	ll ans = 0;
	for (ll i = 0; i < 3 * n; i++) {
		tuple<ll, vll, vll> x = { INF, {}, {} };
		x = min(x, test({ 2, 2, -3 }));
		x = min(x, test({ -1, -1, 3 }));
		x = min(x, test({ -2, 3 }));
		x = min(x, test({ 1 }));
		x = min(x, test({ -1, 2 }));
		x = min(x, test({ 2, -1 }));
		ans += get<0>(x);
		cout << ans << '\n';
		apply(get<1>(x), get<2>(x));
	}
}