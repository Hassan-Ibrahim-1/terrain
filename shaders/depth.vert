#version 410 core

layout (location = 0) in vec3 a_position;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(a_position, 1.0f);
}

