#include <bits/stdc++.h>
#define V_MIN -1000000000000000001ll * 300000ll
using namespace std;
using ll = long long;

vector<pair<ll, ll>> edge[300005];
vector<pair<ll, ll>> nodes; // (m값, node넘버)
bool visited[300005];
bool toggled[300005];

vector<pair<ll, ll>> arr; //{depth, NODE number }
ll firstAppear[300005];
ll toroot[300005];

void ett(ll p, ll depth, ll dist) {
	visited[p] = true;
	toroot[p] = dist;
	firstAppear[p] = arr.size();
	arr.push_back({ depth, p });
	for (auto [next, cost] : edge[p]) {
		if (visited[next]) continue;
		ett(next, depth + 1, dist + cost);
		arr.push_back({ depth, p });
	}
}

vector<pair<ll, ll>> smin[21];

ll LCA(ll a, ll b) {
	ll l = firstAppear[a], r = firstAppear[b], idx = 0;
	if (l > r) swap(l, r);
	while ((2ll << idx) < r - l + 1)
		idx++;
	return min(smin[idx][l], smin[idx][r - (1ll << idx) + 1]).second;
}

ll get_distance(ll a, ll b) {
	return toroot[a] + toroot[b] - 2 * toroot[LCA(a, b)];
}

ll parent[300005];
ll diameter_s[300005], diameter_e[300005];
ll gmd = V_MIN, gmd_s, gmd_e; //global max diameter

ll find(ll p) {
	if (parent[p] == p) return p;
	return parent[p] = find(parent[p]);
}

void unite(ll a, ll b, ll cost) {
	vector<ll> points = {
		diameter_e[find(a)],
		diameter_s[find(a)],
		diameter_e[find(b)],
		diameter_s[find(b)]
	};
	ll maxdist = V_MIN, p1, p2;
	for (ll i = 0; i < 4; i++) {
		for (ll j = i + 1; j < 4; j++) {
			ll dist = get_distance(points[i], points[j]);
			if (maxdist < dist)
				maxdist = dist, p1 = points[i], p2 = points[j];
		}
	}
	parent[find(b)] = find(a);
	diameter_e[find(a)] = p1;
	diameter_s[find(a)] = p2;
	if (gmd < maxdist)
		gmd = maxdist, gmd_s = p1, gmd_e = p2;
}

int main() {
	cin.tie(0)->sync_with_stdio(0); cout.tie(0);

	ll n, q;
	cin >> n;

	for (ll i = 0; i < n; i++) {
		ll m; cin >> m;
		parent[i + 1] = i + 1;
		diameter_e[i + 1] = i + 1;
		diameter_s[i + 1] = i + 1;
		nodes.push_back({ m, i + 1 });
	}

	vector<ll> inputs;
	for (ll i = 0; i < n - 1; i++) {
		ll x; cin >> x;
		inputs.push_back(x);
	}
	for (ll i = 0; i < n - 1; i++) {
		ll x; cin >> x;
		edge[i + 1].push_back({ inputs[i], x });
		edge[inputs[i]].push_back({ i + 1, x });
	}

	ett(1, 0, 0);

	//RMQ 전처리
	smin[0] = arr;
	for (ll i = 1; i < 21; i++) {
		ll len = 1ll << i;
		for (ll j = 0; j < (ll)arr.size() - len + 1; j++) {
			smin[i].push_back(
				min(smin[i - 1][j], smin[i - 1][j + len / 2])
			);
		}
	}

	sort(nodes.begin(), nodes.end());

	ll ans = V_MIN, p1, p2;
	for (ll i = 0; i < n; i++) {
		//disjoint set에 nodes[i].second 번을 추가하면 됨.
		ll cur = nodes[i].second;
		toggled[cur] = true;
		for (auto [next, cost] : edge[cur]) {
			if (toggled[next])
				unite(cur, next, cost); //cur과 next를 cost비용으로 이음
		}
		ll tmp = gmd - nodes[i].first;
		if (ans < tmp)
			ans = tmp, p1 = gmd_s, p2 = gmd_e;
	}

	cout << min(p1, p2) << ' ' << max(p1, p2);
}