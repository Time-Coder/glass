#include <cmath>

#include "glass/Uniform"
#include "glass/common.h"
#include "glass/SpotLight"
#include "glass/samplerCube"
#include "glass/utils/transform.h"
#include "glass/utils/helper.h"

using namespace glass;

SpotLight::SpotLight()
{
	setCoverage(32);
	update_internal();
	update_mat();
}

void SpotLight::update_internal()
{
	__direction = vec3(-cos(__pitch)*sin(__yaw),
                        sin(__pitch),
		               -cos(__pitch)*cos(__yaw));

	shape.yawTo(__yaw);
	shape.pitchTo(__pitch);

	sync();
}

void SpotLight::update_mat()
{
	double epsilon = 0.03;
	double Delta = Kl*Kl - 4*Kq*(1 - __brightness*dot(vec3(0.299, 0.587, 0.114), __color)/epsilon);
	__radius = 0.5 * (sqrt(Delta) - Kl) / Kq;

	if(!__using_shadow)
	{
		shape.moveTo(__position);
		sync();
		return;
	}

	mat4 projection = perspective(PI/2, 1, 1, __radius);
	mat4 view = translate(__position);

	__mat[0] = projection * Rz(PI) * Ry(-PI/2) * view;
	__mat[1] = projection * Rz(PI) * Ry( PI/2) * view;
	__mat[2] = projection * Rx( PI/2) * view;
	__mat[3] = projection * Rx(-PI/2) * view;
	__mat[4] = projection * Rz(PI) * Ry( PI  ) * view;
	__mat[5] = projection * Rz(PI) * view;

	shape.moveTo(__position);

	sync();
}

vec3 SpotLight::direction()const
{
	return __direction;
}

void SpotLight::setDirection(const vec3& new_direction)
{
	__direction = normalize(new_direction);
	__yaw = atan2(-__direction.x, -__direction.z);
	__pitch = atan(__direction.y/sqrt(__direction.x*__direction.x + __direction.z*__direction.z));
	
	shape.yawTo(__yaw);
	shape.pitchTo(__pitch);
	
	sync();
}

void SpotLight::setDirection(float x, float y, float z)
{
	__direction = normalize(vec3(x, y, z));
	__yaw = atan2(-__direction.x, -__direction.z);
	__pitch = atan(__direction.y/sqrt(__direction.x*__direction.x + __direction.z*__direction.z));
	
	shape.yawTo(__yaw);
	shape.pitchTo(__pitch);

	sync();
}

void SpotLight::yaw(float dyaw)
{
	__yaw += dyaw;
	update_internal();
}

void SpotLight::pitch(float dpitch)
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

float SpotLight::yaw()const
{
	return __yaw;
}

float SpotLight::pitch()const
{
	return __pitch;
}

void SpotLight::yawTo(float _yaw)
{
	__yaw = _yaw;
	update_internal();
}

void SpotLight::pitchTo(float _pitch)
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

mat4& SpotLight::mat(int i)
{
	if(!__using_shadow)
	{
		throw glass::RuntimeError("using shadow is false, no mat avaiable!");
	}

	if(i < 0 || i > 5)
	{
		throw glass::IndexError(0, 5, i);
	}

	return __mat[i];
}

void SpotLight::useShadow(bool flag)
{
	__using_shadow = flag;
	update_mat();
}

void SpotLight::setCoverage(float d)
{
	__coverage = fabs(d);
	Kl = 3.651720188286232 / pow(__coverage - 1.379181323137789, 0.956790970458513);
	Kq = 27.101525310782399 / pow(__coverage - 2.191989674193149, 1.727016118197271);
	update_mat();
}

float SpotLight::coverage()const
{
	return __coverage;
}

float SpotLight::radius()const
{
	return __radius;
}

vec3 SpotLight::position()const
{
	return __position;
}

vec3 SpotLight::color()const
{
	return __color;
}

void SpotLight::setColor(const vec3& _color)
{
	__color = _color;
	update_mat();
}

void SpotLight::setColor(float r, float g, float b)
{
	setColor(vec3(r, g, b));
}

float SpotLight::brightness()const
{
	return __brightness;
}

void SpotLight::setBrightness(float _brightness)
{
	__brightness = _brightness;
	update_mat();
}

void SpotLight::setPosition(const vec3& v)
{
	__position = v;
	update_mat();
}

void SpotLight::setPosition(float x, float y, float z)
{
	__position = vec3(x, y, z);
	update_mat();
}

void SpotLight::move(float dx, float dy, float dz)
{
	__position += vec3(dx, dy, dz);
	update_mat();
}

void SpotLight::move(const vec3& v)
{
	__position += v;
	update_mat();
}

void SpotLight::moveTo(float x, float y, float z)
{
	__position = vec3(x, y, z);
	update_mat();
}

void SpotLight::moveTo(const vec3& v)
{
	__position = v;
	update_mat();
}

void SpotLight::moveX(float dx)
{
	__position.x += dx;
	update_mat();
}

void SpotLight::moveY(float dy)
{
	__position.y += dy;
	update_mat();
}

void SpotLight::moveZ(float dz)
{
	__position.z += dz;
	update_mat();
}

void SpotLight::moveXTo(float x)
{
	__position.x = x;
	update_mat();
}

void SpotLight::moveYTo(float y)
{
	__position.y = y;
	update_mat();
}

void SpotLight::moveZTo(float z)
{
	__position.z = z;
	update_mat();
}

void SpotLight::setShape(const Model& model)
{
	shape = model;
	shape.moveTo(__position);
	shape.yawTo(__yaw);
	shape.pitchTo(__pitch);
}

bool SpotLight::usingShadow()const
{
	return __using_shadow;
}

float SpotLight::cutoffAngle()const
{
	return __cutoff_angle;
}

void SpotLight::setCutoffAngle(float angle)
{
	__cutoff_angle = angle;
	sync(__cutoff_angle);
}

float SpotLight::softDistance()const
{
	return __soft_distance;
}

void SpotLight::setSoftDistance(float distance)
{
	__soft_distance = distance;
	sync(__soft_distance);
}

void SpotLight::open()
{
	is_open = true;
}

void SpotLight::close()
{
	is_open = false;
}

bool SpotLight::isOpen()const
{
	return is_open;
}

bool SpotLight::isClose()const
{
	return !is_open;
}