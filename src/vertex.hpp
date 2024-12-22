#pragma once

#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position = glm::vec3(0);
    glm::vec3 normal = glm::vec3(0);
    glm::vec2 tex_coords = glm::vec2(0);

    Vertex() = default;
    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 tex_coords);
    Vertex(float px, float py, float pz, float nx, float ny, float nz, float tx, float ty)
        : Vertex(glm::vec3(px, py, pz), glm::vec3(nx, ny, nz), glm::vec2(tx, ty)) {}
};

