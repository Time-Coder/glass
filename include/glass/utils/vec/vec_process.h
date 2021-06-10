#ifndef __VEC_PROCESS__
#define __VEC_PROCESS__

double round(double value, uint n);
double fix(double value);
bool is_zero(double value);
bool is_equal(double value1, double value2);
bool is_int(double value);
double rad2deg(double rad);
double deg2rad(double deg);

dvec2 pow(const dvec2& v1, const dvec2& v2);
dvec2 pow(const dvec2& v, double value);
dvec2 pow(double value, const dvec2& v);

double length(const dvec2& v);
dvec2 normalize(const dvec2& v);
dvec2 reflect(dvec2 v_in, dvec2 v_norm);
double dot(const dvec2& v1, const dvec2& v2);
double cross(const dvec2& v1, const dvec2& v2);
dvec2 mix(const dvec2& v1, const dvec2& v2, double factor);
dvec2 sin(const dvec2& v);
dvec2 cos(const dvec2& v);
dvec2 tan(const dvec2& v);
dvec2 sinh(const dvec2& v);
dvec2 cosh(const dvec2& v);
dvec2 tanh(const dvec2& v);
dvec2 asin(const dvec2& v);
dvec2 acos(const dvec2& v);
dvec2 atan(const dvec2& v);
dvec2 asinh(const dvec2& v);
dvec2 acosh(const dvec2& v);
dvec2 atanh(const dvec2& v);
dvec2 atan2(const dvec2& Y, const dvec2& X);
dvec2 exp(const dvec2& v);
dvec2 log(const dvec2& v);
dvec2 log2(const dvec2& v);
dvec2 log10(const dvec2& v);
dvec2 abs(const dvec2& v);
dvec2 floor(const dvec2& v);
dvec2 ceil(const dvec2& v);
dvec2 trunc(const dvec2& v);
dvec2 round(const dvec2& v, uint n = 0);
dvec2 fix(const dvec2& v);

dvec3 pow(const dvec3& v, const dvec3& v2);
dvec3 pow(const dvec3& v, double value);
dvec3 pow(double value, const dvec3& v);

double length(const dvec3& v);
dvec3 normalize(const dvec3& v);
dvec3 reflect(dvec3 v_in, dvec3 v_norm);
double dot(const dvec3& v1, const dvec3& v2);
dvec3 cross(const dvec3& v1, const dvec3& v2);
dvec3 mix(const dvec3& v1, const dvec3& v2, double factor);
dvec3 sin(const dvec3& v);
dvec3 cos(const dvec3& v);
dvec3 tan(const dvec3& v);
dvec3 sinh(const dvec3& v);
dvec3 cosh(const dvec3& v);
dvec3 tanh(const dvec3& v);
dvec3 asin(const dvec3& v);
dvec3 acos(const dvec3& v);
dvec3 atan(const dvec3& v);
dvec3 asinh(const dvec3& v);
dvec3 acosh(const dvec3& v);
dvec3 atanh(const dvec3& v);
dvec3 atan2(const dvec3& Y, const dvec3& X);
dvec3 exp(const dvec3& v);
dvec3 log(const dvec3& v);
dvec3 log2(const dvec3& v);
dvec3 log10(const dvec3& v);
dvec3 abs(const dvec3& v);
dvec3 floor(const dvec3& v);
dvec3 ceil(const dvec3& v);
dvec3 round(const dvec3& v, uint n);
dvec3 trunc(const dvec3& v);
dvec3 fix(const dvec3& v);

dvec4 pow(const dvec4& v, const dvec4& v2);
dvec4 pow(const dvec4& v, double value);
dvec4 pow(double value, const dvec4& v);

double length(const dvec4& v);
dvec4 normalize(const dvec4& v);
double dot(const dvec4& v1, const dvec4& v2);
dvec4 mix(const dvec4& v1, const dvec4& v2, double factor);
dvec4 sin(const dvec4& v);
dvec4 cos(const dvec4& v);
dvec4 tan(const dvec4& v);
dvec4 sinh(const dvec4& v);
dvec4 cosh(const dvec4& v);
dvec4 tanh(const dvec4& v);
dvec4 asin(const dvec4& v);
dvec4 acos(const dvec4& v);
dvec4 atan(const dvec4& v);
dvec4 asinh(const dvec4& v);
dvec4 acosh(const dvec4& v);
dvec4 atanh(const dvec4& v);
dvec4 atan2(const dvec4& Y, const dvec4& X);
dvec4 exp(const dvec4& v);
dvec4 log(const dvec4& v);
dvec4 log2(const dvec4& v);
dvec4 log10(const dvec4& v);
dvec4 abs(const dvec4& v);
dvec4 floor(const dvec4& v);
dvec4 ceil(const dvec4& v);
dvec4 trunc(const dvec4& v);
dvec4 round(const dvec4& v, uint n);
dvec4 fix(const dvec4& v);

vec2 pow(const vec2& v1, const vec2& v2);
vec2 pow(const vec2& v, double value);
vec2 pow(double value, const vec2& v);

double length(const vec2& v);
vec2 normalize(const vec2& v);
vec2 reflect(vec2 v_in, vec2 v_norm);
double dot(const vec2& v1, const vec2& v2);
double cross(const vec2& v1, const vec2& v2);
vec2 mix(const vec2& v1, const vec2& v2, double factor);
vec2 sin(const vec2& v);
vec2 cos(const vec2& v);
vec2 tan(const vec2& v);
vec2 sinh(const vec2& v);
vec2 cosh(const vec2& v);
vec2 tanh(const vec2& v);
vec2 asin(const vec2& v);
vec2 acos(const vec2& v);
vec2 atan(const vec2& v);
vec2 asinh(const vec2& v);
vec2 acosh(const vec2& v);
vec2 atanh(const vec2& v);
vec2 atan2(const vec2& Y, const vec2& X);
vec2 exp(const vec2& v);
vec2 log(const vec2& v);
vec2 log2(const vec2& v);
vec2 log10(const vec2& v);
vec2 abs(const vec2& v);
vec2 floor(const vec2& v);
vec2 ceil(const vec2& v);
vec2 trunc(const vec2& v);
vec2 round(const vec2& v, uint n = 0);
vec2 fix(const vec2& v);

vec3 pow(const vec3& v, const vec3& v2);
vec3 pow(const vec3& v, double value);
vec3 pow(double value, const vec3& v);

double length(const vec3& v);
vec3 normalize(const vec3& v);
vec3 reflect(vec3 v_in, vec3 v_norm);
double dot(const vec3& v1, const vec3& v2);
vec3 cross(const vec3& v1, const vec3& v2);
vec3 mix(const vec3& v1, const vec3& v2, double factor);
vec3 sin(const vec3& v);
vec3 cos(const vec3& v);
vec3 tan(const vec3& v);
vec3 sinh(const vec3& v);
vec3 cosh(const vec3& v);
vec3 tanh(const vec3& v);
vec3 asin(const vec3& v);
vec3 acos(const vec3& v);
vec3 atan(const vec3& v);
vec3 asinh(const vec3& v);
vec3 acosh(const vec3& v);
vec3 atanh(const vec3& v);
vec3 atan2(const vec3& Y, const vec3& X);
vec3 exp(const vec3& v);
vec3 log(const vec3& v);
vec3 log2(const vec3& v);
vec3 log10(const vec3& v);
vec3 abs(const vec3& v);
vec3 floor(const vec3& v);
vec3 ceil(const vec3& v);
vec3 round(const vec3& v, uint n);
vec3 trunc(const vec3& v);
vec3 fix(const vec3& v);

vec4 pow(const vec4& v, const vec4& v2);
vec4 pow(const vec4& v, double value);
vec4 pow(double value, const vec4& v);

double length(const vec4& v);
vec4 normalize(const vec4& v);
double dot(const vec4& v1, const vec4& v2);
vec4 mix(const vec4& v1, const vec4& v2, double factor);
vec4 sin(const vec4& v);
vec4 cos(const vec4& v);
vec4 tan(const vec4& v);
vec4 sinh(const vec4& v);
vec4 cosh(const vec4& v);
vec4 tanh(const vec4& v);
vec4 asin(const vec4& v);
vec4 acos(const vec4& v);
vec4 atan(const vec4& v);
vec4 asinh(const vec4& v);
vec4 acosh(const vec4& v);
vec4 atanh(const vec4& v);
vec4 atan2(const vec4& Y, const vec4& X);
vec4 exp(const vec4& v);
vec4 log(const vec4& v);
vec4 log2(const vec4& v);
vec4 log10(const vec4& v);
vec4 abs(const vec4& v);
vec4 floor(const vec4& v);
vec4 ceil(const vec4& v);
vec4 trunc(const vec4& v);
vec4 round(const vec4& v, uint n);
vec4 fix(const vec4& v);

#endif