#include <cstddef>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include "renderer.hpp"
#include "debug.hpp"
#include "draw_command.hpp"
#include "engine.hpp"
#include "fs.hpp"

Renderer::Renderer(Camera& main_camera, Scene& scene)
    : main_camera(&main_camera), main_scene(&scene) {
    generate_circle_vertices();
    init_models();
    init_vbos();
    init_vaos();
    init_shaders();
    init_ubos();
    init_screen_rect();
}

Renderer::~Renderer() {
    glDeleteBuffers(1, &_points_vbo);
    glDeleteBuffers(1, &_rects_vbo);
    glDeleteVertexArrays(1, &_points_vao);
    glDeleteVertexArrays(1, &_rects_vao);
}

void Renderer::draw_point(const Point& point) {
    _points.push_back(point);
}

void Renderer::draw_point(glm::vec3 position, Color color) {
    _points.push_back(Point(position, color.clamped_vec4()));
}

void Renderer::draw_line(Line& line) {
    _line_points.push_back(line.p1);
    _line_points.push_back(line.p2);
}

void Renderer::draw_line(Point p1, Point p2) {
    Line line(p1, p2);
    line.set_color(p1.color);
    _line_points.push_back(p1);
    _line_points.push_back(p2);
}

void Renderer::add_shader(Shader& shader) {
    _user_shaders.push_back(&shader);
}

void Renderer::send_matrices_to_shader(Shader& shader) {
    uint index = glGetUniformBlockIndex(shaders.point.ID, "Matrices");
    glUniformBlockBinding(shaders.point.ID, index, 0);
}

void Renderer::reload_shaders() {
    LOG("Reloading shaders");

    /*shaders.rect.reload();*/
    /*shaders.circle.reload();*/
    /*shaders.cube.reload();*/
    /*shaders.sphere.reload();*/
    /**/
    /*shaders.textured_rect.reload();*/
    /*shaders.textured_cube.reload();*/
    /*shaders.textured_sphere.reload();*/

    shaders.point.reload();
    shaders.line.reload();
    shaders.basic_mesh.reload();
    shaders.basic_textured_mesh.reload();
    shaders.light_mesh.reload();
    shaders.light_textured_mesh.reload();
    shaders.skybox.reload();
    shaders.depth.reload();
    shaders.screen.reload();

    for (Shader* shader : _user_shaders) {
        shader->reload();
    }
}

void Renderer::set_matrices(const glm::mat4& view, const glm::mat4& projection) {
    glBindBuffer(GL_UNIFORM_BUFFER, _matrices_ubo);
    // Set this in the same order as declared in the shader
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    shaders.skybox.use();
    // the casting removes the translation part of the matrix
    // causing the skybox to not move.
    shaders.skybox.set_mat4("view", glm::mat4(glm::mat3(view)));
    shaders.skybox.set_mat4("projection", projection);
}

void Renderer::update_matrices() {
    set_matrices(
        main_camera->get_view_matrix(),
        main_camera->get_perspective_matrix()
    );
}

void Renderer::start_frame() {
    ASSERT(main_camera != nullptr, "Renderer::main_camera is a nullptr");
    ASSERT(main_scene != nullptr, "Renderer::main_scene is a nullptr");
    _n_draw_calls = 0;

    clear_screen();

    if (draw_as_hud) {
        set_matrices(glm::mat4(1), glm::mat4(1));
    }
    else {
        set_matrices(
            main_camera->get_view_matrix(),
            main_camera->get_perspective_matrix()
        );
    }

    update_vbos();

    if (depth_test_enabled) {
        glEnable(GL_DEPTH_TEST);
    }
    if (stencil_test_enabled) {
        glEnable(GL_STENCIL_TEST);
    }
    if (depth_view_enabled) {
        shaders.depth.use();
        shaders.depth.set_float("near", main_camera->near);
        shaders.depth.set_float("far", main_camera->far);
    }
    if (wireframe_enabled) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void Renderer::render() {
    ASSERT(main_camera != nullptr, "Renderer::main_camera is a nullptr");
    ASSERT(main_scene != nullptr, "Renderer::main_scene is a nullptr");

    if (_user_framebuffer.has_value()) {
        ASSERT(_user_framebuffer.value()->is_complete(), "incomplete user framebuffer");
        // Gets unbound at the end of each frame
        _user_framebuffer.value()->bind();

        // Probably bad to have this here
        clear_screen();
        glEnable(GL_DEPTH_TEST);
    }

    // ** RENDER CALLS **
    render_points();
    render_lines();
    render_game_objects();
    render_lights();

    // Draw skybox after everything else has been drawn
    // skybox depth is always 1 in ndc
    if (main_scene->has_skybox()) {
        render_skybox(main_scene->get_skybox());
    }

    if (_user_framebuffer.has_value()) {
        _user_framebuffer.value()->unbind();
        /*render_framebuffer(*_user_framebuffer.value());*/
    }
}

void Renderer::end_frame() {
    _points.clear();
    _line_points.clear();
}

void Renderer::clear_screen() {
    auto color = engine::clear_color.clamped_vec3();
    glClearColor(color.r, color.g, color.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

uint Renderer::rect_vao() {
    return _rects_vao;
}
uint Renderer::circle_vao() {
    return _circles_vao;
}
uint Renderer::cube_vao() {
    return _cubes_vao;
}
uint Renderer::sphere_vao() {
    ASSERT(_sphere_model.loaded(), "Renderer::_sphere_model not loaded");
    return _sphere_model.meshes.front().vao();
}

const DrawCommand& Renderer::sphere_mesh_draw_command() {
    ASSERT(_sphere_model.loaded(), "Sphere model not loaded");
    return _sphere_model.meshes.front().draw_command;
}

void Renderer::render_points() {
    shaders.point.use();
    glBindVertexArray(_points_vao);
    glDrawArrays(GL_POINTS, 0, _points.size());
    _n_draw_calls++;
}

void Renderer::render_lines() {
    Shader* shader;
    if (depth_view_enabled) {
        shader = &shaders.depth;
    }
    else {
        shader = &shaders.line;
    }
    shader->use();
    glBindVertexArray(_lines_vao);
    glDrawArrays(GL_LINES, 0, _line_points.size());
    _n_draw_calls++;
}

void Renderer::render_game_objects() {
    send_light_data(shaders.light_mesh);
    send_light_data(shaders.light_textured_mesh);

    for (GameObject* obj : main_scene->game_objects) {
        if (obj->hidden) {
            continue;
        }
        Shader* shader = nullptr;
        glm::mat4 model = obj->transform.get_mat4();
        // TODO: FIX THIS NESTING
        if (obj->material.shader) {
            shader = obj->material.shader.value();
            shader->use();
        }
        else {
            // Depth
            if (depth_view_enabled) {
                shader = &shaders.depth;
                shader->use();
            }
            // Textures
            else if (obj->material.has_diffuse_textures()) {
            // TODO: this probably isn't right - should check for other textures?
                if (!engine::get_scene().has_lights()) {
                    shader = &shaders.basic_textured_mesh;
                    shader->use();
                }
                // Only do lighting stuff if needed
                // Still do texture stuff afterwards tho
                else {
                    shader = &shaders.light_textured_mesh;
                    shader->use();

                    shader->set_float("material.shininess", obj->material.shininess);
                    shader->set_mat3(
                        "inverse_model",
                        glm::transpose(
                            glm::inverse(
                                model
                            )
                        )
                    );
                    /*send_light_data(*shader);*/
                }
                send_texture_data(obj->material, *shader);
            }
            // Textureless Lighting
            else {
                if (!engine::get_scene().has_lights()) {
                    shader = &shaders.basic_mesh;
                    shader->use();
                }
                else {
                    shader = &shaders.light_mesh;
                    shader->use();

                    shader->set_float("material.shininess", obj->material.shininess);
                    shader->set_mat3(
                        "inverse_model",
                        glm::transpose(
                            glm::inverse(
                                model
                            )
                        )
                    );
                    /*send_light_data(*shader);*/
                }
            }
        }
        shader->set_mat4("model", model);
        shader->set_vec3("material.color", obj->material.color.clamped_vec3());


        for (auto& mesh : obj->meshes) {
            render_mesh(mesh);
        }
    }
}

void Renderer::render_lights() {
    Scene& scene = engine::get_scene();
    Shader& shader = depth_view_enabled ? shaders.depth : shaders.basic_mesh;
    Transform sphere_transform;
    sphere_transform.scale = glm::vec3(0.1f);

    shader.use();

    // Point lights
    for (uint i = 0; i < scene.point_lights_used(); i++) {
        auto& light = *scene.point_lights[i];
        if (light.hidden) {
            continue;
        }
        sphere_transform.position = light.position;
        glm::mat4 model = sphere_transform.get_mat4();
        shader.set_mat4("model", model);
        shader.set_vec3("material.color", light.diffuse.clamped_vec3());
        render_mesh(_sphere_model.meshes.front());
    }

    // Spot lights
    Mesh square_pyramids_mesh;
    square_pyramids_mesh.set_vao(_square_pyramids_vao);
    square_pyramids_mesh.draw_command = {
        DrawCommandType::DRAW_ELEMENTS,
        DrawCommandMode::TRIANGLES,
        18
    };

    Transform square_pyramid_transform;
    square_pyramid_transform.scale = glm::vec3(0.2f);

    square_pyramid_transform.rotation.roll = 0;

    for (uint i = 0; i < scene.spot_lights_used(); i++) {
        auto& light = *scene.spot_lights[i];
        if (light.hidden) {
            continue;
        }
        square_pyramid_transform.position = light.position;

        glm::vec3 dir = glm::normalize(light.direction);
        square_pyramid_transform.rotation.yaw = glm::degrees(glm::atan(dir.x, dir.z));
        // HACK: -90 to make pitch look downward when direction is 0, -1, 0 and other stuff
        square_pyramid_transform.rotation.pitch = glm::degrees(glm::asin(-dir.y)) - 90;

        glm::mat4 model = square_pyramid_transform.get_mat4();
        shader.set_mat4("model", model);
        shader.set_vec3("material.color", light.diffuse.clamped_vec3());
        render_mesh(square_pyramids_mesh);
    }
}

void Renderer::render_skybox(Skybox& skybox) {
    ASSERT(skybox.loaded(), "skybox not loaded");

    if (wireframe_enabled) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    shaders.skybox.use();
    glActiveTexture(GL_TEXTURE0);
    shaders.skybox.set_int("skybox", 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.get_texture_id());

    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(_cubes_vao);

    Mesh mesh;
    mesh.draw_command = Cube::cube_draw_command;
    mesh.set_vao(_cubes_vao);
    render_mesh(mesh);

    glDepthFunc(GL_LESS);

    if (wireframe_enabled) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}

void Renderer::render_framebuffer(Framebuffer& fb, std::optional<Texture2D*> tex) {
    ASSERT(fb.is_complete(), "incomplete framebuffer provided");

    Texture2D* texture;

    if (tex) {
        texture = tex.value();
    }
    else {
        // _screen_rect only has one texture
        // that being the contents of whatever framebuffer is being rendered
        if (_screen_rect.material.diffuse_texture_count() > 0) {
            texture = &_screen_rect.material.diffuse_textures.front();
        }
        else {
            auto& t = _screen_rect.material.create_diffuse_texture();
            t = Texture2D(fb.get_color_attachment_by_index(0));
            texture = &t;
        }
    }
    
    // HACK: only renders the first color attachment
    // probably shouldn't be like this if there are multiple color attachments
     *texture = Texture2D(fb.get_color_attachment_by_index(0));

    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glActiveTexture(GL_TEXTURE0);
    texture->bind();
    shaders.screen.use();
    shaders.screen.set_int("screen_texture", 0);
    /*LOG("error: %u", glGetError());*/
    /*render_mesh(_screen_rect.meshes.front());*/

    if (depth_test_enabled) {
        glEnable(GL_DEPTH_TEST);
    }
    if (wireframe_enabled) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void Renderer::render_mesh(const Mesh& mesh) {
    glBindVertexArray(mesh.vao());
    uint mode = draw_command_utils::draw_command_mode_to_gl_mode(mesh.draw_command.mode);
    switch (mesh.draw_command.type) {
    case DrawCommandType::DRAW_ARRAYS:
        glDrawArrays(mode, 0, mesh.draw_command.vertex_count);
        break;
    case DrawCommandType::DRAW_ELEMENTS:
        glDrawElements(mode, mesh.draw_command.vertex_count, GL_UNSIGNED_INT, 0);
        break;
    case DrawCommandType::DRAW_ARRAYS_INSTANCED:
        glDrawArraysInstanced(
            mode,
            0,
            mesh.draw_command.vertex_count,
            mesh.draw_command.instance_count
        );
        break;
    case DrawCommandType::DRAW_ELEMENTS_INSTANCED:
        glDrawElementsInstanced(
            mode,
            mesh.draw_command.vertex_count,
            GL_UNSIGNED_INT,
            0,
            mesh.draw_command.instance_count
        );
        break;
    default:
        ERROR("Invalid draw command");
        break;

    }
    /*if (mesh.draw_command.type == DrawCommandType::DRAW_ARRAYS) {*/
    /*    glDrawArrays(mode, 0, mesh.draw_command.vertex_count);*/
    /*}*/
    /*else if (mesh.draw_command.type == DrawCommandType::DRAW_ELEMENTS) {*/
    /*    glDrawElements(mode, mesh.draw_command.vertex_count, GL_UNSIGNED_INT, 0);*/
    /*}*/
    glBindVertexArray(0);
    _n_draw_calls++;
}

void Renderer::init_models() {
    if (!_sphere_model.loaded()) {
        _sphere_model.load("models/sphere/sphere.obj");
    }
}

void Renderer::init_screen_rect() {
    Mesh& mesh = _screen_rect.meshes.front();
    mesh.vertices = {
        {1.f, 1.f, 0.0f,    0.0f, 0.0f, 1.0f,   1.0f, 1.0f}, // top right
        {1.f, -1.f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f}, // bottom right
        {-1.f, -1.f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f}, // bottom left
        {-1.f, 1.f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f}  // top left

    };
    mesh.indices = {
        0, 1, 3,
        1, 2, 3
    };
    mesh.create_buffers();
}

void Renderer::init_vbos() {
    glGenBuffers(1, &_points_vbo);
    glGenBuffers(1, &_rects_vbo);
    glGenBuffers(1, &_rects_vbo);
    glGenBuffers(1, &_circles_vbo);
    glGenBuffers(1, &_lines_vbo);
    glGenBuffers(1, &_cubes_vbo);
    glGenBuffers(1, &_square_pyramids_vbo);
}

void Renderer::init_vaos() {
    // NOTE: Do not add shaders that don't take in Matrices. Causes GL_INVALID_VALUE

    // points
    glGenVertexArrays(1, &_points_vao);
    glBindVertexArray(_points_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * _points.size(), _points.data(), GL_DYNAMIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Point), (void*)0);
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Point), (void*)offsetof(Point, color));
    glEnableVertexAttribArray(1);

    // Rects
    glGenVertexArrays(1, &_rects_vao);
    glBindVertexArray(_rects_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _rects_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _rect_vertices.size(),
                 _rect_vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &_rects_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rects_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * _rect_indices.size(),
                 _rect_indices.data(), GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 8, (void*)0);
    glEnableVertexAttribArray(0);

    // Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(float) * 8, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    // Texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(float) * 8, (void*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    // circles
    glGenVertexArrays(1, &_circles_vao);
    glBindVertexArray(_circles_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _circles_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _circle_vertices.size(), _circle_vertices.data(), GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    // NOTE: Circles don't send any normal or texture coordinates rn

    // Lines
    glGenVertexArrays(1, &_lines_vao);
    glBindVertexArray(_lines_vao);

    // data is sent every frame
    glBindBuffer(GL_ARRAY_BUFFER, _lines_vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Point), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Point), (void*)offsetof(Point, color));
    glEnableVertexAttribArray(1);

    // Cubes
    glGenVertexArrays(1, &_cubes_vao);
    glBindVertexArray(_cubes_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _cubes_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _cube_vertices.size(), _cube_vertices.data(), GL_STATIC_DRAW);

    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 8, (void*)0);
    glEnableVertexAttribArray(0);

    // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(float) * 8, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    // Texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(float) * 8, (void*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    // Square pyramids
    glGenVertexArrays(1, &_square_pyramids_vao);
    glBindVertexArray(_square_pyramids_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _square_pyramids_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _square_pyramid_vertices.size(), _square_pyramid_vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &_square_pyramids_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _square_pyramids_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * _square_pyramid_indices.size(),
                 _square_pyramid_indices.data(), GL_STATIC_DRAW);
    
    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 6, (void*)0);
    glEnableVertexAttribArray(0);

    // normals
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(float) * 6, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);
}

void Renderer::init_ubos() {
    glGenBuffers(1, &_matrices_ubo);

    // Only contains the projection and view matrices, in that order
    glBindBuffer(GL_UNIFORM_BUFFER, _matrices_ubo);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, _matrices_ubo, 0, 2 * sizeof(glm::mat4));

    // Bind shaders to _matrices_ubo
    uint index = glGetUniformBlockIndex(shaders.point.ID, "Matrices");
    glUniformBlockBinding(shaders.point.ID, index, 0);

    index = glGetUniformBlockIndex(shaders.line.ID, "Matrices");
    glUniformBlockBinding(shaders.line.ID, index, 0);

    index = glGetUniformBlockIndex(shaders.basic_mesh.ID, "Matrices");
    glUniformBlockBinding(shaders.basic_mesh.ID, index, 0);

    index = glGetUniformBlockIndex(shaders.basic_textured_mesh.ID, "Matrices");
    glUniformBlockBinding(shaders.basic_textured_mesh.ID, index, 0);

    index = glGetUniformBlockIndex(shaders.light_mesh.ID, "Matrices");
    glUniformBlockBinding(shaders.light_mesh.ID, index, 0);

    index = glGetUniformBlockIndex(shaders.light_textured_mesh.ID, "Matrices");
    glUniformBlockBinding(shaders.light_textured_mesh.ID, index, 0);

    // No skybox because it needs a specular view matrix

    index = glGetUniformBlockIndex(shaders.depth.ID, "Matrices");
    glUniformBlockBinding(shaders.depth.ID, index, 0);

    /*index = glGetUniformBlockIndex(shaders.screen.ID, "Matrices");*/
    /*glUniformBlockBinding(shaders.screen.ID, index, 0);*/
}

void Renderer::update_vbos() {
    glBindBuffer(GL_ARRAY_BUFFER, _lines_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * _line_points.size(), _line_points.data(), GL_DYNAMIC_DRAW);
    glBindVertexArray(_points_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * _points.size(), _points.data(), GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
}

void Renderer::init_shaders() {
    /*shaders.rect.load(fs::shader_path("rect.vert"), fs::shader_path("rect.frag"));*/
    /*shaders.circle.load(fs::shader_path("circle.vert"), fs::shader_path("circle.frag"));*/
    /*shaders.cube.load(fs::shader_path("cube.vert"), fs::shader_path("cube.frag"));*/
    /*shaders.sphere.load(fs::shader_path("sphere.vert"), fs::shader_path("sphere.frag"));*/
    /**/
    /*shaders.textured_rect.load(fs::shader_path("textured_rect.vert"),*/
    /*                           fs::shader_path("textured_rect.frag"));*/
    /*shaders.textured_cube.load(fs::shader_path("textured_cube.vert"),*/
    /*                           fs::shader_path("textured_cube.frag"));*/
    /*shaders.textured_sphere.load(fs::shader_path("textured_sphere.vert"),*/
    /*                             fs::shader_path("textured_sphere.frag"));*/

    shaders.point.load(fs::shader_path("point.vert"), fs::shader_path("point.frag"));
    shaders.line.load(fs::shader_path("line.vert"), fs::shader_path("line.frag"));
    shaders.basic_mesh.load(fs::shader_path("basic_mesh.vert"), fs::shader_path("basic_mesh.frag"));
    shaders.basic_textured_mesh.load(
        fs::shader_path("basic_textured_mesh.vert"),
        fs::shader_path("basic_textured_mesh.frag")
    );
    shaders.light_mesh.load(fs::shader_path("light_mesh.vert"), fs::shader_path("light_mesh.frag"));
    shaders.light_textured_mesh.load(
        fs::shader_path("light_textured_mesh.vert"),
        fs::shader_path("light_textured_mesh.frag")
    );
    shaders.skybox.load(
        fs::shader_path("skybox.vert"),
        fs::shader_path("skybox.frag")
    );
    shaders.depth.load(fs::shader_path("depth.vert"), fs::shader_path("depth.frag"));

    shaders.screen.load(
        fs::shader_path("screen.vert"),
        fs::shader_path("screen.frag")
    );
}

void Renderer::send_light_data(Shader& shader) {
    Scene& scene = engine::get_scene();
    shader.use();

    shader.set_uint("n_point_lights_used", scene.point_lights_used());
    shader.set_uint("n_spot_lights_used", scene.spot_lights_used());
    shader.set_uint("n_dir_lights_used", scene.dir_lights_used());
    shader.set_vec3("view_pos", main_camera->transform.position);

    for (size_t i = 0; i < scene.point_lights_used(); i++) {
        auto& light = scene.point_lights[i];
        std::string name = "point_lights[" + std::to_string(i) + "]";
        light->send_to_shader(name, shader);
    }
    for (size_t i = 0; i < scene.spot_lights_used(); i++) {
        auto& light = scene.spot_lights[i];
        std::string name = "spot_lights[" + std::to_string(i) + "]";
        light->send_to_shader(name, shader);
    }
    for (size_t i = 0; i < scene.dir_lights_used(); i++) {
        auto& light = scene.directional_lights[i];
        std::string name = "dir_lights[" + std::to_string(i) + "]";
        light->send_to_shader(name, shader);
    }
}

void Renderer::send_texture_data(Material& mat, Shader& shader) {
    shader.use();
    for (int i = 0; i < mat.diffuse_texture_count(); i++) {
        // TODO: is this GL_TEXTURE0 and GL_TEXTURE1 stuff right?
        // doesn't that just get overwritten on the iteration?
        glActiveTexture(GL_TEXTURE0 + i);
        mat.diffuse_textures[i].bind();
        shader.set_int(
            "material.texture_diffuse" + std::to_string(i + 1),
            i
        );
    }
    for (int i = 0; i < mat.specular_texture_count(); i++) {
        // is this okay?
        // loading both specular and diffuse textures with the same active texture
        glActiveTexture(GL_TEXTURE0 + i);
        mat.specular_textures[i].bind();
        shader.set_int(
            "material.texture_specular" + std::to_string(i + 1),
            i
        );
    }
}

void Renderer::send_texture_data(Texture2D& texture, Shader& shader, const std::string& name, int bind_location) {
    shader.use();
    glActiveTexture(GL_TEXTURE0 + bind_location);
    texture.bind();
    shader.set_int(
        name,
        bind_location
    );
}

void Renderer::generate_circle_vertices() {
    static constexpr float two_pi = M_PI * 2;
    static constexpr float radius = 0.5f;

    for (size_t i = 0; i < _n_circle_segments; i++) {
        // x, y, z
        _circle_vertices.push_back(_circle_start.x + radius * cos((float) i / _n_circle_segments * two_pi));
        _circle_vertices.push_back(_circle_start.y + radius * sin((float) i / _n_circle_segments * two_pi));
        _circle_vertices.push_back(0);
    }
}

