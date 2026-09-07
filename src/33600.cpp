#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;
typedef vector<pll> vpll;

//#define cin fin
ifstream fin("input.txt");

ll n, ps[205];
ll from[505][505][205];
ll dpv[505][505][205];
ll dfs(ll cur_l, ll nxt_gen, ll unsel) { //이번 줄의 비선택자, 다음 줄(이전 레벨에서 대시), 남은 개수
	ll ans = INT32_MAX;
	if (unsel == 0 || cur_l + nxt_gen == 0) {
		from[cur_l][nxt_gen][unsel] = 0; //종료
		if (unsel == 0) return 0;
		else return INT32_MAX;
	}
	if (dpv[cur_l][nxt_gen][unsel])
		return dpv[cur_l][nxt_gen][unsel];
	if (cur_l < unsel) {
		ll v = dfs(cur_l + nxt_gen, cur_l, unsel) + ps[unsel];
		if (v < ans) ans = v, from[cur_l][nxt_gen][unsel] = 1; //다음줄로 이동
	}
	if (cur_l > 0) {
		ll v = dfs(cur_l - 1, nxt_gen, unsel - 1);
		if (v < ans) ans = v, from[cur_l][nxt_gen][unsel] = 2; //이번줄에서 리프 지정
	}
	return dpv[cur_l][nxt_gen][unsel] = ans;
}

vector<string> ans;
void track() {
	ll cur_l = 1, nxt_gen = 0, unsel = n;
	vector<string> cur = { "" }, nxt;
	while (ll sel = from[cur_l][nxt_gen][unsel]) {
		if (sel == 1) {
			vector<string> t_nxt = cur;
			for (string& i : cur) i += '.';
			for (string& i : nxt) cur.push_back(i + '-');
			nxt = t_nxt;
			
			ll t_nxt_gen = cur_l;
			cur_l += nxt_gen;
			nxt_gen = t_nxt_gen;
		}
		else if (sel == 2) {
			ans.push_back(cur.back());
			cur.pop_back();

			cur_l--, unsel--;
		}
	}
	reverse(ans.begin(), ans.end());
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	cin >> n;
	vpll costs;
	for (ll i = 0; i < n; i++) {
		ll x; char p;
		cin >> x >> p >> x;
		costs.push_back({ x, i });
	}
	sort(costs.begin(), costs.end());
	for (ll i = 0; i < n; i++)
		ps[i + 1] = ps[i] + costs[i].first;

	cerr << dfs(1, 0, n) << '\n';
	track();

	vector<pair<ll, string>> out;
	for (ll i = 0; i < n; i++)
		out.push_back({ costs[i].second, ans[i] });
	sort(out.begin(), out.end());
	for (ll i = 0; i < n; i++)
		cout << out[i].second << '\n';
}