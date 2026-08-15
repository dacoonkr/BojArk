#include <bits/stdc++.h>
using namespace std;
using ll = long long;

//#define cout fout
//ofstream fout("output.txt");

struct Dinic {
    struct Edge {
        int to, cap, flow, rev;
    };

    int n;
    vector<vector<Edge>> adj;
    vector<int> level, work;

    Dinic(int n) : n(n), adj(n), level(n), work(n) {}

    void add_edge(int from, int to, int cap, int rev_cap = 0) {
        adj[from].push_back({ to, cap, 0, (int)adj[to].size() });
        adj[to].push_back({ from, rev_cap, 0, (int)adj[from].size() - 1 });
    }

    bool bfs(int src, int sink) {
        fill(level.begin(), level.end(), -1);
        level[src] = 0;
        queue<int> q;
        q.push(src);

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
        return level[sink] != -1;
    }

    int dfs(int curr, int sink, int flow) {
        if (curr == sink) return flow;

        for (int& i = work[curr]; i < adj[curr].size(); ++i) {
            Edge& edge = adj[curr][i];

            if (level[edge.to] == level[curr] + 1 && edge.cap - edge.flow > 0) {
                int min_flow = dfs(edge.to, sink, min(flow, edge.cap - edge.flow));

                if (min_flow > 0) {
                    edge.flow += min_flow;
                    adj[edge.to][edge.rev].flow -= min_flow;
                    return min_flow;
                }
            }
        }
        return 0;
    }

    int max_flow(int src, int sink) {
        int total_flow = 0;
        while (bfs(src, sink)) {
            fill(work.begin(), work.end(), 0);
            while (int flow = dfs(src, sink, 2e9)) {
                total_flow += flow;
            }
        }
        return total_flow;
    }
};

int main() {
	cin.tie(0)->sync_with_stdio(0); cout.tie(0);

    ll n; cin >> n;

    Dinic graph(2 + n * n + 2 * n);
    //0, 1 / 2n번부터순서대로 / n*n개

    ll zeros = 0;
    for (ll i = 0; i < n; i++) {
        ll st1 = 2 + i;
        for (ll j = 0; j < n; j++) {
            ll st2 = 2 + n + j;
            ll cur = 2 + 2 * n + i * n + j;

            ll x; cin >> x;
            if (x == 0) {
                graph.add_edge(st1, cur, 1);
                graph.add_edge(st2, cur, 1);
                graph.add_edge(cur, 1, 1);
                zeros++;
            }
            else if (x > 1) {
                graph.add_edge(0, cur, x - 1);
                graph.add_edge(cur, st1, x - 1);
                graph.add_edge(cur, st2, x - 1);
            }
        }
    }

    ll fstep = graph.max_flow(0, 1);

    //cout << "fstep: " << fstep << '\n';
    cout << fstep + 2 * (zeros - fstep);
}