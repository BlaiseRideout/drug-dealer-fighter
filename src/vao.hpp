#ifndef VAO_H
#define VAO_H

#include "buffer.hpp"
#include "shader.hpp"

#include <GL/glew.h>


class VAO {
	public:
		VAO();
		void bind();
		void unbind();
		void setAttrib(GLuint attribute, Buffer const &b, int size, GLenum type, bool normalized, int divisor);
		void setAttrib(GLuint attribute, Buffer const &b, int size, GLenum type, bool normalized);
		void setAttrib(GLuint attribute, Buffer const &b, int size, bool normalized);
		void setAttrib(GLuint attribute, Buffer const &b, int size);
		void setAttrib(ShaderProgram &s, std::string name, Buffer const &b, int size, GLenum type, bool normalized, int divisor);
		void setAttrib(ShaderProgram &s, std::string name, Buffer const &b, int size, GLenum type, bool normalized);
		void setAttrib(ShaderProgram &s, std::string name, Buffer const &b, int size, bool normalized);
		void setAttrib(ShaderProgram &s, std::string name, Buffer const &b, int size);
		void setAttrib(ShaderProgram &&s, std::string name, Buffer const &b, int size, GLenum type, bool normalized, int divisor);
		void setAttrib(ShaderProgram &&s, std::string name, Buffer const &b, int size, GLenum type, bool normalized);
		void setAttrib(ShaderProgram &&s, std::string name, Buffer const &b, int size, bool normalized);
		void setAttrib(ShaderProgram &&s, std::string name, Buffer const &b, int size);
	protected:
		GLuint id = GL_MAX_VERTEX_ATTRIBS;
		std::map<GLuint, Buffer> attribs;
};

#endif