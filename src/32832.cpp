#include <bits/stdc++.h>
using ll = long long;
using namespace std;

ll board[1005][1005];
ll n, m;
ll tmp[1005][1005];
ll cnt = 0;
ll ans = INT_MAX;

ll inBoard(ll i, ll j) {
    if (0 <= i && i < n) {
        if (0 <= j && j < m) return 1;
    }
    return 0;
}

void run() {
    cnt += tmp[0][0] + tmp[1][0] + tmp[0][1];
    for (ll i = 2; i < n + m - 1; i++) {
        ll x = i, y = 0;
        ll same = 0, flip = 0;
        while(x >= 0) {
            if (inBoard(y, x) && inBoard(y - 1, x - 1)) {
                if (tmp[y][x] != tmp[y-1][x-1]) flip++;
                else same++;
            }
            x--, y++;
        }
        if (same == 0) {
            cnt++;
            x = i, y = 0;
            while(x >= 0) {
                if (inBoard(y, x)) {
                    tmp[y][x] ^= 1;
                }
                x--, y++;
            }
        }
        else if (flip == 0) {

        }
        else return; //불가

        x = i, y = 0;
        while(x >= 0) {
            if (inBoard(y, x) && !inBoard(y - 1, x - 1)) {
                cnt += tmp[y][x];
            }
            x--, y++;
        }
    }
    ans = min(ans, cnt);
}

int main() {
    cin.tie(0)->sync_with_stdio(0); cout.tie(0);

    cin >> n >> m;
    ll oc = 0;
    for (ll i = 0; i < n; i++) {
        for (ll j = 0; j < m; j++) {
            cin >> board[i][j];
            if (board[i][j] == 1) oc++;
        }
    }

    if(n == 1 || m == 1) {
        cout << oc;
        return 0;
    }

    memcpy(tmp, board, sizeof board);
    cnt = 0;
    run();
    
    memcpy(tmp, board, sizeof board);
    cnt = 1;
    tmp[0][0] ^= 1;
    run();
    
    memcpy(tmp, board, sizeof board);
    cnt = 1;
    tmp[0][1] ^= 1;
    tmp[1][0] ^= 1;
    run();
    
    memcpy(tmp, board, sizeof board);
    cnt = 2;
    tmp[0][0] ^= 1;
    tmp[0][1] ^= 1;
    tmp[1][0] ^= 1;
    run();

    if(ans == INT_MAX) {
        cout << "-1";
    } else cout << ans;
}