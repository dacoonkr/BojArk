#include <bits/stdc++.h>
using namespace std;
using ll = int;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;
typedef vector<pll> vpll;

//#define cin fin
ifstream fin("input.txt");

typedef tuple<ll, ll, ll> posi;
ll n, m;
ll di[] = { -1, 0, 1, 0 };
ll dj[] = { 0, 1, 0, -1 };
ll cst[1005][1005][4];
bool ended[1005][1005][4];
posi frm[1005][1005][4];
string pins[1005];

bool out(ll i, ll j) {
	if (i <= 0 || j <= 0 || i > n || j > m) return true;
	return false;
}

deque<tuple<ll, ll, ll>> dq; //i, j, dir
posi ans = { -1, -1, -1 };
bool push(ll i, ll j, ll dir, ll ccost, ll plus, posi from) {
	ll ncost = ccost + plus;
	if (ncost >= cst[i][j][dir]) return false;
	cst[i][j][dir] = ncost;
	frm[i][j][dir] = from;
	if (plus) dq.push_back({ i, j, dir });
	else dq.push_front({ i, j, dir });
	return false;
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	cin >> n >> m;
	ll si, sj;
	for (ll i = 1; i <= n; i++) {
		cin >> pins[i];
		pins[i] = ' ' + pins[i];
		for (ll j = 1; j <= m; j++)
			if (pins[i][j] == 'S') si = i, sj = j, pins[i][j] = '.';
	}
	for (ll i = 0; i <= n + 1; i++)
		for (ll j = 0; j <= m + 1; j++)
			for (ll d = 0; d < 4; d++)
				cst[i][j][d] = INT32_MAX;
	//0-1bfs
	for (ll i = 0; i < 4; i++) {
		cst[si][sj][i] = 1;
		frm[si][sj][i] = { -1, -1, -1 };
		dq.push_back({ si, sj, i });
	}
	while (!dq.empty()) {
		posi state = dq.front(); dq.pop_front();
		auto [i, j, dir] = state;
		if (out(i, j)) {
			ans = { i, j, dir };
			break;
		}
		if (ended[i][j][dir]) continue;
		ended[i][j][dir] = true;
		ll ccost = cst[i][j][dir];
		if (pins[i][j] == '.') { //진행
			if (push(i + di[dir], j + dj[dir], dir, ccost, 0, state)) break;
		}
		if (pins[i][j] == '/') { //0:1, 2:3
			ll ndir = dir ^ 1;
			if (push(i + di[ndir], j + dj[ndir], ndir, ccost, 0, state)) break;
			if (push(i + di[dir], j + dj[dir], dir, ccost, 1, state)) break;
		}
		if (pins[i][j] == '\\') { //0:3, 1:2
			ll ndir = 3 - dir;
			if (push(i + di[ndir], j + dj[ndir], ndir, ccost, 0, state)) break;
			if (push(i + di[dir], j + dj[dir], dir, ccost, 1, state)) break;
		}
		if (pins[i][j] == '#') { //0:2, 1:3
			ll ndir = dir ^ 2;
			if (push(i + di[ndir], j + dj[ndir], ndir, ccost, 0, frm[i][j][dir])) break;
		}
	}
	if (get<0>(ans) == -1) {
		cout << "NO"; return 0;
	}
	cout << "YES\n";
	vector<posi> track;
	do {
		track.push_back(ans);
		auto [i, j, dir] = ans;
		ans = frm[i][j][dir];
	} while (get<0>(ans) != -1);

	auto [i, j, dir] = track[0];
	reverse(track.begin(), track.end());
	cout << "URDL"[get<2>(track[0])] << '\n';
	cout << cst[i][j][dir] - 1 << '\n';

	for (ll r = 0; r < track.size() - 1; r++) {
		auto [i, j, dir] = track[r];
		auto [ni, nj, ndir] = track[r + 1];
		if (cst[i][j][dir] < cst[ni][nj][ndir])
			cout << r << ' ' << i << ' ' << j << '\n';
	}
}