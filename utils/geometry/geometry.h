#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include "matrix.hpp"
#include "glsl_objects.h"
#include "camera.h"
#include "model.h"
#include "vec.h"

Model cube(double a = 1.0f);
Model sphere(double R = 1.0f, unsigned int n_theta = 200, unsigned int n_phi = 0);
Model floor(float blocks_per_meter = 1.0f);
Model plane();
void generateTBN(const vector<vec3>& positions, const vector<vec4>& tex_coords, const vector<uvec3>& indices, vector<vec3>& tangents, vector<vec3>& bitangents, vector<vec3>& normals);
void generateTBN(const vector<vec3>& positions, const vector<vec3>& normals, const vector<vec4>& tex_coords, const vector<uvec3>& indices, vector<vec3>& tangents, vector<vec3>& bitangents);

#endif