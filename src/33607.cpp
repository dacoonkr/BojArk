#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;
typedef vector<pll> vpll;

//#define cin fin
ifstream fin("input.txt");

//TeamNote
struct Dinic {
    const ll INF = 1e18;

    struct Edge {
        int to;
        ll cap;
        ll flow;
        int rev;
    };

    int n;
    vector<vector<Edge>> adj;
    vector<int> level;
    vector<int> work;

    Dinic(int n) : n(n), adj(n), level(n), work(n) {}

    ll newn() {
        n++;
        adj.resize(n);
        level.resize(n);
        work.resize(n);
        return n - 1;
    }

    void add_edge(int from, int to, ll cap) {
        adj[from].push_back({ to, cap, 0, (int)adj[to].size() });
        adj[to].push_back({ from, 0, 0, (int)adj[from].size() - 1 });
        //cerr << from << " -> " << to << " : " << cap << '\n';
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);

        while (!q.empty()) {
            int curr = q.front();
            q.pop();

            for (const auto& edge : adj[curr]) {
                if (level[edge.to] == -1 && edge.cap - edge.flow > 0) {
                    level[edge.to] = level[curr] + 1;
                    q.push(edge.to);
                }
            }
        }
        return level[t] != -1;
    }

    ll dfs(int curr, int t, ll flow) {
        if (curr == t) return flow;

        for (int& i = work[curr]; i < adj[curr].size(); ++i) {
            Edge& edge = adj[curr][i];

            if (level[edge.to] == level[curr] + 1 && edge.cap - edge.flow > 0) {
                ll min_flow = dfs(edge.to, t, min(flow, edge.cap - edge.flow));

                if (min_flow > 0) {
                    edge.flow += min_flow;
                    adj[edge.to][edge.rev].flow -= min_flow;
                    return min_flow;
                }
            }
        }
        return 0;
    }

    ll max_flow(int s, int t) {
        ll total_flow = 0;
        while (bfs(s, t)) {
            fill(work.begin(), work.end(), 0);
            while (ll flow = dfs(s, t, INF)) {
                total_flow += flow;
            }
        }
        return total_flow;
    }
};

void solve() {
    ll n; cin >> n;
    map<ll, ll> cnt;
    for (ll i = 0; i < n; i++) {
        ll a; cin >> a;
        cnt[a]++;
    }
    Dinic graph(2 + cnt.size());
    map<ll, vll> candi;
    unordered_map<ll, ll> src;
    for (auto [i, c] : cnt) {
        src[i] = (ll)src.size() + 2;
        graph.add_edge(0, src[i], c);
        candi[i] = { i };
    }
    ll match = 0;
    while (1) {
        auto it = candi.begin();
        ll j = it->first;
        ll c = graph.newn();
        for (ll t : it->second)
            graph.add_edge(src[t], c, 1);
        graph.add_edge(c, 1, 1);
        ll f = graph.max_flow(0, 1);
        match += f;
        if (match == n) {
            cout << j << '\n';
            return;
        }
        if (f > 0) {
            for (ll t : it->second) {
                ll nxt = j + t;
                if (candi.find(nxt) == candi.end()) candi[nxt] = {};
                candi[nxt].push_back(t);
            }
        }
        candi.erase(it);
    }
}

int main() {
    cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

    ll t; cin >> t;
    while (t--) solve();
}