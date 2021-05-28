#include <cmath>

#include "glass/Uniform"
#include "glass/common.h"
#include "glass/Camera"
#include "glass/DirLight"
#include "glass/samplerCube"

#include "glass/utils/transform.h"
#include "glass/utils/helper.h"

using namespace glass;

Camera::Camera(uint _screen_width, uint _screen_height, double _z_near, double _z_far, double _fov) : 
__screen_width(_screen_width),
__screen_height(_screen_height),
__z_near(_z_near),
__z_far(_z_far),
__fov(_fov)
{
	head_light.close();
	head_light.setBrightness(0.05);
	head_light.setCoverage(70);
	head_light.setCutoffAngle(PI/10);
	update_mat();
}

mat4 Camera::getLightMat(DirLight* dir_light, double cut1, double cut2)
{
	double k = tan_fov2*tan_fov2 * (1 + __aspect*__aspect);
	double z_center = std::min(cut2, 0.5 * (cut1 + cut2) * (1 + k));
	double R = sqrt(pow(cut2-z_center, 2) + cut2*cut2*k);

	vec4 center = dir_light->__direction_mat * inv_view_mat * vec4(0, 0, -z_center, 1);
	return glass::ortho(center.x - R, center.x + R,
                       center.y - R, center.y + R,
                       center.z - R, center.z + R + 10) * dir_light->__direction_mat;
}

void Camera::addDirLight(DirLight& dir_light)
{
	dir_lights.push_back(&dir_light);
	if(!dir_light.usingShadow())
	{
		return;
	}

	dir_light.__mat[0] = getLightMat(&dir_light, cut[0], cut[1]);
	dir_light.__mat[1] = getLightMat(&dir_light, cut[1], cut[2]);
	dir_light.__mat[2] = getLightMat(&dir_light, cut[2], cut[3]);
	dir_light.__mat[3] = getLightMat(&dir_light, cut[3], cut[4]);
	dir_light.sync(dir_light.__mat);

	dir_light.binded_camera = true;
}

void Camera::update_dir_lights_mat()
{
	if(dir_lights.empty())
	{
		return;
	}

	for(DirLight* dir_light : dir_lights)
	{
		if(!(dir_light->usingShadow()))
		{
			continue;
		}

		dir_light->__mat[0] = getLightMat(dir_light, cut[0], cut[1]);
		dir_light->__mat[1] = getLightMat(dir_light, cut[1], cut[2]);
		dir_light->__mat[2] = getLightMat(dir_light, cut[2], cut[3]);
		dir_light->__mat[3] = getLightMat(dir_light, cut[3], cut[4]);
		dir_light->sync(dir_light->__mat);
	}
}

void Camera::screenOffset(float dx, float dy)
{
	__screen_offset_x += dx;
	__screen_offset_y += dy;
	update_screen_mat();
	__mat = __screen_mat * __projection_mat * __view_mat;
	sync();
}

void Camera::screenZoom(float s)
{
	__screen_zoom_x *= s;
	__screen_zoom_y *= s;
	update_screen_mat();
	__mat = __screen_mat * __projection_mat * __view_mat;
	sync();
}

void Camera::screenZoom(float sx, float sy)
{
	__screen_zoom_x *= sx;
	__screen_zoom_y *= sy;
	update_screen_mat();
	__mat = __screen_mat * __projection_mat * __view_mat;
	sync();
}

vec2 Camera::screenOffset()const
{
	return vec2(__screen_offset_x, __screen_offset_y);
}

vec2 Camera::screenZoom()const
{
	return vec2(__screen_zoom_x, __screen_zoom_y);
}

uint Camera::screenWidth()const
{
	return __screen_width;
}

uint Camera::screenHeight()const
{
	return __screen_height;
}

double Camera::aspect()const
{
	return __aspect;
}

double Camera::zNear()const
{
	return __z_near;
}

double Camera::zFar()const
{
	return __z_far;
}

double Camera::fov()const
{
	return __fov;
}

void Camera::screenResize(uint width, uint height)
{
	__screen_width = width;
	__screen_height = height;
	update_mat();
}

void Camera::setZNear(double _z_near)
{
	__z_near = _z_near;
	update_mat();
}

void Camera::setZFar(double _z_far)
{
	__z_far = _z_far;
	update_mat();
}

void Camera::setFov(double _fov)
{
	__fov = _fov;
	update_mat();
}

void Camera::move(float dx, float dy, float dz)
{
	__position += vec3(dx, dy, dz);
	update_mat();
}

void Camera::move(const vec3& v)
{
	__position += v;
	update_mat();
}

void Camera::moveTo(float x, float y, float z)
{
	__position = vec3(x, y, z);
	update_mat();
}

void Camera::moveTo(const vec3& v)
{
	__position = v;
	update_mat();
}

vec3 Camera::position()const
{
	return __position;
}

void Camera::setPosition(const vec3& v)
{
	__position = v;
	update_mat();
}

void Camera::setPosition(float x, float y, float z)
{
	__position = vec3(x, y, z);
	update_mat();
}

void Camera::moveX(float dx)
{
	__position.x += dx;
	update_mat();
}

void Camera::moveY(float dy)
{
	__position.y += dy;
	update_mat();
}

void Camera::moveZ(float dz)
{
	__position.z += dz;
	update_mat();
}

void Camera::moveXTo(float x)
{
	__position.x = x;
	update_mat();
}

void Camera::moveYTo(float y)
{
	__position.y = y;
	update_mat();
}

void Camera::moveZTo(float z)
{
	__position.z = z;
	update_mat();
}

void Camera::moveForward(float d)
{
	__position += d * front();
	update_mat();
}

void Camera::moveBack(float d)
{
	__position += d * back();
	update_mat();
}

void Camera::moveLeft(float d)
{
	__position += d * left();
	update_mat();
}

void Camera::moveRight(float d)
{
	__position += d * right();
	update_mat();
}

void Camera::moveUp(float d)
{
	__position += d * up();
	update_mat();
}

void Camera::moveDown(float d)
{
	__position += d * down();
	update_mat();
}

void Camera::yaw(float dyaw)
{
	__yaw += dyaw;
	update_mat();
}

void Camera::pitch(float dpitch)
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
	update_mat();
}

void Camera::roll(float droll)
{
	__roll += droll;
	update_mat();
}

float Camera::yaw()const
{
	return __yaw;
}

float Camera::pitch()const
{
	return __pitch;
}

float Camera::roll()const
{
	return __roll;
}

void Camera::yawTo(float _yaw)
{
	__yaw = _yaw;
	update_mat();
}

void Camera::pitchTo(float _pitch)
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
	update_mat();
}

void Camera::rollTo(float _roll)
{
	__roll = _roll;
	update_mat();
}

void Camera::lookAt(float xt, float yt, float zt)
{
	float dx = xt - __position.x;
	float dy = yt - __position.y;
	float dz = zt - __position.z;

	yawTo(-atan2(dz, dx)-PI/2);
	pitchTo( atan(dy/sqrt(dx*dx+dz*dz)) );
}

void Camera::lookAt(const vec3& target)
{
	float dx = target.x - __position.x;
	float dy = target.y - __position.y;
	float dz = target.z - __position.z;

	yawTo(-atan2(dz, dx)-PI/2);
	pitchTo( atan(dy/sqrt(dx*dx+dz*dz)) );
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

	head_light.moveTo(__position);
	head_light.yawTo(__yaw);
	head_light.pitchTo(__pitch);

	sync();
}

void Camera::update_screen_mat()
{
	__screen_mat = glass::translate(-2.0*__screen_offset_x/__screen_width, 2.0*__screen_offset_y/__screen_height, 0) * glass::zoom(__screen_zoom_x, __screen_zoom_y, 1.0f);
}

void Camera::update_rotation_mat()
{
	__rotation_mat = Rz(-__roll)*Rx(__pitch)*Ry(__yaw);
}

void Camera::update_view_mat()
{
	update_rotation_mat();
	__view_mat = __rotation_mat * translate(__position);
	inv_view_mat = translate(-__position) * Ry(-__yaw) * Rx(-__pitch) * Rz(__roll);
}

void Camera::update_projection_mat()
{
	__aspect = 1.0 * __screen_width / __screen_height;
	tan_fov2 = tan(0.5 * __fov);
	update_cut();
	__projection_mat = perspective(__fov, __aspect, __z_near, __z_far);
}

void Camera::update_cut()
{
	int n = 4;
	double lambda = 0.4;

	for(int i = 0; i <= n; i++)
	{
		double process = 1.0*i/n;
		cut[i] = lambda * __z_near * pow(__z_far / __z_near, process) + (1 - lambda) * (__z_near + (__z_far - __z_near)*process);
	}
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