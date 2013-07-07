#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <vector>
#include <stdexcept>
#include <glm/glm.hpp>

#include "shader.hpp"

ShaderProgram *ShaderProgram::current = NULL;

Shader::~Shader() {
	glDeleteShader(this->id);
}

void Shader::load(std::string filename) {
    std::string ShaderCode = this->loadCode(filename);

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

std::string Shader::loadCode(std::string filename) {
    // Read the shader code from the file
    std::string ShaderCode;
    std::ifstream ShaderStream(filename.c_str(), std::ios::in);

    if(ShaderStream.is_open()){
        std::string Line = "";
        while(getline(ShaderStream, Line))
            ShaderCode += "\n" + Line;
        ShaderStream.close();
    }
    return ShaderCode;
}

std::string VertexShader::loadCode(std::string filename) {
    // Read the shader code from the file
    std::string ShaderCode;
    std::ifstream ShaderStream(filename.c_str(), std::ios::in);

    if(ShaderStream.is_open()){
        std::string Line = "";
        getline(ShaderStream, Line);
        while(Line[0] == '#') {
            ShaderCode += "\n" + Line;
            getline(ShaderStream, Line);
        }
        ShaderCode += "\nlayout(location = 0) in vec3 vertexPosition;"
        "\nlayout(location = 1) in vec3 vertexTexCoord;"
        "\nlayout(location = 2) in vec3 vertexNormal;";
        ShaderCode += "\n" + Line;
        
        while(getline(ShaderStream, Line))
            ShaderCode += "\n" + Line;
        ShaderStream.close();
    }
    return ShaderCode;
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

template<>
void ShaderProgram::setUniform(std::string name, int value) {
    GLint uid = this->getUniformLocation(name);
    glUniform1i(uid, value);
}

template<>
void ShaderProgram::setUniform(std::string name, float value) {
    GLint uid = this->getUniformLocation(name);
    glUniform1f(uid, value);
}

template<>
void ShaderProgram::setUniform(std::string name, glm::vec4 value) {
    GLint uid = this->getUniformLocation(name);
    glUniform4f(uid, value.x, value.y, value.z, value.w);
}

template<>
void ShaderProgram::setUniform(std::string name, glm::vec3 value) {
    GLint uid = this->getUniformLocation(name);
    glUniform3f(uid, value.x, value.y, value.z);
}

template<>
void ShaderProgram::setUniform(std::string name, glm::vec2 value) {
    GLint uid = this->getUniformLocation(name);
    glUniform2f(uid, value.x, value.y);
}

template<>
void ShaderProgram::setUniform(std::string name, glm::mat4 value) {
    GLint uid = this->getUniformLocation(name);
    glUniformMatrix4fv(uid, 1, GL_FALSE, &value[0][0]);
}

template<class T>
void ShaderProgram::setUniform(std::string name, T value) {
   throw std::runtime_error("Unknown type when setting shader uniform: " + name);
}

GLint ShaderProgram::getUniformLocation(std::string name) {
    auto iterid = this->uids.find(name);
    if(iterid != this->uids.end())
        return iterid->second;
    else{
        GLint uid = glGetUniformLocation(this->id, name.c_str());
        if(uid == -1)
            throw std::runtime_error("Uniform not defined by shader: " + name);
        else
            this->uids.insert(std::pair<std::string, GLint>(name, uid));
        return uid;
    }
}

bool ShaderProgram::isSet(std::string name) {
    auto iterid = this->uids.find(name);
    return iterid != this->uids.end();
}

void ShaderProgram::draw(Model &m) {
    this->use();
    m.draw();
}

ShaderProgram *ShaderProgram::getCurrent() {
    return ShaderProgram::current;
}

void ShaderProgram::use() {
	glUseProgram(this->id);
    ShaderProgram::current = this;
}