#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;
typedef vector<pll> vpll;

//#define cin fin
ifstream fin("input.txt");

struct P { ll x, y; };
struct RC { P p1, p2; };

ll ccw(P pivot, P a, P b) {
	a = { a.x - pivot.x, a.y - pivot.y };
	b = { b.x - pivot.x, b.y - pivot.y };
	return a.x * b.y - a.y * b.x;
}

ll modeC(P p1, P p2, RC& rec) {
	ll upC = 0, downC = 0, ret;
	ret = ccw(p1, p2, { rec.p1.x, rec.p1.y });
	if (0 > ret) downC++; if (0 < ret) upC++;
	ret = ccw(p1, p2, { rec.p1.x, rec.p2.y });
	if (0 > ret) downC++; if (0 < ret) upC++;
	ret = ccw(p1, p2, { rec.p2.x, rec.p2.y });
	if (0 > ret) downC++; if (0 < ret) upC++;
	ret = ccw(p1, p2, { rec.p2.x, rec.p1.y });
	if (0 > ret) downC++; if (0 < ret) upC++;
	if (upC == 0) return 1;
	if (downC == 0) return 0;
	return 2;
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	ll n; cin >> n;
	vector<P> corner;
	vector<RC> rec;
	vector<bitset<100>> up, down, bad;
	bitset<100> start;
	for (ll i = 0; i < n; i++) {
		RC x; cin >> x.p1.x >> x.p1.y >> x.p2.x >> x.p2.y;
		corner.push_back({ x.p1.x, x.p1.y });
		corner.push_back({ x.p1.x, x.p2.y });
		corner.push_back({ x.p2.x, x.p2.y });
		corner.push_back({ x.p2.x, x.p1.y });
		start.set(i);
		rec.push_back(x);
	}
	for (ll i = 0; i < 4 * n; i++) {
		for (ll j = i + 1; j < 4 * n; j++) {
			bitset<100> iup, idown, ibad;
			for (ll k = 0; k < n; k++) {
				ll ret = modeC(corner[i], corner[j], rec[k]);
				if (ret == 0) iup.set(k);
				if (ret == 1) idown.set(k);
				if (ret == 2) ibad.set(k);
			}
			up.push_back(iup);
			down.push_back(idown);
			bad.push_back(ibad);
		}
	}
	queue<bitset<100>> que;
	que.push(start);
	bool impos = false;
	while (!que.empty()) {
		auto top = que.front(); que.pop();
		if (top.count() <= 2) continue;
		bool valid = false;
		for (ll i = 0; i < up.size(); i++) {
			bitset<100> iup = up[i] & top, idown = down[i] & top, ibad = bad[i] & top;
			if (ibad.none() && iup.any() && idown.any()) {
				valid = true;
				que.push(iup);
				que.push(idown);
				break;
			}
		}
		if (!valid) {
			impos = true; break;
		}
	}
	if (impos) cout << "impossible";
	else cout << n - 1;
}