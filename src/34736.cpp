#include <bits/stdc++.h>
using ll = long long;
using namespace std;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

vll adj[100005], sub_st[100005];
ll in[100005], out[100005]; //ett
ll bef_cl[100005], bef_scl[100005], aft_cl[100005], aft_scl[100005]; //전후의 closest, second closest
vector<pll> p_ls[100005];

ll cnt = 0;
void ett(ll p) {
	in[p] = ++cnt;
	for (ll t : adj[p]) {
		if (in[t] == 0) ett(t);
	}
	out[p] = cnt;
	for (ll t : adj[p]) {
		if (in[t] > in[p]) sub_st[p].push_back(in[t]);
	}
	sub_st[p].push_back(out[p] + 1);
}

struct MmSEG {
	vll segM, segm, segpos;
	ll st = 1, sz = 1;
	MmSEG(ll n) {
		sz = n;
		while (st < n) st <<= 1;
		segM.assign(st * 2, -1);
		segm.assign(st * 2, INT32_MAX);
		segpos.assign(st * 2, -1);
	}
	ll mergepos(ll max1, ll pos1, ll pos2) {
		if (pos2 >= max1) return pos2;
		return pos1;
	}
	void update(ll i, ll v) { //1-base
		ll p = st + i - 1;
		segM[p] = segm[p] = v;
		segpos[p] = (v <= i ? i : -1);
		while (p > 1) {
			p >>= 1;
			segM[p] = max(segM[p * 2], segM[p * 2 + 1]);
			segm[p] = min(segm[p * 2], segm[p * 2 + 1]);
			segpos[p] = mergepos(segM[p * 2], segpos[p * 2], segpos[p * 2 + 1]);
		}
	}
	ll Mquery(ll l, ll r, ll cl, ll cr, ll p) {
		if (r < cl || cr < l) return -1;
		if (l <= cl && cr <= r) return segM[p];
		ll mid = (cl + cr) >> 1;
		return max(Mquery(l, r, cl, mid, p * 2), Mquery(l, r, mid + 1, cr, p * 2 + 1));
	}
	ll mquery(ll l, ll r, ll cl, ll cr, ll p) {
		if (r < cl || cr < l) return INT32_MAX;
		if (l <= cl && cr <= r) return segm[p];
		ll mid = (cl + cr) >> 1;
		return min(mquery(l, r, cl, mid, p * 2), mquery(l, r, mid + 1, cr, p * 2 + 1));
	}
	vll pss;
	void query(ll l, ll r, ll cl, ll cr, ll p){
		if (r < cl || cr < l) return;
		if (l <= cl && cr <= r) {
			pss.push_back(p); return;
		}
		ll mid = (cl + cr) >> 1;
		query(l, r, cl, mid, p * 2); query(l, r, mid + 1, cr, p * 2 + 1);
	}
};

ll getmax(MmSEG& seg, vector<pll> q) {
	ll ans = -1;
	for (pll& qi : q)
		if (1 <= qi.first && qi.first <= qi.second && qi.second <= seg.sz)
			ans = max(ans, seg.Mquery(qi.first, qi.second, 1, seg.st, 1));
	return ans;
}
ll getmin(MmSEG& seg, vector<pll> q) {
	ll ans = INT32_MAX;
	for (pll& qi : q)
		if (1 <= qi.first && qi.first <= qi.second && qi.second <= seg.sz)
			ans = min(ans, seg.mquery(qi.first, qi.second, 1, seg.st, 1));
	return ans;
}

ll findRel(ll p, ll nxt) { //속한 서브트리 찾기
	if (in[nxt] <= in[p] || out[p] < out[nxt]) return -1;
	ll idx = upper_bound(sub_st[p].begin(), sub_st[p].end(), in[nxt]) - sub_st[p].begin() - 1;
	return idx;
}

vector<pll> findExcept(ll p, ll nxt, ll sz) {
	//p기준, nxt가 있는 서브트리 제외
	ll rel = findRel(p, nxt);
	if (rel == -1) return { { in[p], out[p] } };
	pll subt = { sub_st[p][rel], sub_st[p][rel + 1] - 1 };
	return { { 1, subt.first - 1 }, { subt.second + 1, sz } };
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	ll n; cin >> n;
	for (ll i = 0; i < n - 1; i++) {
		ll u, v; cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	ett(1);
	ll p; cin >> p;
	vector<pair<bool, ll>> pros(1); //true=비즈니스, 1base
	for (ll i = 0; i < p; i++) {
		char c; ll u; cin >> c >> u;
		pros.push_back({ c == 'b', u });
	}
	//정방향 스위핑
	ll bef = -1;
	MmSEG seg1(n);
	for (ll i = 1; i <= p; i++) {
		auto& [type, pos] = pros[i];
		if (type) { //비즈니스
			bef_cl[i] = bef;
			if (bef > 0 && pros[bef].second != pos)
				bef_scl[i] = getmax(seg1, findExcept(pos, pros[bef].second, n));
			else bef_scl[i] = -1;
		}
		else {
			seg1.update(in[pos], i);
			bef = i;
		}
	}
	//역방향 스위핑
	ll aft = INT32_MAX;
	MmSEG seg2(n);
	for (ll i = p; i >= 1; i--) {
		auto& [type, pos] = pros[i];
		if (type) { //비즈니스
			aft_cl[i] = aft;
			if (aft <= p && pros[aft].second != pos)
				aft_scl[i] = getmin(seg2, findExcept(pos, pros[aft].second, n));
			else aft_scl[i] = INT32_MAX;
		}
		else {
			seg2.update(in[pos], i);
			aft = i;
		}
	}
	//요규 규격 판별 [s,t]를 포함하면 반드시 가능
	for (ll i = 1; i <= p; i++) {
		auto& [type, pos] = pros[i];
		vector<pll> req;
		if (type) {
			ll bef_pos = bef_cl[i] != -1 ? pros[bef_cl[i]].second : 0;
			ll aft_pos = aft_cl[i] != INT32_MAX ? pros[aft_cl[i]].second : 0;
			if (bef_cl[i] != -1 && bef_pos == pos)
				req.push_back({ bef_cl[i], i });
			else if (bef_scl[i] != -1)
				req.push_back({ bef_scl[i], i });
			if (aft_cl[i] != INT32_MAX && aft_pos == pos)
				req.push_back({ i, aft_cl[i] });
			else if (aft_scl[i] != INT32_MAX)
				req.push_back({ i, aft_scl[i] });
			if (bef_cl[i] != -1 && aft_cl[i] != INT32_MAX && pos != bef_pos && pos != aft_pos
				&& findRel(pos, bef_pos) != findRel(pos, aft_pos))
				req.push_back({ bef_cl[i], aft_cl[i] });
		}
		for (pll qi : req)
			p_ls[qi.first].push_back({ qi.second, i });
	}
	//정답 스위핑
	MmSEG seg(p);
	vll ans(p + 1);
	for (ll i = 1; i <= p; i++) {
		if (pros[i].first)
			seg.update(i, INT32_MAX);
		else seg.update(i, i);
	}
	for (ll i = p; i >= 1; i--) {
		for (pll& qi : p_ls[i])
			seg.update(qi.second, qi.first);
		seg.pss.clear();
		seg.query(i, p, 1, seg.st, 1);
		ll pos = -1, maxv = -1;
		for (ll p : seg.pss) {
			pos = seg.mergepos(maxv, pos, seg.segpos[p]);
			maxv = max(maxv, seg.segM[p]);
		}
		ans[i] = (pos == -1) ? 0 : pos - i + 1;
	}
	for (ll i = 1; i <= p; i++) cout << ans[i] << '\n';
}