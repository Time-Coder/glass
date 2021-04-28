#version 330 core

out vec4 frag_color;
in vec2 tex_coord;

uniform sampler2D depth_map;

void main()
{
    frag_color = vec4(vec3(texture(depth_map, tex_coord).r), 1.0);
    // frag_color = vec4(1,0,0,1);
}