#include "game.hpp"
#include <GL/glfw.h>
#include <time.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Game::Game() {
  this->running = true;
}

void Game::update() {
  running = running && glfwGetKey(GLFW_KEY_ESC) == GLFW_RELEASE;
}

void Game::draw() {
  Graphics::clearScreen();
  this->p->draw((Model&)*this->m);
}

void Game::init() {
  Graphics::init(800, 600, false);
  FragmentShader &f = (FragmentShader&)*new FragmentShader("res/screen.frag");
  VertexShader &v = (VertexShader&)*new VertexShader("res/screen.vert");

  this->p = new ShaderProgram(v, f);
  this->p->setUniform("P", glm::perspective(45.0f, (float)Graphics::width / (float)Graphics::height, 0.1f, 100.0f));
  this->p->setUniform("V", glm::lookAt(glm::vec3(-4, 0, -4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
  this->p->setUniform("M", glm::scale(glm::mat4(1.0f), glm::vec3(.01)));
  this->p->setUniform("color", glm::vec3(0.5f, 0.5f, 0.5f));
  this->m = new Model("res/whale.obj");
}

void Game::run() {
  while(this->running && glfwGetWindowParam(GLFW_OPENED)) {
    update();
    draw();
    glfwSwapBuffers();
  }
}

void Game::cleanup() {
  glfwTerminate();
}
