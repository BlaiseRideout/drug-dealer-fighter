#ifndef MODEL_H
#define MODEL_H
#include <string>
#include <glm/glm.hpp>

#include "vertex.hpp"
#include "buffer.hpp"
#include "vao.hpp"
#include "shader.hpp"

#include <string>


class Model {
	public:
		Model();
		Model(Model const &s);
		Model(std::string filename, GLuint positionAttribute, GLuint normalAttribute, GLuint uvAttribute);
		Model(std::string filename, ShaderProgram &s, std::string positionName, std::string normalName, std::string uvName);
		Model(std::string filename, ShaderProgram &&s, std::string positionName, std::string normalName, std::string uvName);
		Model(std::string filename, ShaderProgram &s);
		Model(std::string filename, ShaderProgram &&s);

		void draw();
	private:
		void loadModel(std::string filename, GLuint positionAttribute, GLuint normalAttribute, GLuint uvAttribute);

		Buffer indices;
		VAO vao;
};

#endif