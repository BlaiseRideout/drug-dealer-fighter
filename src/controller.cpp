#include "controller.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

Controller::Controller() {
	if(!SDL_WasInit(SDL_INIT_JOYSTICK))
		if(SDL_Init(SDL_INIT_JOYSTICK) < 0)
	        throw std::runtime_error(std::string("Couldn't initialize SDL: ") + SDL_GetError());


    for(int i = 0; i < SDL_NumJoysticks(); ++i)
    	if((this->controller = SDL_JoystickOpen(i)) != NULL)
    		return;

    if(this->controller == NULL)
    	throw std::runtime_error("Couldn't initialize game controller");
}

Controller::Controller(int index) {
	if(!SDL_WasInit(SDL_INIT_JOYSTICK))
		if(SDL_Init(SDL_INIT_JOYSTICK) < 0)
	        throw std::runtime_error(std::string("Couldn't initialize SDL: ") + SDL_GetError());

	this->controller = SDL_JoystickOpen(index);

	if(this->controller == NULL)
    	throw std::runtime_error(std::string("Couldn't initialize game controller ") + std::to_string(index));
}

void Controller::update() {
	SDL_GameControllerUpdate();
}

float Controller::getAxis(int axis) const {
	float val = SDL_JoystickGetAxis(this->controller, axis) / 32767.0f;
	//if(abs(val) < 0.1f)
	//	val = 0.0f;
	return val;
}

glm::vec2 Controller::getStick(int axis1, int axis2) const {
	glm::vec2 val = glm::vec2(getAxis(axis1), getAxis(axis2));
	if(glm::length(val) < 0.1f)
		val *= 0.0f;
	return val;
}

bool Controller::getButton(int button ) const {
	return SDL_JoystickGetButton(this->controller, button) == 1;
}