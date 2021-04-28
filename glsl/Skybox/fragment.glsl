#version 330 core
out vec4 frag_color;

in vec3 tex_coord;

struct Material
{
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
    float shininess;

    sampler2D reflection_map;
    sampler2D diffuse_map;
    sampler2D specular_map;
    sampler2D bump_map;
    sampler2D normal_map;

    bool using_reflection_map;
    bool using_diffuse_map;
    bool using_specular_map;
    bool using_bump_map;
    bool using_normal_map;
};

uniform samplerCube skybox_map;
uniform Material material;

void main()
{    
    // frag_color = vec4(tex_coord, 1.0f);
    frag_color = texture(skybox_map, tex_coord);
}