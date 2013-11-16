#include "buffer.hpp"

Buffer::Buffer() {
}

Buffer::Buffer(Buffer const &v) : id(v.id), size(v.size), target(v.target) {
    auto iterid = Buffer::refCount.find(this->id);
    if(iterid != Buffer::refCount.end())
        iterid->second = iterid->second + 1;
}

Buffer::Buffer(Buffer &&v) : id(v.id), size(v.size), target(v.target) {
    auto iterid = Buffer::refCount.find(this->id);
    if(iterid != Buffer::refCount.end())
        iterid->second = iterid->second + 1;
}

Buffer::~Buffer() {
    auto iterid = Buffer::refCount.find(this->id);
    if(iterid != Buffer::refCount.end()) {
        iterid->second = iterid->second - 1;
        if(iterid->second == 0)
            glDeleteBuffers(1, &this->id);
    }
}

Buffer &Buffer::operator=(Buffer const &b) {
    auto iterid = Buffer::refCount.find(this->id);
    if(iterid != Buffer::refCount.end()) {
        iterid->second = iterid->second - 1;
        if(iterid->second == 0)
            glDeleteBuffers(1, &this->id);
    }
    this->id = b.id;
    this->size = b.size;
    this->target = target;
    iterid = Buffer::refCount.find(this->id);
    if(iterid != Buffer::refCount.end())
        iterid->second = iterid->second + 1;
    return *this;
}

Buffer &Buffer::operator=(Buffer &&b) {
    std::swap(this->id, b.id);
    std::swap(this->size, b.size);
    std::swap(this->target, b.target);
    return *this;
}

Buffer::operator GLuint() {
    return this->id;
}

void Buffer::bind(GLenum target) const {
    glBindBuffer(target, this->id);
}

void Buffer::bind() const {
    bind(this->target);
}

void Buffer::unbind(GLenum target) const {
    glBindBuffer(this->target, 0);
}

void Buffer::unbind() const {
    unbind(this->target);
}

void Buffer::setAttrib(GLuint attribute, int size, GLenum type, bool normalized, int divisor) const {
    glEnableVertexAttribArray(attribute);
    this->bind();
    glVertexAttribPointer(
        attribute,
        size,
        type,
        normalized?GL_TRUE:GL_FALSE,
        0,                                // stride
        (void*)0                          // array buffer offset
    );
    glVertexAttribDivisor(attribute, divisor);
}

void Buffer::setAttrib(GLuint attribute, int size, GLenum type, bool normalized) const {
    setAttrib(attribute, size, type, normalized, 0);
}

void Buffer::setAttrib(GLuint attribute, int size, bool normalized) const {
    setAttrib(attribute, size, GL_FLOAT, normalized);
}

void Buffer::setAttrib(GLuint attribute, int size) const {
    setAttrib(attribute, size, GL_FLOAT, false);
}

void Buffer::setAttrib(ShaderProgram &s, std::string name, int size, GLenum type, bool normalized, int divisor) const {
    setAttrib(s.getAttribLocation(name), size, type, normalized, divisor);
}

void Buffer::setAttrib(ShaderProgram &s, std::string name, int size, GLenum type, bool normalized) const {
    setAttrib(s.getAttribLocation(name), size, type, normalized);
}

void Buffer::setAttrib(ShaderProgram &s, std::string name, int size, bool normalized) const {
    setAttrib(s, name, size, GL_FLOAT, normalized);
}

void Buffer::setAttrib(ShaderProgram &s, std::string name, int size) const {
    setAttrib(s, name, size, GL_FLOAT, false);
}


void Buffer::setAttrib(ShaderProgram &&s, std::string name, int size, GLenum type, bool normalized, int divisor) const {
    setAttrib(s.getAttribLocation(name), size, type, normalized, divisor);
}

void Buffer::setAttrib(ShaderProgram &&s, std::string name, int size, GLenum type, bool normalized) const {
    setAttrib(s.getAttribLocation(name), size, type, normalized);
}

void Buffer::setAttrib(ShaderProgram &&s, std::string name, int size, bool normalized) const {
    setAttrib(s, name, size, GL_FLOAT, normalized);
}

void Buffer::setAttrib(ShaderProgram &&s, std::string name, int size) const {
    setAttrib(s, name, size, GL_FLOAT, false);
}

void Buffer::drawArrays() {
    drawArrays(GL_TRIANGLES);
}

void Buffer::drawArrays(GLenum mode) {
    glDrawArrays(mode, 0, this->size);
}

void Buffer::drawElements() {
    drawElements(GL_TRIANGLES, GL_UNSIGNED_INT);
}

void Buffer::drawElements(GLenum mode, GLenum type) {
    this->bind(GL_ELEMENT_ARRAY_BUFFER);
    glDrawElements(
        mode,
        this->size,
        type,
        (void*)0           // element array buffer offset
    );
}

void Buffer::drawInstanced(int count) {
    drawInstanced(GL_TRIANGLES, count);
}

void Buffer::drawInstanced(GLenum mode, int count) {
    drawInstanced(mode, count, GL_UNSIGNED_INT);
}

void Buffer::drawInstanced(GLenum mode, int count, GLenum type) {
    this->bind(GL_ELEMENT_ARRAY_BUFFER);
    glDrawElementsInstanced(
        mode,
        this->size,
        type,
        (void*)0,
        count
    );
}



std::map<GLuint, unsigned int> Buffer::refCount;