#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 texturecoord;

// texture samplers (set from CPU code)
uniform sampler2D texture1;


void main()
{

	FragColor = texture(texture1, texturecoord) * vec4(color, 1.0f);
}