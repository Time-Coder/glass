#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "gl_objects.h"

#include "type.h"
using namespace glass;

class DirLight;
class Camera
{
	// for glsl
	vec3 __position;
	mat4 __mat;
	mat4 __screen_mat;
	mat4 __projection_mat;
	mat4 __view_mat;
	mat4 __rotation_mat;
	// end

private:
	double __yaw = 0.0f, __pitch = 0.0f, __roll = 0.0f;
	double __screen_zoom_x = 1.0f, __screen_zoom_y = 1.0f;
	double __screen_offset_x = 0.0, __screen_offset_y = 0.0;

	double __z_near = 0.1f;
	double __z_far = 100.0f;
	double __fov = pi/4;

	uint __screen_width = 800;
	uint __screen_height = 600;

	vector<DirLight*> dir_lights;
	vector<vec4> corner_points;

private:
	void update_screen_mat();
	void update_projection_mat();
	void update_view_mat();
	void update_rotation_mat();
	void update_mat();
	void update_dir_lights_mat();

public:
	vec3 position()const;
	void position(const vec3& v);
	void position(double x, double y, double z);
	mat4 mat()const;
	mat4 screen_mat()const;
	mat4 projection_mat()const;
	mat4 view_mat()const;
	mat4 rotation_mat()const;
	vec2 screen_offset()const;
	vec2 screen_zoom()const;
	
public:
	Camera(uint _screen_width = 800, uint _screen_height = 600, double _z_near = 0.1f, double _z_far = 100.0f, double _fov = pi/4);
	uint screen_width()const;
	uint screen_height()const;
	float aspect()const;
	double z_near()const;
	double z_far()const;
	double fov()const;

	void addDirLight(DirLight& dir_light);
	void screen_size(uint width, uint height);
	void z_near(double _z_near);
	void z_far(double _z_far);
	void fov(double _fov);

	void move(double dx, double dy, double dz);
	void move(const vec3& v);
	void move_to(double x, double y, double z);
	void move_to(const vec3& v);
	void move_x(double dx);
	void move_x_to(double x);
	void move_y(double dy);
	void move_y_to(double y);
	void move_z(double dz);
	void move_z_to(double z);

	void screen_offset(double dx, double dy);
	void screen_zoom(double s);
	void screen_zoom(double sx, double sy);

	void reset();

	void move_forward(double d);
	void move_back(double d);
	void move_left(double d);
	void move_right(double d);
	void move_up(double d);
	void move_down(double d);

	void yaw(double dpsi); // 偏航
	void pitch(double dtheta); // 俯仰
	void roll(double dphi); // 滚转

	double yaw()const; // 偏航
	double pitch()const; // 俯仰
	double roll()const; // 滚转

	void yaw_to(double _psi); // 偏航
	void pitch_to(double _theta); // 俯仰
	void roll_to(double _phi); // 滚转

	void look_at(double _x, double _y, double _z);
	vec4 projection(const vec4& vec)const;

	vec3 left()const;
	vec3 right()const;
	vec3 front()const;
	vec3 back()const;
	vec3 up()const;
	vec3 down()const;
};

#endif