#ifndef GAME_H
#define GAME_H

#include "window.hpp"

#include <glm/glm.hpp>


class Game {
    public:
	Game(std::string title, int majorVersion, int minorVersion);
        Game(std::string title);
        Game();
        virtual ~Game();
        void run();
    protected:
        virtual void update(double time) = 0;

	double time;
        bool running;
        Window window;
};

#endif
