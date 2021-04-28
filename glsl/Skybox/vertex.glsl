#version 330 core

layout (location = 0) in vec3 position;

out vec3 tex_coord;

uniform mat4 projection_mat;
uniform mat4 rotation_mat;

void main()
{
    tex_coord = position;
    vec4 pos = projection_mat * rotation_mat * vec4(position, 1.0);
    gl_Position = pos.xyww;
}