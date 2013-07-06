#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

class Vertex {
  public:
    Vertex();
    Vertex(glm::vec3, glm::vec2, glm::vec3);
    bool operator!=(const Vertex *const)const;
    bool operator==(const Vertex *const)const;
    bool operator!=(const Vertex&)const;
    bool operator==(const Vertex&)const;

    glm::vec3 position;
    glm::vec2 texture;
    glm::vec3 normal;
};

#endif