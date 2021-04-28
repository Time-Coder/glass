#version 330 core

layout (location = 0) in vec4 original_position;
layout (location = 1) in vec3 norm_vec;
layout (location = 2) in vec4 base_color;

out vec3 frag_pos;
out vec3 norm;
out vec4 frag_base_color;

uniform mat4 transform_mat;
uniform mat4 model_mat;

void main()
{
	gl_Position = transform_mat * original_position;
	frag_pos = (model_mat * original_position).xyz;
	norm = normalize(mat3(model_mat) * normalize(norm_vec));
	frag_base_color = base_color;
}