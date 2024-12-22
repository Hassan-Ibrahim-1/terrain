#include "app.hpp"
#include "utils.hpp"

void App::init() {
    scene.add_point_light(&light);
    light.position.y = 5;

    // Weird behavior when scale is not 0
    // Terrain moves by scale.x whenever scaling up
    terrain.generate(nvertices);
    terrain.gobj.transform.scale = {
        10, 1, 10
    };
    terrain_pos = terrain.gobj.transform.position;
    terrain.gobj.material.shininess = 1;
    scene.add_game_object(&terrain.gobj);

    terrain.gobj.transform.position.x = terrain_pos.x;
    terrain.gobj.transform.position.x -= terrain.gobj.transform.scale.x * 1;
}

void App::update() {
    if (engine::cursor_enabled) {
        utils::imgui_point_light("light", light);

        utils::imgui_game_object("terrain", terrain.gobj);

        utils::imgui_color_edit3("terrain", terrain.gobj.material.color);
        if (ImGui::DragFloat3("terrain pos", (float*)&terrain_pos)) {
            terrain.gobj.transform.position.y = terrain_pos.y;
            terrain.gobj.transform.position.z = terrain_pos.z;
            terrain.gobj.transform.position.x = terrain_pos.x;
            terrain.gobj.transform.position.x -= terrain.gobj.transform.scale.x * 1;
        }
        if (ImGui::DragFloat3("terrain scale", (float*)&terrain.gobj.transform.scale)) {
            terrain.gobj.transform.position.x = terrain_pos.x;
            terrain.gobj.transform.position.x -= terrain.gobj.transform.scale.x * 1;
        }
        ImGui::DragFloat3("terrain rotation", (float*)&terrain.gobj.transform.rotation);
        ImGui::DragFloat("shine", &terrain.gobj.material.shininess);

        ImGui::Spacing();

        if (ImGui::DragInt("nvertices", &nvertices, 2, 1)) {
            terrain.generate(nvertices);
        }
    }
}

void App::cleanup() {

}

