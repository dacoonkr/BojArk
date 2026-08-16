#include <bits/stdc++.h>
using namespace std;
using ll = long long;

vector<pair<ll, ll>> removal[200005]; //깊이 N

int main() {
	cin.tie(0)->sync_with_stdio(0); cout.tie(0);

	ll n; cin >> n;
	vector<ll> v;

	ll maxV = -1;
	ll firstZero = -1;
	for (ll i = 0; i < n; i++) {
		ll x; cin >> x;
		v.push_back(x);
		if (maxV < x)
			maxV = x, firstZero = i;
	}
	for (ll i = 0; i < n; i++) {
		v[i] = maxV - v[i];
	}
	ll diffsum = 0;
	for (ll i = 1; i < n; i++) {
		ll diff = v[i] - v[i - 1];
		diffsum += max(0ll, diff);
	}

	ll fullCnt = maxV;
	ll bef = 0;

	stack<pair<ll, ll>> stSet; //start, level
	ll removalC = 0;
	ll ans = maxV;
	for (ll t = 0; t <= n; t++) {
		ll i = (firstZero + t) % n;
		if (t > 0) {
			ll diff = v[i] - bef;
			while (diff > 0) {
				if (ans > 200000) {
					break;
				}
				stSet.push({ i, v[i] - (diff - 1) });
				diff--;
			}
			while (diff < 0) {
				if (ans > 200000) {
					removalC -= diff;
					ans = maxV + max(0ll, removalC - maxV);
					break;
				}

				removalC++;
				ans = maxV + max(0ll, removalC - maxV);
				removal[stSet.top().second].push_back({stSet.top().first, i - 1});
				stSet.pop();
				diff++;
			}
			bef = v[i];
		}
	}

	cout << ans << '\n';

	if (ans > 200000) return 0;

	priority_queue<pair<ll, ll>> level_has;
	for (ll i = 1; i <= maxV; i++) {
		if (removal[i].size() == 0 && level_has.size() > 0) {
			auto top = level_has.top();
			if (top.first == 0) continue;
			removal[i].push_back(removal[top.second].back());
			removal[top.second].pop_back();
			level_has.pop();
			level_has.push({ top.first - 1, top.second });
		}
		level_has.push({ removal[i].size(), i });
	}
	for (ll i = 1; i <= maxV; i++) {
		if (removal[i].size() == 0)
			cout << 1 << ' ' << n << '\n';
		else {
			sort(removal[i].begin(), removal[i].end());
			ll st = removal[i].back().second + 1;
			for (ll j = 0; j < removal[i].size(); j++) {
				ll ed = removal[i][j].first;
				if (ed == 0) ed = n;
				cout << st + 1 << ' ' << ed << '\n';
				st = removal[i][j].second + 1;
			}
		}
	}
}