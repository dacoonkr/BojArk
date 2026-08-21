#include <bits/stdc++.h>
using ll = long long;
using namespace std;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

typedef bitset<1501> num;

ll n, k;
num base[1501], makes[1501], solves;

void push(num x, ll idx) {
	num making;
	making.set(idx);
	for (ll i = k - 1; i >= 0; i--) {
		if (x.test(i)) {
			if (base[i].any()) {
				x ^= base[i];
				making ^= makes[i];
			}
			else {
				base[i] = x;
				makes[i] = making;
				break;
			}
		}
		if (x.none()) solves = making;
	}
}

int main() {
	cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

	cin >> n >> k;
	k++; //패리티비트 추가
	for (ll i = 0; i < n; i++) {
		string s; cin >> s;
		s += "1";
		num cur(s);
		push(cur, i);
	}
	if (solves.any()) {
		ll half = solves.count() / 2;
		ll cnt = 0;
		for (ll i = 0; i < n; i++) {
			if (solves[i]) {
				if (cnt++ < half) cout << 1;
				else cout << 2;
			}
			else cout << 0;
		}
	}
	else cout << "*";
}