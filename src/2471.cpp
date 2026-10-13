#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;
typedef vector<pll> vpll;

//#define cin fin
ifstream fin("input.txt");

//(0(1(0)))
// 
//[0-8]: 0[2-7]

string s;
ll n, k, cls[205];

vector<string> st[105]; ll used = 0;

ll nth(ll l, ll r) {
	ll cur = used++;
	vll chIDX;
	//자식 탐색
	for (ll i = l + 1; i < r; i++) {
		if (s[i] == '(') {
			chIDX.push_back(nth(i, cls[i]));
			i = cls[i];
		}
	}
	if (chIDX.empty()) { //leaf
		string cs = s.substr(l + 1, r - l - 1);
		st[cur].push_back(cs);
		reverse(cs.begin(), cs.end());
		st[cur].push_back(cs);
	}
	else {
		vll sel(chIDX.size()), dsel(chIDX.size());
		for (ll i = 0; i < k; i++) {
			string cs, rs;
			ll itemI = 0;
			for (ll j = l + 1; j < r; j++) {
				if (s[j] == '(') {
					cs += st[chIDX[itemI]][sel[itemI]];
					string dc = st[chIDX[itemI]][dsel[itemI]];
					reverse(dc.begin(), dc.end());
					rs += dc;
					j = cls[j], itemI++;
				}
				else cs += s[j], rs += s[j];
			}
			reverse(rs.begin(), rs.end());
			st[cur].push_back(cs);
			st[cur].push_back(rs);
			//increment
			sel.back()++;
			dsel[0]++;
			bool end = false;
			for (ll j = (ll)sel.size() - 1; j >= 0; j--) {
				if (sel[j] == st[chIDX[j]].size()) {
					if (j == 0) { end = true;  break; }
					sel[j - 1]++, sel[j] = 0;
				}
			}
			if (end) break;
			for (ll j = 0; j < dsel.size(); j++) {
				if (dsel[j] == st[chIDX[j]].size()) {
					dsel[j + 1]++, dsel[j] = 0;
				}
			}
		}
	}
	//병합
	sort(st[cur].begin(), st[cur].end());
	st[cur].erase(unique(st[cur].begin(), st[cur].end()), st[cur].end());
	st[cur].resize(min(k, (ll)st[cur].size()));
	return cur;
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	cin >> s >> k;
	n = s.size();
	stack<ll> stk;
	for (ll i = 0; i < n; i++) {
		if (s[i] == '(') stk.push(i);
		else if (s[i] == ')')
			cls[stk.top()] = i, cls[i] = stk.top(), stk.pop();
	}
	ll wr = nth(0, n - 1);
	if (st[wr].size() < k) cout << "NO";
	else cout << st[wr][k - 1];
}