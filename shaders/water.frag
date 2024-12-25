#version 410 core

out vec4 FragColor;

in vec4 clip_space;
in vec2 tex_coords;
in vec3 to_camera;
in vec3 frag_pos;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    vec3 color;
    float shininess;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D flow_map;
uniform sampler2D normal_map;
uniform Material material;
uniform float move_factor;
uniform float reflection_strength;

const float distortion_strength = 0.01;
const float wave_strength = 0.02;

uniform PointLight pointlight;

vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir);

void main() {
    // div by 2 and add 0.5 to convert ndc to screen space coords
    vec2 ndc = clip_space.xy / clip_space.w / 2.0 + 0.5;

    vec2 distorted_tex_coords =
        texture(flow_map, vec2(tex_coords.x + move_factor, tex_coords.y)).rg;
    distorted_tex_coords *= distortion_strength;
    distorted_tex_coords =
        tex_coords + vec2(distorted_tex_coords.x, distorted_tex_coords.y + move_factor);

    // The normal map i'm using seems weird

    // vec3 normal = vec3(0.0f, 1.0f, 0.0f);
    vec3 norm = texture(normal_map, distorted_tex_coords).rgb;
    // vec3 normal = vec3(norm.r * 2.0 - 1.0, norm.g, norm.b * 2.0 - 1.0);
    vec3 normal = norm;
    normal = normalize(normal);
    vec3 view_pos = normalize(to_camera);
    float transparency = dot(view_pos, normal);
    transparency = pow(transparency, reflection_strength);
    transparency = clamp(transparency, 0.0001, 0.9999);

    vec2 total_distortion = texture(flow_map, distorted_tex_coords).rg * 2.0 - 1.0;
    total_distortion *= wave_strength;

    vec2 reflection_uv = vec2(ndc.x, -ndc.y) + total_distortion;
    vec2 refraction_uv = ndc + total_distortion;

    reflection_uv.x = clamp(reflection_uv.x, 0.001, 0.999);
    reflection_uv.y = clamp(reflection_uv.y, -0.999, -0.001);
    refraction_uv = clamp(refraction_uv, 0.001, 0.999);

    vec3 reflection_color = vec3(texture(material.texture_diffuse1, reflection_uv));
    vec3 refraction_color = vec3(texture(material.texture_diffuse2, refraction_uv));

    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 result = calc_point_light(pointlight, normal, frag_pos, view_pos);

    FragColor = 
        vec4(material.color, 1) * mix(vec4(reflection_color, 1), vec4(refraction_color, 1), transparency);
    FragColor.rgb += result;

    // FragColor = vec4(normal, 1);
}

vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir) {
    vec3 light_dir = normalize(light.position - frag_pos);
    // diffuse shading
    float diff = max(dot(normal, light_dir), 0.0);
    // specular shading
    vec3 half_dir = normalize(light_dir + view_dir);
    float spec = pow(max(dot(normal, half_dir), 0.0), material.shininess);

    spec = clamp(spec, 0.0001, 0.9999);

    vec3 ambient = light.ambient * material.color;
    vec3 diffuse = light.diffuse * diff * material.color;
    vec3 specular = light.specular * spec * material.color;

    // attenuation
    float distance = length(light.position - frag_pos);
    float attenuation = 1.0 / distance;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

