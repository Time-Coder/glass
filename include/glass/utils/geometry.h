#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include "glass/Model"
#include "glass/vec"
#include "glass/utils/helper.h"

namespace glass
{
	Model cube(float a = 1.0f);
	Model sphere(float R = 1.0f, unsigned int n_theta = 200, unsigned int n_phi = 0);
	Model floor(float blocks_per_meter = 1.0f);
	Model plane();
	Model pyramid(float R, float H, uint n);
	Model pyramid(float R_bottom, float R_top, float H, uint n);
	Model regularTetrahedron(float a);
	Model cone(float R, float H, uint n = 100);
	Model cone(float R_bottom, float R_top, float H, uint n = 100);
	Model prism(float R, float H, uint n);
	Model box(float Lx, float Ly, float Lz);
	Model parallelepiped(const vec3& a, const vec3& b, const vec3& c);
	Model cylinder(float R, float H, uint n = 100);
	Model torus(float r, float R, uint n_theta = 200, uint n_phi = 0);

	void generateTBN(const std::vector<vec3>& positions, const std::vector<vec2>& tex_coords, const std::vector<uvec3>& indices, std::vector<vec3>& tangents, std::vector<vec3>& bitangents, std::vector<vec3>& normals);
	void generateTBN(const std::vector<vec3>& positions, const std::vector<vec3>& normals, const std::vector<vec2>& tex_coords, const std::vector<uvec3>& indices, std::vector<vec3>& tangents, std::vector<vec3>& bitangents);
}

#endif