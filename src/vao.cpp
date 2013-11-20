#include "vao.hpp"

#include <iostream>


VAO::VAO() : id(0) {
}

void VAO::setAttrib(GLuint attribute, Buffer const &b, int size, GLenum type, bool normalized, int divisor) {
	this->tempBind();
	b.setAttrib(attribute, size, type, normalized, divisor);
	this->attribs[attribute] = b;
	VAO::curBind();
}

void VAO::setAttrib(GLuint attribute, Buffer const &b, int size, GLenum type, bool normalized) {
	setAttrib(attribute, b, size, type, normalized, 0);
}

void VAO::setAttrib(GLuint attribute, Buffer const &b, int size, bool normalized) {
	setAttrib(attribute, b, size, GL_FLOAT, normalized);
}

void VAO::setAttrib(GLuint attribute, Buffer const &b, int size) {
	setAttrib(attribute, b, size, GL_FLOAT, false);
}


void VAO::setAttrib(ShaderProgram &s, std::string name, Buffer const &b, int size, GLenum type, bool normalized, int divisor) {
	setAttrib(s.getAttribLocation(name), b, size, type, normalized, divisor);
}

void VAO::setAttrib(ShaderProgram &s, std::string name, Buffer const &b, int size, GLenum type, bool normalized) {
	setAttrib(s.getAttribLocation(name), b, size, type, normalized, 0);
}

void VAO::setAttrib(ShaderProgram &s, std::string name, Buffer const &b, int size, bool normalized) {
	setAttrib(s, name, b, size, GL_FLOAT, normalized);
}

void VAO::setAttrib(ShaderProgram &s, std::string name, Buffer const &b, int size) {
	setAttrib(s, name, b, size, GL_FLOAT, false);
}


void VAO::setAttrib(ShaderProgram &&s, std::string name, Buffer const &b, int size, GLenum type, bool normalized, int divisor) {
	setAttrib(s.getAttribLocation(name), b, size, type, normalized, divisor);
}

void VAO::setAttrib(ShaderProgram &&s, std::string name, Buffer const &b, int size, GLenum type, bool normalized) {
	setAttrib(s.getAttribLocation(name), b, size, type, normalized, 0);
}

void VAO::setAttrib(ShaderProgram &&s, std::string name, Buffer const &b, int size, bool normalized) {
	setAttrib(s, name, b, size, GL_FLOAT, normalized);
}

void VAO::setAttrib(ShaderProgram &&s, std::string name, Buffer const &b, int size) {
	setAttrib(s, name, b, size, GL_FLOAT, false);
}

void VAO::bind() {
	this->tempBind();
	VAO::currentVAO = this->id;
}

void VAO::unbind() {
	glBindVertexArray(0);
	VAO::currentVAO = 0;
}

void VAO::tempBind() {
	if(this->id == 0)
		glGenVertexArrays(1, &this->id);
	glBindVertexArray(this->id);
}

void VAO::curBind() {
	glBindVertexArray(VAO::currentVAO);
}

GLuint VAO::currentVAO = 0;