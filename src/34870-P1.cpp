#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef pair<ll, ll> pll;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

struct Point {
	ll x, y;

	Point operator-(Point b) {
		return { x - b.x, y - b.y };
	}
	bool operator==(Point b) {
		return (x == b.x && y == b.y);
	}
};
ll dot(Point a, Point b) {
	return a.x * b.x + a.y * b.y;
}
ll cross(Point a, Point b) {
	return a.x * b.y - a.y * b.x;
}

ll n;

ll next(ll p) {
	if (p == n - 1) return 1;
	return p + 1;
}
ll prev(ll p) {
	if (p == 1) return n - 1;
	return p - 1;
}

Point pivot;

bool upper(Point b) {
	return b.y >= pivot.y;
}

bool inner_p90(Point a, Point b) {
	if (cross(a - pivot, b - pivot) <= 0 && dot(a - pivot, b - pivot) > 0)
		return true;
	return false;
}

bool inner_m90(Point a, Point b) {
	if (cross(a - pivot, b - pivot) >= 0 && dot(a - pivot, b - pivot) > 0)
		return true;
	return false;
}

bool inner_p180(Point a, Point b) {
	return cross(a - pivot, b - pivot) <= 0;
}

bool cmp(Point a, Point b) {
	if (a == pivot) return true;
	if (b == pivot) return false;
	if (upper(a) == upper(b)) {
		return cross(a - pivot, b - pivot) < 0;
	}
	else if (upper(b)) return true;
	else return false;
}

ll counts(ll a, ll b) {
	if (a <= b) return b - a + 1;
	return (n - a) + b; //0번제외
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	cin >> n;
	vector<Point> v, org;
	vector<ll> ans(n - 1);
	for (ll i = 0; i < n; i++) {
		ll x, y; cin >> x >> y;
		org.push_back({ x, y });
		v.push_back({ x, y });
	}

	for (ll t = 0; t < n; t++) {
		pivot = org[t];
		sort(v.begin(), v.end(), cmp);
		//pivot점은 0번에

		ll m90 = 1, p90 = 1, p180 = 1;
		while (inner_m90(v[1], v[prev(m90)])) m90 = prev(m90);
		for (ll f = 1; f < n; f++) {
			//현재 watching: f
			while (!inner_m90(v[f], v[m90])) m90 = next(m90);
			while (!inner_p90(v[f], v[p90])) p90 = next(p90);
			while (!inner_p180(v[f], v[p180])) p180 = next(p180);
			while (next(p90) != f && inner_p90(v[f], v[next(p90)]))
				p90 = next(p90);
			while (next(p180) != f && inner_p180(v[f], v[next(p180)]))
				p180 = next(p180);

			ll a = counts(m90, f) - 1;
			ll b = counts(f, p90) - 1;
			ll c = counts(p90, p180) - 1;
			ll d = n - 2 - a - b - c;

			ans[b]++;
			ans[b + c]--;
			ans[a]++;
			ans[a + d]--;
		}
	}

	ll sum = 0;
	for (ll i = 0; i < n - 2; i++) {
		sum += ans[i];
		cout << sum << '\n';
	}
}