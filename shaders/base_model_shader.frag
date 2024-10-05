#version 410 core

out vec4 FragColor;

in vec2 tex_coords;

struct Material {
    sampler2D texture_diffuse1;
};
uniform Material material;
// uniform bool test;

void main() {
    FragColor = vec4(1, 1, 1, 1);
    // FragColor = texture(material.texture_diffuse1, tex_coords);
    // FragColor = vec4(1, 0, 0, 1);
}
