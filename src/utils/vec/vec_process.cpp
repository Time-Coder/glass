#include <cmath>

#include "glass/vec"
#include "glass/utils/str.h"
#include "glass/utils/exceptions.h"
#include "glass/utils/helper.h"

double round(double value, uint n)
{
	double ten_n = pow(10, n);
	return round(value * ten_n) / ten_n;
}

double fix(double value)
{
	return value > 0 ? floor(value) : ceil(value);
}

bool is_zero(double value)
{
	return (value < 1E-6 && value > -1E-6);
}

bool is_int(double value)
{
	double abs_value = fabs(value);
	return (abs_value - int(abs_value) < 1E-6);
}

bool is_equal(double value1, double value2)
{
	double delta = value1 - value2;
	return (delta < 1E-6 && delta > -1E-6);
}

double rad2deg(double rad)
{
	return rad/PI*180;
}

double deg2rad(double deg)
{
	return deg/180*PI;
}

dvec2 pow(const dvec2& v1, const dvec2& v2)
{
	return dvec2(pow(v1.x, v2.x), pow(v1.y, v2.y));
}

dvec2 pow(const dvec2& v, double value)
{
	return dvec2(pow(v.x, value), pow(v.y, value));
}

dvec2 pow(double value, const dvec2& v)
{
	return dvec2(pow(value, v.x), pow(value, v.y));
}

double length(const dvec2& v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}

dvec2 normalize(const dvec2& v)
{
	double l = sqrt(v.x*v.x + v.y*v.y);
	return is_zero(l) ? dvec2(0.0f, 0.0f) : dvec2(v.x / l, v.y / l);
}

dvec2 reflect(dvec2 v_in, dvec2 v_norm)
{
	v_in = normalize(v_in);
	v_norm = normalize(v_norm);
	return v_in - 2*dot(v_in, v_norm)*v_norm;
}

double dot(const dvec2& v1, const dvec2& v2)
{
	return v1.x*v2.x + v1.y*v2.y;
}

double cross(const dvec2& v1, const dvec2& v2)
{
	return v1.x*v2.y - v2.x*v1.x;
}

dvec2 mix(const dvec2& v1, const dvec2& v2, double factor)
{
	return factor * v1 + (1.0f - factor) * v2;
}

dvec2 sin(const dvec2& v)
{
	return dvec2(sin(v.x), sin(v.y));
}

dvec2 cos(const dvec2& v)
{
	return dvec2(cos(v.x), cos(v.y));
}

dvec2 tan(const dvec2& v)
{
	double k = v.x/PI-0.5;
	if(is_int(k))
	{
		throw glass::ArithmeticError("v.x can not be (k+0.5)*PI.\nBut v.x = " + str::str(v.x) + " = (" + str::str(int(k)) + "+0.5) * PI now.");
	}
	k = v.y/PI-0.5;
	if(is_int(k))
	{
		throw glass::ArithmeticError("v.y can not be (k+0.5)*PI.\nBut v.y = " + str::str(v.y) + " = (" + str::str(int(k)) + "+0.5) * PI now.");
	}
	return dvec2(tan(v.x), tan(v.y));
}

dvec2 sinh(const dvec2& v)
{
	return dvec2(sinh(v.x), sinh(v.y));
}

dvec2 cosh(const dvec2& v)
{
	return dvec2(cosh(v.x), cosh(v.y));
}

dvec2 tanh(const dvec2& v)
{
	return dvec2(tanh(v.x), tanh(v.y));
}

dvec2 asin(const dvec2& v)
{
	if(v.x < -1 || v.x > 1)
	{
		throw glass::ArithmeticError("v.x must be in range [-1, 1].\nBut v.x = " + str::str(v.x) + " now.");
	}
	if(v.y < -1 || v.y > 1)
	{
		throw glass::ArithmeticError("v.y must be in range [-1, 1].\nBut v.y = " + str::str(v.y) + " now.");
	}
	return dvec2(asin(v.x), asin(v.y));
}

dvec2 acos(const dvec2& v)
{
	if(v.x < -1 || v.x > 1)
	{
		throw glass::ArithmeticError("v.x must be in range [-1, 1].\nBut v.x = " + str::str(v.x) + " now.");
	}
	if(v.y < -1 || v.y > 1)
	{
		throw glass::ArithmeticError("v.y must be in range [-1, 1].\nBut v.y = " + str::str(v.y) + " now.");
	}
	return dvec2(acos(v.x), acos(v.y));
}

dvec2 atan(const dvec2& v)
{
	return dvec2(atan(v.x), atan(v.y));
}

dvec2 asinh(const dvec2& v)
{
	return dvec2(asinh(v.x), asinh(v.y));
}

dvec2 acosh(const dvec2& v)
{
	if(v.x < 1)
	{
		throw glass::ArithmeticError("v.x cannot be less than 1.\nBut now v.x = " + str::str(v.x));
	}
	if(v.y < 1)
	{
		throw glass::ArithmeticError("v.y cannot be less than 1.\nBut now v.y = " + str::str(v.y));
	}
	return dvec2(acosh(v.x), acosh(v.y));
}

dvec2 atanh(const dvec2& v)
{
	if(v.x <= -1 || v.x >= 1)
	{
		throw glass::ArithmeticError("v.x must be in range (-1, 1).\nBut v.x = " + str::str(v.x) + " now.");
	}
	if(v.y <= -1 || v.y >= 1)
	{
		throw glass::ArithmeticError("v.y must be in range (-1, 1).\nBut v.y = " + str::str(v.y) + " now.");
	}
	return dvec2(atanh(v.x), atanh(v.y));
}

dvec2 atan2(const dvec2& Y, const dvec2& X)
{
	return dvec2(atan2(Y.x, X.x), atan2(Y.y, X.y));
}

dvec2 exp(const dvec2& v)
{
	return dvec2(exp(v.x), exp(v.y));
}

dvec2 log(const dvec2& v)
{
	if(v.x <= 0)
	{
		throw glass::ArithmeticError("v.x must be greater than 0.\nBut v.x = " + str::str(v.x) + " now.");
	}
	if(v.y <= 0)
	{
		throw glass::ArithmeticError("v.y must be greater than 0.\nBut v.y = " + str::str(v.y) + " now.");
	}
	return dvec2(log(v.x), log(v.y));
}

dvec2 log2(const dvec2& v)
{
	if(v.x <= 0)
	{
		throw glass::ArithmeticError("v.x must be greater than 0.\nBut v.x = " + str::str(v.x) + " now.");
	}
	if(v.y <= 0)
	{
		throw glass::ArithmeticError("v.y must be greater than 0.\nBut v.y = " + str::str(v.y) + " now.");
	}
	return dvec2(log2(v.x), log2(v.y));
}

dvec2 log10(const dvec2& v)
{
	if(v.x <= 0)
	{
		throw glass::ArithmeticError("v.x must be greater than 0.\nBut v.x = " + str::str(v.x) + " now.");
	}
	if(v.y <= 0)
	{
		throw glass::ArithmeticError("v.y must be greater than 0.\nBut v.y = " + str::str(v.y) + " now.");
	}
	return dvec2(log10(v.x), log10(v.y));
}

dvec2 abs(const dvec2& v)
{
	return dvec2(fabs(v.x), fabs(v.y));
}

dvec2 floor(const dvec2& v)
{
	return dvec2(floor(v.x), floor(v.y));
}

dvec2 ceil(const dvec2& v)
{
	return dvec2(ceil(v.x), ceil(v.y));
}

dvec2 round(const dvec2& v, uint n)
{
	return dvec2(round(v.x, n), round(v.y, n));
}

dvec2 trunc(const dvec2& v)
{
	return dvec2(trunc(v.x), trunc(v.y));
}

dvec2 fix(const dvec2& v)
{
	return dvec2(fix(v.x), fix(v.y));
}

dvec3 pow(const dvec3& v1, const dvec3& v2)
{
	return dvec3(pow(v1.x, v2.x), pow(v1.y, v2.y), pow(v1.z, v2.z));
}

dvec3 pow(const dvec3& v, double value)
{
	return dvec3(pow(v.x, value), pow(v.y, value), pow(v.z, value));
}

dvec3 pow(double value, const dvec3& v)
{
	return dvec3(pow(value, v.x), pow(value, v.y), pow(value, v.z));
}

double length(const dvec3& v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

dvec3 normalize(const dvec3& v)
{
	double l = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	return is_zero(l) ? dvec3(0.0f, 0.0f, 0.0f) : dvec3(v.x / l, v.y / l, v.z / l);
}

dvec3 reflect(dvec3 v_in, dvec3 v_norm)
{
	v_in = normalize(v_in);
	v_norm = normalize(v_norm);
	return v_in - 2*dot(v_in, v_norm)*v_norm;
}

double dot(const dvec3& v1, const dvec3& v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

dvec3 cross(const dvec3& v1, const dvec3& v2)
{
	return dvec3(v1.y*v2.z - v2.y*v1.z,
		        v2.x*v1.z - v1.x*v2.z,
		        v1.x*v2.y - v2.x*v1.y);
}

dvec3 mix(const dvec3& v1, const dvec3& v2, double factor)
{
	return factor * v1 + (1.0f - factor) * v2;
}

dvec3 sin(const dvec3& v)
{
	return dvec3(sin(v.x), sin(v.y), sin(v.z));
}

dvec3 cos(const dvec3& v)
{
	return dvec3(cos(v.x), cos(v.y), cos(v.z));
}

dvec3 tan(const dvec3& v)
{
	double k = v.x/PI - 0.5;
	if(is_int(k))
	{
		throw glass::ArithmeticError("v.x cannot be (0.5+k)*PI. But now v.x = " + str::str(v.x) + " = (0.5 + " + str::str(int(k)) + ") * PI");
	}

	k = v.y/PI - 0.5;
	if(is_int(k))
	{
		throw glass::ArithmeticError("v.y cannot be (0.5+k)*PI. But now v.y = " + str::str(v.y) + " = (0.5 + " + str::str(int(k)) + ") * PI");
	}

	k = v.z/PI - 0.5;
	if(is_int(k))
	{
		throw glass::ArithmeticError("v.z cannot be (0.5+k)*PI. But now v.z = " + str::str(v.z) + " = (0.5 + " + str::str(int(k)) + ") * PI");
	}

	return dvec3(tan(v.x), tan(v.y), tan(v.z));
}

dvec3 sinh(const dvec3& v)
{
	return dvec3(sinh(v.x), sinh(v.y), sinh(v.z));
}

dvec3 cosh(const dvec3& v)
{
	return dvec3(cosh(v.x), cosh(v.y), cosh(v.z));
}

dvec3 tanh(const dvec3& v)
{
	return dvec3(tanh(v.x), tanh(v.y), tanh(v.z));
}

dvec3 asin(const dvec3& v)
{
	if(v.x < -1 || v.x > 1)
	{
		throw glass::ArithmeticError("v.x must be in range [-1, 1]. But now v.x = " + str::str(v.x));
	}
	if(v.y < -1 || v.y > 1)
	{
		throw glass::ArithmeticError("v.y must be in range [-1, 1]. But now v.y = " + str::str(v.y));
	}
	if(v.z < -1 || v.z > 1)
	{
		throw glass::ArithmeticError("v.z must be in range [-1, 1]. But now v.z = " + str::str(v.z));
	}

	return dvec3(asin(v.x), asin(v.y), asin(v.z));
}

dvec3 acos(const dvec3& v)
{
	if(v.x < -1 || v.x > 1)
	{
		throw glass::ArithmeticError("v.x must be in range [-1, 1]. But now v.x = " + str::str(v.x));
	}
	if(v.y < -1 || v.y > 1)
	{
		throw glass::ArithmeticError("v.y must be in range [-1, 1]. But now v.y = " + str::str(v.y));
	}
	if(v.z < -1 || v.z > 1)
	{
		throw glass::ArithmeticError("v.z must be in range [-1, 1]. But now v.z = " + str::str(v.z));
	}

	return dvec3(acos(v.x), acos(v.y), acos(v.z));
}

dvec3 atan(const dvec3& v)
{
	return dvec3(atan(v.x), atan(v.y), atan(v.z));
}

dvec3 asinh(const dvec3& v)
{
	return dvec3(asinh(v.x), asinh(v.y), asinh(v.z));
}

dvec3 acosh(const dvec3& v)
{
	if(v.x < 1)
	{
		throw glass::ArithmeticError("v.x cannot be less than 1. But now v.x = " + str::str(v.x));
	}
	if(v.y < 1)
	{
		throw glass::ArithmeticError("v.y cannot be less than 1. But now v.y = " + str::str(v.y));
	}
	if(v.z < 1)
	{
		throw glass::ArithmeticError("v.z cannot be less than 1. But now v.z = " + str::str(v.z));
	}

	return dvec3(acosh(v.x), acosh(v.y), acosh(v.z));
}

dvec3 atanh(const dvec3& v)
{
	if(v.x <= -1 || v.x >= 1)
	{
		throw glass::ArithmeticError("v.x must be in range (-1, 1). But now v.x = " + str::str(v.x));
	}
	if(v.y <= -1 || v.y >= 1)
	{
		throw glass::ArithmeticError("v.y must be in range (-1, 1). But now v.y = " + str::str(v.y));
	}
	if(v.z <= -1 || v.z >= 1)
	{
		throw glass::ArithmeticError("v.z must be in range (-1, 1). But now v.z = " + str::str(v.z));
	}

	return dvec3(atanh(v.x), atanh(v.y), atanh(v.z));
}

dvec3 atan2(const dvec3& Y, const dvec3& X)
{
	return dvec3(atan2(Y.x, X.x), atan2(Y.y, X.y), atan2(Y.z, X.z));
}

dvec3 exp(const dvec3& v)
{
	return dvec3(exp(v.x), exp(v.y), exp(v.z));
}

dvec3 log(const dvec3& v)
{
	if(v.x <= 0)
	{
		throw glass::ArithmeticError("v.x must be greater than 0. But now v.x = " + str::str(v.x));
	}
	if(v.y <= 0)
	{
		throw glass::ArithmeticError("v.y must be greater than 0. But now v.y = " + str::str(v.y));
	}
	if(v.z <= 0)
	{
		throw glass::ArithmeticError("v.z must be greater than 0. But now v.z = " + str::str(v.z));
	}

	return dvec3(log(v.x), log(v.y), log(v.z));
}

dvec3 log2(const dvec3& v)
{
	if(v.x <= 0)
	{
		throw glass::ArithmeticError("v.x must be greater than 0. But now v.x = " + str::str(v.x));
	}
	if(v.y <= 0)
	{
		throw glass::ArithmeticError("v.y must be greater than 0. But now v.y = " + str::str(v.y));
	}
	if(v.z <= 0)
	{
		throw glass::ArithmeticError("v.z must be greater than 0. But now v.z = " + str::str(v.z));
	}

	return dvec3(log2(v.x), log2(v.y), log2(v.z));
}

dvec3 log10(const dvec3& v)
{
	if(v.x <= 0)
	{
		throw glass::ArithmeticError("v.x must be greater than 0. But now v.x = " + str::str(v.x));
	}
	if(v.y <= 0)
	{
		throw glass::ArithmeticError("v.y must be greater than 0. But now v.y = " + str::str(v.y));
	}
	if(v.z <= 0)
	{
		throw glass::ArithmeticError("v.z must be greater than 0. But now v.z = " + str::str(v.z));
	}

	return dvec3(log10(v.x), log10(v.y), log10(v.z));
}

dvec3 abs(const dvec3& v)
{
	return dvec3(fabs(v.x), fabs(v.y), fabs(v.z));
}

dvec3 floor(const dvec3& v)
{
	return dvec3(floor(v.x), floor(v.y), floor(v.z));
}

dvec3 ceil(const dvec3& v)
{
	return dvec3(ceil(v.x), ceil(v.y), ceil(v.z));
}

dvec3 round(const dvec3& v, uint n)
{
	return dvec3(round(v.x, n), round(v.y, n), round(v.z, n));
}

dvec3 trunc(const dvec3& v)
{
	return dvec3(trunc(v.x), trunc(v.y), trunc(v.z));
}

dvec3 fix(const dvec3& v)
{
	return dvec3(fix(v.x), fix(v.y), fix(v.z));
}

dvec4 pow(const dvec4& v1, const dvec4& v2)
{
	return dvec4(pow(v1.x, v2.x), pow(v1.y, v2.y), pow(v1.z, v2.z), pow(v1.w, v2.w));
}

dvec4 pow(const dvec4& v, double value)
{
	return dvec4(pow(v.x, value), pow(v.y, value), pow(v.z, value), pow(v.w, value));
}

dvec4 pow(double value, const dvec4& v)
{
	return dvec4(pow(value, v.x), pow(value, v.y), pow(value, v.z), pow(value, v.w));
}

double length(const dvec4& v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
}

dvec4 normalize(const dvec4& v)
{
	double l = sqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
	return is_zero(l) ? dvec4(0.0f, 0.0f, 0.0f, 0.0f) : dvec4(v.x / l, v.y / l, v.z / l, v.w / l);
}

double dot(const dvec4& v1, const dvec4& v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
}

dvec4 mix(const dvec4& v1, const dvec4& v2, double factor)
{
	return factor * v1 + (1.0f - factor) * v2;
}

dvec4 sin(const dvec4& v)
{
	return dvec4(sin(v.x), sin(v.y), sin(v.z), sin(v.w));
}

dvec4 cos(const dvec4& v)
{
	return dvec4(cos(v.x), cos(v.y), cos(v.z), sin(v.w));
}

dvec4 tan(const dvec4& v)
{
	double k = v.x/PI - 0.5;
	if(is_int(k))
	{
		throw glass::ArithmeticError("v.x cannot be (0.5 + k) * PI. But now v.x = " + str::str(v.x) + " = (0.5 + " + str::str(int(k)) + ") * PI");
	}

	k = v.y/PI - 0.5;
	if(is_int(k))
	{
		throw glass::ArithmeticError("v.y cannot be (0.5 + k) * PI. But now v.y = " + str::str(v.y) + " = (0.5 + " + str::str(int(k)) + ") * PI");
	}

	k = v.z/PI - 0.5;
	if(is_int(k))
	{
		throw glass::ArithmeticError("v.z cannot be (0.5 + k) * PI. But now v.z = " + str::str(v.z) + " = (0.5 + " + str::str(int(k)) + ") * PI");
	}

	k = v.w/PI - 0.5;
	if(is_int(k))
	{
		throw glass::ArithmeticError("v.w cannot be (0.5 + k) * PI. But now v.w = " + str::str(v.w) + " = (0.5 + " + str::str(int(k)) + ") * PI");
	}

	return dvec4(tan(v.x), tan(v.y), tan(v.z), tan(v.w));
}

dvec4 sinh(const dvec4& v)
{
	return dvec4(sinh(v.x), sinh(v.y), sinh(v.z), sinh(v.w));
}

dvec4 cosh(const dvec4& v)
{
	return dvec4(cosh(v.x), cosh(v.y), cosh(v.z), cosh(v.w));
}

dvec4 tanh(const dvec4& v)
{
	return dvec4(tanh(v.x), tanh(v.y), tanh(v.z), tanh(v.w));
}

dvec4 asin(const dvec4& v)
{
	if(v.x < -1 || v.x > 1)
	{
		throw glass::ArithmeticError("v.x must be in range [-1, 1]. But now v.x = " + str::str(v.x));
	}
	if(v.y < -1 || v.y > 1)
	{
		throw glass::ArithmeticError("v.y must be in range [-1, 1]. But now v.y = " + str::str(v.y));
	}
	if(v.z < -1 || v.z > 1)
	{
		throw glass::ArithmeticError("v.z must be in range [-1, 1]. But now v.z = " + str::str(v.z));
	}
	if(v.w < -1 || v.w > 1)
	{
		throw glass::ArithmeticError("v.w must be in range [-1, 1]. But now v.w = " + str::str(v.w));
	}

	return dvec4(asin(v.x), asin(v.y), asin(v.z), asin(v.w));
}

dvec4 acos(const dvec4& v)
{
	if(v.x < -1 || v.x > 1)
	{
		throw glass::ArithmeticError("v.x must be in range [-1, 1]. But now v.x = " + str::str(v.x));
	}
	if(v.y < -1 || v.y > 1)
	{
		throw glass::ArithmeticError("v.y must be in range [-1, 1]. But now v.y = " + str::str(v.y));
	}
	if(v.z < -1 || v.z > 1)
	{
		throw glass::ArithmeticError("v.z must be in range [-1, 1]. But now v.z = " + str::str(v.z));
	}
	if(v.w < -1 || v.w > 1)
	{
		throw glass::ArithmeticError("v.w must be in range [-1, 1]. But now v.w = " + str::str(v.w));
	}

	return dvec4(acos(v.x), acos(v.y), acos(v.z), acos(v.w));
}

dvec4 atan(const dvec4& v)
{
	return dvec4(atan(v.x), atan(v.y), atan(v.z), atan(v.w));
}

dvec4 asinh(const dvec4& v)
{
	return dvec4(asinh(v.x), asinh(v.y), asinh(v.z), asinh(v.w));
}

dvec4 acosh(const dvec4& v)
{
	if(v.x < 1)
	{
		throw glass::ArithmeticError("v.x cannot be less than 1. But now v.x = " + str::str(v.x));
	}
	if(v.y < 1)
	{
		throw glass::ArithmeticError("v.y cannot be less than 1. But now v.y = " + str::str(v.y));
	}
	if(v.z < 1)
	{
		throw glass::ArithmeticError("v.z cannot be less than 1. But now v.z = " + str::str(v.z));
	}
	if(v.w < 1)
	{
		throw glass::ArithmeticError("v.w cannot be less than 1. But now v.w = " + str::str(v.w));
	}

	return dvec4(acosh(v.x), acosh(v.y), acosh(v.z), acosh(v.w));
}

dvec4 atanh(const dvec4& v)
{
	if(v.x <= -1 || v.x >= 1)
	{
		throw glass::ArithmeticError("v.x must be in range (-1, 1). But now v.x = " + str::str(v.x));
	}
	if(v.y <= -1 || v.y >= 1)
	{
		throw glass::ArithmeticError("v.y must be in range (-1, 1). But now v.y = " + str::str(v.y));
	}
	if(v.z <= -1 || v.z >= 1)
	{
		throw glass::ArithmeticError("v.z must be in range (-1, 1). But now v.z = " + str::str(v.z));
	}
	if(v.w <= -1 || v.w >= 1)
	{
		throw glass::ArithmeticError("v.w must be in range (-1, 1). But now v.w = " + str::str(v.w));
	}

	return dvec4(atanh(v.x), atanh(v.y), atanh(v.z), atanh(v.w));
}

dvec4 atan2(const dvec4& Y, const dvec4& X)
{
	return dvec4(atan2(Y.x, X.x), atan2(Y.y, X.y), atan2(Y.z, X.z), atan2(Y.w, X.w));
}

dvec4 exp(const dvec4& v)
{
	return dvec4(exp(v.x), exp(v.y), exp(v.z), exp(v.w));
}

dvec4 log(const dvec4& v)
{
	if(v.x <= 0)
	{
		throw glass::ArithmeticError("v.x must be greater than 0. But now v.x = " + str::str(v.x));
	}
	if(v.y <= 0)
	{
		throw glass::ArithmeticError("v.y must be greater than 0. But now v.y = " + str::str(v.y));
	}
	if(v.z <= 0)
	{
		throw glass::ArithmeticError("v.z must be greater than 0. But now v.z = " + str::str(v.z));
	}
	if(v.w <= 0)
	{
		throw glass::ArithmeticError("v.w must be greater than 0. But now v.w = " + str::str(v.w));
	}

	return dvec4(log(v.x), log(v.y), log(v.z), log(v.w));
}

dvec4 log2(const dvec4& v)
{
	if(v.x <= 0)
	{
		throw glass::ArithmeticError("v.x must be greater than 0. But now v.x = " + str::str(v.x));
	}
	if(v.y <= 0)
	{
		throw glass::ArithmeticError("v.y must be greater than 0. But now v.y = " + str::str(v.y));
	}
	if(v.z <= 0)
	{
		throw glass::ArithmeticError("v.z must be greater than 0. But now v.z = " + str::str(v.z));
	}
	if(v.w <= 0)
	{
		throw glass::ArithmeticError("v.w must be greater than 0. But now v.w = " + str::str(v.w));
	}

	return dvec4(log2(v.x), log2(v.y), log2(v.z), log2(v.w));
}

dvec4 log10(const dvec4& v)
{
	if(v.x <= 0)
	{
		throw glass::ArithmeticError("v.x must be greater than 0. But now v.x = " + str::str(v.x));
	}
	if(v.y <= 0)
	{
		throw glass::ArithmeticError("v.y must be greater than 0. But now v.y = " + str::str(v.y));
	}
	if(v.z <= 0)
	{
		throw glass::ArithmeticError("v.z must be greater than 0. But now v.z = " + str::str(v.z));
	}
	if(v.w <= 0)
	{
		throw glass::ArithmeticError("v.w must be greater than 0. But now v.w = " + str::str(v.w));
	}

	return dvec4(log10(v.x), log10(v.y), log10(v.z), log10(v.w));
}

dvec4 abs(const dvec4& v)
{
	return dvec4(fabs(v.x), fabs(v.y), fabs(v.z), fabs(v.w));
}

dvec4 floor(const dvec4& v)
{
	return dvec4(floor(v.x), floor(v.y), floor(v.z), floor(v.w));
}

dvec4 ceil(const dvec4& v)
{
	return dvec4(ceil(v.x), ceil(v.y), ceil(v.z), ceil(v.w));
}

dvec4 round(const dvec4& v, uint n)
{
	return dvec4(round(v.x, n), round(v.y, n), round(v.z, n), round(v.w, n));
}

dvec4 trunc(const dvec4& v)
{
	return dvec4(trunc(v.x), trunc(v.y), trunc(v.z), trunc(v.w));
}

dvec4 fix(const dvec4& v)
{
	return dvec4(fix(v.x), fix(v.y), fix(v.z), fix(v.w));
}

vec2 pow(const vec2& v1, const vec2& v2)
{
	return vec2(pow(v1.x, v2.x), pow(v1.y, v2.y));
}

vec2 pow(const vec2& v, double value)
{
	return vec2(pow(v.x, value), pow(v.y, value));
}

vec2 pow(double value, const vec2& v)
{
	return vec2(pow(value, v.x), pow(value, v.y));
}

double length(const vec2& v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}

vec2 normalize(const vec2& v)
{
	double l = sqrt(v.x*v.x + v.y*v.y);
	return is_zero(l) ? vec2(0.0f, 0.0f) : vec2(v.x / l, v.y / l);
}

vec2 reflect(vec2 v_in, vec2 v_norm)
{
	v_in = normalize(v_in);
	v_norm = normalize(v_norm);
	return v_in - 2*dot(v_in, v_norm)*v_norm;
}

double dot(const vec2& v1, const vec2& v2)
{
	return v1.x*v2.x + v1.y*v2.y;
}

double cross(const vec2& v1, const vec2& v2)
{
	return v1.x*v2.y - v2.x*v1.x;
}

vec2 mix(const vec2& v1, const vec2& v2, double factor)
{
	return factor * v1 + (1.0f - factor) * v2;
}

vec2 sin(const vec2& v)
{
	return vec2(sin(v.x), sin(v.y));
}

vec2 cos(const vec2& v)
{
	return vec2(cos(v.x), cos(v.y));
}

vec2 tan(const vec2& v)
{
	double k = v.x/PI-0.5;
	if(is_int(k))
	{
		throw glass::ArithmeticError("v.x can not be (k+0.5)*PI.\nBut v.x = " + str::str(v.x) + " = (" + str::str(int(k)) + "+0.5) * PI now.");
	}
	k = v.y/PI-0.5;
	if(is_int(k))
	{
		throw glass::ArithmeticError("v.y can not be (k+0.5)*PI.\nBut v.y = " + str::str(v.y) + " = (" + str::str(int(k)) + "+0.5) * PI now.");
	}
	return vec2(tan(v.x), tan(v.y));
}

vec2 sinh(const vec2& v)
{
	return vec2(sinh(v.x), sinh(v.y));
}

vec2 cosh(const vec2& v)
{
	return vec2(cosh(v.x), cosh(v.y));
}

vec2 tanh(const vec2& v)
{
	return vec2(tanh(v.x), tanh(v.y));
}

vec2 asin(const vec2& v)
{
	if(v.x < -1 || v.x > 1)
	{
		throw glass::ArithmeticError("v.x must be in range [-1, 1].\nBut v.x = " + str::str(v.x) + " now.");
	}
	if(v.y < -1 || v.y > 1)
	{
		throw glass::ArithmeticError("v.y must be in range [-1, 1].\nBut v.y = " + str::str(v.y) + " now.");
	}
	return vec2(asin(v.x), asin(v.y));
}

vec2 acos(const vec2& v)
{
	if(v.x < -1 || v.x > 1)
	{
		throw glass::ArithmeticError("v.x must be in range [-1, 1].\nBut v.x = " + str::str(v.x) + " now.");
	}
	if(v.y < -1 || v.y > 1)
	{
		throw glass::ArithmeticError("v.y must be in range [-1, 1].\nBut v.y = " + str::str(v.y) + " now.");
	}
	return vec2(acos(v.x), acos(v.y));
}

vec2 atan(const vec2& v)
{
	return vec2(atan(v.x), atan(v.y));
}

vec2 asinh(const vec2& v)
{
	return vec2(asinh(v.x), asinh(v.y));
}

vec2 acosh(const vec2& v)
{
	if(v.x < 1)
	{
		throw glass::ArithmeticError("v.x cannot be less than 1.\nBut now v.x = " + str::str(v.x));
	}
	if(v.y < 1)
	{
		throw glass::ArithmeticError("v.y cannot be less than 1.\nBut now v.y = " + str::str(v.y));
	}
	return vec2(acosh(v.x), acosh(v.y));
}

vec2 atanh(const vec2& v)
{
	if(v.x <= -1 || v.x >= 1)
	{
		throw glass::ArithmeticError("v.x must be in range (-1, 1).\nBut v.x = " + str::str(v.x) + " now.");
	}
	if(v.y <= -1 || v.y >= 1)
	{
		throw glass::ArithmeticError("v.y must be in range (-1, 1).\nBut v.y = " + str::str(v.y) + " now.");
	}
	return vec2(atanh(v.x), atanh(v.y));
}

vec2 atan2(const vec2& Y, const vec2& X)
{
	return vec2(atan2(Y.x, X.x), atan2(Y.y, X.y));
}

vec2 exp(const vec2& v)
{
	return vec2(exp(v.x), exp(v.y));
}

vec2 log(const vec2& v)
{
	if(v.x <= 0)
	{
		throw glass::ArithmeticError("v.x must be greater than 0.\nBut v.x = " + str::str(v.x) + " now.");
	}
	if(v.y <= 0)
	{
		throw glass::ArithmeticError("v.y must be greater than 0.\nBut v.y = " + str::str(v.y) + " now.");
	}
	return vec2(log(v.x), log(v.y));
}

vec2 log2(const vec2& v)
{
	if(v.x <= 0)
	{
		throw glass::ArithmeticError("v.x must be greater than 0.\nBut v.x = " + str::str(v.x) + " now.");
	}
	if(v.y <= 0)
	{
		throw glass::ArithmeticError("v.y must be greater than 0.\nBut v.y = " + str::str(v.y) + " now.");
	}
	return vec2(log2(v.x), log2(v.y));
}

vec2 log10(const vec2& v)
{
	if(v.x <= 0)
	{
		throw glass::ArithmeticError("v.x must be greater than 0.\nBut v.x = " + str::str(v.x) + " now.");
	}
	if(v.y <= 0)
	{
		throw glass::ArithmeticError("v.y must be greater than 0.\nBut v.y = " + str::str(v.y) + " now.");
	}
	return vec2(log10(v.x), log10(v.y));
}

vec2 abs(const vec2& v)
{
	return vec2(fabs(v.x), fabs(v.y));
}

vec2 floor(const vec2& v)
{
	return vec2(floor(v.x), floor(v.y));
}

vec2 ceil(const vec2& v)
{
	return vec2(ceil(v.x), ceil(v.y));
}

vec2 round(const vec2& v, uint n)
{
	return vec2(round(v.x, n), round(v.y, n));
}

vec2 trunc(const vec2& v)
{
	return vec2(trunc(v.x), trunc(v.y));
}

vec2 fix(const vec2& v)
{
	return vec2(fix(v.x), fix(v.y));
}

vec3 pow(const vec3& v1, const vec3& v2)
{
	return vec3(pow(v1.x, v2.x), pow(v1.y, v2.y), pow(v1.z, v2.z));
}

vec3 pow(const vec3& v, double value)
{
	return vec3(pow(v.x, value), pow(v.y, value), pow(v.z, value));
}

vec3 pow(double value, const vec3& v)
{
	return vec3(pow(value, v.x), pow(value, v.y), pow(value, v.z));
}

double length(const vec3& v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

vec3 normalize(const vec3& v)
{
	double l = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	return is_zero(l) ? vec3(0.0f, 0.0f, 0.0f) : vec3(v.x / l, v.y / l, v.z / l);
}

vec3 reflect(vec3 v_in, vec3 v_norm)
{
	v_in = normalize(v_in);
	v_norm = normalize(v_norm);
	return v_in - 2*dot(v_in, v_norm)*v_norm;
}

double dot(const vec3& v1, const vec3& v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

vec3 cross(const vec3& v1, const vec3& v2)
{
	return vec3(v1.y*v2.z - v2.y*v1.z,
		        v2.x*v1.z - v1.x*v2.z,
		        v1.x*v2.y - v2.x*v1.y);
}

vec3 mix(const vec3& v1, const vec3& v2, double factor)
{
	return factor * v1 + (1.0f - factor) * v2;
}

vec3 sin(const vec3& v)
{
	return vec3(sin(v.x), sin(v.y), sin(v.z));
}

vec3 cos(const vec3& v)
{
	return vec3(cos(v.x), cos(v.y), cos(v.z));
}

vec3 tan(const vec3& v)
{
	double k = v.x/PI - 0.5;
	if(is_int(k))
	{
		throw glass::ArithmeticError("v.x cannot be (0.5+k)*PI. But now v.x = " + str::str(v.x) + " = (0.5 + " + str::str(int(k)) + ") * PI");
	}

	k = v.y/PI - 0.5;
	if(is_int(k))
	{
		throw glass::ArithmeticError("v.y cannot be (0.5+k)*PI. But now v.y = " + str::str(v.y) + " = (0.5 + " + str::str(int(k)) + ") * PI");
	}

	k = v.z/PI - 0.5;
	if(is_int(k))
	{
		throw glass::ArithmeticError("v.z cannot be (0.5+k)*PI. But now v.z = " + str::str(v.z) + " = (0.5 + " + str::str(int(k)) + ") * PI");
	}

	return vec3(tan(v.x), tan(v.y), tan(v.z));
}

vec3 sinh(const vec3& v)
{
	return vec3(sinh(v.x), sinh(v.y), sinh(v.z));
}

vec3 cosh(const vec3& v)
{
	return vec3(cosh(v.x), cosh(v.y), cosh(v.z));
}

vec3 tanh(const vec3& v)
{
	return vec3(tanh(v.x), tanh(v.y), tanh(v.z));
}

vec3 asin(const vec3& v)
{
	if(v.x < -1 || v.x > 1)
	{
		throw glass::ArithmeticError("v.x must be in range [-1, 1]. But now v.x = " + str::str(v.x));
	}
	if(v.y < -1 || v.y > 1)
	{
		throw glass::ArithmeticError("v.y must be in range [-1, 1]. But now v.y = " + str::str(v.y));
	}
	if(v.z < -1 || v.z > 1)
	{
		throw glass::ArithmeticError("v.z must be in range [-1, 1]. But now v.z = " + str::str(v.z));
	}

	return vec3(asin(v.x), asin(v.y), asin(v.z));
}

vec3 acos(const vec3& v)
{
	if(v.x < -1 || v.x > 1)
	{
		throw glass::ArithmeticError("v.x must be in range [-1, 1]. But now v.x = " + str::str(v.x));
	}
	if(v.y < -1 || v.y > 1)
	{
		throw glass::ArithmeticError("v.y must be in range [-1, 1]. But now v.y = " + str::str(v.y));
	}
	if(v.z < -1 || v.z > 1)
	{
		throw glass::ArithmeticError("v.z must be in range [-1, 1]. But now v.z = " + str::str(v.z));
	}

	return vec3(acos(v.x), acos(v.y), acos(v.z));
}

vec3 atan(const vec3& v)
{
	return vec3(atan(v.x), atan(v.y), atan(v.z));
}

vec3 asinh(const vec3& v)
{
	return vec3(asinh(v.x), asinh(v.y), asinh(v.z));
}

vec3 acosh(const vec3& v)
{
	if(v.x < 1)
	{
		throw glass::ArithmeticError("v.x cannot be less than 1. But now v.x = " + str::str(v.x));
	}
	if(v.y < 1)
	{
		throw glass::ArithmeticError("v.y cannot be less than 1. But now v.y = " + str::str(v.y));
	}
	if(v.z < 1)
	{
		throw glass::ArithmeticError("v.z cannot be less than 1. But now v.z = " + str::str(v.z));
	}

	return vec3(acosh(v.x), acosh(v.y), acosh(v.z));
}

vec3 atanh(const vec3& v)
{
	if(v.x <= -1 || v.x >= 1)
	{
		throw glass::ArithmeticError("v.x must be in range (-1, 1). But now v.x = " + str::str(v.x));
	}
	if(v.y <= -1 || v.y >= 1)
	{
		throw glass::ArithmeticError("v.y must be in range (-1, 1). But now v.y = " + str::str(v.y));
	}
	if(v.z <= -1 || v.z >= 1)
	{
		throw glass::ArithmeticError("v.z must be in range (-1, 1). But now v.z = " + str::str(v.z));
	}

	return vec3(atanh(v.x), atanh(v.y), atanh(v.z));
}

vec3 atan2(const vec3& Y, const vec3& X)
{
	return vec3(atan2(Y.x, X.x), atan2(Y.y, X.y), atan2(Y.z, X.z));
}

vec3 exp(const vec3& v)
{
	return vec3(exp(v.x), exp(v.y), exp(v.z));
}

vec3 log(const vec3& v)
{
	if(v.x <= 0)
	{
		throw glass::ArithmeticError("v.x must be greater than 0. But now v.x = " + str::str(v.x));
	}
	if(v.y <= 0)
	{
		throw glass::ArithmeticError("v.y must be greater than 0. But now v.y = " + str::str(v.y));
	}
	if(v.z <= 0)
	{
		throw glass::ArithmeticError("v.z must be greater than 0. But now v.z = " + str::str(v.z));
	}

	return vec3(log(v.x), log(v.y), log(v.z));
}

vec3 log2(const vec3& v)
{
	if(v.x <= 0)
	{
		throw glass::ArithmeticError("v.x must be greater than 0. But now v.x = " + str::str(v.x));
	}
	if(v.y <= 0)
	{
		throw glass::ArithmeticError("v.y must be greater than 0. But now v.y = " + str::str(v.y));
	}
	if(v.z <= 0)
	{
		throw glass::ArithmeticError("v.z must be greater than 0. But now v.z = " + str::str(v.z));
	}

	return vec3(log2(v.x), log2(v.y), log2(v.z));
}

vec3 log10(const vec3& v)
{
	if(v.x <= 0)
	{
		throw glass::ArithmeticError("v.x must be greater than 0. But now v.x = " + str::str(v.x));
	}
	if(v.y <= 0)
	{
		throw glass::ArithmeticError("v.y must be greater than 0. But now v.y = " + str::str(v.y));
	}
	if(v.z <= 0)
	{
		throw glass::ArithmeticError("v.z must be greater than 0. But now v.z = " + str::str(v.z));
	}

	return vec3(log10(v.x), log10(v.y), log10(v.z));
}

vec3 abs(const vec3& v)
{
	return vec3(fabs(v.x), fabs(v.y), fabs(v.z));
}

vec3 floor(const vec3& v)
{
	return vec3(floor(v.x), floor(v.y), floor(v.z));
}

vec3 ceil(const vec3& v)
{
	return vec3(ceil(v.x), ceil(v.y), ceil(v.z));
}

vec3 round(const vec3& v, uint n)
{
	return vec3(round(v.x, n), round(v.y, n), round(v.z, n));
}

vec3 trunc(const vec3& v)
{
	return vec3(trunc(v.x), trunc(v.y), trunc(v.z));
}

vec3 fix(const vec3& v)
{
	return vec3(fix(v.x), fix(v.y), fix(v.z));
}

vec4 pow(const vec4& v1, const vec4& v2)
{
	return vec4(pow(v1.x, v2.x), pow(v1.y, v2.y), pow(v1.z, v2.z), pow(v1.w, v2.w));
}

vec4 pow(const vec4& v, double value)
{
	return vec4(pow(v.x, value), pow(v.y, value), pow(v.z, value), pow(v.w, value));
}

vec4 pow(double value, const vec4& v)
{
	return vec4(pow(value, v.x), pow(value, v.y), pow(value, v.z), pow(value, v.w));
}

double length(const vec4& v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
}

vec4 normalize(const vec4& v)
{
	double l = sqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
	return is_zero(l) ? vec4(0.0f, 0.0f, 0.0f, 0.0f) : vec4(v.x / l, v.y / l, v.z / l, v.w / l);
}

double dot(const vec4& v1, const vec4& v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
}

vec4 mix(const vec4& v1, const vec4& v2, double factor)
{
	return factor * v1 + (1.0f - factor) * v2;
}

vec4 sin(const vec4& v)
{
	return vec4(sin(v.x), sin(v.y), sin(v.z), sin(v.w));
}

vec4 cos(const vec4& v)
{
	return vec4(cos(v.x), cos(v.y), cos(v.z), sin(v.w));
}

vec4 tan(const vec4& v)
{
	double k = v.x/PI - 0.5;
	if(is_int(k))
	{
		throw glass::ArithmeticError("v.x cannot be (0.5 + k) * PI. But now v.x = " + str::str(v.x) + " = (0.5 + " + str::str(int(k)) + ") * PI");
	}

	k = v.y/PI - 0.5;
	if(is_int(k))
	{
		throw glass::ArithmeticError("v.y cannot be (0.5 + k) * PI. But now v.y = " + str::str(v.y) + " = (0.5 + " + str::str(int(k)) + ") * PI");
	}

	k = v.z/PI - 0.5;
	if(is_int(k))
	{
		throw glass::ArithmeticError("v.z cannot be (0.5 + k) * PI. But now v.z = " + str::str(v.z) + " = (0.5 + " + str::str(int(k)) + ") * PI");
	}

	k = v.w/PI - 0.5;
	if(is_int(k))
	{
		throw glass::ArithmeticError("v.w cannot be (0.5 + k) * PI. But now v.w = " + str::str(v.w) + " = (0.5 + " + str::str(int(k)) + ") * PI");
	}

	return vec4(tan(v.x), tan(v.y), tan(v.z), tan(v.w));
}

vec4 sinh(const vec4& v)
{
	return vec4(sinh(v.x), sinh(v.y), sinh(v.z), sinh(v.w));
}

vec4 cosh(const vec4& v)
{
	return vec4(cosh(v.x), cosh(v.y), cosh(v.z), cosh(v.w));
}

vec4 tanh(const vec4& v)
{
	return vec4(tanh(v.x), tanh(v.y), tanh(v.z), tanh(v.w));
}

vec4 asin(const vec4& v)
{
	if(v.x < -1 || v.x > 1)
	{
		throw glass::ArithmeticError("v.x must be in range [-1, 1]. But now v.x = " + str::str(v.x));
	}
	if(v.y < -1 || v.y > 1)
	{
		throw glass::ArithmeticError("v.y must be in range [-1, 1]. But now v.y = " + str::str(v.y));
	}
	if(v.z < -1 || v.z > 1)
	{
		throw glass::ArithmeticError("v.z must be in range [-1, 1]. But now v.z = " + str::str(v.z));
	}
	if(v.w < -1 || v.w > 1)
	{
		throw glass::ArithmeticError("v.w must be in range [-1, 1]. But now v.w = " + str::str(v.w));
	}

	return vec4(asin(v.x), asin(v.y), asin(v.z), asin(v.w));
}

vec4 acos(const vec4& v)
{
	if(v.x < -1 || v.x > 1)
	{
		throw glass::ArithmeticError("v.x must be in range [-1, 1]. But now v.x = " + str::str(v.x));
	}
	if(v.y < -1 || v.y > 1)
	{
		throw glass::ArithmeticError("v.y must be in range [-1, 1]. But now v.y = " + str::str(v.y));
	}
	if(v.z < -1 || v.z > 1)
	{
		throw glass::ArithmeticError("v.z must be in range [-1, 1]. But now v.z = " + str::str(v.z));
	}
	if(v.w < -1 || v.w > 1)
	{
		throw glass::ArithmeticError("v.w must be in range [-1, 1]. But now v.w = " + str::str(v.w));
	}

	return vec4(acos(v.x), acos(v.y), acos(v.z), acos(v.w));
}

vec4 atan(const vec4& v)
{
	return vec4(atan(v.x), atan(v.y), atan(v.z), atan(v.w));
}

vec4 asinh(const vec4& v)
{
	return vec4(asinh(v.x), asinh(v.y), asinh(v.z), asinh(v.w));
}

vec4 acosh(const vec4& v)
{
	if(v.x < 1)
	{
		throw glass::ArithmeticError("v.x cannot be less than 1. But now v.x = " + str::str(v.x));
	}
	if(v.y < 1)
	{
		throw glass::ArithmeticError("v.y cannot be less than 1. But now v.y = " + str::str(v.y));
	}
	if(v.z < 1)
	{
		throw glass::ArithmeticError("v.z cannot be less than 1. But now v.z = " + str::str(v.z));
	}
	if(v.w < 1)
	{
		throw glass::ArithmeticError("v.w cannot be less than 1. But now v.w = " + str::str(v.w));
	}

	return vec4(acosh(v.x), acosh(v.y), acosh(v.z), acosh(v.w));
}

vec4 atanh(const vec4& v)
{
	if(v.x <= -1 || v.x >= 1)
	{
		throw glass::ArithmeticError("v.x must be in range (-1, 1). But now v.x = " + str::str(v.x));
	}
	if(v.y <= -1 || v.y >= 1)
	{
		throw glass::ArithmeticError("v.y must be in range (-1, 1). But now v.y = " + str::str(v.y));
	}
	if(v.z <= -1 || v.z >= 1)
	{
		throw glass::ArithmeticError("v.z must be in range (-1, 1). But now v.z = " + str::str(v.z));
	}
	if(v.w <= -1 || v.w >= 1)
	{
		throw glass::ArithmeticError("v.w must be in range (-1, 1). But now v.w = " + str::str(v.w));
	}

	return vec4(atanh(v.x), atanh(v.y), atanh(v.z), atanh(v.w));
}

vec4 atan2(const vec4& Y, const vec4& X)
{
	return vec4(atan2(Y.x, X.x), atan2(Y.y, X.y), atan2(Y.z, X.z), atan2(Y.w, X.w));
}

vec4 exp(const vec4& v)
{
	return vec4(exp(v.x), exp(v.y), exp(v.z), exp(v.w));
}

vec4 log(const vec4& v)
{
	if(v.x <= 0)
	{
		throw glass::ArithmeticError("v.x must be greater than 0. But now v.x = " + str::str(v.x));
	}
	if(v.y <= 0)
	{
		throw glass::ArithmeticError("v.y must be greater than 0. But now v.y = " + str::str(v.y));
	}
	if(v.z <= 0)
	{
		throw glass::ArithmeticError("v.z must be greater than 0. But now v.z = " + str::str(v.z));
	}
	if(v.w <= 0)
	{
		throw glass::ArithmeticError("v.w must be greater than 0. But now v.w = " + str::str(v.w));
	}

	return vec4(log(v.x), log(v.y), log(v.z), log(v.w));
}

vec4 log2(const vec4& v)
{
	if(v.x <= 0)
	{
		throw glass::ArithmeticError("v.x must be greater than 0. But now v.x = " + str::str(v.x));
	}
	if(v.y <= 0)
	{
		throw glass::ArithmeticError("v.y must be greater than 0. But now v.y = " + str::str(v.y));
	}
	if(v.z <= 0)
	{
		throw glass::ArithmeticError("v.z must be greater than 0. But now v.z = " + str::str(v.z));
	}
	if(v.w <= 0)
	{
		throw glass::ArithmeticError("v.w must be greater than 0. But now v.w = " + str::str(v.w));
	}

	return vec4(log2(v.x), log2(v.y), log2(v.z), log2(v.w));
}

vec4 log10(const vec4& v)
{
	if(v.x <= 0)
	{
		throw glass::ArithmeticError("v.x must be greater than 0. But now v.x = " + str::str(v.x));
	}
	if(v.y <= 0)
	{
		throw glass::ArithmeticError("v.y must be greater than 0. But now v.y = " + str::str(v.y));
	}
	if(v.z <= 0)
	{
		throw glass::ArithmeticError("v.z must be greater than 0. But now v.z = " + str::str(v.z));
	}
	if(v.w <= 0)
	{
		throw glass::ArithmeticError("v.w must be greater than 0. But now v.w = " + str::str(v.w));
	}

	return vec4(log10(v.x), log10(v.y), log10(v.z), log10(v.w));
}

vec4 abs(const vec4& v)
{
	return vec4(fabs(v.x), fabs(v.y), fabs(v.z), fabs(v.w));
}

vec4 floor(const vec4& v)
{
	return vec4(floor(v.x), floor(v.y), floor(v.z), floor(v.w));
}

vec4 ceil(const vec4& v)
{
	return vec4(ceil(v.x), ceil(v.y), ceil(v.z), ceil(v.w));
}

vec4 round(const vec4& v, uint n)
{
	return vec4(round(v.x, n), round(v.y, n), round(v.z, n), round(v.w, n));
}

vec4 trunc(const vec4& v)
{
	return vec4(trunc(v.x), trunc(v.y), trunc(v.z), trunc(v.w));
}

vec4 fix(const vec4& v)
{
	return vec4(fix(v.x), fix(v.y), fix(v.z), fix(v.w));
}