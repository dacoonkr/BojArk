#include <bits/stdc++.h>
using ll = long long;
using namespace std;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;

bool vis[5005][5005];
vector<ll> adj[5005][26];
vector<ll> inv[5005][26];

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	ll n, m; cin >> n >> m;
	queue<pll> que;
	for (ll i = 0; i < m; i++) {
		ll u, v; char p;
		cin >> u >> v >> p;
		adj[u][p - 'a'].push_back(v);
		inv[v][p - 'a'].push_back(u);
		if (vis[u][v] == 0) {
			vis[u][v] = 1;
			que.push({ u, v });
		}
	}
	for (ll i = 1; i <= n; i++) {
		vis[i][i] = 1;
		que.push({ i, i });
		for (ll c = 0; c < 26; c++) {
			vll& p = adj[i][c];
			sort(p.begin(), p.end());
			p.erase(unique(p.begin(), p.end()), p.end());
		}
		for (ll c = 0; c < 26; c++) {
			vll& p = inv[i][c];
			sort(p.begin(), p.end());
			p.erase(unique(p.begin(), p.end()), p.end());
		}
	}
	while (!que.empty()) {
		auto [u, v] = que.front(); que.pop();
		for (ll c = 0; c < 26; c++) {
			for (ll nu : inv[u][c]) {
				for (ll nv : adj[v][c]) {
					if (vis[nu][nv] == 0) {
						vis[nu][nv] = 1;
						que.push({ nu, nv });
					}
				}
			}
		}
	}
	ll ans = 0;
	for (ll u = 1; u <= n; u++)
		for (ll v = 1; v <= n; v++)
			ans += vis[u][v];
	cout << ans - n;
}