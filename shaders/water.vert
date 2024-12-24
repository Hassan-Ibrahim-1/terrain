#version 410 core

layout (location = 0) in vec3 a_position;
// layout (location = 1) in vec3 a_normal;
// layout (location = 2) in vec2 a_tex_coords;

out vec4 clip_space;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main() {
    clip_space = projection * view * model * vec4(a_position, 1.0f);
    gl_Position = clip_space;
}

