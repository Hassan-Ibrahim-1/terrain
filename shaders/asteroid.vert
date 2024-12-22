#version 410 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coords;
layout (location = 3) in mat4 a_model;

out vec2 tex_coords;

uniform mat4 projection;
uniform mat4 view;

void main() {
    gl_Position = projection * view * a_model * vec4(a_position, 1.0f);
    tex_coords = a_tex_coords;
}
