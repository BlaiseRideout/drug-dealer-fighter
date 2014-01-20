#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>
#include <map>

class Texture {
	public:
		Texture();
		Texture(Texture const &t);
		Texture(Texture &&t);
		Texture(std::string name);
		Texture(std::string name, GLint filter);
		Texture(std::string name, GLint mag_filter, GLint min_filter);
		~Texture();
		Texture &operator=(Texture const &s);
		Texture &operator=(Texture &&s);
		bool operator==(Texture const &s);

		unsigned int width();
		unsigned int height();
		void bind() const;
	protected:
		void del();
		GLuint id;

		static GLuint currentTexture;
		static std::map<GLuint, unsigned int> refCount;
};

#endif