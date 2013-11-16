#ifndef GAME_H
#define GAME_H

#include "window.hpp"

#include <glm/glm.hpp>


class Game {
    public:
        Game();
        virtual ~Game();
        void run();
    protected:
        virtual void update() = 0;
        virtual void draw() = 0;

        bool running;
        Window window;
};

#endif