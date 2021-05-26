#include <cmath>

#include "glass/PointLight"
#include "glass/Uniform"
#include "glass/utils/transform.h"
#include "glass/utils/helper.h"

using namespace glass;

PointLight::PointLight()
{
	setCoverage(32);
	update_mat();
}

void PointLight::update_mat()
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

mat4& PointLight::mat(int i)
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

void PointLight::useShadow(bool flag)
{
	__using_shadow = flag;
	update_mat();
}

void PointLight::setCoverage(float d)
{
	__coverage = fabs(d);
	Kl = 3.651720188286232 / pow(__coverage - 1.379181323137789, 0.956790970458513);
	Kq = 27.101525310782399 / pow(__coverage - 2.191989674193149, 1.727016118197271);
	update_mat();
}

float PointLight::coverage()const
{
	return __coverage;
}

float PointLight::radius()const
{
	return __radius;
}

vec3 PointLight::position()const
{
	return __position;
}

vec3 PointLight::color()const
{
	return __color;
}

void PointLight::setColor(const vec3& _color)
{
	__color = _color;
	shape.setMaskColor(__brightness * __color);
	update_mat();
}

void PointLight::setColor(float r, float g, float b)
{
	setColor(vec3(r, g, b));
}

float PointLight::brightness()const
{
	return __brightness;
}

void PointLight::setBrightness(float _brightness)
{
	__brightness = _brightness;
	shape.setMaskColor(__brightness * __color);
	update_mat();
}

void PointLight::setPosition(const vec3& v)
{
	__position = v;
	update_mat();
}

void PointLight::setPosition(float x, float y, float z)
{
	__position = vec3(x, y, z);
	update_mat();
}

void PointLight::move(float dx, float dy, float dz)
{
	__position += vec3(dx, dy, dz);
	update_mat();
}

void PointLight::move(const vec3& v)
{
	__position += v;
	update_mat();
}

void PointLight::moveTo(float x, float y, float z)
{
	__position = vec3(x, y, z);
	update_mat();
}

void PointLight::moveTo(const vec3& v)
{
	__position = v;
	update_mat();
}

void PointLight::moveX(float dx)
{
	__position.x += dx;
	update_mat();
}

void PointLight::moveY(float dy)
{
	__position.y += dy;
	update_mat();
}

void PointLight::moveZ(float dz)
{
	__position.z += dz;
	update_mat();
}

void PointLight::moveXTo(float x)
{
	__position.x = x;
	update_mat();
}

void PointLight::moveYTo(float y)
{
	__position.y = y;
	update_mat();
}

void PointLight::moveZTo(float z)
{
	__position.z = z;
	update_mat();
}

void PointLight::setShape(const Model& model)
{
	shape = model;
	shape.setAsLight(true);
	shape.setMaskColor(__brightness * __color);
	shape.moveTo(__position);
}

bool PointLight::usingShadow()const
{
	return __using_shadow;
}

void PointLight::open()
{
	is_open = true;
}

void PointLight::close()
{
	is_open = false;
}

bool PointLight::isOpen()const
{
	return is_open;
}

bool PointLight::isClose()const
{
	return !is_open;
}