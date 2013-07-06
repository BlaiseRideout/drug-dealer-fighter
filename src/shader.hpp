#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glfw.h>

class ShaderProgram;
class VertexShader;
class FragmentShader;

class Shader {
	public:
		~Shader();
	protected:
		GLuint id;

		void load(std::string filename);
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

class ShaderProgram {
	public:
		ShaderProgram(VertexShader &vshader, FragmentShader &pshader);
		void use();
	protected:
		GLuint id;
		VertexShader &vshader;
		FragmentShader &fshader;
};

#endif