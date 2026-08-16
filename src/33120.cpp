#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef pair<ll, ll> pll;

//#define cin fin
//ifstream fin("input.txt");
//#define cout fout
//ofstream fout("output.txt");

ll findMinFactor(ll k) {
	for (ll i = 2; i * i <= k; i++) {
		if (k % i == 0) return i;
	}
	return k;
}

int main() {
	cin.tie(0)->sync_with_stdio(0); cout.tie(0);

	ll n, d, s;
	cin >> n >> d >> s;

	ll maxjump = n / s;
	ll needk = d / s; //needk이하의 수 중 가장 뒤 찾기

	ll last = min(maxjump / 2, needk) * 2;
	
	if (maxjump == 1) {
		cout << s; return 0;
	}

	if (last < maxjump) {
		ll mfactor = findMinFactor(last + 1);
		ll njump = (last + 1) / mfactor * (mfactor - 1);
		if (njump <= needk) last++; //다음도가능
	}

	cout << s * last;
}