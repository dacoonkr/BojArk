#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;
typedef vector<pll> vpll;

//#define cin fin
ifstream fin("input.txt");

ll mai[4], n, k;
double dp[1505][1505][2]; //[0][2][3][step%2]=도달확률

double leftsel(ll lo, ll ls, ll lt, ll selc) {
	lo = mai[0] - lo;
	ls = mai[2] - ls;
	lt = mai[3] - lt;
	double sum = lo + ls + lt;
	if (selc == 0) return lo / sum;
	if (selc == 2) return ls / sum;
	if (selc == 3) return lt / sum;
	return -1;
}

double selI(ll lo, ll ls, ll lt, ll i, ll selc) {
	ll nn = i % 2;
	double befp = dp[lo][ls][!nn];
	double selp = leftsel(lo, ls, lt, selc);
	if (selc == 0 && ls * 2 + lt * 3 == i - 1 && i < n) return 0; //impossible
	return befp * selp;
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));
	cout << fixed << setprecision(10);

	cin >> n >> k;
	for (ll i = 0; i <= k; i++)
		cin >> mai[i];

	if (n == 1) {
		cout << 1; return 0;
	}
	double lastnotone = 1;
	if (1 * mai[1] + 2 * mai[2] + 3 * mai[3] == n - 1)
		lastnotone = (double)(n - mai[1]) / n;
		
	n -= mai[1];
	if (n == 0) {
		cout << 1; return 0;
	}
	
	dp[0][0][0] = 1;
	for (ll i = 1; i <= n; i++) {
		//o, s, t : 0, 2, 3 매수
		for (ll o = 0; o <= mai[0] && o <= i; o++) {
			for (ll s = 0; s <= mai[2] && o + s <= i; s++) {
				ll t = i - o - s; if (t > mai[3]) continue;
				
				ll nn = i % 2;
				dp[o][s][nn] = 0;
				if (o > 0) dp[o][s][nn] += selI(o - 1, s, t, i, 0);
				if (s > 0) dp[o][s][nn] += selI(o, s - 1, t, i, 2);
				if (t > 0) dp[o][s][nn] += selI(o, s, t - 1, i, 3);
			}
		}
	}

	cout << dp[mai[0]][mai[2]][n % 2] * lastnotone;
}