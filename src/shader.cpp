#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <vector>

#include "shader.hpp"

Shader::~Shader() {
	glDeleteShader(this->id);
}

void Shader::load(std::string filename) {
    // Read the shader code from the file
    std::string ShaderCode;
    std::ifstream ShaderStream(filename.c_str(), std::ios::in);
    if(ShaderStream.is_open()){
        std::string Line = "";
        while(getline(ShaderStream, Line))
            ShaderCode += "\n" + Line;
        ShaderStream.close();
    }
 
    GLint Result = GL_FALSE;
    int InfoLogLength;
 
    // Compile Shader
    std::cout << "Compiling shader : " << filename << std::endl;
    char const * SourcePointer = ShaderCode.c_str();
    glShaderSource(this->id, 1, &SourcePointer , NULL);
    glCompileShader(this->id);
 
    // Check Shader
    glGetShaderiv(this->id, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(this->id, InfoLogLength, NULL, &ShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &ShaderErrorMessage[0]);
}

VertexShader::VertexShader(std::string filename) {
	this->id = glCreateShader(GL_VERTEX_SHADER);
	this->load(filename);
}

FragmentShader::FragmentShader(std::string filename) {
	this->id = glCreateShader(GL_FRAGMENT_SHADER);
	this->load(filename);
}

ShaderProgram::ShaderProgram(VertexShader &vshader, FragmentShader &fshader) : vshader(vshader), fshader(fshader) {
	GLint Result = GL_FALSE;
    int InfoLogLength;

	this->vshader = vshader;
	this->fshader = fshader;

	// Link the program
    fprintf(stdout, "Linking program\n");
    this->id = glCreateProgram();
    glAttachShader(this->id, vshader.id);
    glAttachShader(this->id, fshader.id);
    glLinkProgram(this->id);
 
    // Check the program
    glGetProgramiv(this->id, GL_LINK_STATUS, &Result);
    glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(this->id, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
}

void ShaderProgram::use() {
	glUseProgram(this->id);
}