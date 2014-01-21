#version 140
in vec2 fragUV;

uniform sampler2D tex;
uniform vec3 color;

out vec4 fragColor;

void main(void) {
	vec4 pixel = texture2D(tex, vec2(fragUV.x, fragUV.y));
    fragColor = vec4(pixel.r * color.r, pixel.g * color.g, pixel.b * color.b, pixel.a);
}
