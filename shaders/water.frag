#version 410 core

out vec4 FragColor;

in vec4 clip_space;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    vec3 color;
};

uniform Material material;

void main() {
    // div by 2 and add 0.5 to convert ndc to screen space coords
    vec2 ndc = clip_space.xy / clip_space.w / 2.0 + 0.5;

    vec4 reflection_color = texture(material.texture_diffuse1, vec2(ndc.x, -ndc.y));
    vec4 refraction_color = texture(material.texture_diffuse2, ndc);

    FragColor = vec4(material.color, 1) * mix(reflection_color, refraction_color, 0.5);
    // FragColor = texture(material.texture_diffuse2, ndc);

    // FragColor = vec4(material.color, 1) * texture(material.texture_diffuse1, ndc);
    // FragColor = vec4(1, 0, 0, 1);
}

