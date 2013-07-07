#version 330 core
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main() {
  mat4 MVP = P * V * M;
  gl_Position = MVP * vec4(vertexPosition, 1);
  //gl_Position = vertexPosition;
}