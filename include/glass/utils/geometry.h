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
	void generateTBN(const std::vector<vec3>& positions, const std::vector<vec4>& tex_coords, const std::vector<uvec3>& indices, std::vector<vec3>& tangents, std::vector<vec3>& bitangents, std::vector<vec3>& normals);
	void generateTBN(const std::vector<vec3>& positions, const std::vector<vec3>& normals, const std::vector<vec4>& tex_coords, const std::vector<uvec3>& indices, std::vector<vec3>& tangents, std::vector<vec3>& bitangents);
}

#endif