#include <bits/stdc++.h>
using ll = long long;
using namespace std;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

ll clamp(ll v) {
	if (v > 1e9) return 1e9;
	return v;
}

vll cards[2]; ll atk = 0;
pll usage(ll p, ll add, ll mul, ll atk) {
	for (ll i = 0; i < add; i++) p = clamp(p + cards[0][i]);
	for (ll i = 0; i < mul; i++) p = clamp(p * cards[1][i]);
	ll ans = 0;
	for (ll i = 0; i < atk; i++) ans = clamp(ans + p);
	return { p, ans };
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	ll n, p, h; cin >> n >> p >> h;
	bool inc = false;
	for (ll i = 0; i < n; i++) {
		char c; cin >> c;
		if (c == '!') atk++;
		else {
			ll n; cin >> n;
			if (c == '+') cards[0].push_back(n), inc = true;
			if (c == '*') cards[1].push_back(n), inc |= (n > 1);
		}
	}
	sort(cards[0].rbegin(), cards[0].rend());
	sort(cards[1].rbegin(), cards[1].rend());
	if (atk == 0) cout << "*";
	else if (!inc) {
		if (p == 0) cout << "*";
		else {
			ll use = (h + p - 1) / p; //!
			ll cards = ((use - 1) / atk) * (n - atk);
			cout << use + cards;
		}
	}
	else {
		ll ans = 0;
		while (1) {
			auto [np, now] = usage(p, cards[0].size(), cards[1].size(), atk);
			if (now < h) h -= now, p = np, ans += n;
			else break;
		}
		vector<ll> max_n(n + 1); //카드 N장 썼을 때 최대 어택
		for (ll i = 0; i <= cards[0].size(); i++) {
			for (ll j = 0; j <= cards[1].size(); j++) {
				for (ll k = 1; k <= atk; k++) {
					auto [np, now] = usage(p, i, j, k);
					max_n[i + j + k] = max(max_n[i + j + k], now);
				}
			}
		}
		for (ll i = 1; i <= n; i++) {
			if (max_n[i] >= h) {
				cout << ans + i;
				break;
			}
		}
	}
}