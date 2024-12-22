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

    terrain_shader.load(
        "shaders/terrain.vert",
        "shaders/terrain.frag"
    );

    renderer.add_shader(terrain_shader);
    renderer.send_matrices_to_shader(terrain_shader);

    terrain.gobj.transform.position.x = terrain_pos.x;
    terrain.gobj.transform.position.x -= terrain.gobj.transform.scale.x * 1;
    terrain.gobj.material.shader = &terrain_shader;
    terrain.gobj.material.shininess = 1000;
}

void App::update() {
    if (engine::cursor_enabled) {
        ImGui::DragFloat("gr boundary", &ground_boundary, 0.01);
        utils::imgui_color_edit3("ground", ground_color);
        utils::imgui_color_edit3("grass", grass_color);

        utils::imgui_point_light("light", light);

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

    renderer.send_light_data(terrain_shader);

    terrain_shader.use();
    terrain_shader.set_float(
        "material.shininess",
        terrain.gobj.material.shininess
    );
    terrain_shader.set_mat3(
        "inverse_model",
        glm::transpose(
            glm::inverse(
                terrain.gobj.transform.get_mat4()
             )
        )
    );

    terrain_shader.set_float("ground_boundary", ground_boundary);
    terrain_shader.set_vec3("ground_color", ground_color.clamped_vec3());
    terrain_shader.set_vec3("grass_color", grass_color.clamped_vec3());
}

void App::cleanup() {

}

