#version 430 core

#include "../common/utils.glsl"

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

// // Instanced Array
// layout (location = 5) in mat4 model_mat;
// layout (location = 6) in mat4 normal_mat;

out VertexOut
{
	vec3 position;
	vec2 tex_coord;
	mat3 TBN;
	mat3 normalTBN;
	vec3 to_camera_tangent;
}vertex_out;

uniform Camera camera;
uniform Model model;
// uniform Material material;

void main()
{
	gl_Position = camera.mat * model.mat * vec4(position, 1.0);
	
	vertex_out.position = vec3(model.mat * vec4(position, 1.0));
	vertex_out.tex_coord = tex_coord;
	vertex_out.TBN = model.normal_mat * mat3(tangent, bitangent, normal);
	vertex_out.normalTBN = model.normal_mat * mat3(tangent/dot(tangent, tangent), bitangent/dot(bitangent, bitangent), 0.1*normal/model.material.height_factor);
	vertex_out.to_camera_tangent = inverse(vertex_out.TBN) * (camera.position - vertex_out.position);
}