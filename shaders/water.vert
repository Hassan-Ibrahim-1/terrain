#version 410 core

layout (location = 0) in vec3 a_position;
// layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coords;

out vec4 clip_space;
out vec2 tex_coords;
out vec3 to_camera;
out vec3 frag_pos;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

uniform mat4 model;
uniform vec3 camera_position;

const float tiling = 6.0;

void main() {
    vec4 position = model * vec4(a_position, 1.0f);
    clip_space = projection * view * position;
    gl_Position = clip_space;
    tex_coords = a_tex_coords * tiling;

    to_camera = vec3(position);
    frag_pos = vec3(position);
}

