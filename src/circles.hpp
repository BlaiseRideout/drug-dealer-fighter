#ifndef CIRCLES_H
#define CIRCLES_H

#include "game.hpp"
#include "buffer.hpp"
#include "shader.hpp"


class Circles : public Game {
	public:
		Circles();
		~Circles();
	protected:
		virtual void update(double time);
	private:
		ShaderProgram s;
		Buffer points;
		Buffer colors;
};

#endif
