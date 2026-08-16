#include <bits/stdc++.h>
using ll = long long;
using namespace std;

using cd = complex<long double>;
const long double PI = acosl(-1.0L);

void fft(vector<cd>& a, bool inv) {
    int n = a.size();

    static vector<int> rev;
    static vector<cd> roots{ 0, 1 };

    if ((int)rev.size() != n) {
        int lg = 0;
        while ((1 << lg) < n) lg++;

        rev.assign(n, 0);
        for (int i = 0; i < n; i++)
            rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (lg - 1));
    }

    if ((int)roots.size() < n) {
        int k = 0;
        while ((1 << k) < (int)roots.size()) k++;

        roots.resize(n);
        while ((1 << k) < n) {
            long double ang = 2 * PI / (1 << (k + 1));
            for (int i = 1 << (k - 1); i < (1 << k); i++) {
                roots[2 * i] = roots[i];
                long double a = ang * (2 * i + 1 - (1 << k));
                roots[2 * i + 1] = cd(cosl(a), sinl(a));
            }
            k++;
        }
    }

    for (int i = 0; i < n; i++)
        if (i < rev[i])
            swap(a[i], a[rev[i]]);

    for (int len = 1; len < n; len <<= 1) {
        for (int i = 0; i < n; i += 2 * len) {
            for (int j = 0; j < len; j++) {
                cd u = a[i + j];
                cd v = a[i + j + len] * roots[len + j];
                a[i + j] = u + v;
                a[i + j + len] = u - v;
            }
        }
    }

    if (inv) {
        reverse(a.begin() + 1, a.end());
        for (auto& x : a) x /= n;
    }
}

vector<ll> multiply(const vector<ll>& A, const vector<ll>& B) {
    if (A.empty() || B.empty()) return {};

    int need = A.size() + B.size() - 1;
    int n = 1;
    while (n < need) n <<= 1;

    vector<cd> fa(n), fb(n);
    for (int i = 0; i < (int)A.size(); i++) fa[i] = A[i];
    for (int i = 0; i < (int)B.size(); i++) fb[i] = B[i];

    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < n; i++) fa[i] *= fb[i];
    fft(fa, true);

    vector<ll> res(need);
    for (int i = 0; i < need; i++)
        res[i] = llround(fa[i].real());

    return res;
}

int main() {
    cin.tie(0)->sync_with_stdio(0); cout.tie(0);

    vector<ll> R, P, S;
    vector<ll> mR, mP, mS;
    ll n, m; cin >> n >> m;
    string a, b; cin >> a >> b;

    for (ll i = 0; i < n; i++) {
        R.push_back(a[i] == 'R');
        P.push_back(a[i] == 'P');
        S.push_back(a[i] == 'S');
    }

    for (ll i = m - 1; i >= 0; i--) {
        mR.push_back(b[i] == 'P');
        mP.push_back(b[i] == 'S');
        mS.push_back(b[i] == 'R');
    }

    vector<ll> m1 = multiply(R, mR);
    vector<ll> m2 = multiply(P, mP);
    vector<ll> m3 = multiply(S, mS);

    ll ans = 0;
    for (ll i = 0; i < n; i++) {
        ans = max(ans, m1[i + m - 1] + m2[i + m - 1] + m3[i + m - 1]);
    }

    cout << ans;
} 