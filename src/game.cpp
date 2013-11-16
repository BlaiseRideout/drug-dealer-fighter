#include "game.hpp"


Game::Game() : window(800, 600, false, 3, 3) {
  this->running = true;
}

Game::~Game() {
  glfwTerminate();
}

void Game::run() {
  while(this->running && !this->window.shouldClose()) {
    update();
    draw();
    this->window.swapBuffers();
    glfwPollEvents();
  }
}
