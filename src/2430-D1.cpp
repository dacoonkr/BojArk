#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef pair<ll, ll> pll;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

ll n, m;
vector<ll> adj[100005];
ll deg[100005];
bool incycle[100005], getCycle = false;
ll vis[100005]; ll connected = 0;
ll treeName[100005];
ll distA[100005], distB[100005];

ll treeNamePool = 0;
struct TRIE {
	ll value = -1;
	map<ll, TRIE*> node;
};
TRIE global;

ll pushAndGet(vector<ll>& code) {
	TRIE* cur = &global;
	for (ll t : code) {
		if (cur->node.find(t) == cur->node.end()) { //없다
			cur->node[t] = new TRIE();
		}
		cur = cur->node[t];
	}
	if (cur->value == -1)
		cur->value = ++treeNamePool;
	return cur->value;
}

void dfs(ll p, ll pr) {
	if (vis[p]) return;
	vis[p] = pr, connected++;
	for (ll t : adj[p]) {
		if (vis[t] && t != pr && !getCycle) {
			getCycle = true;
			ll x = p;
			incycle[x] = true;
			while (x != t) {
				x = vis[x];
				incycle[x] = true;
			}
		}
		dfs(t, p);
	}
}

pair<ll, ll> mirror_pair;
void dfs2(ll p) {
	if (vis[p]) return;
	vis[p] = 1;
	for (ll t : adj[p]) {
		if (incycle[t]) {
			if (mirror_pair.first == -1)
				mirror_pair.first = t;
			else if (mirror_pair.second == -1)
				mirror_pair.second = t;
		}
		else dfs2(t);
	}
}

void rootDist(ll p, ll dist[]) {
	dist[p] = 1;
	queue<ll> que; que.push(p);
	while (!que.empty()) {
		ll t = que.front(); que.pop();
		for (ll n : adj[t]) {
			if (dist[n] == 0) {
				dist[n] = dist[t] + 1;
				que.push(n);
			}
		}
	}
}

void shapeMake(ll p) {
	if (treeName[p] > 0) return;
	vis[p] = 1;
	vector<ll> nxt;
	for (ll t : adj[p]) {
		if (vis[t]) continue;
		shapeMake(t);
		nxt.push_back(treeName[t]);
	}
	sort(nxt.begin(), nxt.end());
	treeName[p] = pushAndGet(nxt);
}

bool fixRoot(ll a, ll b) {
	rootDist(a, distA);
	rootDist(b, distB);
	for (ll i = 1; i <= n; i++) {
		if (distA[i] == distB[i]) {
			treeName[i] = ++treeNamePool;
		}
	}
	//복제 후 노드 2N-L 간선2N-2개 L:리프수
	//M=N+L-2
	if (m != n + treeNamePool - 2) return false;
	
	memset(vis, 0, sizeof vis);
	shapeMake(a);
	shapeMake(b);
	return treeName[a] == treeName[b];
}

bool is_mirror() {
	dfs(1, -1);
	if (connected < n) return false;
	vector<ll> oneDeg;
	ll twoDeg = 0;
	for (ll i = 1; i <= n; i++) {
		if (deg[i] == 1) oneDeg.push_back(i);
		twoDeg += (deg[i] == 2);
	}
	if (oneDeg.size() == 2)
		return fixRoot(oneDeg[0], oneDeg[1]);
	if (oneDeg.size() == 1 || oneDeg.size() > 2)
		return false;
	if (twoDeg == n) //환형
		return (n % 2 == 0);
	//사이클 경로들을 제외하고 사이클의 원소를 2개 포함하는지 판정
	memset(vis, 0, sizeof vis);
	for (ll i = 1; i <= n; i++) {
		if (!incycle[i] && vis[i] == 0) {
			mirror_pair = { -1, -1 };
			dfs2(i);
			if (mirror_pair.second > -1) break;
		}
	}
	if (mirror_pair.second == -1 || mirror_pair.first == mirror_pair.second)
		return false;
	return fixRoot(mirror_pair.first, mirror_pair.second);
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));
	
	cin >> n >> m;
	for (ll i = 0; i < m; i++) {
		ll x, y; cin >> x >> y;
		adj[x].push_back(y);
		adj[y].push_back(x);
		deg[x]++, deg[y]++;
	}
	cout << (is_mirror() ? "YES" : "NO");
}