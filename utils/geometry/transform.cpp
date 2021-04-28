#include <transform.hpp>
#include <cmath>

mat4 tran::perspective(float fov, float ratio, float z_near, float z_far)
{
	double cot = 1.0/tan(0.5*fov);
	return mat4(cot/ratio, 0.0, 0.0,  0.0,
		        0.0      , cot, 0.0,  0.0,
		        0.0      , 0.0, -1.0*(z_far + z_near)/(z_far - z_near), -1.0,
		        0.0      , 0.0, -2.0*z_near*z_far/(z_far - z_near),  0.0);
}

mat4 tran::ortho(float x_min, float x_max, float y_min, float y_max, float z_min, float z_max)
{
	return mat4(2.0/(x_max - x_min),            0.0,                             0.0,                            0.0,
		        0.0,                            2.0/(y_max - y_min),             0.0,                            0.0,
		        0.0,                            0.0,                            -2.0/(z_max - z_min),            0.0,
		        1.0*(x_max + x_min)/(x_min - x_max), 1.0*(y_max + y_min)/(y_min - y_max),  1.0*(z_max + z_min)/(z_max - z_min), 1.0);
}

mat4 tran::Rx(float phi)
{
	return mat4(1.0, 0.0     ,  0.0     , 0.0,
		        0.0, cos(phi), -sin(phi), 0.0,
		        0.0, sin(phi),  cos(phi), 0.0,
		        0.0, 0.0     ,  0.0     , 1.0);
}

mat4 tran::Ry(float theta)
{
	return mat4( cos(theta), 0.0, sin(theta), 0.0,
		         0.0       , 1.0, 0.0       , 0.0,
		        -sin(theta), 0.0, cos(theta), 0.0,
		         0.0       , 0.0, 0.0       , 1.0);
}

mat4 tran::Rz(float psi)
{
	return mat4(cos(psi), -sin(psi), 0.0, 0.0,
		        sin(psi),  cos(psi), 0.0, 0.0,
		        0.0     ,  0.0     , 1.0, 0.0,
		        0.0     ,  0.0     , 0.0, 1.0);
}

mat4 tran::axis_angle(const vec3& n, float angle)
{	
	double cos_angle = cos(angle);
	mat3 sub_mat = (1.0 - cos_angle)*n*transpose(mat3(n,n,n)) + mat3(cos_angle) + sin(angle)*cross(n);
	mat4 result;

	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			result[j][i] = sub_mat[j][i];
		}
	}

	return result;
}

mat4 tran::translate(float x, float y, float z)
{
	return mat4(1.0f,  0.0f,  0.0f, 0.0f,
		        0.0f,  1.0f,  0.0f, 0.0f,
		        0.0f,  0.0f,  1.0f, 0.0f,
		        -x  ,  -y  ,  -z  , 1.0f);
}

mat4 tran::translate(const vec3& v)
{
	return mat4(1.0f,  0.0f,  0.0f, 0.0f,
		        0.0f,  1.0f,  0.0f, 0.0f,
		        0.0f,  0.0f,  1.0f, 0.0f,
		        -v.x,  -v.y,  -v.z, 1.0f);
}

mat4 tran::zoom(float sx, float sy, float sz)
{
	return mat4(1.0f/sx, 0.0f   , 0.0f   , 0.0f,
		        0.0f   , 1.0f/sy, 0.0f   , 0.0f,
		        0.0f   , 0.0f   , 1.0f/sz, 0.0f,
		        0.0f   , 0.0f   , 0.0f   , 1.0f);
}

mat4 tran::zoom(const vec3& v)
{
	return mat4(1.0f/v.x, 0.0f    , 0.0f    , 0.0f,
		        0.0f    , 1.0f/v.y, 0.0f    , 0.0f,
		        0.0f    , 0.0f    , 1.0f/v.z, 0.0f,
		        0.0f    , 0.0f    , 0.0f    , 1.0f);
}

mat4 tran::zoom(float s)
{
	return mat4(1.0f/s, 0.0f  , 0.0f  , 0.0f,
		        0.0f  , 1.0f/s, 0.0f  , 0.0f,
		        0.0f  , 0.0f  , 1.0f/s, 0.0f,
		        0.0f  , 0.0f  , 0.0f  , 1.0f);
}