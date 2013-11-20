#ifndef buffer_H
#define buffer_H

#include "shader.hpp"

#include <map>
#include <vector>
#include <GL/glew.h>
#include <string>
#include <iostream>


class Buffer {
	public:
		Buffer();
		Buffer(GLenum target);
		Buffer(Buffer const &b);
		~Buffer();
		Buffer &operator=(Buffer const &b);
		Buffer &operator=(Buffer &&b);
		operator GLuint();

		template<typename T>
		Buffer(std::vector<T> const &data, GLenum target, GLenum usage) : Buffer() {
			loadData(data, target, usage);
		}

		template<typename T>
		Buffer(std::vector<T> const &data, GLenum target) : Buffer(data, target, GL_STATIC_DRAW) {
		}

		template<typename T>
		Buffer(std::vector<T> const &data) : Buffer(data, GL_ARRAY_BUFFER) {
		}

		template<typename T>
		void loadData(std::vector<T> const &data, GLenum target, GLenum usage) {
			if(this->id == 0) {
				glGenBuffers(1, &this->id);
    			Buffer::refCount.insert(std::pair<GLint, unsigned>(this->id, 1));
    		}
			glBindBuffer(target, this->id);
			glBufferData(target, data.size() * sizeof(T), &data[0], usage);
			this->target = target;
			this->size = data.size();
		}

		template<typename T>
		void loadData(std::vector<T> const &data, GLenum target) {
			loadData(data, target, GL_STATIC_DRAW);
		}

		template<typename T>
		void loadData(std::vector<T> const &data) {
			loadData(data, this->target);
		}

		void bind(GLenum target) const;
		void bind() const;
		void unbind(GLenum target) const;
		void unbind() const;

		void setAttrib(GLuint attribute, int size, GLenum type, bool normalized, int divisor) const;
		void setAttrib(GLuint attribute, int size, GLenum type, bool normalized) const;
		void setAttrib(GLuint attribute, int size, bool normalized) const;
		void setAttrib(GLuint attribute, int size) const;
		void setAttrib(ShaderProgram &s, std::string name, int size, GLenum type, bool normalized, int divisor) const;
		void setAttrib(ShaderProgram &s, std::string name, int size, GLenum type, bool normalized) const;
		void setAttrib(ShaderProgram &s, std::string name, int size, bool normalized) const;
		void setAttrib(ShaderProgram &s, std::string name, int size) const;
		void setAttrib(ShaderProgram &&s, std::string name, int size, GLenum type, bool normalized, int divisor) const;
		void setAttrib(ShaderProgram &&s, std::string name, int size, GLenum type, bool normalized) const;
		void setAttrib(ShaderProgram &&s, std::string name, int size, bool normalized) const;
		void setAttrib(ShaderProgram &&s, std::string name, int size) const;

		void drawArrays(GLenum mode);
		void drawArrays();
		void drawElements(GLenum mode, GLenum type);
		void drawElements(GLenum mode);
		void drawElements();
		void drawInstanced(int count);
		void drawInstanced(GLenum mode, int count);
		void drawInstanced(GLenum mode, int count, GLenum type);
	protected:
		void del();

		GLuint id;
		unsigned int size;
		GLenum target;
		static std::map<GLuint, unsigned int> refCount;
};

class IndexBuffer : public Buffer {
	public:
		IndexBuffer();
		IndexBuffer(IndexBuffer const &b);
		IndexBuffer(std::vector<unsigned> const &data, GLenum usage);
		IndexBuffer(std::vector<unsigned> const &data);
};

#endif
