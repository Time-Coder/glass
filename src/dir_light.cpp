#include <cmath>

#include "glass/Uniform"
#include "glass/common.h"
#include "glass/DirLight"
#include "glass/samplerCube"
#include "glass/utils/transform.h"
#include "glass/utils/helper.h"

DirLight::DirLight()
{
	update_internal();
}

void DirLight::update_internal()
{
	__direction = vec3(-cos(__pitch)*sin(__yaw),
                        sin(__pitch),
		               -cos(__pitch)*cos(__yaw));
	__direction_mat = glass::Rx(__pitch) * glass::Ry(__yaw);
	sync();
}

vec3 DirLight::direction()const
{
	return __direction;
}

void DirLight::setDirection(const vec3& new_direction)
{
	__direction = normalize(new_direction);
	__yaw = atan2(-__direction.x, -__direction.z);
	__pitch = atan(__direction.y/sqrt(__direction.x*__direction.x + __direction.z*__direction.z));
	__direction_mat = glass::Rx(__pitch) * glass::Ry(__yaw);
	sync();
}

void DirLight::setDirection(float x, float y, float z)
{
	__direction = normalize(vec3(x, y, z));
	__yaw = atan2(-__direction.x, -__direction.z);
	__pitch = atan(__direction.y/sqrt(__direction.x*__direction.x + __direction.z*__direction.z));
	__direction_mat = glass::Rx(__pitch) * glass::Ry(__yaw);
	sync();
}

void DirLight::yaw(float dyaw)
{
	__yaw += dyaw;
	update_internal();
}

void DirLight::pitch(float dpitch)
{
	__pitch += dpitch;
	if(__pitch > PI/2-1E-6)
	{
		__pitch = PI/2-1E-6;
	}
	else if(__pitch < -PI/2+1E-6)
	{
		__pitch = -PI/2+1E-6;
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

void DirLight::yawTo(float _yaw)
{
	__yaw = _yaw;
	update_internal();
}

void DirLight::pitchTo(float _pitch)
{
	__pitch = _pitch;
	if(__pitch > PI/2-1E-6)
	{
		__pitch = PI/2-1E-6;
	}
	else if(__pitch < -PI/2+1E-6)
	{
		__pitch = -PI/2+1E-6;
	}
	
	update_internal();
}

mat4& DirLight::mat(uint i)
{
	if(i >= 4)
	{
		throw glass::IndexError(i, 0, 3);
	}

	return __mat[i];
}

bool DirLight::usingShadow()const
{
	return __using_shadow;
}

bool DirLight::bindedCamera()const
{
	return binded_camera;
}

void DirLight::useShadow(bool flag)
{
	__using_shadow = flag;
}

vec3 DirLight::color()const
{
	return __color;
}

void DirLight::setColor(const vec3& _color)
{
	__color = _color;
}

void DirLight::setColor(float r, float g, float b)
{
	__color = vec3(r, g, b);
}

void DirLight::open()
{
	is_open = true;
}

void DirLight::close()
{
	is_open = false;
}

bool DirLight::isOpen()const
{
	return is_open;
}

bool DirLight::isClose()const
{
	return !is_open;
}