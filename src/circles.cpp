#include "circles.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


Circles::Circles() : Game("Circles"), s(VertexShader("res/circle.vert"), FragmentShader("res/circle.frag")) {
	std::vector<glm::vec3> pointData;
	pointData.push_back(glm::vec3(50.0f, 50.0f, 1.0f));
	this->points = Buffer(pointData);

	std::vector<glm::vec3> colorData;
	colorData.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	this->colors = Buffer(colorData);

	this->points.setAttrib(this->s, "vertexPosition", 3);
	this->colors.setAttrib(this->s, "vertexColor", 3);

	this->s["P"] = glm::ortho(0.0f, (float)this->window.width, (float)this->window.height, 0.0f, -0.1f, 10.0f);
	this->s["V"] = glm::mat4(1.0f);
	this->s["M"] = glm::mat4(1.0f);

	this->s["noColor"] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	this->s["size"] = 5.0f;
}

Circles::~Circles() {
	
}

void Circles::update(double time) {
	running = running && this->window.getKey(GLFW_KEY_ESCAPE) == GLFW_RELEASE;
	
	this->window.clearScreen();

	this->s.use();
	this->points.bind();
	this->colors.bind();
	this->points.drawArrays(GL_POINTS);
}

int main(int argc, char **argv) {
	try {
		Circles game;
		game.run();
		return 0;
	}
	catch(const std::exception &exc) {
		std::cerr << exc.what() << std::endl;
	}
}
