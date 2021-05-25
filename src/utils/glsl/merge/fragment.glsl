#version 460 core

#include "../common/utils.glsl"

out vec4 frag_color;
in vec2 tex_coord;

uniform sampler2D color_without_lights;
uniform sampler2D color_lights_clear;
uniform sampler2D color_lights_blur;
uniform sampler2D skybox_map;

void main()
{
	vec4 color_lights = texture0(color_lights_clear, tex_coord);
	vec4 color_clear = texture0(color_without_lights, tex_coord) + color_lights;
	float factor = 1-0.9*soft_step(length(color_lights.rgb)-0.1, 0.1);
    frag_color = color_clear + factor * texture0(color_lights_blur, tex_coord) + texture0(skybox_map, tex_coord);
}