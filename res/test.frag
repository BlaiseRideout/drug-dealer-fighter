#version 330 core
uniform vec3 color;

out vec3 vertColor;

void main() {
  vertColor = color;
}