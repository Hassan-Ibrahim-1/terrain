#version 410 core

layout (location = 0) in vec3 a_position;
// layout (location = 1) in vec4 a_color;

// out vec4 color;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

uniform mat4 model; // converts vectors to world_space

void main() {
    // vec4 pos = projection * view * model * vec4(a_position, 1.0f);
    // pos.z = 0.0f;
    gl_Position = projection * view * model * vec4(a_position, 1);
    // color = a_color;
    gl_PointSize = gl_Position.z;
}

