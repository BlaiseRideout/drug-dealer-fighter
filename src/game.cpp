#include "game.hpp"
#include <GL/glfw.h>
#include <time.h>

Game::Game() {
  this->running = true;
}

void Game::update() {
  running = running && glfwGetKey(GLFW_KEY_ESC) == GLFW_RELEASE;
}

void Game::draw() {
  Graphics::clearScreen();
  glfwSwapBuffers();
}

void Game::init() {
  Graphics::init(800, 600, false);
}

void Game::run() {
  while(this->running && glfwGetWindowParam(GLFW_OPENED)) {
    update();
    draw();
  }
}

void Game::cleanup() {
  glfwTerminate();
}
