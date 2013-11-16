#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <vector>

#include "texture.hpp"

class Shader;
class VertexShader;
class FragmentShader;
class ShaderProgram;

class Shader {
	public:
		Shader(Shader const &s);
		Shader(Shader &&s);
		~Shader();
	protected:
		Shader();
		GLuint id;

		void load(std::string filename);
		virtual std::string loadCode(std::string filename);
		static std::map<GLuint, unsigned int> refCount;
};

class VertexShader : Shader {
	public:
		VertexShader(std::string filename);
	friend class ShaderProgram;
};

class FragmentShader : Shader {
	public:
		FragmentShader(std::string filename);
	friend class ShaderProgram;
};

class Uniform {
	public:
		Uniform(GLint program, GLint id);
		Uniform(GLint program, GLint id, GLenum type);
		Uniform &operator=(int val);
		Uniform &operator=(unsigned int val);
		Uniform &operator=(float val);
		Uniform &operator=(glm::vec4 const &val);
		Uniform &operator=(glm::vec4 &&val);
		Uniform &operator=(glm::vec3 const &val);
		Uniform &operator=(glm::vec3 &&val);
		Uniform &operator=(glm::vec2 const &val);
		Uniform &operator=(glm::vec2 &&val);
		Uniform &operator=(glm::mat4 const &val);
		Uniform &operator=(glm::mat4 &&val);
		Uniform &operator=(Texture const &val);
		Uniform &operator=(Texture &&val);

		std::string getName();
	protected:
		GLuint program;
		GLuint id;
		GLenum type;
};

class ShaderProgram {
	public:
		ShaderProgram();
		ShaderProgram(ShaderProgram const &s);
		ShaderProgram(ShaderProgram &&s);
		ShaderProgram(VertexShader, FragmentShader);
		ShaderProgram(FragmentShader, VertexShader);
		~ShaderProgram();

		ShaderProgram &operator=(ShaderProgram const &s);
		ShaderProgram &operator=(ShaderProgram &&s);

		void setUniform(std::string name, int value);
		void setUniform(std::string name, unsigned int value);
		void setUniform(std::string name, float value);
		void setUniform(std::string name, glm::vec4 const &value);
		void setUniform(std::string name, glm::vec4 &&value);
		void setUniform(std::string name, glm::vec3 const &value);
		void setUniform(std::string name, glm::vec3 &&value);
		void setUniform(std::string name, glm::vec2 const &value);
		void setUniform(std::string name, glm::vec2 &&value);
		void setUniform(std::string name, glm::mat4 const &value);
		void setUniform(std::string name, glm::mat4 &&value);
		void setUniform(std::string name, Texture const &value);
		void setUniform(std::string name, Texture &&value);

		GLuint getUniformLocation(std::string name);
		GLuint getAttribLocation(std::string name);

		Uniform operator[](std::string name);
		bool isSet(std::string);
		void use();

	protected:
		void del();

		GLuint id;
		std::map<std::string, GLuint> uids;
		std::map<std::string, GLuint> aids;
		std::shared_ptr<std::vector<Texture>> textures;
		static std::map<GLuint, std::shared_ptr<std::vector<Texture>>> gtextures;
		static std::map<GLuint, unsigned int> refCount;
		static GLint currentProgram;
	friend class Uniform;
	friend class VAO;
};



#endif