#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <vector>
#include <iostream>

#include "texture.hpp"

class Shader;
class VertexShader;
class FragmentShader;
class ShaderProgram;

class Shader {
	public:
		Shader();
		Shader(std::string const &filename, GLenum type);
		Shader(Shader const &s);
		~Shader();

		Shader &operator=(Shader const &s);
		Shader &operator=(Shader &&s);
		operator GLuint();

		GLuint id;
	protected:
		void load(std::string const &filename);
		std::string loadCode(std::string const &filename);
		void del();

		static std::map<GLuint, unsigned int> refCount;
};

class VertexShader : public Shader {
	public:
		VertexShader(std::string const &filename);
};

class FragmentShader : public Shader {
	public:
		FragmentShader(std::string const &filename);
};

class GeometryShader : public Shader {
	public:
		GeometryShader(std::string const &filename);
};

#ifdef GL_VERSION_4_3
class ComputeShader : public Shader {
	public:
		ComputeShader(std::string const &filename);
};
#endif

class TessControlShader : public Shader {
	public:
		TessControlShader(std::string const &filename);
};

class TessEvaluationShader : public Shader {
	public:
		TessEvaluationShader(std::string const &filename);
};

class Uniform {
	public:
		Uniform(GLuint program, GLuint id);
		Uniform &operator=(int val);
		Uniform &operator=(unsigned int val);
		Uniform &operator=(float val);
		Uniform &operator=(glm::vec4 const &val);
		Uniform &operator=(glm::vec3 const &val);
		Uniform &operator=(glm::vec2 const &val);
		Uniform &operator=(glm::mat4 const &val);
		Uniform &operator=(Texture const &val);

		std::string getName();
		GLenum getType();
		GLint getSize();
	protected:
		GLuint program;
		GLuint id;
};

class ShaderProgram {
	public:
		ShaderProgram();
		ShaderProgram(ShaderProgram const &s);
		template<typename ...S>
		ShaderProgram(S... s);
		~ShaderProgram();

		ShaderProgram &operator=(ShaderProgram const &s);
		ShaderProgram &operator=(ShaderProgram &&s);
		operator GLuint();

		void setUniform(std::string const &name, int value);
		void setUniform(std::string const &name, unsigned int value);
		void setUniform(std::string const &name, float value);
		void setUniform(std::string const &name, glm::vec4 const &value);
		void setUniform(std::string const &name, glm::vec3 const &value);
		void setUniform(std::string const &name, glm::vec2 const &value);
		void setUniform(std::string const &name, glm::mat4 const &value);
		void setUniform(std::string const &name, Texture const &value);

		GLuint getUniformLocation(std::string const &name);
		GLuint getAttribLocation(std::string const &name);

		Uniform operator[](std::string const &name);
		Uniform operator[](const char *name);
		bool isSet(std::string);
		void use();

		GLuint id;
	protected:
		void del();
		std::vector<Texture> &textures();

		std::map<std::string, GLuint> uids;
		std::map<std::string, GLuint> aids;
		static std::map<GLuint, std::vector<Texture>> gtextures;
		static std::map<GLuint, unsigned> refCount;
		static GLuint currentProgram;
	friend class Uniform;
};

template<typename ...S>
ShaderProgram::ShaderProgram(S... s) {
	GLint Result = GL_FALSE;
    int InfoLogLength;

	// Link the program
    std::cout << "Linking program" << std::endl;
    this->id = glCreateProgram();
    std::vector<Shader> shaders({s...});
    for(auto i = shaders.begin(); i != shaders.end(); ++i)
    	glAttachShader(this->id, i->id);
    glLinkProgram(this->id);
 
    // Check the program
    glGetProgramiv(this->id, GL_LINK_STATUS, &Result);
    if(Result == GL_FALSE) {
        glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &InfoLogLength);
        std::string ProgramErrorMessage;
        ProgramErrorMessage.resize(std::max(InfoLogLength, int(1)));
        glGetProgramInfoLog(this->id, InfoLogLength, NULL, &ProgramErrorMessage[0]);

        throw std::runtime_error(ProgramErrorMessage);
    }

    ShaderProgram::refCount.insert(std::pair<GLuint, unsigned>(this->id, 1));
    ShaderProgram::gtextures.insert(std::pair<GLuint, std::vector<Texture>>(this->id, std::vector<Texture>()));
}


#endif