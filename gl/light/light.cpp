#include <cmath>

#include "light.h"
#include "str.h"
#include "type.h"
#include "transform.hpp"

using namespace glass;

PointLight::PointLight()
{
	coverage(32);
}

void PointLight::coverage(float d)
{
	__coverage = fabs(d);
	Kl = 3.651720188286232 / pow(__coverage - 1.379181323137789, 0.956790970458513);
	Kq = 27.101525310782399 / pow(__coverage - 2.191989674193149, 1.727016118197271);
}

float PointLight::coverage()const
{
	return __coverage;
}

vec3 PointLight::position()const
{
	return __position;
}

void PointLight::position(const vec3& v)
{
	__position = v;
}

void PointLight::position(float x, float y, float z)
{
	__position = vec3(x, y, z);
}

void PointLight::move(float dx, float dy, float dz)
{
	__position += vec3(dx, dy, dz);
}

void PointLight::move(const vec3& v)
{
	__position += v;
}

void PointLight::move_to(float x, float y, float z)
{
	__position = vec3(x, y, z);
}

void PointLight::move_to(const vec3& v)
{
	__position = v;
}

void PointLight::move_x(float dx)
{
	__position.x += dx;
}

void PointLight::move_y(float dy)
{
	__position.y += dy;
}

void PointLight::move_z(float dz)
{
	__position.z += dz;
}

void PointLight::move_x_to(float x)
{
	__position.x = x;
}

void PointLight::move_y_to(float y)
{
	__position.y = y;
}

void PointLight::move_z_to(float z)
{
	__position.z = z;
}

DirLight::DirLight()
{
	update_internal();
}

void DirLight::update_internal()
{
	__direction = vec3(-cos(__pitch)*sin(__yaw),
                        sin(__pitch),
		               -cos(__pitch)*cos(__yaw));
	__direction_mat = tran::Rx(__pitch) * tran::Ry(__yaw);
}

vec3 DirLight::direction()const
{
	return __direction;
}

void DirLight::direction(const vec3& new_direction)
{
	__direction = normalize(new_direction);
	__yaw = atan2(-__direction.x, -__direction.z);
	__pitch = atan(__direction.y/sqrt(__direction.x*__direction.x + __direction.z*__direction.z));
	__direction_mat = tran::Rx(__pitch) * tran::Ry(__yaw);
}

void DirLight::direction(float x, float y, float z)
{
	__direction = normalize(vec3(x, y, z));
	__yaw = atan2(-__direction.x, -__direction.z);
	__pitch = atan(__direction.y/sqrt(__direction.x*__direction.x + __direction.z*__direction.z));
	__direction_mat = tran::Rx(__pitch) * tran::Ry(__yaw);
}

void DirLight::yaw(float dyaw)
{
	__yaw += dyaw;
	update_internal();
}

void DirLight::pitch(float dpitch)
{
	__pitch += dpitch;
	if(__pitch > pi/2-1E-6)
	{
		__pitch = pi/2-1E-6;
	}
	else if(__pitch < -pi/2+1E-6)
	{
		__pitch = -pi/2+1E-6;
	}

	update_internal();
}

float DirLight::yaw()const
{
	return __yaw;
}

float DirLight::pitch()const
{
	return __pitch;
}

void DirLight::yaw_to(float _yaw)
{
	__yaw = _yaw;
	update_internal();
}

void DirLight::pitch_to(float _pitch)
{
	__pitch = _pitch;
	if(__pitch > pi/2-1E-6)
	{
		__pitch = pi/2-1E-6;
	}
	else if(__pitch < -pi/2+1E-6)
	{
		__pitch = -pi/2+1E-6;
	}
	
	update_internal();
}

mat4 DirLight::mat()const
{
	return __mat;
}