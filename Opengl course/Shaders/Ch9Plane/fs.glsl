#version 330 core
out vec4 FragColor;


in vec2 texturecoord;

// texture samplers (set from CPU code)
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(texture1, texturecoord), texture(texture2, texturecoord), 0.4);
}