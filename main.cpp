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
                prefmiss[i][j + 1] --;
                prefmiss[i + 1][j] --;
                prefmiss[i + 1][j + 1] ++;
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
        cout << "adding ship #" << ship << ", ship size " << ssize << "\n";
        for (int i = q.size(); i > 0; i --) {
            vector<Ship> cur = q.front(); q.pop();
            if (d->quit) return {{}, 0};
            d->curcomb = cur;
            d->update();
            cout << "comb " << i << " " << cur.size() << "\n";
            for (int dir = 0; dir < 2; dir ++) { // direction
                cout << "dir " << dir << "\n";
                for (int y = 0; y < GRIDSIZE; y ++) {
                    for (int x = 0; x < GRIDSIZE; x ++) {
                        Ship curs (x, y, ssize, (bool)dir);
                        if (curs.check(prefmiss, cur)) {
                            cout << "coord " << x << " " << y << "\n";
                            cur.push_back(curs);
                            q.push(cur);
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
    // continue working here

    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < m; j ++) {
            if (inp[i][j] != 0) ans[i][j] = 0;
            most = max(most, ans[i][j]);
        }
    }

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