#include <bits/stdc++.h>
using ll = long long;
using namespace std;

typedef pair<ll, pair<ll, ll>> item; //{}
const ll mod = 998244353;

vector<item> parse(string s) {
	stack<pair<ll, ll>> stk;
	vector<item> ret;
	ll oneC = 0;
	for (char c : s) {
		if (c == '1') {
			oneC++;
			stk.push({ oneC, oneC });
		}
		else if (c == ')') {
			auto b = stk.top(); stk.pop();
			auto a = stk.top(); stk.pop();
			ret.push_back({ a.second, { a.first, b.second } });
			stk.push({ a.first, b.second });
		}
	}
	return ret;
}

int main() {
	cin.tie(0)->sync_with_stdio(0); cout.tie(0);

	string a, b; cin >> a >> b;

	vector<item> ap = parse(a), bp = parse(b);
	
	sort(ap.begin(), ap.end());
	sort(bp.begin(), bp.end());

	ll ans = 1;
	for (ll i = 0; i < ap.size(); i++) {
		ll l = max(ap[i].second.first, bp[i].second.first);
		ll r = min(ap[i].second.second, bp[i].second.second);
		ll cut = ap[i].first;
		ans *= (cut - l + 1) * (r - cut) % mod;
		ans %= mod;
	}

	cout << ans;
}