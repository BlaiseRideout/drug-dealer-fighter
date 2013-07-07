#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <vector>
#include <stdexcept>
#include <glm/glm.hpp>

#include "shader.hpp"

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
    this->ints.insert(std::pair<GLint, int>(uid, value));
}

template<>
void ShaderProgram::setUniform(std::string name, float value) {
    GLint uid = this->getUniformLocation(name);
    this->floats.insert(std::pair<GLint, float>(uid, value));
}

template<>
void ShaderProgram::setUniform(std::string name, glm::vec4 value) {
    GLint uid = this->getUniformLocation(name);
    this->vec4s.insert(std::pair<GLint, glm::vec4>(uid, value));
}

template<>
void ShaderProgram::setUniform(std::string name, glm::vec3 value) {
    GLint uid = this->getUniformLocation(name);
    this->vec3s.insert(std::pair<GLint, glm::vec3>(uid, value));
}

template<>
void ShaderProgram::setUniform(std::string name, glm::vec2 value) {
    GLint uid = this->getUniformLocation(name);
    this->vec2s.insert(std::pair<GLint, glm::vec2>(uid, value));
}

template<>
void ShaderProgram::setUniform(std::string name, glm::mat4 value) {
    GLint uid = this->getUniformLocation(name);
    this->mat4s.insert(std::pair<GLint, glm::mat4>(uid, value));
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

void ShaderProgram::prepareContext() {
    for(auto i = this->ints.begin(); i != this->ints.end(); ++i)
        glUniform1i(i->first, i->second);
    for(auto i = this->floats.begin(); i != this->floats.end(); ++i)
        glUniform1f(i->first, i->second);
    for(auto i = this->vec4s.begin(); i != this->vec4s.end(); ++i)
        glUniform4f(i->first, i->second.x, i->second.y, i->second.z, i->second.w);
    for(auto i = this->vec3s.begin(); i != this->vec3s.end(); ++i)
        glUniform3f(i->first, i->second.x, i->second.y, i->second.z);
    for(auto i = this->vec2s.begin(); i != this->vec2s.end(); ++i)
        glUniform2f(i->first, i->second.x, i->second.y);
    for(auto i = this->mat4s.begin(); i != this->mat4s.end(); ++i)
        glUniformMatrix4fv(i->first, 1, GL_FALSE, &i->second[0][0]);
}

void ShaderProgram::draw(Model &m) {
    glUseProgram(this->id);
    this->prepareContext();
    m.draw();
}