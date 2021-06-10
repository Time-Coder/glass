#version 460 core

#include "../common/utils.glsl"

out vec4 frag_color;
in vec2 tex_coord;

uniform sampler2D color_map;

uniform bool horizontal;
uniform float weights[5];

void main()
{
    vec2 tex_offset = 1.0 / textureSize(color_map, 0); // gets size of single texel
    vec3 result = texture(color_map, tex_coord).rgb * weights[0]; // current fragment's contribution
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(color_map, tex_coord + vec2(tex_offset.x * i, 0.0)).rgb * weights[i];
            result += texture(color_map, tex_coord - vec2(tex_offset.x * i, 0.0)).rgb * weights[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(color_map, tex_coord + vec2(0.0, tex_offset.y * i)).rgb * weights[i];
            result += texture(color_map, tex_coord - vec2(0.0, tex_offset.y * i)).rgb * weights[i];
        }
    }
    frag_color = vec4(result, 1.0);
}