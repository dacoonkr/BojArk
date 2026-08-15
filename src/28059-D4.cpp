#include <bits/stdc++.h>
using namespace std;
using ll = long long;

//#define cin fin
//ifstream fin("in.txt");

vector<pair<ll, ll>> comb;
vector<vector<ll>> ans;
multiset<ll> S;
vector<ll> v;

ll mask = 0, sum = 0;

void rep(ll i) {
    if (i == v.size()) {
        if (mask > 0)
            comb.push_back({ sum, mask });
    }
    else {
        rep(i + 1);
        mask |= (1ll << i);
        sum += v[i];
        rep(i + 1);
        sum -= v[i];
        mask &= ~(1ll << i);
    }
}

void moving(ll bitA, ll bitB) {
    vector<ll> aList, bList;
    ll common = bitA & bitB;
    bitA &= ~common;
    bitB &= ~common;
    for (ll i = 0; i < v.size(); i++) {
        if ((1ll << i) & bitA) aList.push_back(v[i]);
        if ((1ll << i) & bitB) bList.push_back(v[i]);
    }
    while (!aList.empty() && !bList.empty()) {
        S.erase(aList.back());
        S.erase(bList.back());
        S.insert(abs(aList.back() - bList.back()));
        ans.push_back({ aList.back(), bList.back() });

        if (aList.back() < bList.back()) {
            bList.back() -= aList.back();
            aList.pop_back();
        }
        else {
            aList.back() -= bList.back();
            bList.pop_back();
        }
    }
}

int main() {
    cin.tie(0)->sync_with_stdio(0); cout.tie(0);

    ll n; cin >> n;
    for (ll i = 0; i < n; i++) {
        ll x; cin >> x;
        v.push_back(x);
        S.insert(x);
    }

    if (n == 2) {
        cout << "1\n2 " << v[0] << ' ' << v[1];
        return 0;
    }

    sort(v.begin(), v.end());
    v.resize(min(22ll, n - 1));

    rep(0);
    sort(comb.begin(), comb.end());

    ll minDiff = comb[1].first - comb[0].first;
    ll minIdx = 0;
    for (ll i = 1; i < (ll)comb.size() - 1; i++) {
        ll diff = comb[i + 1].first - comb[i].first;
        if (minDiff > diff) {
            minDiff = diff;
            minIdx = i;
        }
    }
    moving(comb[minIdx].second, comb[minIdx + 1].second);

    ans.push_back({});
    for (ll t : S)
        ans.back().push_back(t);

    cout << ans.size() << '\n';
    for (ll i = 0; i < ans.size(); i++) {
        cout << ans[i].size() << ' ';
        for (ll j = 0; j < ans[i].size(); j++)
            cout << ans[i][j] << ' ';
        cout << '\n';
    }
}