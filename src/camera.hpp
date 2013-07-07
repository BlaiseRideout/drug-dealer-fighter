#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
  public:
    Camera();
    Camera(glm::vec3, glm::vec3);

    glm::vec3 position;
    glm::vec3 lookAt;
    glm::mat4 camera;
};

#endif