#version 430 core

#include "../common/utils.glsl"

out vec4 frag_color;
in vec2 tex_coord;

uniform sampler2D test_image;

void main()
{
    frag_color = texture(test_image, tex_coord);
}