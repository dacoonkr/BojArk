#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef pair<ll, ll> pll;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

struct SEG_PM { //구간더하기와, 전체민 지원
	vector<ll> tree, sum;
	ll st = -1;

	SEG_PM(ll n) {
		st = 1;
		while (st < n) st *= 2;
		tree.resize(2 * st);
		sum.resize(2 * st);
	}

	ll diff = 0;
	void update(ll l, ll r, ll cl, ll cr, ll p) {
		if (l <= cl && cr <= r) {
			sum[p] += diff;
			tree[p] += diff;
			return;
		}
		if (r < cl || cr < l) return;
		ll mid = (cl + cr) / 2;
		update(l, r, cl, mid, p * 2);
		update(l, r, mid + 1, cr, p * 2 + 1);
		tree[p] = min(tree[p * 2], tree[p * 2 + 1]) + sum[p];
	}

	void addQuery(ll l, ll r, ll v) {
		diff = v;
		if (l > r) return;
		update(l, r, 1, st, 1);
	}

	ll globalMin() {
		return tree[1];
	}
};

int main() {
	cin.tie(0)->sync_with_stdio(0); cout.tie(0);

	ll n; cin >> n;
	vector<pair<bool, ll>> v;
	vector<ll> pile(n); //+이면, 세그에서 위치 0based
	vector<pair<ll, ll>> pos; //숫자, idx
	for (ll i = 0; i < n; i++) {
		string s; cin >> s;
		bool sign = (s[0] == '+');
		ll n = stoll(s.substr(1));
		v.push_back({ sign, n });
		if (sign) pos.push_back({ n, i });
	}

	sort(pos.begin(), pos.end());
	map<ll, ll> stP;
	for (ll i = 0; i < pos.size(); i++) {
		pile[pos[i].second] = i + 1;

		//첫등장기록
		if (stP.find(pos[i].first) == stP.end())
			stP[pos[i].first] = i + 1;
	}

	SEG_PM seg(pos.size()); //쿼리: 1베이스

	seg.addQuery(1, seg.st, 2e9); //모든 숫자 미등장
	ll pileC = 0;

	for (ll i = 0; i < n; i++) {
		if (v[i].first) {
			ll pos = pile[i];
			seg.addQuery(pos, pos, -2e9);
			seg.addQuery(pos, pos, v[i].second);
			seg.addQuery(1, pos - 1, 1);
			pileC++;
		}
		else {
			ll pos = stP[v[i].second]++;
			seg.addQuery(pos, pos, 2e9); //미등장
			seg.addQuery(1, pos - 1, -1);
			pileC--;
		}

		cout << min(seg.globalMin(), pileC) << ' ';
	}
}