#include "engine.hpp"
#include "terrain.hpp"

class App : public Application {
public:
    void init() override;
    void update() override;
    void cleanup() override;

    PointLight& light = *new PointLight;
    Terrain terrain;
    Rect bounds;
    int nvertices = 10000;
};

