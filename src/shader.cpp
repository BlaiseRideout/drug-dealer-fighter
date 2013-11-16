#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <vector>
#include <stdexcept>
#include <glm/glm.hpp>
#include <sstream>

#include "shader.hpp"

Shader::Shader(Shader const &s) : id(s.id) {
    auto iterid = Shader::refCount.find(this->id);
    if(iterid != Shader::refCount.end())
        iterid->second = iterid->second + 1;
}

Shader::Shader(Shader &&s) : id(s.id) {
    auto iterid = Shader::refCount.find(this->id);
    if(iterid != Shader::refCount.end())
        iterid->second = iterid->second + 1;
}

Shader::Shader() {
}

Shader::~Shader() {
    auto iterid = Shader::refCount.find(this->id);
    if(iterid != Shader::refCount.end()) {
        iterid->second = iterid->second - 1;
        if(iterid->second == 0)
            glDeleteShader(this->id);
    }
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

VertexShader::VertexShader(std::string filename) : Shader() {
	this->id = glCreateShader(GL_VERTEX_SHADER);
    Shader::refCount.insert(std::pair<GLint, int>(this->id, 1));
	this->load(filename);
}

FragmentShader::FragmentShader(std::string filename) : Shader() {
	this->id = glCreateShader(GL_FRAGMENT_SHADER);
    Shader::refCount.insert(std::pair<GLint, int>(this->id, 1));
	this->load(filename);
}

ShaderProgram::ShaderProgram() {
}

ShaderProgram::ShaderProgram(ShaderProgram const &s) : id(s.id), textures(s.textures) {
    auto iterid = ShaderProgram::refCount.find(this->id);
    if(iterid != ShaderProgram::refCount.end())
        iterid->second = iterid->second + 1;
}

ShaderProgram::ShaderProgram(ShaderProgram &&s) : id(s.id), textures(s.textures) {
    auto iterid = ShaderProgram::refCount.find(this->id);
    if(iterid != ShaderProgram::refCount.end())
        iterid->second = iterid->second + 1;
}

ShaderProgram::ShaderProgram(VertexShader vshader, FragmentShader fshader) {
	GLint Result = GL_FALSE;
    int InfoLogLength;

	// Link the program
    fprintf(stdout, "Linking program\n");
    this->id = glCreateProgram();
    ShaderProgram::refCount.insert(std::pair<GLint, int>(this->id, 1));
    glAttachShader(this->id, vshader.id);
    glAttachShader(this->id, fshader.id);
    glLinkProgram(this->id);
 
    // Check the program
    glGetProgramiv(this->id, GL_LINK_STATUS, &Result);
    glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(this->id, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

    this->textures.reset(new std::vector<Texture>());
    ShaderProgram::gtextures.insert(std::pair<GLuint, std::shared_ptr<std::vector<Texture>>>(this->id, this->textures));
}

ShaderProgram::ShaderProgram(FragmentShader f, VertexShader v) : ShaderProgram(v, f) {
}

ShaderProgram::~ShaderProgram() {
    auto iterid = ShaderProgram::refCount.find(this->id);
    if(iterid != ShaderProgram::refCount.end()) {
        iterid->second = iterid->second - 1;
        if(iterid->second == 0)
            del();
    }
}

void ShaderProgram::del() {
    glDeleteProgram(this->id);
    
    auto iterid = ShaderProgram::refCount.find(this->id);
    if(iterid != ShaderProgram::refCount.end())
        ShaderProgram::refCount.erase(iterid);

    auto i = ShaderProgram::gtextures.find(this->id);
    if(i != ShaderProgram::gtextures.end())
        ShaderProgram::gtextures.erase(i);
}

ShaderProgram &ShaderProgram::operator=(ShaderProgram const&s) {
    auto iterid = ShaderProgram::refCount.find(this->id);
    if(iterid != ShaderProgram::refCount.end()) {
        iterid->second = iterid->second - 1;
        if(iterid->second == 0)
            del();
    }
    this->id = s.id;
    iterid = ShaderProgram::refCount.find(this->id);
    if(iterid != ShaderProgram::refCount.end())
        iterid->second = iterid->second + 1;
    this->textures = s.textures;
    return *this;
}

ShaderProgram &ShaderProgram::operator=(ShaderProgram &&s) {
    std::swap(this->id, s.id);
    std::swap(this->textures, s.textures);
    return *this;
}

void ShaderProgram::setUniform(std::string name, int value) {
    glUseProgram(this->id);
    GLint uid = this->getUniformLocation(name);
    glUniform1i(uid, value);
    glUseProgram(ShaderProgram::currentProgram);
}

void ShaderProgram::setUniform(std::string name, unsigned int value) {
    glUseProgram(this->id);
    GLint uid = this->getUniformLocation(name);
    glUniform1ui(uid, value);
    glUseProgram(ShaderProgram::currentProgram);
}

void ShaderProgram::setUniform(std::string name, float value) {
    glUseProgram(this->id);
    GLint uid = this->getUniformLocation(name);
    glUniform1f(uid, value);
    glUseProgram(ShaderProgram::currentProgram);
}

void ShaderProgram::setUniform(std::string name, glm::vec4 const &value) {
    glUseProgram(this->id);
    GLint uid = this->getUniformLocation(name);
    glUniform4f(uid, value.x, value.y, value.z, value.w);
    glUseProgram(ShaderProgram::currentProgram);
}

void ShaderProgram::setUniform(std::string name, glm::vec4 &&value) {
    glUseProgram(this->id);
    GLint uid = this->getUniformLocation(name);
    glUniform4f(uid, value.x, value.y, value.z, value.w);
    glUseProgram(ShaderProgram::currentProgram);
}

void ShaderProgram::setUniform(std::string name, glm::vec3 const &value) {
    glUseProgram(this->id);
    GLint uid = this->getUniformLocation(name);
    glUniform3f(uid, value.x, value.y, value.z);
    glUseProgram(ShaderProgram::currentProgram);
}

void ShaderProgram::setUniform(std::string name, glm::vec3 &&value) {
    glUseProgram(this->id);
    GLint uid = this->getUniformLocation(name);
    glUniform3f(uid, value.x, value.y, value.z);
    glUseProgram(ShaderProgram::currentProgram);
}

void ShaderProgram::setUniform(std::string name, glm::vec2 const &value) {
    glUseProgram(this->id);
    GLint uid = this->getUniformLocation(name);
    glUniform2f(uid, value.x, value.y);
    glUseProgram(ShaderProgram::currentProgram);
}

void ShaderProgram::setUniform(std::string name, glm::vec2 &&value) {
    glUseProgram(this->id);
    GLint uid = this->getUniformLocation(name);
    glUniform2f(uid, value.x, value.y);
    glUseProgram(ShaderProgram::currentProgram);
}

void ShaderProgram::setUniform(std::string name, glm::mat4 const &value) {
    glUseProgram(this->id);
    GLint uid = this->getUniformLocation(name);
    glUniformMatrix4fv(uid, 1, GL_FALSE, &value[0][0]);
    glUseProgram(ShaderProgram::currentProgram);
}

void ShaderProgram::setUniform(std::string name, glm::mat4 &&value) {
    glUseProgram(this->id);
    GLint uid = this->getUniformLocation(name);
    glUniformMatrix4fv(uid, 1, GL_FALSE, &value[0][0]);
    glUseProgram(ShaderProgram::currentProgram);
}

Uniform ShaderProgram::operator[](std::string name) {
    return Uniform(this->id, getUniformLocation(name));
}

GLuint ShaderProgram::getUniformLocation(std::string name) {
    auto iterid = this->uids.find(name);
    if(iterid != this->uids.end())
        return iterid->second;
    else {
        GLint uid = glGetUniformLocation(this->id, name.c_str());
        if(uid == -1)
            throw std::runtime_error("Uniform not defined by shader: " + name);
        else
            this->uids.insert(std::pair<std::string, GLuint>(name, uid));
        return uid;
    }
}

GLuint ShaderProgram::getAttribLocation(std::string name) {
    auto iterid = this->aids.find(name);
    if(iterid != this->aids.end())
        return iterid->second;
    else {
        GLint aid = glGetAttribLocation(this->id, name.c_str());
        if(aid == -1)
            throw std::runtime_error("Attribute not defined by shader: " + name);
        else
            this->aids.insert(std::pair<std::string, GLuint>(name, aid));
        return aid;
    }
    
}

bool ShaderProgram::isSet(std::string name) {
    auto iterid = this->uids.find(name);
    return iterid != this->uids.end();
}

void ShaderProgram::use() {
    glUseProgram(this->id);
    for(unsigned i = 0; i < this->textures->size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        (*this->textures)[i].bind();
    }
    ShaderProgram::currentProgram = this->id;
}

Uniform::Uniform(GLint program, GLint id) : program(program), id(id) {
    GLint size;
    GLchar name;
    glGetActiveUniform(program, id, 0, NULL, &size, &this->type, &name);
}

Uniform::Uniform(GLint program, GLint id, GLenum type) : program(program), id(id), type(type) {
}

Uniform &Uniform::operator=(int val) {
    glUseProgram(this->program);
    glUniform1i(this->id, val);
    glUseProgram(ShaderProgram::currentProgram);
    return *this;
}

Uniform &Uniform::operator=(unsigned int val) {
    glUseProgram(this->program);
    glUniform1ui(this->id, val);
    glUseProgram(ShaderProgram::currentProgram);
    return *this;
}

Uniform &Uniform::operator=(float val) {
    glUseProgram(this->program);
    glUniform1f(this->id, val);
    glUseProgram(ShaderProgram::currentProgram);
    return *this;
}

Uniform &Uniform::operator=(glm::vec4 const &val) {
    glUseProgram(this->program);
    glUniform4f(this->id, val.x, val.y, val.z, val.w);
    glUseProgram(ShaderProgram::currentProgram);
    return *this;
}

Uniform &Uniform::operator=(glm::vec4 &&val) {
    glUseProgram(this->program);
    glUniform4f(this->id, val.x, val.y, val.z, val.w);
    glUseProgram(ShaderProgram::currentProgram);
    return *this;
}

Uniform &Uniform::operator=(glm::vec3 const &val) {
    glUseProgram(this->program);
    glUniform3f(this->id, val.x, val.y, val.z);
    glUseProgram(ShaderProgram::currentProgram);
    return *this;
}

Uniform &Uniform::operator=(glm::vec3 &&val) {
    glUseProgram(this->program);
    glUniform3f(this->id, val.x, val.y, val.z);
    glUseProgram(ShaderProgram::currentProgram);
    return *this;
}

Uniform &Uniform::operator=(glm::vec2 const &val) {
    glUseProgram(this->program);
    glUniform2f(this->id, val.x, val.y);
    glUseProgram(ShaderProgram::currentProgram);
    return *this;
}

Uniform &Uniform::operator=(glm::vec2 &&val) {
    glUseProgram(this->program);
    glUniform2f(this->id, val.x, val.y);
    glUseProgram(ShaderProgram::currentProgram);
    return *this;
}

Uniform &Uniform::operator=(glm::mat4 const &val) {
    glUseProgram(this->program);
    glUniformMatrix4fv(this->id, 1, GL_FALSE, &val[0][0]);
    glUseProgram(ShaderProgram::currentProgram);
    return *this;
}

Uniform &Uniform::operator=(glm::mat4 &&val) {
    glUseProgram(this->program);
    glUniformMatrix4fv(this->id, 1, GL_FALSE, &val[0][0]);
    glUseProgram(ShaderProgram::currentProgram);
    return *this;
}

Uniform &Uniform::operator=(Texture const &val) {
    glUseProgram(this->program);
    
    auto i = ShaderProgram::gtextures.find(this->program);
    if(i != ShaderProgram::gtextures.end()) {
        std::shared_ptr<std::vector<Texture>> textures = i->second;
        int index = textures->size();
        glUniform1i(this->id, index);

        glActiveTexture(GL_TEXTURE0 + index);
        val.bind();
        textures->push_back(val);
    }

    glUseProgram(ShaderProgram::currentProgram);
    return *this;
}

Uniform &Uniform::operator=(Texture &&val) {
    glUseProgram(this->program);
    
    auto i = ShaderProgram::gtextures.find(this->program);
    if(i != ShaderProgram::gtextures.end()) {
        std::shared_ptr<std::vector<Texture>> textures = i->second;
        int index = textures->size();
        glUniform1i(this->id, index);

        glActiveTexture(GL_TEXTURE0 + index);
        val.bind();
        textures->push_back(val);
    }

    glUseProgram(ShaderProgram::currentProgram);
    return *this;
}

std::string Uniform::getName() {
    GLint size;
    GLenum type;
    GLchar name[256];
    glGetActiveUniform(program, id, 255, NULL, &size, &type, name);
    return std::string(name);
}

GLint ShaderProgram::currentProgram;
std::map<GLuint, unsigned int> Shader::refCount;
std::map<GLuint, unsigned int> ShaderProgram::refCount;
std::map<GLuint, std::shared_ptr<std::vector<Texture>>> ShaderProgram::gtextures;