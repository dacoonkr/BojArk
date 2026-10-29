#include <bits/stdc++.h>
using namespace std;
using ll = long long;

string concat(vector<char> x) {
    string s = "";
    for (char c : x) s += c;
    return s;
}

vector<pair<pair<ll, ll>, string>> nnext(string s) {
    vector<pair<pair<ll, ll>, string>> ans;
    ans.push_back({ {1, 2}, concat({s[3], s[0], s[1], s[2], s[4]}) });
    ans.push_back({ {1, 3}, concat({s[3], s[4], s[0], s[1], s[2]}) });
    ans.push_back({ {2, 3}, concat({s[0], s[4], s[1], s[2], s[3]}) });
    ans.push_back({ {2, 1}, concat({s[1], s[2], s[3], s[0], s[4]}) });
    ans.push_back({ {3, 2}, concat({s[0], s[2], s[3], s[4], s[1]}) });
    ans.push_back({ {3, 1}, concat({s[2], s[3], s[4], s[0], s[1]}) });
    return ans;
}

vector<pair<ll, ll>> inst;

void doinst(vector<ll>& txt, ll i, ll j) {
    inst.push_back({ i, j });
    vector<ll> v = { txt[i], txt[i + 1], txt[i + 2] };
    txt.erase(txt.begin() + i);
    txt.erase(txt.begin() + i);
    txt.erase(txt.begin() + i);
    txt.insert(txt.begin() + j, v[0]);
    txt.insert(txt.begin() + j + 1, v[1]);
    txt.insert(txt.begin() + j + 2, v[2]);
}

int main() {
    cout.tie((ostream*)!cin.tie(0)->sync_with_stdio(0));

    map<string, pair<ll, ll>> vis;
    queue<string> que;
    que.push("12345");
    vis["12345"] = { -1, -1 };

    ll cnt = 1;
    while (!que.empty()) {
        string top = que.front(); que.pop();
        for (auto [method, nxt] : nnext(top)) {
            if (vis.find(nxt) == vis.end()) {
                vis[nxt] = method;
                cnt++;
                que.push(nxt);
            }
        }
    }

    ll n; cin >> n;
    vector<ll> txt(n + 1);
    for (ll i = 1; i <= n; i++)
        cin >> txt[i];
    for (ll i = 1; i <= n - 5; i++) {
        ll pos = i;
        for (; pos <= n; pos++) if (txt[pos] == i) break;
        if (n - 1 <= pos) {
            doinst(txt, n - 2, n - 4);
            pos -= 2;
        }
        doinst(txt, pos, i);
    }
    while (1) {
        string x = "";
        for (ll i = n - 4; i <= n; i++) {
            x += (char)(txt[i] - (n - 4) + '1');
        }
        // cout << x << '\n';

        pair<ll, ll> inst = vis[x];
        if (inst.first == -1) break;
        doinst(txt, n - 5 + inst.second, n - 5 + inst.first);
    }

    // for (ll i = 1; i <= n; i++)
    //     cout << txt[i];

    cout << inst.size() << '\n';
    for (auto [l, r] : inst) cout << l << ' ' << r << '\n';
}