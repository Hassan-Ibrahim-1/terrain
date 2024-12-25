#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "app.hpp"
#include "debug.hpp"
#include "framebuffer.hpp"
#include "skybox.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <optional>

void App::init() {
    engine::render_after_user_update = false;

    scene.set_skybox({
        "textures/skybox-sky/right.png",
        "textures/skybox-sky/left.png",
        "textures/skybox-sky/top.png",
        "textures/skybox-sky/bottom.png",
        "textures/skybox-sky/front.png",
        "textures/skybox-sky/back.png",
    });

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
    
    water_shader.load(
        "shaders/water.vert",
        "shaders/water.frag"
    );

    renderer.add_shader(terrain_shader);
    renderer.send_matrices_to_shader(terrain_shader);

    renderer.add_shader(water_shader);
    renderer.send_matrices_to_shader(water_shader);

    terrain.gobj.transform.position.x = terrain_pos.x;
    terrain.gobj.transform.position.x -= terrain.gobj.transform.scale.x * 1;
    terrain.gobj.material.shader = &terrain_shader;
    terrain.gobj.material.shininess = 4000;

    scene.add_primitive(&water_rect);
    water_rect.transform.rotation.pitch = -90;
    water_rect.material.color = Color(167, 197, 217);
    update_water_rect();

    // Framebuffer
    ColorAttachmentCreateInfo cinfo;
    cinfo.format = GL_RGB;
    reflection_fb.create_color_attachment(cinfo);
    reflection_fb.create_rbo_attachment({});
    ASSERT(reflection_fb.is_complete(), "fbo is not complete");

    refraction_fb.create_color_attachment(cinfo);
    refraction_fb.create_rbo_attachment({});
    ASSERT(refraction_fb.is_complete(), "fbo is not complete");

    // TODO: there is no need for water boundary
    // just use y component of water_rect's transform

    scene.add_primitive(&reflection_rect);
    reflection_rect.material.create_diffuse_texture();
    scene.add_primitive(&refraction_rect);
    refraction_rect.material.create_diffuse_texture();
    refraction_rect.transform.position.x = -2;
    
    water_rect.material.create_diffuse_texture();
    water_rect.material.create_diffuse_texture();

    flow_map = Texture2D("textures/flow_map.png");
    normal_map = Texture2D("textures/water_normal2.png");
}

void App::update() {
    if (engine::cursor_enabled) {
        ImGui::DragFloat("wave speed", &wave_speed, 0.001);
        ImGui::DragFloat("refl strength", &reflection_strength, 0.1);

        utils::imgui_rect("refl rect", reflection_rect);
        utils::imgui_rect("refra rect", refraction_rect);
        utils::imgui_rect("water rect", water_rect);

        ImGui::DragFloat("gr boundary", &ground_boundary, 0.01);
        if (ImGui::DragFloat("wt boundary", &water_boundary, 0.01)) {
            update_water_rect();
        }
        utils::imgui_color_edit3("ground", ground_color);
        utils::imgui_color_edit3("grass", grass_color);
        utils::imgui_color_edit3("water", water_color);

        utils::imgui_point_light("light", light);

        utils::imgui_color_edit3("terrain", terrain.gobj.material.color);
        if (ImGui::DragFloat3("terrain pos", (float*)&terrain_pos)) {
            terrain.gobj.transform.position.y = terrain_pos.y;
            terrain.gobj.transform.position.z = terrain_pos.z;
            terrain.gobj.transform.position.x = terrain_pos.x;
            terrain.gobj.transform.position.x -= terrain.gobj.transform.scale.x * 1;
            update_water_rect();
        }
        if (ImGui::DragFloat3("terrain scale", (float*)&terrain.gobj.transform.scale)) {
            terrain.gobj.transform.position.x = terrain_pos.x;
            terrain.gobj.transform.position.x -= terrain.gobj.transform.scale.x * 1;
            update_water_rect();
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
    terrain_shader.set_float("water_boundary", water_boundary);
    terrain_shader.set_vec3("ground_color", ground_color.clamped_vec3());
    terrain_shader.set_vec3("grass_color", grass_color.clamped_vec3());
    terrain_shader.set_vec3("water_color", water_color.clamped_vec3());

    glEnable(GL_CLIP_DISTANCE0);

    water_rect.hidden = true;
    reflection_rect.hidden = true;
    refraction_rect.hidden = true;

    reflection_clip_plane = {
        0, 1, 0, -water_boundary
    };
    refraction_clip_plane = {
        0, -1, 0, water_boundary
    };

    scene.hide_skybox();
    create_reflection_texture();
    create_refraction_texture();
    scene.show_skybox();

    // this is dumb
    water_rect.material.shader = &water_shader;
    set_water_rect_textures();
    renderer.send_texture_data(water_rect.material, water_shader);
    renderer.send_texture_data(flow_map, water_shader, "flow_map", 2);
    renderer.send_texture_data(normal_map , water_shader, "normal_map", 3);
    renderer.send_light_data(water_shader);

    move_factor += wave_speed * sin(glfwGetTime());
    water_shader.set_float("move_factor", move_factor);
    water_shader.set_float("reflection_strength", reflection_strength);
    water_shader.set_vec3("camera_position", camera.transform.position);
    send_pointlight_data();

    // regular scene
    glDisable(GL_CLIP_DISTANCE0);
    water_rect.hidden = false;
    reflection_rect.hidden = false;
    refraction_rect.hidden = false;
    renderer.reset_framebuffer();
    renderer.render();
}

void App::cleanup() {
    flow_map.unload();
    normal_map.unload();
}

void App::update_water_rect() {
    water_rect.transform.scale = terrain.gobj.transform.scale;
    water_rect.transform.position =
        terrain.gobj.transform.position + terrain.gobj.transform.scale / 2.0f;
    // idk why this is 1.5
    water_rect.transform.position.x =
        terrain.gobj.transform.position.x + terrain.gobj.transform.scale.x * 1.5;
    water_rect.transform.position.y = water_boundary;
}

void App::create_reflection_texture() {
    float dist = 2 * (camera.transform.position.y - water_boundary);
    camera.transform.position.y -= dist;
    camera.invert_pitch();

    renderer.update_matrices();

    terrain_shader.use();
    terrain_shader.set_vec4("clip_plane", reflection_clip_plane);
    renderer.set_framebuffer(reflection_fb);
    renderer.render();
    renderer.render_framebuffer(reflection_fb, &reflection_rect.material.diffuse_textures.front());

    camera.transform.position.y += dist;
    camera.invert_pitch();

    renderer.update_matrices();
}

void App::create_refraction_texture() {
    terrain_shader.use();
    terrain_shader.set_vec4("clip_plane", refraction_clip_plane);
    renderer.set_framebuffer(refraction_fb);
    renderer.render();
    renderer.render_framebuffer(refraction_fb, &refraction_rect.material.diffuse_textures.front());
}

void App::set_water_rect_textures() {
    water_rect.material.diffuse_textures[0] = { reflection_rect.material.diffuse_textures.front().ID };
    water_rect.material.diffuse_textures[1] = { refraction_rect.material.diffuse_textures.front().ID };
}

void App::send_pointlight_data() {
    light.send_to_shader("pointlight", water_shader);
}

