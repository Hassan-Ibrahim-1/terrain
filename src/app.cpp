#include "app.hpp"
#include "utils.hpp"

void App::init() {
    scene.add_point_light(&light);
    light.position.y = 5;

    Rect bounds;
    bounds.transform.scale = {
        10,
        1,
        10
    };
    terrain.generate(90000, bounds);
    terrain.gobj.transform.position = {
        -13,
        -5,
        0
    };
    terrain.gobj.material.shininess = 1;
    scene.add_game_object(&terrain.gobj);
}

void App::update() {
    if (engine::cursor_enabled) {
        utils::imgui_point_light("light", light);
        utils::imgui_game_object("terrain", terrain.gobj);

        // Terrain config
    }
}

void App::cleanup() {

}

