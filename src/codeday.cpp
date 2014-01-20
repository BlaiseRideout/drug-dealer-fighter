#include "codeday.hpp"

#include <iostream>
#include <stdlib.h>
#include <time.h>


Codeday::Codeday() : Game("Codeday"), world(0.8f), player(world, false), player2(world, true) {
	//srand(time(0));
}

Codeday::~Codeday() {
	
}

void Codeday::update(double time) {
	running = running && this->window.getKey(GLFW_KEY_ESCAPE) == GLFW_RELEASE;

	if(this->window.getKey('R')) {
		this->player.health = this->player2.health = 100.0f;
		this->player.alive = this->player2.alive = true;
	}


	this->player.update();
	this->player2.update();
	
	this->world.update();

	draw();
}

void Codeday::draw() {
	this->window.clearScreen();

	this->player.draw();
	this->player2.draw();
}

int main(int argc, char **argv) {
	try {
		Codeday game;
		game.run();
		return 0;
	}
	catch(const std::exception &exc) {
		std::cerr << exc.what() << std::endl;
	}
}
