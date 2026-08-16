#include <bits/stdc++.h>
using ll = long long;
using namespace std;

ll humil[100005];
bool vis[100005];
vector<pair<ll, ll>> adj[100005];

int main() {
	cin.tie(0)->sync_with_stdio(0); cout.tie(0);

	ll n, m; cin >> n >> m;

	for (ll i = 1; i <= n; i++) {
		ll x; cin >> x;
		humil[i] = x;
	}
	for (ll i = 0; i < m; i++){
		ll vi, vj, q; cin >> vi >> vj >> q;
		adj[vi].push_back({ vj, q });
		adj[vj].push_back({ vi, q });
	}

	ll sum = 0;
	vis[1] = true;

	priority_queue<pair<ll, ll>> prim; //-len, idx
	for (auto& t : adj[1]) {
		prim.push({ -t.second, t.first });
	}

	while (!prim.empty()) {
		auto top = prim.top(); prim.pop();
		ll next = top.second;

		if (vis[next]) continue; //이미갔음
		sum += -top.first * humil[next];
		vis[next] = true;

		for (auto& t : adj[next]) {
			ll nlen = (t.second - top.first);
			if (vis[t.first]) continue;
			prim.push({ -nlen, t.first });
		}
	}
	cout << sum;
}