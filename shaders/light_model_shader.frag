#version 410 core

out vec4 FragColor;

in vec3 normal;
in vec2 tex_coords;
in vec3 frag_pos;

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    float shininess;
    // sampler2D texture_diffuse1;
    // sampler2D texture_specular1;
    vec3 color;
};

vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir);

uniform Material material;
uniform PointLight pointlight;
uniform bool attenuation_enabled;
// uniform bool test;

void main() {
    vec3 norm = normalize(normal);
    vec3 view_direction = normalize(-frag_pos);
    // FragColor = texture(material.texture_diffuse1, tex_coords);
    vec3 result = calc_point_light(pointlight, norm, frag_pos, view_direction);
    // FragColor = vec4(1, 0, 0, 1);
    FragColor = vec4(result, 1);
}

vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir) {
    vec3 light_dir = normalize(light.position - frag_pos);
    // diffuse shading
    float diff = max(dot(normal, light_dir), 0.0);
    // specular shading
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0),
    material.shininess);
    /*
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1,
    tex_coords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1,
    tex_coords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1,
    tex_coords));
    */
    vec3 ambient = light.ambient * material.color;
    vec3 diffuse = light.diffuse * diff * material.color;
    vec3 specular = light.specular * spec * material.color;

    // attenuation
    if (attenuation_enabled) {
        float distance = length(light.position - frag_pos);
        float attenuation = 1.0 / (light.constant + light.linear * distance +
            light.quadratic * (distance * distance));
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
    }
    return (ambient + diffuse + specular);
}

