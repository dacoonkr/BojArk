#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;
typedef vector<pll> vpll;

//#define cin fin
ifstream fin("input.txt");

vll edge[100005];
ll cnt[100005]; bool det[100005];
vpll tree_rule[100005];

bool vis[100005]; ll val[100005]; ll minv = INT32_MAX;
void dfs(ll n) {
	minv = min(minv, val[n]);
	for (auto [t, c] : tree_rule[n]) {
		if (vis[t]) continue;
		val[t] = val[n] + c, vis[t] = true;
		dfs(t);
	}
}

void prop(ll n) {
	ll fe = edge[n][0];
	for (ll i = 1; i < edge[n].size(); i++) {
		ll t = edge[n][i];
		tree_rule[fe].push_back({ t, cnt[fe] - cnt[t] });
		tree_rule[t].push_back({ fe, cnt[t] - cnt[fe] });
		det[fe] = det[t] = true;
	}
	for (ll t : edge[n]) cnt[t]--;
	cnt[n] += edge[n].size();
}

int main() {
	ll n; cin >> n;
	for (ll i = 0; i < n - 1; i++) {
		ll u, v; cin >> u >> v;
		edge[u].push_back(v);
		edge[v].push_back(u);
	}
	for (ll i = 1; i <= n; i++) prop(i);
	for (ll i = 1; i <= n; i++)
		if (!vis[i]) { vis[i] = true; dfs(i); }
	for (ll i = 1; i <= n; i++) {
		val[i] += (n - minv);
		cout << val[i] << ' ';
	}
}