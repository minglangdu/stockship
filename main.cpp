#include <iostream>
#include <SDL2/SDL.h>
#include <random>

#include "constants.h"
#include "display.h"

using namespace std;

random_device rd;
mt19937 mt(rd());
uniform_int_distribution<int> dist(0, 15); // placeholder for actual

pair<vector<vector<double>>, double> simall(vector<vector<int>> inp) {
    int n = inp.size(); int m = inp[0].size();
    vector<vector<double>> ans (inp.size(), vector<double> (inp[0].size(), 0));
    double most = 0.1;

    // placeholder
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < m; j ++) {
            if (inp[i][j] != 0) ans[i][j] = 0;
            else ans[i][j] = dist(mt);
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
    }
    return make_pair(ans, most);
}

int main() {
    SDLH::BInterface* d = new SDLH::BInterface(GRIDSIZE);
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