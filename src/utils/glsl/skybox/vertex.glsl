#version 430 core
layout (location = 0) in vec3 position;

out vec3 tex_coord;

#include "../common/utils.glsl"

uniform Camera camera;

void main()
{
    tex_coord = position;
    vec4 pos = camera.projection_mat * camera.rotation_mat * vec4(position, 1.0);
    gl_Position = pos.xyww;
}