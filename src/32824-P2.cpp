#include <bits/stdc++.h>
using namespace std;
using ll = long long;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

ll vis[100005];
ll low[100005];
vector<pair<ll, ll>> adj[100005];

struct EdgeCutting {
    ll visc = 0;
    vector<ll> usedV;
    vector<ll> cuts;

    EdgeCutting(vector<ll>& used) {
        usedV = used;
        for (ll i : usedV) {
            vis[i] = 0;
            low[i] = 0;
            adj[i].clear();
        }
    }

    void add_edge(ll i, ll j, ll num) {
        adj[i].push_back({ j, num });
        adj[j].push_back({ i, num });
    }

    void getCut() {
        cuts.clear();
        for (ll t : usedV) {
            if (vis[t] == 0) {
                visc = 0;
                cut(t, -1);
            }
        }
    }

    void cut(ll p, ll par) {
        vis[p] = ++visc;
        low[p] = vis[p];
        ll goup = 0;

        for (auto [t, num] : adj[p]) {
            if (t == par) {
                if (goup == 0) {
                    goup = 1;
                    continue;
                }
                low[p] = min(low[p], vis[par]);
            }
            else if (vis[t]) {
                low[p] = min(low[p], vis[t]);
            }
            else {
                cut(t, p);
                low[p] = min(low[p], low[t]);
                if (vis[p] < low[t]) {
                    cuts.push_back(num);
                }
            }
        }
    }
};

ll pr[100005];
vector<tuple<ll, ll, ll>> edges[100005];

ll find(ll p) {
	if (pr[p] == p) return p;
	return pr[p] = find(pr[p]);
}

void merge(ll a, ll b) {
	if (find(a) == find(b)) return;
	pr[find(a)] = find(b);
}

int main() {
	cin.tie(0)->sync_with_stdio(0); cout.tie(0);

	ll n, m; cin >> n >> m;

	vector<ll> ans(m, 3);

	for (ll i = 1; i <= n; i++) pr[i] = i;
	for (ll i = 0; i < m; i++) {
		ll x, y, z; cin >> x >> y >> z;
		edges[z].push_back({ x, y, i });
	}
	
	for (ll t = 1; t <= 100000; t++) {
		if (edges[t].empty()) continue;

		vector<ll> usedVertex;
		for (ll i = 0; i < edges[t].size(); i++) {
			ll x = get<0>(edges[t][i]), y = get<1>(edges[t][i]);
			if (find(x) != find(y)) {
				usedVertex.push_back(find(x));
				usedVertex.push_back(find(y));
			}
		}
		if (usedVertex.empty()) continue;

		EdgeCutting cutter(usedVertex);

		for (ll i = 0; i < edges[t].size(); i++) {
			ll x = get<0>(edges[t][i]), y = get<1>(edges[t][i]);
			if (find(x) != find(y)) {
				cutter.add_edge(find(x), find(y), get<2>(edges[t][i]));
				ans[get<2>(edges[t][i])] = 2;
			}
		}

		cutter.getCut();

		for (ll x : cutter.cuts)
            ans[x] = 1; //단절선은 필수

		for (ll i = 0; i < edges[t].size(); i++) {
			ll x = get<0>(edges[t][i]), y = get<1>(edges[t][i]);
			merge(x, y);
		}
	}

	for (ll i = 0; i < m; i++) {
		cout << ans[i] << '\n';
	}
}