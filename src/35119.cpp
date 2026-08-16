#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
	cin.tie(0)->sync_with_stdio(0); cout.tie(0);

	ll n, l; cin >> n >> l;

	vector<pair<ll, ll>> ttm;
	for (ll i = 0; i < n; i++) {
		ll x, y; cin >> y >> x;
		ttm.push_back({ x, y - 1 });
	}

	if (n % 2 == 1) {
		cout << "no";
		return 0;
	}

	sort(ttm.begin(), ttm.end());

	vector<pair<ll, ll>> merged; // 10, 01, 11
	for (ll i = 0; i < n; i++) {
		if (merged.empty() || merged.back().first != ttm[i].first)
			merged.push_back({ ttm[i].first, 1ll << ttm[i].second });
		else
			merged.back().second = 0b11;
	}

	ll startIDX = -1, startShape, ans = 0, oneoneCnt = 0;
	for (ll i = 0; i < merged.size(); i++) {
		if (merged[i].second == 0b11) {
			if (startIDX == -1) continue;
			oneoneCnt++;
		}
		else if (startIDX == -1) {
			startIDX = merged[i].first;
			startShape = merged[i].second;
			oneoneCnt = 0;
		}
		else {
			ans += oneoneCnt;
			ll allEmpty = (merged[i].first - startIDX - 1 - oneoneCnt);

			if (startShape == merged[i].second && allEmpty % 2 == 1)
				ans++;
			if (startShape != merged[i].second && allEmpty % 2 == 0)
				ans++;

			startIDX = -1;
		}
	}

	cout << ans;
}