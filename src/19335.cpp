#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;
typedef vector<pll> vpll;

//#define cin fin
ifstream fin("input.txt");

vpll adj[200005]; //입력그래프
vll madj[200005], mradj[200005]; //최단거리그래프(DAG)
vll domin[200005]; //도미네이터 트리
ll up[200005][20];
ll cost[200005], vis[200005], indeg[200005];

ll lca(ll a, ll b) {
	if (vis[a] < vis[b]) swap(a, b);
	for (ll i = 19; i >= 0; i--) {
		ll dep = vis[a] - (1ll << i);
		if (dep >= vis[b]) a = up[a][i];
	}
	if (a == b) return a;
	for (ll i = 19; i >= 0; i--) {
		if (up[a][i] != up[b][i])
			a = up[a][i], b = up[b][i];
	}
	return up[a][0];
}

void push(ll pr, ll p) {
	domin[pr].push_back(p);
	up[p][0] = pr;
	for (ll i = 1; i < 20; i++)
		up[p][i] = up[up[p][i - 1]][i - 1];
	vis[p] = vis[pr] + 1;
}

void cDomin(ll p) {
	vis[p] = 1;
	for (ll t : domin[p]) {
		cDomin(t);
		vis[p] += vis[t];
	}
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	ll n, m; cin >> n >> m;
	vpll edges;
	for (ll i = 0; i < m; i++) {
		ll u, v, c; cin >> u >> v >> c;
		adj[u].push_back({ v, c });
		adj[v].push_back({ u, c });
		edges.push_back({ u, v });
	}
	priority_queue<pair<ll, ll>> que; //-cost, n
	que.push({ 0, 1 });
	while (!que.empty()) {
		auto [c, n] = que.top(); que.pop();
		if (vis[n]) continue;
		vis[n] = 1, cost[n] = -c;
		for (auto [t, ct] : adj[n]) {
			if (vis[t] == 0) que.push({ c - ct, t });
		}
	}
	for (ll i = 1; i <= n; i++) {
		vis[i] = 0;
		for (auto [t, ct] : adj[i]) {
			if (cost[i] + ct == cost[t]) {
				madj[i].push_back(t);
				mradj[t].push_back(i);
				indeg[t]++;
				//cerr << i << " > " << t << '\n';
			}
		}
	}
	up[1][0] = 1;
	for (ll i = 1; i < 20; i++)
		up[1][i] = up[up[1][i - 1]][i - 1];
	queue<ll> tps;
	tps.push(1);
	while (!tps.empty()) {
		ll top = tps.front(); tps.pop();
		if (top != 1) {
			ll clca = mradj[top][0];
			for (ll t : mradj[top])
				clca = lca(clca, t);
			push(clca, top);
			//cerr << clca << " > " << top << '\n';
		}
		for (ll t : madj[top])
			if (--indeg[t] == 0) tps.push(t);
	}
	for (ll i = 1; i <= n; i++) vis[i] = 0;
	cDomin(1);
	for (auto [u, v] : edges) {
		if (up[u][0] == v) cout << vis[u] << '\n';
		else if (up[v][0] == u) cout << vis[v] << '\n';
		else cout << "0\n";
	}
}