#ifndef MODEL_H
#define MODEL_H
#include <string>
#include <glm/glm.hpp>

class Model;

#include "shader.hpp"
#include "vertex.hpp"
#include "camera.hpp"

class Model {
	public:
    Model(std::string);

    void draw();
	private:
    GLuint positions;
    GLuint uvs;
    GLuint normals;
    GLuint indices;
    unsigned int numIndices;
};

#endif