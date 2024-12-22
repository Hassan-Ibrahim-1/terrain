#version 410 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
// layout (location = 2) in vec2 a_tex_coord;

out vec3 normal;
out vec3 frag_pos; // fragment position
out vec3 color;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

uniform mat4 model; // converts vectors to world_space
uniform mat3 inverse_model;

// Ground and grass
// Grass is above
uniform float ground_boundary;
uniform vec3 ground_color;
uniform vec3 grass_color;

void main() {
    vec3 position = (model * vec4(a_position, 1)).xyz;

    gl_Position = projection * view * vec4(position, 1);
    frag_pos = vec3(model * vec4(a_position, 1.0f));
    normal = normalize(inverse_model * a_normal);

    if (position.y > ground_boundary) {
        color = grass_color;
    }
    else {
        color = ground_color;
    }
    // color = vec3(1);

    // normal = (inverse_view * a_normal);
    // normal = normalize(inverse_view * a_normal);
    // normal = normalize(mat3(transpose(inverse(model))) * a_normal);
}

