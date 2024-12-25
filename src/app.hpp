#include "engine.hpp"
#include "terrain.hpp"

class App : public Application {
public:
    void init() override;
    void update() override;
    void cleanup() override;

    PointLight& light = *new PointLight;

    // Scene without water rect
    Framebuffer reflection_fb = Framebuffer(window.width(), window.height());
    Framebuffer refraction_fb = Framebuffer(window.width(), window.height());
    Rect& reflection_rect = *new Rect;
    Rect& refraction_rect = *new Rect;
    glm::vec4 reflection_clip_plane;
    glm::vec4 refraction_clip_plane;

    Texture2D flow_map;
    Texture2D normal_map;

    Shader water_shader;
    
    Shader terrain_shader;
    Terrain terrain;
    glm::vec3 terrain_pos;
    Rect& water_rect = *new Rect;

    float ground_boundary = 0.0;
    Color ground_color = Color(99, 63, 39);
    Color grass_color = Color(111, 168, 32);
    Color water_color = Color(43, 142, 204);

    int nvertices = 100000;

    void update_water_rect();
    void create_reflection_texture();
    void create_refraction_texture();
    void set_water_rect_textures();
    void send_pointlight_data();

    float move_factor = 0.0f;
    float wave_speed = 0.00009f;
    float reflection_strength = 2.0f;
};

