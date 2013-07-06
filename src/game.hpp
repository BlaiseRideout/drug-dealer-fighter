#ifndef GAME_H
#define GAME_H

#include "graphics.hpp"

class Game {
    public:
        Game();
        void init();
        void run();
        void cleanup();
    protected:
        void update();
        void draw();
    private:
        bool running;
};

#endif