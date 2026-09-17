#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;
typedef vector<pll> vpll;

//#define cin fin
ifstream fin("input.txt");

ll n, m;
vll adj[200005];

ll par[200005], dep[200005], dsu[200005];
vll bfsOrder, childs[200005], non_tree[200005]; //양방향

multimap<ll, ll> esc[200005]; ll lazy[200005]; //탈출 (d+lazy, u)
ll ans1[200005]; //bfs트리에서 직전의 간선이 끊겼을 때, 서브트리로 돌아가 루트로 가는 최소경로
ll ans2[200005];

ll find(ll p) {
	if (dsu[p] == p) return p;
	return dsu[p] = find(dsu[p]);
}

void buildBFStree() {
	queue<ll> q;
	q.push(n); par[n] = -1;
	while (!q.empty()) {
		ll top = q.front(); q.pop();
		bfsOrder.push_back(top);
		for (ll t : adj[top]) {
			if (t == par[top]) continue;
			if (par[t] == 0) { //treeEdge
				par[t] = top;
				childs[top].push_back(t);
				dep[t] = dep[top] + 1;
				q.push(t);
				//cerr << "tree-edge: " << top << ' ' << t << '\n';
			}
			else {
				non_tree[top].push_back(t);
				non_tree[t].push_back(top);
				//cerr << "non-tree-edge: " << top << ' ' << t << '\n';
			}
		}
	}
}

void get_ans1() {
	reverse(bfsOrder.begin(), bfsOrder.end());
	for (ll u : bfsOrder) {
		if (childs[u].size() > 0) {
			ll topChd = childs[u][0];
			for (ll i = 1; i < childs[u].size(); i++) {
				if (esc[topChd].size() < esc[childs[u][i]].size())
					topChd = childs[u][i];
			}
			esc[u] = move(esc[topChd]);
			lazy[u] = lazy[topChd];
			for (ll chd : childs[u]) {
				dsu[chd] = u;
				if (chd == topChd) continue;
				for (auto [key, value] : esc[chd])
					esc[u].insert({ key - (lazy[u] - lazy[chd]), value });
			}
		}
		lazy[u]++;
		for (ll t : non_tree[u])
			esc[u].insert({ 1 + dep[t] - lazy[u], t });
		while (!esc[u].empty()) {
			auto it = esc[u].begin();
			if (find(it->second) == u) esc[u].erase(it);
			else break;
		}
		if (esc[u].empty()) ans1[u] = INT32_MAX;
		else ans1[u] = esc[u].begin()->first + lazy[u];
	}
	ans1[n] = 0;
}

void get_ans2() {
	memset(ans2, -1, sizeof ans2);
	priority_queue<pair<ll, ll>> pq; //-cost, p
	pq.push({ 0, n });
	while (!pq.empty()) {
		auto [cost, u] = pq.top(); pq.pop();
		if (ans2[u] != -1) continue;
		ans2[u] = max(ans1[u], -cost);
		for (ll t : adj[u]) {
			if (ans2[t] != -1) continue;
			pq.push({ -ans2[u] - 1, t });
		}
	}
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	cin >> n >> m;
	for (ll i = 1; i <= n; i++)
		dsu[i] = i;
	for (ll i = 0; i < m; i++) {
		ll u, v; cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	buildBFStree();
	get_ans1();
	//for (ll i = 1; i <= n; i++) cerr << ans1[i] << ' ';
	get_ans2();
	//for (ll i = 1; i <= n; i++) cerr << ans2[i] << ' ';

	if (ans2[1] >= INT32_MAX) cout << -1;
	else cout << ans2[1];
}