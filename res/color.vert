#version 330 core
in vec3 vertexPosition;

uniform mat4 P;

void main() {
  gl_Position = P * vec4(vertexPosition, 1);
}
