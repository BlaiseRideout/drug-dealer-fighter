#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

class VertexPTN {
  public:
    VertexPTN();
    VertexPTN(glm::vec3, glm::vec2, glm::vec3);
    bool operator!=(const VertexPTN *const)const;
    bool operator==(const VertexPTN *const)const;
    bool operator!=(const VertexPTN&)const;
    bool operator==(const VertexPTN&)const;

    glm::vec3 position;
    glm::vec2 texture;
    glm::vec3 normal;
};

#endif