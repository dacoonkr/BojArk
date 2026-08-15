#include <bits/stdc++.h>
using namespace std;
using ll = long long;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

vector<pair<ll, ll>> u, v;
vector<string> board;
ll idx[405][405];

void selEdge(ll x, ll y) {
	pair<ll, ll> a = (x > u.size()) ? v[x - u.size() - 1] : u[x - 1];
    pair<ll, ll> b = (y > u.size()) ? v[y - u.size() - 1] : u[y - 1];
	a = { 2 * a.first + 1, 2 * a.second + 1 };
	b = { 2 * b.first + 1, 2 * b.second + 1 };
    board[(a.first + b.first) / 2][(a.second + b.second) / 2] = '.';
}

const int INF = 1e9;
//1-base 이분매칭
struct HopcroftKarp {
    int n, m;
    vector<vector<int>> adj;
    vector<int> pairA, pairB, dist;

    HopcroftKarp(int n, int m) : n(n), m(m), adj(n + 1), pairA(n + 1, 0), pairB(m + 1, 0), dist(n + 1) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    bool bfs() {
        queue<int> q;
        for (int u = 1; u <= n; u++) {
            if (pairA[u] == 0) {
                dist[u] = 0;
                q.push(u);
            }
            else {
                dist[u] = INF;
            }
        }
        dist[0] = INF;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            if (dist[u] < dist[0]) {
                for (int v : adj[u]) {
                    if (dist[pairB[v]] == INF) {
                        dist[pairB[v]] = dist[u] + 1;
                        q.push(pairB[v]);
                    }
                }
            }
        }
        return dist[0] != INF;
    }

    bool dfs(int u) {
        if (u == 0) return true;

        for (int v : adj[u]) {
            if (dist[pairB[v]] == dist[u] + 1) {
                if (dfs(pairB[v])) {
                    pairA[u] = v;
                    pairB[v] = u;
                    return true;
                }
            }
        }
        dist[u] = INF;
        return false;
    }

    int max_matching() {
        int match = 0;
        while (bfs()) {
            for (int u = 1; u <= n; u++) {
                if (pairA[u] == 0 && dfs(u)) {
                    match++;
                }
            }
        }
        return match;
    }
};

struct DFS {
    vector<vector<int>> adj;
    vector<int> vis;

    DFS(int n) : adj(n + 1), vis(n + 1) {}

    void add_edge(int a, int b) {
        adj[a].push_back(b);
    }

    void dfs(int p, int num) {
        vis[p] = num;
        for (int i = 0; i < adj[p].size(); i++) {
            int nxt = adj[p][i];
            if (vis[nxt]) continue;
            selEdge(p, nxt);
            dfs(nxt, num);
        }
    }
};

struct DSU {
    vector<ll> pr;

	DSU(ll n) : pr(n + 1) {
		for (ll i = 1; i <= n; i++) pr[i] = i;
	}

	ll find(ll x) {
		if (pr[x] == x) return x;
		return pr[x] = find(pr[x]);
	}

	ll merge(ll a, ll b) {
		a = find(a);
		b = find(b);
		if (a == b) return 0;
		pr[b] = a;
		return 1;
	}
};

int main() {
    cin.tie(0)->sync_with_stdio(0); cout.tie(0);

    ll n, m; cin >> n >> m;
    vector<pair<ll, ll>> edge;

    for (ll i = 0; i < 2 * n + 1; i++) {
        string s; cin >> s;
        board.push_back(s);
    }

    for (ll i = 0; i < n; i++) {
        for (ll j = 0; j < m; j++) {
            //칸 판정
            if (board[2 * i + 1][2 * j + 1] == '.') {
                if ((i + j) % 2 == 0) {
                    u.push_back({ i, j });
                    idx[i][j] = u.size();
                }
                else {
                    v.push_back({ i, j });
                    idx[i][j] = v.size();
                }
            }
            //왼쪽 통로 판정
            if (board[2 * i + 1][2 * j] == '.') {
                if ((i + j) % 2 == 0)
                    edge.push_back({ idx[i][j], idx[i][j - 1] });
                else
                    edge.push_back({ idx[i][j - 1], idx[i][j] });
				board[2 * i + 1][2 * j] = '#';
            }
            //위쪽 통로 판정
            if (board[2 * i][2 * j + 1] == '.') {
                if ((i + j) % 2 == 0)
                    edge.push_back({ idx[i][j], idx[i - 1][j] });
                else
                    edge.push_back({ idx[i - 1][j], idx[i][j] });
				board[2 * i][2 * j + 1] = '#';
            }
        }
    }
    if (u.size() == v.size()) {
        cout << "No\n"; return 0;
    }
    bool swapped = false;
    if (u.size() > v.size()) swap(u, v), swapped = true;

    HopcroftKarp match(u.size(), v.size());
    for (auto [a, b] : edge) {
        if (swapped) match.add_edge(b, a);
        else match.add_edge(a, b);
    }

    ll matches = match.max_matching();

    if (u.size() > matches) {
        cout << "No\n"; return 0;
    }

    DFS dfs(u.size() + v.size());
    for (ll i = 1; i <= u.size(); i++) {
        for (ll t : match.adj[i]) {
            if (match.pairA[i] == t)
                dfs.add_edge(i, t + u.size());
            else dfs.add_edge(t + u.size(), i);
        }
    }

    ll num = 0;
    for (ll i = 1; i <= v.size(); i++) {
        if (match.pairB[i] == 0 && dfs.vis[u.size() + i] == 0) {
            dfs.dfs(u.size() + i, ++num);
        }
    }

    DSU dsu(u.size() + v.size());
    for (ll i = 1; i <= u.size(); i++) {
        if (dfs.vis[i] == 0) {
            cout << "No\n"; return 0;
        }
        for (ll t : match.adj[i]) {
			ll x = dfs.vis[i], y = dfs.vis[u.size() + t];
            if (x != y) {
                if (dsu.merge(x, y))
                    selEdge(i, u.size() + t);
            }
        }
    }

    cout << "Yes\n";
    for (ll i = 0; i < 2 * n + 1; i++) {
		cout << board[i] << "\n";
    }
}