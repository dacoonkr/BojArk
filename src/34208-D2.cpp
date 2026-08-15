#include <bits/stdc++.h>
using ll = long long;
using namespace std;

struct point {
	ll x, y;
};

point maxPoint(point& a, point& b) {
	if (a.x == b.x) {
		if (a.y > b.y) return a;
		return b;
	}
	if (a.x > b.x) return a;
	return b;
}
point minPoint(point& a, point& b) {
	if (a.x == b.x) {
		if (a.y < b.y) return a;
		return b;
	}
	if (a.x < b.x) return a;
	return b;
}

struct hulls {
	vector<point> p; //hull의 꼭짓점

	void make(bool max_hull) {
		if (p.size() <= 2) return;

		vector<point> a;
		for (ll i = 0; i < p.size(); i++) {
			a.push_back(p[i]);
			while (a.size() > 1) {
				ll t = a.size() - 1;
				if (a[t].x == a[t - 1].x) {
					point last = (max_hull ? maxPoint(a[t], a[t - 1]) : minPoint(a[t], a[t - 1]));
					a.pop_back(); a.pop_back();
					a.push_back(last);
				}
				else break;
			}
			while (a.size() > 2) {
				ll t = a.size() - 1;
				ll diff = (a[t - 1].y - a[t - 2].y) * (a[t - 0].x - a[t - 1].x)
					- (a[t - 1].x - a[t - 2].x) * (a[t - 0].y - a[t - 1].y);
				//lmaxhull이면 기울기1 <= 기울기2 일 때 뺌
				if ((max_hull && diff <= 0) || (!max_hull && diff >= 0)) {
					point last = a.back();
					a.pop_back(); a.pop_back();
					a.push_back(last);
				}
				else break;
			}
		}
		p = a;
	}
	
	void merge(hulls& h, bool max_hull) {
		ll ip = 0, jp = 0;
		vector<point> a;
		while (ip < p.size() && jp < h.p.size()) {
			if (p[ip].x == h.p[jp].x) {
				if (max_hull && p[ip].y >= h.p[jp].y)
					a.push_back(p[ip]);
				else if (!max_hull && p[ip].y <= h.p[jp].y)
					a.push_back(p[ip]);
				else a.push_back(h.p[jp]);
				ip++, jp++;
			}
			else if (p[ip].x > h.p[jp].x) {
				a.push_back(h.p[jp]);
				jp++;
			}
			else {
				a.push_back(p[ip]);
				ip++;
			}
		}
		while (ip < p.size()) a.push_back(p[ip++]);
		while (jp < h.p.size()) a.push_back(h.p[jp++]);
		p = a;
		make(max_hull);
	}
};

typedef pair<ll, ll> frac;

ll fracCompare(frac a, frac b) {
	return a.first * b.second - b.first * a.second;
}

vector<ll> S(1);
hulls maxHull[2100005], minHull[2100005], hull[2100005]; //i,Si의 헐, (i-j,Si-Sj)의 헐

bool cmp(point& a, point& b) {
	return a.x < b.x;
}

void mergeHull(hulls& out, hulls& left_min, hulls& right_max) {
	vector<frac> ends_r, ends_l;
	for (ll i = 1; i < right_max.p.size(); i++) {
		//마이너스 기울기
		ends_r.push_back({ right_max.p[i - 1].y - right_max.p[i].y, right_max.p[i].x - right_max.p[i - 1].x });
	}
	for (ll i = 1; i < left_min.p.size(); i++) {
		//마이너스 기울기
		ends_l.push_back({ left_min.p[i - 1].y - left_min.p[i].y, left_min.p[i].x - left_min.p[i - 1].x });
	}
	ll i = left_min.p.size() - 1, j = 0;
	frac istart = { -1000000001, 1 }, jstart = { -1000000001, 1 };
	while (i > 0 || j < right_max.p.size() - 1) {
		frac jend = ((j == right_max.p.size() - 1) ? make_pair(1000000001ll, 1ll) : ends_r[j]);
		frac iend = ((i == 0) ? make_pair(1000000001ll, 1ll) : ends_l[i - 1]);
		if (fracCompare(iend, jstart) < 0) { //iend < jstart
			istart = iend;
			i--; continue;
		}
		if (fracCompare(jend, istart) < 0) { //jend < istart
			jstart = jend;
			j++; continue;
		}
		ll realJ = right_max.p[j].x;
		ll realI = left_min.p[i].x;
		out.p.push_back({ realJ - realI , S[realJ] - S[realI] }); //매칭성공
		if (fracCompare(iend, jend) < 0) { //iend < jend
			istart = iend;
			i--; continue;
		}
		else {
			jstart = jend;
			j++; continue;
		}
	}
	ll realJ = right_max.p[j].x;
	ll realI = left_min.p[i].x;
	out.p.push_back({ realJ - realI , S[realJ] - S[realI] }); //매칭성공
	sort(out.p.begin(), out.p.end(), cmp);
}

void dnc(ll l, ll r, ll p) {
	if (l == r) {
		maxHull[p].p = { { l, S[l] } };
		minHull[p].p = { { l, S[l] } };
		return;
	}
	ll mid = (l + r) / 2, left = p * 2, right = p * 2 + 1;
	dnc(l, mid, left); dnc(mid + 1, r, right);

	maxHull[p] = maxHull[left];
	maxHull[p].merge(maxHull[right], true);

	minHull[p] = minHull[left];
	minHull[p].merge(minHull[right], false);

	//i<j j-i,Sj-Si
	hull[p] = hull[left];
	hull[p].merge(hull[right], true);

	hulls out; //결합됨
	mergeHull(out, minHull[left], maxHull[right]);
	hull[p].merge(out, true);
}

int main() {
	cin.tie(0)->sync_with_stdio(0); cout.tie(0);


	ll n, q; cin >> n >> q;
	for (ll i = 0; i < n; i++) {
		ll x; cin >> x;
		S.push_back(S.back() + x);
	}

	dnc(0, n, 1); //i<j

	vector<double> cuts = { -1e9 - 1 };
	for (ll i = 1; i < hull[1].p.size(); i++) {
		vector<point>& c = hull[1].p;
		//마이너스 기울기
		cuts.push_back((double)(c[i - 1].y - c[i].y) / (c[i].x - c[i - 1].x));
	}
	cuts.push_back(1e9 + 1);

	for (ll i = 0; i < q; i++) {
		ll p; cin >> p;
		ll idx = lower_bound(cuts.begin(), cuts.end(), (double)p) - cuts.begin() - 1;
		cout << p * hull[1].p[idx].x + hull[1].p[idx].y << '\n';
	}
}