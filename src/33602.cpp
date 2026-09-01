#include <bits/stdc++.h>
using ll = long long;
using namespace std;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;
typedef vector<pll> vpll;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	ll n; cin >> n;

	queue<ll> stidx[26];
	vector<string> strs(n);
	vll uses(n);
	for (ll i = 0; i < n; i++) {
		cin >> strs[i];
		stidx[strs[i][0] - 'a'].push(i);
	}
	string masc; cin >> masc;
	string ans;
	ll curp = 0;
	while (1) {
		ll non_use = -1, use = -1;
		for (ll i = 0; i < 26; i++) {
			if (stidx[i].empty()) continue;
			if (masc[curp] == ('a' + i))
				use = stidx[i].front();
			else non_use = stidx[i].front();
		}
		if (non_use == -1 && use == -1) break;
		ll ruse = non_use;
		if (ruse == -1) {
			ruse = use;
			curp++;
			if (curp == masc.size()) {
				cout << "NO"; return 0;
			}
		}
		char moji = strs[ruse][uses[ruse]];
		ans += moji;
		//clear moji
		ll sz = stidx[moji - 'a'].size();
		while (sz--) {
			ll ruse = stidx[moji - 'a'].front();
			uses[ruse]++;
			stidx[moji - 'a'].pop();
			if (uses[ruse] < strs[ruse].size())
				stidx[strs[ruse][uses[ruse]] - 'a'].push(ruse);
		}
	}
	cout << "YES\n" << ans;
}