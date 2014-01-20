#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>
#include <glm/glm.hpp>

class Controller {
	public:
		Controller();
		Controller(int index);

		static void update();

		float getAxis(int axis) const;
		glm::vec2 getStick(int axis1, int axis2) const;
		bool getButton(int button) const;

		SDL_Joystick *controller;
};

#endif