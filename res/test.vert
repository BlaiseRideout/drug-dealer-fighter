#version 330 core
uniform mat4 MVP;

void main() {
  gl_Position = MVP * vec4(vertexPosition, 1);
}