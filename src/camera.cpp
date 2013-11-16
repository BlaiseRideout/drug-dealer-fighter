#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <iostream>



FPCamera::FPCamera(Window const &window, glm::vec3 position, glm::vec3 lookAt, float sensitivity, float speed) : position(position), sensitivity(sensitivity), speed(speed), window(window), verticalAngle(0), horizontalAngle(0) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(this->window, 0, 0);

	glm::vec3 direction = position - lookAt;

	if(glm::length(direction) > 0) {
		direction = glm::normalize(direction);
		this->horizontalAngle = atan2(direction.x, direction.z);
		direction = glm::vec3(glm::rotate(glm::mat4(1.0f), this->horizontalAngle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(direction, 1.0f));
		this->verticalAngle = atan2(direction.y, direction.z);
	}
}

FPCamera::FPCamera(Window const &window, glm::vec3 position, glm::vec3 lookAt, float sensitivity) : FPCamera(window, position, sensitivity, .05) {
}

FPCamera::FPCamera(Window const &window, glm::vec3 position, glm::vec3 lookAt) : FPCamera(window, position, 0.005) {
}

FPCamera::FPCamera(Window const &window, glm::vec3 position, float sensitivity, float speed) : FPCamera(window, position, glm::vec3(0), sensitivity, speed) {
}

FPCamera::FPCamera(Window const &window, glm::vec3 position, float sensitivity) : FPCamera(window, position, glm::vec3(0), sensitivity) {
}

FPCamera::FPCamera(Window const &window, glm::vec3 position) : FPCamera(window, position, 0.005) {
}

FPCamera::FPCamera(Window const &window, float sensitivity, float speed) : FPCamera(window, glm::vec3(0.0f), sensitivity, speed) {
}

FPCamera::FPCamera(Window const &window, float sensitivity) : FPCamera(window, glm::vec3(0.0f), sensitivity) {
}

FPCamera::FPCamera(Window const &window) : FPCamera(window, .005) {

}

void FPCamera::update() {
	double xpos = 0, ypos = 0;
	glfwGetCursorPos(this->window, &xpos, &ypos);

	horizontalAngle -= sensitivity * float(xpos);
	verticalAngle -= sensitivity * float(ypos);

	if(verticalAngle > 3.1415f / 2.0f)
		verticalAngle = 3.1415f / 2.0f;
	else if(verticalAngle < -3.1415f / 2.0f)
		verticalAngle = -3.1415f / 2.0f;

	glfwSetCursorPos(this->window, 0, 0);

	glm::vec3 direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.1415f/2.0f),
		0,
		cos(horizontalAngle - 3.1415f/2.0f)
	);

	glm::vec3 vel(0);

	if(this->window.getKey('W') != GLFW_RELEASE)
    	vel += glm::normalize(direction);
	if(this->window.getKey('S') != GLFW_RELEASE)
		vel -= glm::normalize(direction);

	if(this->window.getKey('A') != GLFW_RELEASE)
    	vel -= glm::normalize(right);
	if(this->window.getKey('D') != GLFW_RELEASE)
		vel += glm::normalize(right);

	if(glm::length(vel) > 0)
		position += glm::normalize(vel) * speed;
}

glm::mat4 FPCamera::viewMatrix() {
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	return glm::lookAt(position, position + direction, glm::vec3(0, 1, 0));
}