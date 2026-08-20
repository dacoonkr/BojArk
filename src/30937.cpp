#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

struct SEG { //lazy (더하기와 최솟값지원)
	vector<ll> lazy;
	vector<ll> seg; //최소
	ll st = 1;

	SEG(ll n) {
		while (st < n) st *= 2;
		lazy.resize(st * 2);
		seg.resize(st * 2);
	}

	void push(ll p) {
		seg[p] += lazy[p];
		if (st > p) {
			lazy[p * 2] += lazy[p];
			lazy[p * 2 + 1] += lazy[p];
		}
		lazy[p] = 0;
	}

	void update(ll l, ll r, ll cl, ll cr, ll p, ll v) {
		push(p);
		if (l <= cl && cr <= r) {
			lazy[p] += v;
			push(p);
		}
		else if (r < cl || cr < l) return;
		else {
			ll mid = (cl + cr) / 2;
			update(l, r, cl, mid, p * 2, v);
			update(l, r, mid + 1, cr, p * 2 + 1, v);
			seg[p] = min(seg[p * 2], seg[p * 2 + 1]) + lazy[p];
		}
	}

	//min쿼리
	ll query(ll l, ll r, ll cl, ll cr, ll p) {
		push(p);
		if (l <= cl && cr <= r)
			return seg[p];
		if (r < cl || cr < l) return INT32_MAX;

		push(p);
		ll mid = (cl + cr) / 2;
		ll lf = query(l, r, cl, mid, p * 2);
		ll rt = query(l, r, mid + 1, cr, p * 2 + 1);
		
		return min(lf, rt);
	}
};

vector<ll> adj[500005];
ll fatig[500005], subtree[500005], par[500005], segidx[500005], up[500005];
ll segUsed = 0;
ll diy[500005];

void hld1(ll p) {
	subtree[p] = 1;
	for (ll t : adj[p]) {
		hld1(t);
		subtree[p] += subtree[t];
	}
}

void hld2(ll p, ll cpar) {
	par[p] = cpar;
	segidx[p] = ++segUsed;
	ll maxt = -1;
	for (ll t : adj[p]) {
		if (maxt == -1)
			maxt = t;
		else if (subtree[maxt] < subtree[t])
			maxt = t;
	}
	if (maxt != -1)
		hld2(maxt, cpar);
	for (ll t : adj[p]) {
		if (maxt == t) continue;
		else hld2(t, t);
	}
}

vector<pll> paths(ll p) {
	vector<pll> ans;
	while (p > 0) {
		ans.push_back({ segidx[par[p]], segidx[p] });
		p = up[par[p]];
	}
	return ans;
}

bool valid(SEG& seg, ll p) {
	for (auto [l, r] : paths(p)) {
		if (seg.query(l, r, 1, seg.st, 1) == 0)
			return false;
	}
	return true;
}

void alloc(SEG& seg, ll p) {
	for (auto [l, r] : paths(p))
		seg.update(l, r, 1, seg.st, 1, -1);
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	ll n; cin >> n;
	for (ll i = 2; i <= n; i++) {
		ll p; cin >> p;
		adj[p].push_back(i);
		up[i] = p;
	}

	priority_queue<pair<ll, ll>> pq; // {-cost, idx}
	for (ll i = 1; i <= n; i++) {
		cin >> fatig[i];
		pq.push({ -fatig[i], i });
	}

	hld1(1);
	hld2(1, 1);

	SEG seg(n);
	for (ll i = 1; i <= n; i++)
		seg.update(segidx[i], segidx[i], 1, seg.st, 1, subtree[i]);
	for (ll i = n + 1; i <= seg.st; i++)
		seg.update(i, i, 1, seg.st, 1, INT32_MAX);

	ll ans = 0;
	while (!pq.empty()) {
		auto [cost, idx] = pq.top(); pq.pop();
		if (valid(seg, idx)) {
			ans -= cost;
			alloc(seg, idx);
			diy[idx]++;

			ll cost = (diy[idx] + 1) * (diy[idx] + 1) - diy[idx] * diy[idx];
			pq.push({ -fatig[idx] * cost, idx });
		
			//cout << "SUCC: " << idx << '\n';
		}
		//else cout << "FAIL: " << idx << '\n';
	}

	cout << ans;
}