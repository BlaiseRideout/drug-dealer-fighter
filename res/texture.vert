#version 140
in vec2 vertexPosition;
in vec2 vertexUV;

uniform mat4 M;
uniform mat4 P;

out vec2 fragUV;

void main(void) {
	mat4 MP = P * M;
    gl_Position = MP * vec4(vertexPosition, 0.0, 1.0);
    fragUV = vertexUV;
}