#version 460 core

layout (location = 4) out vec3 skybox_color;

in vec3 tex_coord;

uniform samplerCube skybox_map;

void main()
{    
    skybox_color = texture(skybox_map, tex_coord).rgb;
}