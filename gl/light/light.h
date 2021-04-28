#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "glsl_objects.h"
#include <vector>

class Model;
struct PointLight
{
	// for glsl
	vec3 color = vec3(1.0f, 1.0f, 1.0f);

private:
	float Kl = 0.09f, Kq = 0.032f;
	vec3 __position;
	// end

	float __coverage = 50;

public:
	PointLight();

	float coverage()const;
	void coverage(float distance);

	vec3 position()const;
	void position(const vec3& v);
	void position(float x, float y, float z);
	void move(float dx, float dy, float dz);
	void move(const vec3& v);
	void move_to(float x, float y, float z);
	void move_to(const vec3& v);
	void move_x(float dx);
	void move_y(float dy);
	void move_z(float dz);
	void move_x_to(float x);
	void move_y_to(float y);
	void move_z_to(float z);
};

struct DirLight
{
	// for glsl
	vec3 color = vec3(1.0f, 1.0f, 1.0f);
	
private:
	vec3 __direction = vec3(0.0f, 0.0f, -1.0f);
	mat4 __mat; // for Camera to update
	bool using_shadow = false; // for Camera to update

public:
	sampler2D depth_map;
	// end

	float __yaw = 0.0f, __pitch = 0.0f;
	mat4 __direction_mat;

	friend class Camera;

private:
	void update_internal();

public:
	DirLight();
	vec3 direction()const;
	void direction(float x, float y, float z);
	void direction(const vec3& _direction);

	void yaw(float dpsi); // 偏航
	void pitch(float dtheta); // 俯仰

	float yaw()const; // 偏航
	float pitch()const; // 俯仰

	void yaw_to(float _psi); // 偏航
	void pitch_to(float _theta); // 俯仰

	mat4 mat()const;
};

#endif