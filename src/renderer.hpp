#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "camera.hpp"
#include "common.hpp"
#include "framebuffer.hpp"
#include "game_object.hpp"
#include "model.hpp"
#include "point.hpp"
#include "scene.hpp"
#include "shader.hpp"

enum class DrawMode {
    FILL,
    LINE,
};

class Renderer {
public:

    /* NOTE:
     * When adding a new shader. first add it to the shaders struct
     * then add it to: init_shaders, reload_shaders & init_ubo
     */

   /*
    * When creating a custom shader:
    * call add_to_shader and send_matricies_to_shader if needed
    * if sending light data then call send_light_data for the shader each frame
    * set material.shininess yourself if needed
    * also set inverse_model yourself if needed
    */

    // ** STATE **
    bool depth_view_enabled = false;
    bool depth_test_enabled = true;
    bool stencil_test_enabled = false;
    bool wireframe_enabled = false;
    bool draw_as_hud = false;

    struct Shaders {
        Shaders() = default;

        Shader point;
        Shader line;
        Shader basic_mesh;
        Shader basic_textured_mesh;
        Shader light_mesh;
        Shader light_textured_mesh;

        Shader skybox;
        Shader depth;

        Shader screen;

        // TODO: lighting shaders
    } shaders;

    // Camera used to get projection and view matrices
    Camera* main_camera;
    Scene* main_scene;

    // NOTE: when writing shaders don't use a generic name like texture1
    // this clashes with other shades if they have the same name when setting uniforms

    Renderer(Camera& main_camera, Scene& scene);
    ~Renderer();

    void draw_point(const Point& point);
    void draw_point(glm::vec3 position, Color color);
    void draw_line(Line& line);
    void draw_line(Point p1, Point p2);
    // NOTE: immediately renders the mesh
    // assumes a shader is in use
    void render_mesh(const Mesh& mesh);

    // Adds shaders to a user shaders vectors so that it
    // can be reloaded by calling Renderer::reload_shaders.
    // doesn't do anything else with the shader
    void add_shader(Shader& shader);
    void send_matrices_to_shader(Shader& shader);
    void reload_shaders();
    void send_light_data(Shader& shader);

    // call this if you update camera position mid frame
    // or else the view matrix will stay the same
    void update_matrices();
    void set_matrices(const glm::mat4& view, const glm::mat4& projection);

    // Actually render all draw calls
    void start_frame();
    void render();
    void end_frame();

    uint rect_vao();
    uint circle_vao();
    uint cube_vao();
    uint sphere_vao();
    const DrawCommand& sphere_mesh_draw_command();

    uint draw_calls_count() const { return _n_draw_calls; };

    void set_framebuffer(Framebuffer& fb) { _user_framebuffer = &fb; }
    void reset_framebuffer() { _user_framebuffer = std::nullopt; }
    bool has_active_framebuffer() const { return _user_framebuffer.has_value(); }

    void clear_screen();

    // renders a framebuffer to a rect
    // doesn't reset current framebuffer
    // if a user framebuffer is set, it renders to that
    // if none is set than it renders to the screen
    // doesn't bind or unbind any framebuffers
    void render_framebuffer(Framebuffer& fb, std::optional<Texture2D*> texture = std::nullopt);

private:
    std::optional<Framebuffer*> _user_framebuffer;

    uint _n_draw_calls;

    std::vector<Point> _points;
    std::vector<Point> _line_points;

    Model _sphere_model;

    std::vector<Shader*> _user_shaders;

    uint _points_vao;
    uint _points_vbo;

    uint _rects_vao;
    uint _rects_vbo;
    uint _rects_ebo;

    uint _circles_vao;
    uint _circles_vbo;

    uint _lines_vao;
    uint _lines_vbo;

    uint _cubes_vao;
    uint _cubes_vbo;

    uint _square_pyramids_vao;
    uint _square_pyramids_vbo;
    uint _square_pyramids_ebo;

    uint _matrices_ubo;

    // framebuffers are rendered to this rect
    Rect _screen_rect;

    static constexpr std::array<float, 32> _rect_vertices = {
        // positions              // normals           // texture coords
        0.5f, 0.5f, 0.0f,        0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,       0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,      0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f,   0.0f, 1.0f  // top left
    };

    static constexpr std::array<uint, 6> _rect_indices = {
        0, 1, 3,
        1, 2, 3 
    };

    constexpr static std::array<float, 504> _cube_vertices = {
         // positions         // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    static constexpr std::array<float, 30> _square_pyramid_vertices = {
        // Positions             // Normals
        -1.0f, 0.0f, -1.0f,     0.0f, -1.0f, 0.0f,  // V0 (base - bottom-left)
         1.0f, 0.0f, -1.0f,     0.0f, -1.0f, 0.0f,  // V1 (base - bottom-right)
         1.0f, 0.0f,  1.0f,     0.0f, -1.0f, 0.0f,  // V2 (base - top-right)
        -1.0f, 0.0f,  1.0f,     0.0f, -1.0f, 0.0f,  // V3 (base - top-left)
         0.0f, 1.0f,  0.0f,     0.0f,  1.0f, 0.0f   // V4 (apex)
    };

    static constexpr std::array<uint, 18> _square_pyramid_indices = {
        // Base triangles
        0, 1, 2,  // Triangle 1 (V0, V1, V2)
        0, 2, 3,  // Triangle 2 (V0, V2, V3)
        
        // Side triangles
        4, 0, 1,  // Triangle 3 (V4, V0, V1)
        4, 1, 2,  // Triangle 4 (V4, V1, V2)
        4, 2, 3,  // Triangle 5 (V4, V2, V3)
        4, 3, 0   // Triangle 6 (V4, V3, V0)
    };

    glm::vec2 _circle_start = glm::vec2(0.0);
    std::vector<float> _circle_vertices;
    int _n_circle_segments = 150;

    void generate_circle_vertices();

    void push_point(float x, float y, float z);
    void push_point_color(glm::vec4& color);

    void init_models();

    void init_vbos();
    void init_vaos();
    void init_ubos();
    void update_vbos();

    void init_shaders();

    void init_screen_rect();

    void render_points();
    void render_lines();
    void render_game_objects();
    void render_lights();
    // NOTE: this function changes glDepthFunc every frame if a skybox
    // is set should probably just go back to the previous
    // depth function instead
    void render_skybox(Skybox& skybox);
};

