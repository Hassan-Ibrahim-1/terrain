#version 410 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
// layout (location = 2) in vec2 a_tex_coord;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

out vec3 normal;

uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(a_position, 1.0f);
    normal = normalize(mat3(transpose(inverse(model))) * a_normal);
}


