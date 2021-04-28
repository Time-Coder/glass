#include <cmath>

#include "camera.h"
#include "transform.hpp"
#include "light.h"

using namespace tran;

Camera::Camera(uint _screen_width, uint _screen_height, double _z_near, double _z_far, double _fov) : 
__screen_width(_screen_width),
__screen_height(_screen_height),
__z_near(_z_near),
__z_far(_z_far),
__fov(_fov),
corner_points(8)
{
	update_mat();
}

void Camera::addDirLight(DirLight& dir_light)
{
	dir_light.using_shadow = true;
	dir_lights.push_back(&dir_light);

	double ratio = 1.0 * __screen_width / __screen_height;
	double tan_fov2 = tan(__fov/2.0);
	double ratio_tan_fov2 = ratio * tan_fov2;
	
	corner_points[0] = vec4(__z_near * vec3(-ratio_tan_fov2, -tan_fov2, -1), 1);
	corner_points[1] = vec4(__z_near * vec3( ratio_tan_fov2, -tan_fov2, -1), 1);
	corner_points[2] = vec4(__z_near * vec3( ratio_tan_fov2,  tan_fov2, -1), 1);
	corner_points[3] = vec4(__z_near * vec3(-ratio_tan_fov2,  tan_fov2, -1), 1);
	corner_points[4] = vec4(__z_far  * vec3(-ratio_tan_fov2, -tan_fov2, -1), 1);
	corner_points[5] = vec4(__z_far  * vec3( ratio_tan_fov2, -tan_fov2, -1), 1);
	corner_points[6] = vec4(__z_far  * vec3( ratio_tan_fov2,  tan_fov2, -1), 1);
	corner_points[7] = vec4(__z_far  * vec3(-ratio_tan_fov2,  tan_fov2, -1), 1);
	
	mat4 inv_view_mat = tran::translate(-__position) * Ry(-__yaw) * Rx(-__pitch) * Rz(__roll);
	for(int i = 0; i < corner_points.size(); i++)
	{
		corner_points[i] = inv_view_mat * corner_points[i];
	}

	float x_min, x_max, y_min, y_max, z_min, z_max;
	for(int i = 0; i < corner_points.size(); i++)
	{
		vec4 point_light = dir_light.__direction_mat * corner_points[i];
		if(i == 0)
		{
			x_min = x_max = point_light.x;
			y_min = y_max = point_light.y;
			z_min = z_max = point_light.z;
		}
		else
		{
			if(point_light.x < x_min) x_min = point_light.x;
			else if(point_light.x > x_max) x_max = point_light.x;
			
			if(point_light.y < y_min) y_min = point_light.y;
			else if(point_light.y > y_max) y_max = point_light.y;
			
			if(point_light.z < z_min) z_min = point_light.z;
			else if(point_light.z > z_max) z_max = point_light.z;
		}
	}
	z_max += 10;
	dir_light.__mat = tran::ortho(x_min, x_max, y_min, y_max, z_min, z_max) * dir_light.__direction_mat;
}

void Camera::update_dir_lights_mat()
{
	if(dir_lights.empty())
	{
		return;
	}

	double ratio = 1.0 * __screen_width / __screen_height;
	double tan_fov2 = tan(__fov/2.0);
	double ratio_tan_fov2 = ratio * tan_fov2;
	
	corner_points[0] = vec4(__z_near * vec3(-ratio_tan_fov2, -tan_fov2, -1), 1);
	corner_points[1] = vec4(__z_near * vec3( ratio_tan_fov2, -tan_fov2, -1), 1);
	corner_points[2] = vec4(__z_near * vec3( ratio_tan_fov2,  tan_fov2, -1), 1);
	corner_points[3] = vec4(__z_near * vec3(-ratio_tan_fov2,  tan_fov2, -1), 1);
	corner_points[4] = vec4(__z_far  * vec3(-ratio_tan_fov2, -tan_fov2, -1), 1);
	corner_points[5] = vec4(__z_far  * vec3( ratio_tan_fov2, -tan_fov2, -1), 1);
	corner_points[6] = vec4(__z_far  * vec3( ratio_tan_fov2,  tan_fov2, -1), 1);
	corner_points[7] = vec4(__z_far  * vec3(-ratio_tan_fov2,  tan_fov2, -1), 1);
	
	mat4 inv_view_mat = translate(-__position) * Ry(-__yaw) * Rx(-__pitch) * Rz(__roll);

	for(int i = 0; i < corner_points.size(); i++)
	{
		corner_points[i] = inv_view_mat * corner_points[i];
	}

	for(DirLight* dir_light : dir_lights)
	{
		float x_min, x_max, y_min, y_max, z_min, z_max;
		for(int i = 0; i < corner_points.size(); i++)
		{
			vec4 point_light = dir_light->__direction_mat * corner_points[i];
			if(i == 0)
			{
				x_min = (x_max = point_light.x);
				y_min = (y_max = point_light.y);
				z_min = (z_max = point_light.z);
			}
			else
			{
				if(point_light.x < x_min) x_min = point_light.x;
				else if(point_light.x > x_max) x_max = point_light.x;
				
				if(point_light.y < y_min) y_min = point_light.y;
				else if(point_light.y > y_max) y_max = point_light.y;
				
				if(point_light.z < z_min) z_min = point_light.z;
				else if(point_light.z > z_max) z_max = point_light.z;
			}
		}
		z_max += 10;
		dir_light->__mat = tran::ortho(x_min, x_max, y_min, y_max, z_min, z_max) * dir_light->__direction_mat;
	}
}

void Camera::screen_offset(double dx, double dy)
{
	__screen_offset_x += dx;
	__screen_offset_y += dy;
	update_screen_mat();
	__mat = __screen_mat * __projection_mat * __view_mat;
}

void Camera::screen_zoom(double s)
{
	__screen_zoom_x *= s;
	__screen_zoom_y *= s;
	update_screen_mat();
	__mat = __screen_mat * __projection_mat * __view_mat;
}

void Camera::screen_zoom(double sx, double sy)
{
	__screen_zoom_x *= sx;
	__screen_zoom_y *= sy;
	update_screen_mat();
	__mat = __screen_mat * __projection_mat * __view_mat;
}

vec2 Camera::screen_offset()const
{
	return vec2(__screen_offset_x, __screen_offset_y);
}

vec2 Camera::screen_zoom()const
{
	return vec2(__screen_zoom_x, __screen_zoom_y);
}

uint Camera::screen_width()const
{
	return __screen_width;
}

uint Camera::screen_height()const
{
	return __screen_height;
}

float Camera::aspect()const
{
	return 1.0*__screen_width/__screen_height;
}

double Camera::z_near()const
{
	return __z_near;
}

double Camera::z_far()const
{
	return __z_far;
}

double Camera::fov()const
{
	return __fov;
}

void Camera::screen_size(uint width, uint height)
{
	__screen_width = width;
	__screen_height = height;
	update_mat();
}

void Camera::z_near(double _z_near)
{
	__z_near = _z_near;
	update_mat();
}

void Camera::z_far(double _z_far)
{
	__z_far = _z_far;
	update_mat();
}

void Camera::fov(double _fov)
{
	__fov = _fov;
	update_mat();
}

void Camera::move(double dx, double dy, double dz)
{
	__position += vec3(dx, dy, dz);
	update_mat();
}

void Camera::move(const vec3& v)
{
	__position += v;
	update_mat();
}

void Camera::move_to(double x, double y, double z)
{
	__position = vec3(x, y, z);
	update_mat();
}

void Camera::move_to(const vec3& v)
{
	__position = v;
	update_mat();
}

vec3 Camera::position()const
{
	return __position;
}

void Camera::position(const vec3& v)
{
	__position = v;
	update_mat();
}

void Camera::position(double x, double y, double z)
{
	__position = vec3(x, y, z);
	update_mat();
}

void Camera::move_x(double dx)
{
	__position.x += dx;
	update_mat();
}

void Camera::move_y(double dy)
{
	__position.y += dy;
	update_mat();
}

void Camera::move_z(double dz)
{
	__position.z += dz;
	update_mat();
}

void Camera::move_x_to(double x)
{
	__position.x = x;
	update_mat();
}

void Camera::move_y_to(double y)
{
	__position.y = y;
	update_mat();
}

void Camera::move_z_to(double z)
{
	__position.z = z;
	update_mat();
}

void Camera::move_forward(double d)
{
	__position += d * front();
	update_mat();
}

void Camera::move_back(double d)
{
	__position += d * back();
	update_mat();
}

void Camera::move_left(double d)
{
	__position += d * left();
	update_mat();
}

void Camera::move_right(double d)
{
	__position += d * right();
	update_mat();
}

void Camera::move_up(double d)
{
	__position += d * up();
	update_mat();
}

void Camera::move_down(double d)
{
	__position += d * down();
	update_mat();
}

void Camera::yaw(double dyaw)
{
	__yaw += dyaw;
	update_mat();
}

void Camera::pitch(double dpitch)
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
	update_mat();
}

void Camera::roll(double droll)
{
	__roll += droll;
	update_mat();
}

double Camera::yaw()const
{
	return __yaw;
}

double Camera::pitch()const
{
	return __pitch;
}

double Camera::roll()const
{
	return __roll;
}

void Camera::yaw_to(double _yaw)
{
	__yaw = _yaw;
	update_mat();
}

void Camera::pitch_to(double _pitch)
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
	update_mat();
}

void Camera::roll_to(double _roll)
{
	__roll = _roll;
	update_mat();
}

void Camera::look_at(double xt, double yt, double zt)
{
	double dx = xt - __position.x;
	double dy = yt - __position.y;
	double dz = zt - __position.z;

	yaw_to(-atan2(dz, dx)-pi/2);
	pitch_to( atan(dy/sqrt(dx*dx+dz*dz)) );

	update_mat();
}

mat4 Camera::mat()const
{
	return __mat;
}

mat4 Camera::screen_mat()const
{
	return __screen_mat;
}

mat4 Camera::projection_mat()const
{
	return __projection_mat;
}

mat4 Camera::view_mat()const
{
	return __view_mat;
}

mat4 Camera::rotation_mat()const
{
	return __rotation_mat;
}

void Camera::update_mat()
{
	update_screen_mat();
	update_projection_mat();
	update_view_mat();
	__mat = __screen_mat * __projection_mat * __view_mat;

	update_dir_lights_mat();
}

void Camera::update_screen_mat()
{
	__screen_mat = tran::translate(-2.0*__screen_offset_x/__screen_width, 2.0*__screen_offset_y/__screen_height, 0) * tran::zoom(__screen_zoom_x, __screen_zoom_y, 1.0f);
}

void Camera::update_rotation_mat()
{
	__rotation_mat = Rz(-__roll)*Rx(__pitch)*Ry(__yaw);
}

void Camera::update_view_mat()
{
	update_rotation_mat();
	__view_mat = __rotation_mat * translate(__position.x, __position.y, __position.z);
}

void Camera::update_projection_mat()
{
	__projection_mat = perspective(__fov, (double)__screen_width/__screen_height, __z_near, __z_far);
}

vec3 Camera::left()const
{
	return mat3(Ry(-__yaw) * Rx(-__pitch) * Rz(__roll)) * vec3(-1, 0, 0);
}

vec3 Camera::right()const
{
	return mat3(Ry(-__yaw) * Rx(-__pitch) * Rz(__roll)) * vec3(1, 0, 0);
}

vec3 Camera::front()const
{
	return mat3(Ry(-__yaw) * Rx(-__pitch) * Rz(__roll)) * vec3(0, 0, -1);
}

vec3 Camera::back()const
{
	return mat3(Ry(-__yaw) * Rx(-__pitch) * Rz(__roll)) * vec3(0, 0, 1);
}

vec3 Camera::up()const
{
	return mat3(Ry(-__yaw) * Rx(-__pitch) * Rz(__roll)) * vec3(0, 1, 0);
}

vec3 Camera::down()const
{
	return mat3(Ry(-__yaw) * Rx(-__pitch) * Rz(__roll)) * vec3(0, -1, 0);
}

void Camera::reset()
{
	__position = vec3();
	__yaw = 0.0, __pitch = 0.0, __roll = 0.0;
	__screen_zoom_x = 1.0, __screen_zoom_y = 1.0;
	__screen_offset_x = 0.0, __screen_offset_y = 0.0;
	
	update_mat();
}

vec4 Camera::projection(const vec4& v)const
{
	vec4 result = __mat * v;
	return result /= result.w;
}