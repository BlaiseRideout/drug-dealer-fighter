#ifndef GAME_H
#define GAME_H

#include "graphics.hpp"
#include "model.hpp"

class Game {
    public:
        Game();
        void init();
        void run();
        void cleanup();
    protected:
        virtual void update();
        virtual void draw();
    private:
        bool running;
        Model *m;
        ShaderProgram *p;
};

#endif