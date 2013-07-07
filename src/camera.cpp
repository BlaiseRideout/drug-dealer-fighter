#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"

Camera::Camera() {
  this->position = glm::vec3(-1, 0, 0);
  this->lookAt = glm::vec3(0);
  this->camera = glm::lookAt(
    glm::vec3(-1, 0, 0), // camera position
    glm::vec3(0, 0, 0), // look at
    glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
  );
}

Camera::Camera(glm::vec3 position, glm::vec3 lookAt) : position(position), lookAt(lookAt), camera(glm::lookAt(position, lookAt, glm::vec3(0, 1, 0))) {
}