#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;
typedef vector<pll> vpll;

//#define cin fin
ifstream fin("input.txt");

ll n, res;
ll know[505];

void ask1(ll idx) {
	for (ll i = 1; i <= n; i++) {
		if (idx == i) cout << 1 << ' ';
		else cout << n << ' ';
	}
	cout << endl;
	cin >> res;
	if (res > n) {
		know[idx] = 1 + res;
		return;
	}
	for (ll i = 1; i <= n; i++) {
		if (idx == i) cout << 2 * n << ' ';
		else cout << n << ' ';
	}
	cout << endl;
	cin >> res;
	if (res > n) {
		know[idx] = 2 * n - res;
		return;
	}
}

void ask2(ll idx) {
	for (ll i = 1; i <= n; i++) {
		if (know[i]) cout << know[i] << ' ';
		else if (i == idx) cout << n + 2 << ' ';
		else cout << n + 1 << ' ';
	}
	cout << endl;
	cin >> res;
	if (res == 2) know[idx] = n;
	else know[idx] = n + 1;
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	cin >> n;

	if (n == 1) {
		cout << 1 << endl;
		cin >> res;
		if (res == 1) cout << 2 << endl;
		return 0;
	}

	//2N번
	for (ll i = 1; i <= n; i++)
		ask1(i);

	//N번
	for (ll i = 1; i <= n; i++) {
		if (know[i] == 0) ask2(i);
	}

	for (ll i = 1; i <= n; i++)
		cout << know[i] << ' ';
	cout << endl;
}