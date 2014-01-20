#ifndef CODEDAY_H
#define CODEDAY_H

#include "game.hpp"
#include "buffer.hpp"
#include "shader.hpp"
#include "physics.hpp"
#include "vao.hpp"
#include "player.hpp"

class Codeday : public Game {
	public:
		Codeday();
		~Codeday();
	protected:
		virtual void update(double time);
		void draw();
	private:
		World world;
		Player player;
		Player player2;
};

#endif
