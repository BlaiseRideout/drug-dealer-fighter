#ifndef PLAYER_H
#define PLAYER_H

#include "physics.hpp"
#include "buffer.hpp"
#include "vao.hpp"
#include "controller.hpp"
#include "texture.hpp"

class Player {
	public:
		Player(World &w, bool id);

		void draw();
		void update();

		World &w;
		std::shared_ptr<Body> body;
		Controller controller;

		ShaderProgram textureShader;
		ShaderProgram pointShader;

		Texture bodyTex;
		Texture faceTex;
		Texture armTex;
		Texture legTex;
		Texture healthTex;
		Texture barTex;
		Texture blingTex;
		Texture backgroundTex;

		VAO texVao;
		Buffer texIndices;

		Buffer links;
		VAO vao;

		bool alive;
		float health;
		bool id;
		std::vector<glm::vec2> leftStick;
		std::vector<glm::vec2> rightStick;
		int curInput;
		glm::vec3 color;

		static const glm::vec3 colors[3];
	private:
		void initPhysics(float x, float y);
		void initGraphics();
		void hurt();
};

#endif