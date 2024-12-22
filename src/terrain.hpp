#pragma once

#include "common.hpp"

struct Terrain {
    Terrain() {}

    void generate(uint nvertices);

    GameObject& gobj = *new GameObject;

    float color_boundary = 0.3f;
    Color color_low = glm::vec3(0.358, 0.254, 0.037);
    Color color_high = glm::vec3(0.235, 0.863, 0.216);

};
