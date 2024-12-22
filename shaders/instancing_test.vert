#version 410 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 a_offset;

out vec3 color;

// uniform vec2 offsets[100];
//
void main() {
    vec2 pos = a_position * (gl_InstanceID / 100.0);
    gl_Position = vec4(pos + a_offset, 0, 1);
    color = a_color;
}

