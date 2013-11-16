#ifndef CAMERA_H
#define CAMERA_H

#include "window.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


class FPCamera {
	public:
		FPCamera(Window const &window, glm::vec3 position, glm::vec3 lookAt, float sensitivity, float speed);
		FPCamera(Window const &window, glm::vec3 position, glm::vec3 lookAt, float sensitivity);
		FPCamera(Window const &window, glm::vec3 position, glm::vec3 lookAt);
		FPCamera(Window const &window, glm::vec3 position, float sensitivity, float speed);
		FPCamera(Window const &window, glm::vec3 position, float sensitivity);
		FPCamera(Window const &window, glm::vec3 position);
		FPCamera(Window const &window, float sensitivity, float speed);
		FPCamera(Window const &window, float sensitivity);
		FPCamera(Window const &indow);
		void update();
		glm::mat4 viewMatrix();

		glm::vec3 position;
		float sensitivity;
		float speed;
	private:
		Window const &window;
		float verticalAngle;
		float horizontalAngle;

};

#endif