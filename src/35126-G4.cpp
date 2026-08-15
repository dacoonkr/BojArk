#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll psum[2][1005][1005]; //0: uptodown, 1: left2right
ll visit[1005][1005];

bool visiting(ll si, ll sj, ll ei, ll ej) {
	for (ll i = si; i <= ei; i++)
		for (ll j = sj; j <= ej; j++) {
			if (visit[i][j]) return false;
			visit[i][j] = 1;
		}
	return true;
}

void solve() {
	ll n, m, k;
	cin >> n >> m >> k;
	for (ll i = 1; i <= n; i++)
		for (ll j = 1; j <= m; j++)
			psum[0][i][j] = psum[1][i][j] = visit[i][j] = 0;
	vector<string> board;
	for (ll i = 1; i <= n; i++) {
		string s; cin >> s;
		board.push_back(s);
		for (ll j = 1; j <= m; j++) {
			ll v = (s[j - 1] == '#');
			psum[0][i][j] = psum[0][i - 1][j] + v;
			psum[1][i][j] = psum[1][i][j - 1] + v;
		}
	}
	bool success = true;
	for (ll i = 1; i <= n; i++) {
		for (ll j = 1; j <= m; j++) {
			if (board[i - 1][j - 1] == '#' && !visit[i][j]) {
				if (i + k - 1 > n || j + k - 1 > m) success = false;
				else {
					ll upD = (psum[1][i][j + k - 1] - psum[1][i][j - 1] == k);
					ll downD = (psum[1][i + k - 1][j + k - 1] - psum[1][i + k - 1][j - 1] == k);
					ll leftD = (psum[0][i + k - 1][j] - psum[0][i - 1][j] == k);
					ll rightD = (psum[0][i + k - 1][j + k - 1] - psum[0][i - 1][j + k - 1] == k);

					if (upD + downD + leftD + rightD == 3) {
						success &= visiting(i, j, i, j);
						success &= visiting(i, j + k - 1, i, j + k - 1);
						success &= visiting(i + k - 1, j, i + k - 1, j);
						success &= visiting(i + k - 1, j + k - 1, i + k - 1, j + k - 1);
						if (upD) success &= visiting(i, j + 1, i, j + k - 2);
						if (downD) success &= visiting(i + k - 1, j + 1, i + k - 1, j + k - 2);
						if (leftD) success &= visiting(i + 1, j, i + k - 2, j);
						if (rightD) success &= visiting(i + 1, j + k - 1, i + k - 2, j + k - 1);
					}
					else success = false;
				}
				if (!success) break;
			}
		}
		if (!success) break;
	}
	cout << (success ? "yes\n" : "no\n");
}

int main() {
	cin.tie(0)->sync_with_stdio(0); cout.tie(0);

	ll t; cin >> t;
	while (t--) solve();
}