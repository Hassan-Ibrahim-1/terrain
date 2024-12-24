#version 410 core

out vec4 FragColor;

in vec2 tex_coords;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    vec3 color;
};

uniform Material material;

void main() {
    vec4 reflection_color = texture(material.texture_diffuse1, tex_coords);
    vec4 refraction_color = texture(material.texture_diffuse2, tex_coords);


    FragColor = mix(reflection_color, refraction_color, 0.5);
    FragColor = texture(material.texture_diffuse2, tex_coords);

    // FragColor = vec4(material.color, 1) * texture(material.texture_diffuse1, tex_coords);
    // FragColor = vec4(1, 0, 0, 1);
}

