#ifndef __TRANSFORM_HPP__
#define __TRANSFORM_HPP__

#include "glass/mat"

namespace glass
{
	mat4 perspective(float fov, float ratio, float z_near = 0.1f, float z_far = 100.0f);
	mat4 ortho(float x_min, float x_max, float y_min, float y_max, float z_min, float z_max);
	mat4 Rx(float phi);
	mat4 Ry(float theta);
	mat4 Rz(float psi);
	mat4 axis_angle(const vec3& n, float angle);
	mat4 rotate(float phi, float theat, float psi);
	mat4 translate(float x, float y, float z);
	mat4 translate(const vec3& v);
	mat4 zoom(float sx, float sy, float sz);
	mat4 zoom(const vec3& v);
	mat4 zoom(float s);
}

#endif