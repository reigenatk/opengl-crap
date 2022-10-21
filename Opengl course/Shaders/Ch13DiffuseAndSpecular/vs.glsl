
#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;

void main() {
    // multiply by model matrix only to get world space coords of the fragments
    FragPos = vec3(model * vec4(pos, 1.0f));

    // use the normal matrix to map the local space normals to world space
    Normal = mat3(transpose(inverse(model))) * normals;


	gl_Position = projection * view * model * vec4(pos, 1.0f);
}
