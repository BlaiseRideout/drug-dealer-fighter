#include <glm/glm.hpp>

#include "vertex.hpp"

Vertex::Vertex(glm::vec3 position, glm::vec2 texture, glm::vec3 normal) :
position(position), texture(texture), normal(normal) { }

Vertex::Vertex() {
  Vertex(glm::vec3(0), glm::vec2(0), glm::vec3(0));
}

bool Vertex::operator==(const Vertex *const v)const{
  return this->position == v->position && this->texture == v->texture && this->normal == v->normal;
}

bool Vertex::operator!=(const Vertex *const v)const{
  return !(v == this);
}

bool Vertex::operator==(const Vertex &v)const{
  return this->position == v.position && this->texture == v.texture && this->normal == v.normal;
}

bool Vertex::operator!=(const Vertex &v)const{
  return !(v == this);
}
