#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 rgb;
layout (location = 2) in vec2 texturecoords;

// pass these along to fragment shader
out vec3 color;
out vec2 texturecoord;

void main() {
	gl_Position = vec4(pos, 1.0);
	color = rgb;
	texturecoord = texturecoords;
}
