#include <bits/stdc++.h>
using namespace std;
using ll = long long;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

vector<ll> get_fail(string p) {
	vector<ll> f(p.size(), 0);
	for (ll i = 1, j = 0; i < p.size(); i++) {
		while (j && p[i] != p[j]) j = f[j - 1];
		if (p[i] == p[j]) f[i] = ++j;
	}
	return f;
}

vector<ll> max_prefix(string s, string p) {
	vector<ll> f = get_fail(p);
	vector<ll> prefix(s.size());
	for (ll i = 0, j = 0; i < s.size(); i++) {
		while (j && s[i] != p[j])j = f[j - 1];
		if (s[i] == p[j])
			j++;
		if (j > 0) { //j길이매칭
			ll idx = i - (j - 1);
			prefix[idx] = max(prefix[idx], j);
		}
		if (j == p.size())
			j = f[j - 1];
	}
	return prefix;
}

struct SEG {
	vector<ll> seg;
	ll st;
	SEG(ll n) {
		st = 1;
		while (st < n) st *= 2;
		seg.resize(st * 2);
	}
	void update(ll i, ll v) { //0base
		ll p = st + i;
		if (v == INT_MAX || seg[p] > v) {
			seg[p] = v;
			while (p > 1) {
				p >>= 1;
				seg[p] = min(seg[p * 2], seg[p * 2 + 1]);
			}
		}
	}
	ll query(ll l, ll r, ll cl, ll cr, ll p) {
		if (l <= cl && cr <= r) return seg[p];
		if (r < cl || cr < l) return INT_MAX;
		ll mid = (cl + cr) / 2;
		ll left = query(l, r, cl, mid, p * 2);
		ll right = query(l, r, mid + 1, cr, p * 2 + 1);
		return min(left, right);
	}
	ll minOf(ll l, ll r) {
		return query(l, r, 0, st - 1, 1);
	}
};

int main() {
	cin.tie(0)->sync_with_stdio(0); cout.tie(0);

	ll m, n; cin >> m >> n;
	string p, s; cin >> p >> s;

	vector<ll> Mpre = max_prefix(s, p);

	SEG min_dpmi(n + 1); //dp[i] - i
	ll min_dppi = n; //dp[i] + i

	for (ll i = 0; i < min_dpmi.st; i++) {
		min_dpmi.update(i, INT_MAX);
	}

	ll cur = -1;

	//여기서부터 새시작
	vector<ll> reS(n + 1, INT_MAX);
	reS[n] = 0;

	for (ll i = n - 1; i >= 0; i--) {
		//1개씩설치
		cur = min_dppi - i;
		//삭제후설치
		if (Mpre[i] > 0) {
			ll minv = min_dpmi.minOf(i + 1, i + Mpre[i]);
			if (minv < INT_MAX) {
				ll cost = minv + i + m;
				cur = min(cur, cost);
				min_dpmi.update(i, cost - i);
				min_dppi = min(min_dppi, cost + i);
			}
			else min_dpmi.update(i, INT_MAX);
		}
		else min_dpmi.update(i, INT_MAX);
		//전부넣음
		if (Mpre[i] == m) {
			ll cost = reS[i + m];
			cur = min(cur, cost);
			min_dpmi.update(i, cost - i);
			min_dppi = min(min_dppi, cost + i);
		}
		//갱신
		reS[i] = cur;
	}
	cout << cur;
}