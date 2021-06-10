#version 430 core
layout (location = 0) in vec3 position;

uniform mat4 model_mat;

void main()
{
    gl_Position = model_mat * vec4(position, 1.0);
}