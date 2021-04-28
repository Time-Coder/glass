#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 dir_light_mat;
uniform mat4 model_mat;

void main()
{
    vec4 pos = dir_light_mat * model_mat * vec4(position, 1.0f);
 	gl_Position = pos;
}