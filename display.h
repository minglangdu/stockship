#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>

struct Ship {
    std::pair<int, int> coord;
    int size; 
    bool dir; // true: horizontal; false: vertical

    Ship(int x, int y, int size, bool dir);
    Ship();
    bool check(std::vector<std::vector<int>> prefmiss, std::vector<Ship> ships);
    bool checkmiss(std::vector<std::vector<int>> prefmiss);
    bool checkship(Ship ship);
};

namespace SDLH {
    extern bool DID_INIT;
    extern int WINDOWS;
    class Display {
        /*
        Generic display 
        */
        public:
            SDL_Window* window;
            SDL_Renderer* renderer;

            Display(std::string title, int windowsize);
            ~Display();
            bool update();
            virtual void getInputs(SDL_Event e);
            virtual void renderObjects();
    };
    class BInterface : public Display {
        /*
        Display that serves as an interface to simulate battleship and get user inputs.
        */
        public:
            int x, y; // beginning coordinates of board
            std::vector<std::vector<int>> board; // 0=empty; 1=miss; 2=hit
            std::vector<std::vector<double>> res;
            bool input; 
            bool signal; // if true, stockship will toggle between showing results and getting inputs 
            std::vector<Ship> curcomb; // current combination of ships

            BInterface(int boardsize, bool input=true);
            ~BInterface();
            void getInputs(SDL_Event e);
            void renderObjects();
            std::pair<int, int> getTile(int mx, int my);
    };
}