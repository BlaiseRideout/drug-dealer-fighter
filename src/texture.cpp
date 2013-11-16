#include <FreeImage.h>
#include <vector>

#include "texture.hpp"

unsigned int Texture::width() {
	glBindTexture(GL_TEXTURE_2D, this->id);
	GLint width;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &width);
	glBindTexture(GL_TEXTURE_2D, Texture::currentTexture);
	return width;
}

unsigned int Texture::height() {
	glBindTexture(GL_TEXTURE_2D, this->id);
	GLint height;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	glBindTexture(GL_TEXTURE_2D, Texture::currentTexture);
	return height;
}

void Texture::bind() const {
	Texture::currentTexture = this->id;
	glBindTexture(GL_TEXTURE_2D, Texture::currentTexture);
}

Texture::Texture() {
}

Texture::Texture(Texture const &t) : id(t.id) {
	auto iterid = Texture::refCount.find(this->id);
    if(iterid != Texture::refCount.end())
        iterid->second = iterid->second + 1;
}

Texture::Texture(Texture &&t) : id(t.id) {
	auto iterid = Texture::refCount.find(this->id);
    if(iterid != Texture::refCount.end())
        iterid->second = iterid->second + 1;
}

Texture::Texture(std::string filename) : Texture(filename, GL_LINEAR, GL_LINEAR) {
}

Texture::Texture(std::string filename, GLint filter) : Texture(filename, filter, filter) {
}

Texture::Texture(std::string filename, GLint mag_filter, GLint min_filter) {
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.c_str(), 0);
	FIBITMAP* imagen = FreeImage_Load(format, filename.c_str());

	//FREE_IMAGE_COLOR_TYPE color_type = FreeImage_GetColorType(imagen);
	//if(color_type == FIC_PALETTE) {
		FIBITMAP* temp = imagen;
		imagen = FreeImage_ConvertTo32Bits(imagen);
		FreeImage_Unload(temp);
	//}

	unsigned int width = FreeImage_GetWidth(imagen);
	unsigned int height = FreeImage_GetHeight(imagen);
	if(width == 0 || height == 0)
		throw std::runtime_error("Couldn't load image: " + filename);

	std::vector<unsigned char> pixels(width * height * 4);
	unsigned char *pix = FreeImage_GetBits(imagen);

	glGenTextures(1, &this->id);

	glBindTexture(GL_TEXTURE_2D, this->id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pix);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);

	FreeImage_Unload(imagen);

	glBindTexture(GL_TEXTURE_2D, Texture::currentTexture);
}

Texture::~Texture() {
	auto iterid = Texture::refCount.find(this->id);
    if(iterid != Texture::refCount.end()) {
        iterid->second = iterid->second - 1;
        if(iterid->second == 0)
            glDeleteTextures(1, &this->id);
    }
}

Texture &Texture::operator=(Texture const &s) {
	auto iterid = Texture::refCount.find(this->id);
    if(iterid != Texture::refCount.end()) {
        iterid->second = iterid->second - 1;
        if(iterid->second == 0)
            glDeleteTextures(1, &this->id);
    }
    this->id = s.id;
    iterid = Texture::refCount.find(this->id);
    if(iterid != Texture::refCount.end())
        iterid->second = iterid->second + 1;
	return *this;
}

Texture &Texture::operator=(Texture &&s) {
	std::swap(s.id, this->id);
	return *this;
}

GLuint Texture::currentTexture;
std::map<GLuint, unsigned int> Texture::refCount;