#include <bits/stdc++.h>
using ll = long long;
using namespace std;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;
typedef vector<pll> vpll;

//#define cin fin
//ifstream fin("input.txt");

//9, 3 : 6,8,10,12
//9, 4 : 5,7,9,11,13

ll n; pll tar;
vll d;

ll dist(pll a) {
	return abs(a.first) + abs(a.second);
}

bool rangeIn(ll low, ll high, ll d) {
	return (low <= d && d <= high && low % 2 == d % 2);
}

ll slope(pll a, pll b) {
	if (b.second == a.second) return 1;
	return (b.first - a.first) / (b.second - a.second);
}
pll getX(pll a, pll b, ll x) {
	//직선 ab위에 x좌표
	ll d = slope(a, b);
	return { x, a.second + d * (x - a.first) };
}
pll getY(pll a, pll b, ll y) {
	//직선 ab위에 y좌표
	ll d = slope(a, b);
	return { a.first + d * (y - a.second), y };
}
pll step(pll a, pll b, ll step) {
	ll xd = (a.first < b.first ? 1 : -1);
	ll yd = (a.second < b.second ? 1 : -1);
	return { a.first + xd * step, a.second + yd * step };
}

pll make(pll bRange, pll tar, ll D) {
	if (D == 0) return tar;
	//0,0과의 거리가 bRange에 들어오면서, tar와의 거리는 D인 점 반환
	vector<pair<pll, pll>> lines, lines1, lines2; //x축자르기, y축자르기
	lines.push_back({ { tar.first + D, tar.second }, { tar.first, tar.second + D } });
	lines.push_back({ { tar.first + D, tar.second }, { tar.first, tar.second - D } });
	lines.push_back({ { tar.first - D, tar.second }, { tar.first, tar.second + D } });
	lines.push_back({ { tar.first - D, tar.second }, { tar.first, tar.second - D } });
	for (auto& [a, b] : lines) {
		if (a.first > b.first) swap(a, b);
		if (a.first < 0 && 0 < b.first) {
			lines1.push_back({ a, getX(a, b, 0) });
			lines1.push_back({ b, getX(a, b, 1) });
		}
		else lines1.push_back({ a, b });
	}
	for (auto& [a, b] : lines1) {
		if (a.second > b.second) swap(a, b);
		if (a.second < 0 && 0 < b.second) {
			lines2.push_back({ a, getY(a, b, 0) });
			lines2.push_back({ b, getY(a, b, 1) });
		}
		else lines2.push_back({ a, b });
	}
	for (auto& [a, b] : lines2) {
		ll low = dist(a), high = dist(b);
		if (low > high) {
			swap(a, b);
			swap(low, high);
		};
		if (bRange.second < low) continue;
		if (high < bRange.first) continue;
		ll want = max(low, bRange.first);
		return step(a, b, (want - low) / 2);
	}
	cerr << "FAIL\n";
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	cin >> n >> tar.first >> tar.second;
	d.resize(n - 1);
	for (ll& i : d) cin >> i;

	vpll range(n - 1);
	range[0] = { d[0], d[0] };
	for (ll i = 1; i < n - 1; i++) {
		ll blow = range[i - 1].first, bhigh = range[i - 1].second;
		ll nlow = min(abs(blow - d[i]), abs(bhigh - d[i]));
		if (blow <= d[i] && d[i] <= bhigh) {
			if (blow % 2 == d[i] % 2)
				nlow = 0;
			else nlow = 1;
		}
		range[i] = { nlow, bhigh + d[i] };
	}

	ll D = dist(tar);

	auto [low, high] = range[n - 2];
	vpll ans;
	if (rangeIn(low, high, D)) {
		cout << "YES\n";
		ans.push_back(tar);
		for (ll i = n - 3; i >= 0; i--) {
			tar = make(range[i], tar, d[i + 1]);
			ans.push_back(tar);
		}
		ans.push_back({ 0, 0 });
		reverse(ans.begin(), ans.end());
		for (pll& i : ans) cout << i.first << ' ' << i.second << '\n';
	}
	else cout << "NO";
}