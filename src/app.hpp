#include "engine.hpp"
#include "terrain.hpp"

class App : public Application {
public:
    void init() override;
    void update() override;
    void cleanup() override;

    PointLight& light = *new PointLight;
    Shader terrain_shader;
    Terrain terrain;
    glm::vec3 terrain_pos;

    float ground_boundary = 0.0;
    float water_boundary = -0.5;
    Color ground_color = Color(99, 63, 39);
    Color grass_color = Color(111, 168, 32);
    Color water_color = Color(43, 142, 204);

    int nvertices = 100000;
};

