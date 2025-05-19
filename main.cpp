#include <iostream>
#include <SDL2/SDL.h>
#include <random>
#include <queue>

#include "constants.h"
#include "display.h"

using namespace std;

SDLH::BInterface* d;

pair<vector<vector<int>>, vector<pair<int, int>>> preprocess(vector<vector<int>> inp) {
    int n = inp.size(); int m = inp[0].size();
    vector<vector<int>> prefmiss (n + 1, vector<int> (m + 1, 0));
    vector<pair<int, int>> hits;
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < m; j ++) {
            if (inp[i][j] == 0) continue;
            else if (inp[i][j] == 1) {
                prefmiss[i][j] ++;
            } else if (inp[i][j] == 2) {
                hits.push_back(make_pair(j, i));
            }
        }
    }
    for (int i = 0; i <= n; i ++) {
        for (int j = 0; j <= m; j ++) {
            prefmiss[i][j] += ((i > 0) ? prefmiss[i - 1][j] : 0) + ((j > 0) ? prefmiss[i][j - 1] : 0)
            - ((i > 0 && j > 0) ? prefmiss[i - 1][j - 1] : 0);
        }
    }
    return make_pair(prefmiss, hits);
}

pair<vector<vector<double>>, double> simall(vector<vector<int>> inp) {
    int n = inp.size(); int m = inp[0].size();
    vector<vector<double>> ans (inp.size(), vector<double> (inp[0].size(), 0));
    double most = 0.1;
    auto prep = preprocess(inp);
    vector<vector<int>> prefmiss = prep.first;
    vector<pair<int, int>> hits = prep.second;
    for (vector<int> col : prefmiss) {
        for (int r : col) {
            cout << r << " ";
        }
        cout << "\n";
    }
    queue<vector<Ship>> q;
    q.push(vector<Ship> (0));
    int ship = 1;
    for (int ssize : ships) {
        // cout << "adding ship #" << ship << ", ship size " << ssize << "\n";
        for (int i = q.size(); i > 0; i --) {
            vector<Ship> cur = q.front(); q.pop();
            if (d->quit) return {{}, 0};
            // cout << "comb " << i << " " << cur.size() << "\n";
            for (int dir = 0; dir < 2; dir ++) { // direction
                // cout << "dir " << dir << "\n";
                for (int y = 0; y < GRIDSIZE; y ++) {
                    for (int x = 0; x < GRIDSIZE; x ++) {
                        Ship curs (x, y, ssize, (bool)dir);
                        if (curs.check(prefmiss, cur)) {
                            // cout << "coord " << x << " " << y << "\n";
                            cur.push_back(curs);
                            q.push(cur);
                            d->curcomb = cur;
                            d->update();
                            cur.pop_back();
                        }
                    }
                }
            }
        }
        cout << q.size() << " combinations" << "\n";
    }
    d->curcomb = {};
    d->update();
    while (!q.empty()) {
        vector<Ship> cur = q.front(); q.pop();
        for (Ship s : cur) {
            hits = s.getremhits(hits);
        }
        // check for hits - placeholder
        if (hits.size() == 0) { // if all hits have ships
            d->curcomb = cur;
            for (Ship s : cur) {
                // difference array
                int y = s.coord.second, x = s.coord.first;
                ans[y][x] ++;
                if (s.dir) { // horizontal
                    if (y < n - 1) ans[y + 1][x] --;
                    if (x + s.size - 1 < m - 1) ans[y][x + s.size - 1] --;
                    if ((y < n - 1) && (x + s.size - 1 < m - 1))
                    ans[y + 1][x + s.size - 1] ++;
                } else { // vertical
                    if (x < m - 1) ans[y][x + 1] --;
                    if (y + s.size - 1 < n - 1) ans[y + s.size - 1][x] --;
                    if ((x < m - 1) && (y + s.size - 1 < n - 1))
                    ans[y + s.size - 1][x + 1] ++;
                }
            }
        }
        d->update();
    }
    // turn ans into diff array
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < m; j ++) {
            ans[i][j] += ((i > 0) ? ans[i - 1][j] : 0) + ((j > 0) ? ans[i][j - 1] : 0)
            - ((i > 0 && j > 0) ? ans[i - 1][j - 1] : 0);
        }
    }

    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < m; j ++) {
            if (inp[i][j] != 0) ans[i][j] = 0;
            most = max(most, ans[i][j]);
        }
    }
    d->curcomb = {};
    return make_pair(ans, most);
}

pair<vector<vector<double>>, double> simlite(vector<vector<int>> inp) {
    int n = inp.size(); int m = inp[0].size();
    vector<vector<double>> ans (inp.size(), vector<double> (inp[0].size(), 0));
    double most = 0.1;

    int simsleft = SIMS;
    while (simsleft --) {
        //
        if (d->quit) return {{}, 0};

        d->update();
    }
    return make_pair(ans, most);
}

int main() {
    d = new SDLH::BInterface(GRIDSIZE);
    bool quit = false;
    while (!quit) {
        quit = d->update();
        if (d->signal) {
            d->signal = false;
            d->input = !d->input;
            if (!d->input) { // if getting results
                pair<vector<vector<double>>, double> r;
                if (LITE) {
                    r = simlite(d->board);
                } else {
                    r = simall(d->board);
                }
                for (int i = 0; i < r.first.size(); i ++) {
                    for (int j = 0; j < r.first[0].size(); j ++) {
                        r.first[i][j] /= r.second;
                    }
                }
                d->res = r.first;
            }
        }
    }
    delete d;
    d = nullptr;
}