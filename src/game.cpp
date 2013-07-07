#include "game.hpp"
#include <GL/glfw.h>
#include <time.h>
#include <glm/gtc/matrix_transform.hpp>

Game::Game() {
  this->running = true;
}

void Game::update() {
  running = running && glfwGetKey(GLFW_KEY_ESC) == GLFW_RELEASE;
}

void Game::draw() {
  Graphics::clearScreen();
  this->m->draw();
}

void Game::init() {
  Graphics::init(800, 600, false);
  FragmentShader &f = (FragmentShader&)*new FragmentShader("res/test.frag");
  VertexShader &v = (VertexShader&)*new VertexShader("res/test.vert");

  this->p = new ShaderProgram(v, f);
  //this->p->setUniform("in_Color", 0.5f);
  //this->p->setUniform("in_Alpha", 1.0f);
  this->p->setUniform("MVP",  glm::mat4(1.0f) *
                              glm::lookAt(
                                glm::vec3(-1, 1, 0), // camera position
                                glm::vec3(0,0,0), // look at
                                glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                              ) * Graphics::projection);
  this->p->use();
  this->m = new Model("res/test.obj");
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
