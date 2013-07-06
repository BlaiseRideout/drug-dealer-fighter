#include <GL/glew.h>
#include <GL/glfw.h>
#include <stdlib.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics.hpp"

int Graphics::width = 800;
int Graphics::height = 600;
glm::mat4 Graphics::projection = glm::mat4(0);
glm::mat4 Graphics::orthographic = glm::mat4(0);

extern const GLubyte *gluErrorString(GLenum error);

void Graphics::init(int width, int height, bool fullscreen) {
  Graphics::width = width;
  Graphics::height = height;
  Graphics::projection = glm::perspective(45.0f, (float)Graphics::width / (float)Graphics::height, 0.1f, 100.0f);
  Graphics::orthographic = glm::ortho(0, Graphics::width, Graphics::height, 0, -1, 1);

  Graphics::initGlfw(fullscreen);
  Graphics::initGL();
}

void Graphics::init(bool fullscreen) {
  Graphics::init(800, 600, fullscreen);
}

void Graphics::clearScreen() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::initGL() {
  GLenum err = glewInit();
  if (GLEW_OK != err)
    throw std::runtime_error(std::string((const char *)glewGetErrorString(err)));

  glClearColor(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f, 0.0f);
  glViewport(0, 0, Graphics::width, Graphics::height);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_MULTISAMPLE);
  glDisable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);
}

void Graphics::initGlfw(bool fullscreen) {
  //initialize glfw
  if (!glfwInit())
    throw std::runtime_error("Failed to initialize GLFW.");

  //use opengl 3.3
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);
  //user can't resize window
  glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
  glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); // 4x antialiasing

  //open glfw window
  if (!glfwOpenWindow(Graphics::width, Graphics::height, 0, 0, 0, 0, 0, 0, fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW))
    throw std::runtime_error("Failed to open GLFW window.");

  //center window onscreen
  GLFWvidmode vm;
  glfwGetDesktopMode(&vm);
  glfwSetWindowPos(vm.Width / 2 - width / 2, vm.Height / 2 - height / 2);

  //set title
  glfwSetWindowTitle("Divamia");
  //vsync: 0 = no, 1 = yes
  glfwSwapInterval(1);

  std::cout << glGetString(GL_VERSION) << std::endl;
}
