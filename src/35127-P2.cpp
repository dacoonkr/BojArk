#include <bits/stdc++.h>
using namespace std;
using ll = long long;

void tc() {
	ll n; cin >> n;
	string s; cin >> s;
	s += "#";

	vector<pair<ll, ll>> segment; //...
	ll pairs = -1;
	for (ll i = 0; i <= n; i++) {
		if (s[i] == '.') {
			if (pairs == -1) pairs = i;
		}
		else {
			if (pairs >= 0)
				segment.push_back({ pairs, i - 1 });
			pairs = -1;
		}
	}

	if (segment.size() == 1 && segment[0] == make_pair(0ll, n - 1)) {
		if (n == 1) cout << "alice\n";
		else cout << "bob\n";
		return;
	}

	ll cnt = 0;
	for (ll i = 0; i < segment.size(); i++) {
		if (segment[i].first == 0) {
			ll weight = (segment[i].second == 0) ? 2 : 1;
			if (s[segment[i].second + 1] == 'a') cnt -= weight;
			else cnt += weight;
		}
		else if (segment[i].second == n - 1) {
			ll weight = (segment[i].first == n - 1) ? 2 : 1;
			if (s[segment[i].first - 1] == 'a') cnt -= weight;
			else cnt += weight;
		}
		else if (s[segment[i].first - 1] == s[segment[i].second + 1]) {
			if (s[segment[i].first - 1] == 'a') cnt -= 2;
			else cnt += 2;
		}
	}

	if (cnt > 0) cout << "alice\n";
	else cout << "bob\n";
}

int main() {
	cin.tie(0)->sync_with_stdio(0); cout.tie(0);

	ll t; cin >> t;
	while (t--) tc();
}