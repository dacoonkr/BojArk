#include <bits/stdc++.h>
using ll = long long;
using namespace std;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

struct RmvPQ {
	priority_queue<ll> pq, rmvq;
	RmvPQ() { pq.push(0); }
	void push(ll x) { pq.push(x); }
	void rmv(ll x) { rmvq.push(x); }
	ll top() {
		while (!rmvq.empty()) {
			if (pq.top() == rmvq.top()) {
				pq.pop(); rmvq.pop();
			}
			else break;
		}
		return pq.top();
	}
};

struct SEG { //구간 최댓값갱신, 구간최소 지원
	vll minv; vector<RmvPQ> item;
	ll st = 1;
	SEG(ll n) {
		while (st < n) st *= 2;
		item.resize(st * 2);
		minv.resize(st * 2);
	}
	void qadd(ll l, ll r, ll cl, ll cr, ll p, ll h, bool add) {
		if (l <= cl && cr <= r) {
			if (add) item[p].push(h);
			else item[p].rmv(h);
		}
		else if (r < cl || cr < l);
		else {
			ll mid = (cl + cr) / 2;
			qadd(l, r, cl, mid, p * 2, h, add);
			qadd(l, r, mid + 1, cr, p * 2 + 1, h, add);
		}
		if (cl < cr)
			minv[p] = max(min(minv[p * 2], minv[p * 2 + 1]), item[p].top());
		else minv[p] = item[p].top();
	}
	ll qmin(ll l, ll r, ll cl, ll cr, ll p, ll cur_v) {
		ll add = max(cur_v, item[p].top());
		if (l <= cl && cr <= r) return max(add, minv[p]);
		if (r < cl || cr < l) return INT32_MAX;
		ll mid = (cl + cr) / 2;
		ll vl = qmin(l, r, cl, mid, p * 2, add);
		ll vr = qmin(l, r, mid + 1, cr, p * 2 + 1, add);
		return min(vl, vr);
	}
};

vector<ll> coords;
ll conv(ll p) {
	return lower_bound(coords.begin(), coords.end(), p) - coords.begin() + 1;
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	ll n; cin >> n;
	vector<tuple<char, ll, ll, ll>> queries;
	vector<tuple<ll, ll, ll>> signs;
	for (ll i = 0; i < n; i++) {
		char q; cin >> q;
		ll l, r, h;
		if (q == '+') { 
			cin >> l >> r >> h;
			l++;
			signs.push_back({ l, r, h });
			coords.push_back(l - 1);
			coords.push_back(l);
			coords.push_back(r);
			coords.push_back(r + 1);
		}
		if (q == '?') { 
			cin >> l >> r;
			l++;
			coords.push_back(l);
			coords.push_back(r);
		}
		if (q == '-') cin >> h;
		queries.push_back({ q, l, r, h });
	}
	sort(coords.begin(), coords.end());
	coords.erase(unique(coords.begin(), coords.end()), coords.end());
	SEG seg(coords.size());
	for (auto& [q, l, r, h] : queries) {
		if (q == '+') {
			seg.qadd(conv(l), conv(r), 1, seg.st, 1, h, true);
		}
		if (q == '-') {
			auto& [vl, vr, vh] = signs[h - 1];
			seg.qadd(conv(vl), conv(vr), 1, seg.st, 1, vh, false);
		}
		if (q == '?') {
			cout << seg.qmin(conv(l), conv(r), 1, seg.st, 1, 0) << '\n';
		}
	}
}