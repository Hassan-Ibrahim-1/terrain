#version 410 core

out vec4 FragColor;

in vec4 clip_space;
in vec2 tex_coords;
in vec3 to_camera;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    vec3 color;
};

uniform sampler2D flow_map;
uniform sampler2D water_normal;
uniform Material material;
uniform float move_factor;
uniform float reflection_strength;

const float distortion_strength = 0.01;
const float wave_strength = 0.02;

vec3 flow_uvw(vec2 uv, vec2 flow_vector, float time);

void main() {
    // div by 2 and add 0.5 to convert ndc to screen space coords
    vec2 ndc = clip_space.xy / clip_space.w / 2.0 + 0.5;

    vec3 normal = vec3(0.0f, 1.0f, 0.0f);
    vec3 view_pos = normalize(to_camera);
    float transparency = dot(view_pos, normal);
    transparency = pow(transparency, reflection_strength);

    vec2 distorted_tex_coords =
        texture(flow_map, vec2(tex_coords.x + move_factor, tex_coords.y)).rg;
    distorted_tex_coords *= distortion_strength;
    distorted_tex_coords =
        tex_coords + vec2(distorted_tex_coords.x, distorted_tex_coords.y + move_factor);

    vec2 total_distortion = texture(flow_map, distorted_tex_coords).rg * 2.0 - 1.0;
    total_distortion *= wave_strength;

    vec2 reflection_uv = vec2(ndc.x, -ndc.y) + total_distortion;
    vec2 refraction_uv = ndc + total_distortion;

    reflection_uv.x = clamp(reflection_uv.x, 0.001, 0.999);
    reflection_uv.y = clamp(reflection_uv.y, -0.999, -0.001);
    refraction_uv = clamp(refraction_uv, 0.001, 0.999);

    vec3 reflection_color = vec3(texture(material.texture_diffuse1, reflection_uv));
    vec3 refraction_color = vec3(texture(material.texture_diffuse2, refraction_uv));

    FragColor = 
        vec4(material.color, 1) * mix(vec4(reflection_color, 1), vec4(refraction_color, 1), transparency);
}

vec3 flow_uvw(vec2 uv, vec2 flow_vector, float time) {
    float shift = 0;
    float progress = fract(time + shift);
    vec2 jump = vec2(0.02, -0.02);

    vec3 uvw;
    uvw.xy = uv - flow_vector * progress;
    uvw.xy += progress - shift + (time - progress) * jump;
    uvw.z = 1 - abs(1 - 2 * progress);
    uvw.z = 1;
    return uvw;
}

