#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;
typedef vector<pll> vpll;

//#define cin fin
ifstream fin("input.txt");

ll iter = 60;
struct obj {
	ll x, y, v;
};
vector<obj> v;

bool ok(double t) { //t이상인 존재?
	vpll sgn = { {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
	//x1+y1-Tv1 >= x2+y2+Tv2
	for (auto [i, j] : sgn) {
		double leftMax = INT64_MIN;
		double rightMin = INT64_MAX;
		for (obj& o : v) {
			leftMax = max(leftMax, i * o.x + j * o.y - t * o.v);
			rightMin = min(rightMin, i * o.x + j * o.y + t * o.v);
		}
		if (leftMax >= rightMin) return true;
	}
	return false;
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	ll n; cin >> n;
	for (ll i = 0; i < n; i++) {
		ll x, y, z; cin >> x >> y >> z;
		v.push_back({ x, y, z });
	}
	double low = 0, high = 2e6;
	while (iter--) {
		double mid = (low + high) / 2;
		if (ok(mid)) low = mid;
		else high = mid;
	}
	cout << fixed << setprecision(10);
	cout << (low + high) / 2;
}