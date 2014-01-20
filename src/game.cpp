#include "game.hpp"


Game::Game() : window(800, 600, false, 3, 3) {
  this->running = true;
}

<<<<<<< Updated upstream
=======
Game::Game(std::string title) : Game(title, 3, 1) {
}

Game::Game() : Game("Divamia") {
}

>>>>>>> Stashed changes
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
