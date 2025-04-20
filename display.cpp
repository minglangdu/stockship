#include <SDL2/SDL.h>
#include <iostream>
#include <string>

#include "display.h"
#include "constants.h"

using namespace std;

/*
Miscellaneous
*/
bool SDLH::DID_INIT = false;
int SDLH::WINDOWS = 0;

/*
Display
*/

SDLH::Display::Display(string title, int windowsize) {
    if (!SDLH::DID_INIT) {
        SDL_Init(SDL_INIT_EVERYTHING);
        SDLH::DID_INIT = true;
    }

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        windowsize, windowsize, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDLH::WINDOWS ++;

    // check for errors
    if (window == NULL || renderer == NULL) {
        cout << "Error detected: " << SDL_GetError() << "\n";
    }
}

SDLH::Display::~Display() {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;

    if (SDLH::WINDOWS <= 1) {
        SDL_Quit();
        SDLH::DID_INIT = false;
    }
    SDLH::WINDOWS --;
}

bool SDLH::Display::update() {
    // returns whether quit is true;
    SDL_Event e; bool quit = false;
    while (SDL_PollEvent(&e)) { // seems to be causing the error
        if (e.type == SDL_QUIT) quit = true;
        if (e.window.event == SDL_WINDOWEVENT_CLOSE) quit = true;

        getInputs(e);
    }

    SDL_SetRenderDrawColor(renderer, BG[0], BG[1], BG[2], 0xFF);
    SDL_RenderClear(renderer);

    renderObjects();

    SDL_RenderPresent(renderer);
    return quit;
}

void SDLH::Display::getInputs(SDL_Event e) {} // intentionally left blank

void SDLH::Display::renderObjects() {} // intentionally left blank

/*
BInterface
*/

SDLH::BInterface::BInterface(int boardsize, bool input) 
: SDLH::Display::Display("Stockship", WINDOWSIZE) {
    this->input = input;
    board.resize(boardsize, vector<int> (boardsize, 0));
    res.resize(boardsize, vector<double> (boardsize, 0));  
    x = (WINDOWSIZE - GRIDSIZE * TILESIZE) / 2;
    y = x;
}

SDLH::BInterface::~BInterface() {

}

void SDLH::BInterface::getInputs(SDL_Event e) {
    if (input && e.type == SDL_MOUSEBUTTONUP) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        bool left = (e.button.button == SDL_BUTTON_LEFT);
        pair<int, int> pos = getTile(mx, my);
        if (pos.first != -1) {
            if (left) {
                board[pos.second][pos.first] ++;
                board[pos.second][pos.first] %= 3;
            } else {
                board[pos.second][pos.first] --;
                if (board[pos.second][pos.first] < 0) {
                    board[pos.second][pos.first] += 3;
                }
            }
        }
    }
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_SPACE:
                signal = true;
                break;
        }
    }
}

void SDLH::BInterface::renderObjects() {
    SDL_SetRenderDrawColor(renderer, BORDER[0], BORDER[1], BORDER[2], 0xFF);
    for (int i = 0; i <= GRIDSIZE; i ++) { // you need n + 1 lines to enclose columns or rows
        SDL_RenderDrawLine(renderer, x, y + TILESIZE * i, x + GRIDSIZE * TILESIZE, y + TILESIZE * i);
    }
    for (int i = 0; i <= GRIDSIZE; i ++) {
        SDL_RenderDrawLine(renderer, x + TILESIZE * i, y, x + TILESIZE * i, y + GRIDSIZE * TILESIZE);
    }
    for (int i = 0; i < GRIDSIZE; i ++) {
        for (int j = 0; j < GRIDSIZE; j ++) {
            int x1 = x + TILESIZE * j + 1;
            int y1 = y + TILESIZE * i + 1;
            SDL_Rect rect = {x1, y1, TILESIZE - 1, TILESIZE - 1};
            if (input) {
                switch (board[i][j]) {
                    case 0:
                        SDL_SetRenderDrawColor(renderer, EMPTY[0], EMPTY[1], EMPTY[2], 0xFF);
                        break;
                    case 1:
                        SDL_SetRenderDrawColor(renderer, MISS[0], MISS[1], MISS[2], 0xFF);
                        break;
                    case 2:
                        SDL_SetRenderDrawColor(renderer, HIT[0], HIT[1], HIT[2], 0xFF);
                        break;
                }
            } else {
                if (res[i][j] == 1) {
                    SDL_SetRenderDrawColor(renderer, BEST[0], BEST[1], BEST[2], 0xFF);
                } else {
                    int tem = res[i][j] * 255.0;
                    SDL_SetRenderDrawColor(renderer, tem, tem, tem, 0xFF);
                }
            }
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

pair<int, int> SDLH::BInterface::getTile(int mx, int my) {
    mx -= x; my -= y;
    int rx = floor(mx / TILESIZE), ry = floor(my / TILESIZE);
    if (rx < 0 || rx >= GRIDSIZE || ry < 0 || ry >= GRIDSIZE) {
        return make_pair(-1, -1);
    }
    return make_pair(floor(mx / TILESIZE), floor(my / TILESIZE));
}