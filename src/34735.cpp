#include <bits/stdc++.h>
using ll = long long;
using namespace std;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

ll pr[300005];
ll danger[300005];
ll find(ll p) {
	if (pr[p] == p) return p;
	return pr[p] = find(pr[p]);
}

vll adj1[300005], adj2[300005];
ll appear[300005];
ll subtree[300005], ans[300005];

void dfs1(ll p) {
	subtree[p] = 1;
	for (ll t : adj2[p]) {
		if (subtree[t] > 0) continue;
		dfs1(t);
		subtree[p] += subtree[t];
	}
}

void dfs2(ll p, ll v) {
	ans[p] = danger[p] * subtree[p] + v;
	for (ll t : adj2[p]) {
		if (ans[t] > 0) continue;
		ll cursub = subtree[p] - subtree[t];
		dfs2(t, v + danger[p] * cursub);
	}
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	ll n, m; cin >> n >> m;
	vector<pair<ll, ll>> order;
	for (ll i = 1; i <= n; i++) {
		pr[i] = i;
		cin >> danger[i];
		order.push_back({ danger[i], i });
	}
	sort(order.begin(), order.end());
	for (ll i = 0; i < m; i++) {
		ll s, e; cin >> s >> e;
		adj1[s].push_back(e);
		adj1[e].push_back(s);
	}
	ll root = 0;
	for (auto [t, i] : order) {
		appear[i] = 1;
		for (ll nxt : adj1[i]) {
			if (!appear[nxt]) continue;
			ll u = find(i), v = find(nxt);
			if (u == v) continue;
			if (danger[u] > danger[v]) swap(u, v);
			//u < v
			pr[u] = v, root = v;
			adj2[v].push_back(u);
		}
	}
	dfs1(root);
	dfs2(root, 0);
	for (ll i = 1; i <= n; i++)
		cout << ans[i] << ' ';
}