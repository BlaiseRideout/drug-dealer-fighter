#include <glm/glm.hpp>

#include "vertex.hpp"

VertexPTN::VertexPTN(glm::vec3 position, glm::vec2 texture, glm::vec3 normal) :
position(position), texture(texture), normal(normal) { }

VertexPTN::VertexPTN() {
  VertexPTN(glm::vec3(0), glm::vec2(0), glm::vec3(0));
}

bool VertexPTN::operator==(const VertexPTN *const v)const{
  return this->position == v->position && this->texture == v->texture && this->normal == v->normal;
}

bool VertexPTN::operator!=(const VertexPTN *const v)const{
  return !(v == this);
}

bool VertexPTN::operator==(const VertexPTN &v)const{
  return this->position == v.position && this->texture == v.texture && this->normal == v.normal;
}

bool VertexPTN::operator!=(const VertexPTN &v)const{
  return !(v == this);
}
