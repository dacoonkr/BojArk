#include <bits/stdc++.h>
using namespace std;
using ll = long long;

//#define cout fout
//ofstream fout("output.txt");

vector<ll> adj[500005];
ll vis[500005];
ll ring_next[500005];
ll dp_far[500005];
ll dp_dia[500005]; //지름

bool find_ring(ll p, ll tar) {
	vis[p] = 1;
	bool way = false;
	for (ll t : adj[p]) {
		if (vis[t]) continue;
		if (t == tar) {
			ring_next[t] = -1;
			ring_next[p] = -1;
			return true;
		}
		bool nt = find_ring(t, tar);
		if (nt) {
			ring_next[p] = t;
			return true;
		}
	}
	return false;
}

void farp(ll p) {
	vis[p] = 1;
	ll ans = 0;
	vector<ll> nexts;
	dp_dia[p] = 0;
	for (ll t : adj[p]) {
		if (vis[t]) continue;
		if (ring_next[t] != 0) continue;
		farp(t);
		dp_dia[p] = max(dp_dia[p], dp_dia[t]);
		nexts.push_back(dp_far[t] + 1);
		ans = max(ans, dp_far[t] + 1);
	}
	dp_far[p] = ans;

	sort(nexts.rbegin(), nexts.rend());
	if (nexts.size() >= 1) dp_dia[p] = max(dp_dia[p], nexts[0]);
	if (nexts.size() >= 2) dp_dia[p] = max(dp_dia[p], nexts[0] + nexts[1]);
}

void solve() {
	ll n, x, y; cin >> n >> x >> y;
	for (ll i = 1; i <= n; i++) {
		adj[i].clear();
		vis[i] = 0;
		ring_next[i] = 0;
	}
	for (ll i = 0; i < n - 1; i++) {
		ll u, v; cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	find_ring(x, y);
	for (ll i = 1; i <= n; i++)
		vis[i] = 0;

	for (ll t : adj[y]) {
		adj[x].push_back(t);
	}

	vector<ll> diameter, most_far;
	ll on_ring = x, ring_size = 0;
	while (on_ring != -1) {
		farp(on_ring);
		diameter.push_back(dp_dia[on_ring]);
		most_far.push_back(dp_far[on_ring]);
		ring_size++, on_ring = ring_next[on_ring];
	}

	ll ans = 2 * n;

	if (ring_size == 1) {
		ans = 2 * (n - 2) - diameter[0];
	}
	else if (ring_size > 1) {
		ll max_d = 0, max_a = 0, max_b = 0; //ai-i, aj+j
		for (ll i = 0; i < ring_size; i++) {
			//탐색: s,e on one tree
			ans = min(ans, 2 * (n - 1) - ring_size - diameter[i]);

			//탐색: s,e on another tree (i < j)
			if (i > 0) {
				max_d = max(max_d, max_a + most_far[i] + i);
				max_d = max(max_d, ring_size + max_b + most_far[i] - i);
			}
			max_a = max(max_a, most_far[i] - i);
			max_b = max(max_b, most_far[i] + i);
		}

		ans = min(ans, 2 * (n - 2) - max_d);
	}

	cout << ans << '\n';
}

int main() {
	cin.tie(0)->sync_with_stdio(0); cout.tie(0);

	ll t; cin >> t;
	while (t--) solve();
}