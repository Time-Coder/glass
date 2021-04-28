#ifndef __FVEC_H__
#define __FVEC_H__

#include <iostream>

#ifndef TYPE_UINT
typedef unsigned int uint;
#define TYPE_UINT
#endif

using namespace std;

#ifdef ENABLE_SWIZZLE
#define DEFINE_FLOAT_REFERENCE(vec_type, e1) \
struct e1##_t\
{\
	vec_type* parent;\
	operator float()const;\
	float& operator =(float value);\
	friend ostream& operator <<(ostream& out, const e1##_t& v)\
	{\
		out << (float)v;\
		return out;\
	}\
};

#define DEFINE_VEC2_REFERENCE(vec_type, e1, e2) \
struct e1##e2##_t\
{\
	vec_type* parent;\
	operator vec2();\
	float operator [](uint i)const;\
	float& operator [](uint i);\
	vec_type& operator =(const vec2& v);\
	vec_type& operator =(vec2&& v);\
	friend ostream& operator <<(ostream& out, const e1##e2##_t& v)\
	{\
		out << "vec2(" << v[0] << ", " << v[1] << ")";\
		return out;\
	}\
};

#define DEFINE_VEC3_REFERENCE(vec_type, e1, e2, e3) \
struct e1##e2##e3##_t\
{\
	vec_type* parent;\
	operator vec3();\
	float operator [](uint i)const;\
	float& operator [](uint i);\
	vec_type& operator =(const vec3& v);\
	vec_type& operator =(vec3&& v);\
	friend ostream& operator <<(ostream& out, const e1##e2##e3##_t& v)\
	{\
		out << "vec3(" << v[0] << ", " << v[1] << ", " << v[2] << ")";\
		return out;\
	}\
};

#define DEFINE_VEC4_REFERENCE(vec_type, e1, e2, e3, e4) \
struct e1##e2##e3##e4##_t\
{\
	vec_type* parent;\
	operator vec4();\
	float operator [](uint i)const;\
	float& operator [](uint i);\
	vec_type& operator =(const vec4& v);\
	vec_type& operator =(vec4&& v);\
	friend ostream& operator <<(ostream& out, const e1##e2##e3##e4##_t& v)\
	{\
		out << "vec4(" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ")";\
		return out;\
	}\
};
#endif

struct dvec2;
struct dvec3;
struct dvec4;
struct vec2;
struct vec3;
struct vec4;
struct uvec2;
struct uvec3;
struct uvec4;
struct ivec2;
struct ivec3;
struct ivec4;
struct bvec2;
struct bvec3;
struct bvec4;

struct vec2
{
	float x = 0.0f;
	float y = 0.0f;

#ifdef ENABLE_SWIZZLE
private:
	DEFINE_FLOAT_REFERENCE(vec2, x)
	DEFINE_FLOAT_REFERENCE(vec2, y)
	DEFINE_VEC2_REFERENCE(vec2, x, x)
	DEFINE_VEC2_REFERENCE(vec2, x, y)
	DEFINE_VEC2_REFERENCE(vec2, y, x)
	DEFINE_VEC2_REFERENCE(vec2, y, y)
	DEFINE_VEC3_REFERENCE(vec2, x, x, x)
	DEFINE_VEC3_REFERENCE(vec2, x, x, y)
	DEFINE_VEC3_REFERENCE(vec2, x, y, x)
	DEFINE_VEC3_REFERENCE(vec2, x, y, y)
	DEFINE_VEC3_REFERENCE(vec2, y, x, x)
	DEFINE_VEC3_REFERENCE(vec2, y, x, y)
	DEFINE_VEC3_REFERENCE(vec2, y, y, x)
	DEFINE_VEC3_REFERENCE(vec2, y, y, y)
	DEFINE_VEC4_REFERENCE(vec2, x, x, x, x)
	DEFINE_VEC4_REFERENCE(vec2, x, x, x, y)
	DEFINE_VEC4_REFERENCE(vec2, x, x, y, x)
	DEFINE_VEC4_REFERENCE(vec2, x, x, y, y)
	DEFINE_VEC4_REFERENCE(vec2, x, y, x, x)
	DEFINE_VEC4_REFERENCE(vec2, x, y, x, y)
	DEFINE_VEC4_REFERENCE(vec2, x, y, y, x)
	DEFINE_VEC4_REFERENCE(vec2, x, y, y, y)
	DEFINE_VEC4_REFERENCE(vec2, y, x, x, x)
	DEFINE_VEC4_REFERENCE(vec2, y, x, x, y)
	DEFINE_VEC4_REFERENCE(vec2, y, x, x, z)
	DEFINE_VEC4_REFERENCE(vec2, y, x, y, x)
	DEFINE_VEC4_REFERENCE(vec2, y, x, y, y)
	DEFINE_VEC4_REFERENCE(vec2, y, y, x, x)
	DEFINE_VEC4_REFERENCE(vec2, y, y, x, y)
	DEFINE_VEC4_REFERENCE(vec2, y, y, y, x)
	DEFINE_VEC4_REFERENCE(vec2, y, y, y, y)

public:
	union
	{
		vec2* parent;
		xx_t xx;
		xy_t xy;
		yx_t yx;
		yy_t yy;
		xxx_t xxx;
		xxy_t xxy;
		xyx_t xyx;
		xyy_t xyy;
		yxx_t yxx;
		yxy_t yxy;
		yyx_t yyx;
		yyy_t yyy;
		xxxx_t xxxx;
		xxxy_t xxxy;
		xxyx_t xxyx;
		xxyy_t xxyy;
		xyxx_t xyxx;
		xyxy_t xyxy;
		xyyx_t xyyx;
		xyyy_t xyyy;
		yxxx_t yxxx;
		yxxy_t yxxy;
		yxyx_t yxyx;
		yxyy_t yxyy;
		yyxx_t yyxx;
		yyxy_t yyxy;
		yyyx_t yyyx;
		yyyy_t yyyy;

		x_t r;
		y_t g;
		xx_t rr;
		xy_t rg;
		yx_t gr;
		yy_t gg;
		xxx_t rrr;
		xxy_t rrg;
		xyx_t rgr;
		xyy_t rgg;
		yxx_t grr;
		yxy_t grg;
		yyx_t ggr;
		yyy_t ggg;
		xxxx_t rrrr;
		xxxy_t rrrg;
		xxyx_t rrgr;
		xxyy_t rrgg;
		xyxx_t rgrr;
		xyxy_t rgrg;
		xyyx_t rggr;
		xyyy_t rggg;
		yxxx_t grrr;
		yxxy_t grrg;
		yxyx_t grgr;
		yxyy_t grgg;
		yyxx_t ggrr;
		yyxy_t ggrg;
		yyyx_t gggr;
		yyyy_t gggg;

		x_t s;
		y_t t;
		xx_t ss;
		xy_t st;
		yx_t ts;
		yy_t tt;
		xxx_t sss;
		xxy_t sst;
		xyx_t sts;
		xyy_t stt;
		yxx_t tss;
		yxy_t tst;
		yyx_t tts;
		yyy_t ttt;
		xxxx_t ssss;
		xxxy_t ssst;
		xxyx_t ssts;
		xxyy_t sstt;
		xyxx_t stss;
		xyxy_t stst;
		xyyx_t stts;
		xyyy_t sttt;
		yxxx_t tsss;
		yxxy_t tsst;
		yxyx_t tsts;
		yxyy_t tstt;
		yyxx_t ttss;
		yyxy_t ttst;
		yyyx_t ttts;
		yyyy_t tttt;
	};
#endif

	vec2();
	vec2(float _x, float _y);
	vec2(float value);
	vec2(const vec2& v);
	vec2(const vec3& v);
	vec2(const vec4& v);
	vec2(vec2&& v);
	vec2& operator =(const vec2& v);
	vec2& operator =(vec2&& v);

	operator dvec2()const;
	operator uvec2()const;
	operator ivec2()const;
	operator bvec2()const;

	float* data()const;

	float& operator [](uint i);
	float  operator [](uint i)const;

	vec2 operator +()const;
	vec2 operator -()const;

	vec2& operator +=(const vec2& v);
	vec2& operator -=(const vec2& v);
	vec2& operator *=(const vec2& v);
	vec2& operator /=(const vec2& v);

	vec2& operator +=(float value);
	vec2& operator -=(float value);
	vec2& operator *=(float value);
	vec2& operator /=(float value);

	friend vec2 operator +(const vec2& v1, const vec2& v2);
	friend vec2 operator -(const vec2& v1, const vec2& v2);
	friend vec2 operator *(const vec2& v1, const vec2& v2);
	friend vec2 operator /(const vec2& v1, const vec2& v2);

	friend vec2 operator +(const vec2& v1, const uvec2& v2);
	friend vec2 operator -(const vec2& v1, const uvec2& v2);
	friend vec2 operator *(const vec2& v1, const uvec2& v2);
	friend vec2 operator /(const vec2& v1, const uvec2& v2);

	friend vec2 operator +(const uvec2& v1, const vec2& v2);
	friend vec2 operator -(const uvec2& v1, const vec2& v2);
	friend vec2 operator *(const uvec2& v1, const vec2& v2);
	friend vec2 operator /(const uvec2& v1, const vec2& v2);

	friend vec2 operator +(const vec2& v1, const ivec2& v2);
	friend vec2 operator -(const vec2& v1, const ivec2& v2);
	friend vec2 operator *(const vec2& v1, const ivec2& v2);
	friend vec2 operator /(const vec2& v1, const ivec2& v2);

	friend vec2 operator +(const ivec2& v1, const vec2& v2);
	friend vec2 operator -(const ivec2& v1, const vec2& v2);
	friend vec2 operator *(const ivec2& v1, const vec2& v2);
	friend vec2 operator /(const ivec2& v1, const vec2& v2);

	friend dvec2 operator +(double value, const vec2& v2);
	friend dvec2 operator -(double value, const vec2& v2);
	friend dvec2 operator *(double value, const vec2& v2);
	friend dvec2 operator /(double value, const vec2& v2);

	friend dvec2 operator +(const vec2& v2, double value);
	friend dvec2 operator -(const vec2& v2, double value);
	friend dvec2 operator *(const vec2& v2, double value);
	friend dvec2 operator /(const vec2& v2, double value);

	friend vec2 operator +(float value, const vec2& v2);
	friend vec2 operator -(float value, const vec2& v2);
	friend vec2 operator *(float value, const vec2& v2);
	friend vec2 operator /(float value, const vec2& v2);

	friend vec2 operator +(const vec2& v2, float value);
	friend vec2 operator -(const vec2& v2, float value);
	friend vec2 operator *(const vec2& v2, float value);
	friend vec2 operator /(const vec2& v2, float value);

	friend vec2 operator +(uint value, const vec2& v2);
	friend vec2 operator -(uint value, const vec2& v2);
	friend vec2 operator *(uint value, const vec2& v2);
	friend vec2 operator /(uint value, const vec2& v2);

	friend vec2 operator +(const vec2& v2, uint value);
	friend vec2 operator -(const vec2& v2, uint value);
	friend vec2 operator *(const vec2& v2, uint value);
	friend vec2 operator /(const vec2& v2, uint value);

	friend vec2 operator +(int value, const vec2& v2);
	friend vec2 operator -(int value, const vec2& v2);
	friend vec2 operator *(int value, const vec2& v2);
	friend vec2 operator /(int value, const vec2& v2);

	friend vec2 operator +(const vec2& v2, int value);
	friend vec2 operator -(const vec2& v2, int value);
	friend vec2 operator *(const vec2& v2, int value);
	friend vec2 operator /(const vec2& v2, int value);

	friend ostream& operator <<(ostream& out, const vec2& v);
};

struct vec3
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

#ifdef ENABLE_SWIZZLE
private:
	DEFINE_FLOAT_REFERENCE(vec3, x)
	DEFINE_FLOAT_REFERENCE(vec3, y)
	DEFINE_FLOAT_REFERENCE(vec3, z)
	DEFINE_VEC2_REFERENCE(vec3, x, x)
	DEFINE_VEC2_REFERENCE(vec3, x, y)
	DEFINE_VEC2_REFERENCE(vec3, x, z)
	DEFINE_VEC2_REFERENCE(vec3, y, x)
	DEFINE_VEC2_REFERENCE(vec3, y, y)
	DEFINE_VEC2_REFERENCE(vec3, y, z)
	DEFINE_VEC2_REFERENCE(vec3, z, x)
	DEFINE_VEC2_REFERENCE(vec3, z, y)
	DEFINE_VEC2_REFERENCE(vec3, z, z)
	DEFINE_VEC3_REFERENCE(vec3, x, x, x)
	DEFINE_VEC3_REFERENCE(vec3, x, x, y)
	DEFINE_VEC3_REFERENCE(vec3, x, x, z)
	DEFINE_VEC3_REFERENCE(vec3, x, y, x)
	DEFINE_VEC3_REFERENCE(vec3, x, y, y)
	DEFINE_VEC3_REFERENCE(vec3, x, y, z)
	DEFINE_VEC3_REFERENCE(vec3, x, z, x)
	DEFINE_VEC3_REFERENCE(vec3, x, z, y)
	DEFINE_VEC3_REFERENCE(vec3, x, z, z)
	DEFINE_VEC3_REFERENCE(vec3, y, x, x)
	DEFINE_VEC3_REFERENCE(vec3, y, x, y)
	DEFINE_VEC3_REFERENCE(vec3, y, x, z)
	DEFINE_VEC3_REFERENCE(vec3, y, y, x)
	DEFINE_VEC3_REFERENCE(vec3, y, y, y)
	DEFINE_VEC3_REFERENCE(vec3, y, y, z)
	DEFINE_VEC3_REFERENCE(vec3, y, z, x)
	DEFINE_VEC3_REFERENCE(vec3, y, z, y)
	DEFINE_VEC3_REFERENCE(vec3, y, z, z)
	DEFINE_VEC3_REFERENCE(vec3, z, x, x)
	DEFINE_VEC3_REFERENCE(vec3, z, x, y)
	DEFINE_VEC3_REFERENCE(vec3, z, x, z)
	DEFINE_VEC3_REFERENCE(vec3, z, y, x)
	DEFINE_VEC3_REFERENCE(vec3, z, y, y)
	DEFINE_VEC3_REFERENCE(vec3, z, y, z)
	DEFINE_VEC3_REFERENCE(vec3, z, z, x)
	DEFINE_VEC3_REFERENCE(vec3, z, z, y)
	DEFINE_VEC3_REFERENCE(vec3, z, z, z)
	DEFINE_VEC4_REFERENCE(vec3, x, x, x, x)
	DEFINE_VEC4_REFERENCE(vec3, x, x, x, y)
	DEFINE_VEC4_REFERENCE(vec3, x, x, x, z)
	DEFINE_VEC4_REFERENCE(vec3, x, x, y, x)
	DEFINE_VEC4_REFERENCE(vec3, x, x, y, y)
	DEFINE_VEC4_REFERENCE(vec3, x, x, y, z)
	DEFINE_VEC4_REFERENCE(vec3, x, x, z, x)
	DEFINE_VEC4_REFERENCE(vec3, x, x, z, y)
	DEFINE_VEC4_REFERENCE(vec3, x, x, z, z)
	DEFINE_VEC4_REFERENCE(vec3, x, y, x, x)
	DEFINE_VEC4_REFERENCE(vec3, x, y, x, y)
	DEFINE_VEC4_REFERENCE(vec3, x, y, x, z)
	DEFINE_VEC4_REFERENCE(vec3, x, y, y, x)
	DEFINE_VEC4_REFERENCE(vec3, x, y, y, y)
	DEFINE_VEC4_REFERENCE(vec3, x, y, y, z)
	DEFINE_VEC4_REFERENCE(vec3, x, y, z, x)
	DEFINE_VEC4_REFERENCE(vec3, x, y, z, y)
	DEFINE_VEC4_REFERENCE(vec3, x, y, z, z)
	DEFINE_VEC4_REFERENCE(vec3, x, z, x, x)
	DEFINE_VEC4_REFERENCE(vec3, x, z, x, y)
	DEFINE_VEC4_REFERENCE(vec3, x, z, x, z)
	DEFINE_VEC4_REFERENCE(vec3, x, z, y, x)
	DEFINE_VEC4_REFERENCE(vec3, x, z, y, y)
	DEFINE_VEC4_REFERENCE(vec3, x, z, y, z)
	DEFINE_VEC4_REFERENCE(vec3, x, z, z, x)
	DEFINE_VEC4_REFERENCE(vec3, x, z, z, y)
	DEFINE_VEC4_REFERENCE(vec3, x, z, z, z)
	DEFINE_VEC4_REFERENCE(vec3, y, x, x, x)
	DEFINE_VEC4_REFERENCE(vec3, y, x, x, y)
	DEFINE_VEC4_REFERENCE(vec3, y, x, x, z)
	DEFINE_VEC4_REFERENCE(vec3, y, x, y, x)
	DEFINE_VEC4_REFERENCE(vec3, y, x, y, y)
	DEFINE_VEC4_REFERENCE(vec3, y, x, y, z)
	DEFINE_VEC4_REFERENCE(vec3, y, x, z, x)
	DEFINE_VEC4_REFERENCE(vec3, y, x, z, y)
	DEFINE_VEC4_REFERENCE(vec3, y, x, z, z)
	DEFINE_VEC4_REFERENCE(vec3, y, y, x, x)
	DEFINE_VEC4_REFERENCE(vec3, y, y, x, y)
	DEFINE_VEC4_REFERENCE(vec3, y, y, x, z)
	DEFINE_VEC4_REFERENCE(vec3, y, y, y, x)
	DEFINE_VEC4_REFERENCE(vec3, y, y, y, y)
	DEFINE_VEC4_REFERENCE(vec3, y, y, y, z)
	DEFINE_VEC4_REFERENCE(vec3, y, y, z, x)
	DEFINE_VEC4_REFERENCE(vec3, y, y, z, y)
	DEFINE_VEC4_REFERENCE(vec3, y, y, z, z)
	DEFINE_VEC4_REFERENCE(vec3, y, z, x, x)
	DEFINE_VEC4_REFERENCE(vec3, y, z, x, y)
	DEFINE_VEC4_REFERENCE(vec3, y, z, x, z)
	DEFINE_VEC4_REFERENCE(vec3, y, z, y, x)
	DEFINE_VEC4_REFERENCE(vec3, y, z, y, y)
	DEFINE_VEC4_REFERENCE(vec3, y, z, y, z)
	DEFINE_VEC4_REFERENCE(vec3, y, z, z, x)
	DEFINE_VEC4_REFERENCE(vec3, y, z, z, y)
	DEFINE_VEC4_REFERENCE(vec3, y, z, z, z)
	DEFINE_VEC4_REFERENCE(vec3, z, x, x, x)
	DEFINE_VEC4_REFERENCE(vec3, z, x, x, y)
	DEFINE_VEC4_REFERENCE(vec3, z, x, x, z)
	DEFINE_VEC4_REFERENCE(vec3, z, x, y, x)
	DEFINE_VEC4_REFERENCE(vec3, z, x, y, y)
	DEFINE_VEC4_REFERENCE(vec3, z, x, y, z)
	DEFINE_VEC4_REFERENCE(vec3, z, x, z, x)
	DEFINE_VEC4_REFERENCE(vec3, z, x, z, y)
	DEFINE_VEC4_REFERENCE(vec3, z, x, z, z)
	DEFINE_VEC4_REFERENCE(vec3, z, y, x, x)
	DEFINE_VEC4_REFERENCE(vec3, z, y, x, y)
	DEFINE_VEC4_REFERENCE(vec3, z, y, x, z)
	DEFINE_VEC4_REFERENCE(vec3, z, y, y, x)
	DEFINE_VEC4_REFERENCE(vec3, z, y, y, y)
	DEFINE_VEC4_REFERENCE(vec3, z, y, y, z)
	DEFINE_VEC4_REFERENCE(vec3, z, y, z, x)
	DEFINE_VEC4_REFERENCE(vec3, z, y, z, y)
	DEFINE_VEC4_REFERENCE(vec3, z, y, z, z)
	DEFINE_VEC4_REFERENCE(vec3, z, z, x, x)
	DEFINE_VEC4_REFERENCE(vec3, z, z, x, y)
	DEFINE_VEC4_REFERENCE(vec3, z, z, x, z)
	DEFINE_VEC4_REFERENCE(vec3, z, z, y, x)
	DEFINE_VEC4_REFERENCE(vec3, z, z, y, y)
	DEFINE_VEC4_REFERENCE(vec3, z, z, y, z)
	DEFINE_VEC4_REFERENCE(vec3, z, z, z, x)
	DEFINE_VEC4_REFERENCE(vec3, z, z, z, y)
	DEFINE_VEC4_REFERENCE(vec3, z, z, z, z)

public:
	union
	{
		vec3* parent;
		xx_t xx;
		xy_t xy;
		xz_t xz;
		yx_t yx;
		yy_t yy;
		yz_t yz;
		zx_t zx;
		zy_t zy;
		zz_t zz;
		xxx_t xxx;
		xxy_t xxy;
		xxz_t xxz;
		xyx_t xyx;
		xyy_t xyy;
		xyz_t xyz;
		xzx_t xzx;
		xzy_t xzy;
		xzz_t xzz;
		yxx_t yxx;
		yxy_t yxy;
		yxz_t yxz;
		yyx_t yyx;
		yyy_t yyy;
		yyz_t yyz;
		yzx_t yzx;
		yzy_t yzy;
		yzz_t yzz;
		zxx_t zxx;
		zxy_t zxy;
		zxz_t zxz;
		zyx_t zyx;
		zyy_t zyy;
		zyz_t zyz;
		zzx_t zzx;
		zzy_t zzy;
		zzz_t zzz;
		xxxx_t xxxx;
		xxxy_t xxxy;
		xxxz_t xxxz;
		xxyx_t xxyx;
		xxyy_t xxyy;
		xxyz_t xxyz;
		xxzx_t xxzx;
		xxzy_t xxzy;
		xxzz_t xxzz;
		xyxx_t xyxx;
		xyxy_t xyxy;
		xyxz_t xyxz;
		xyyx_t xyyx;
		xyyy_t xyyy;
		xyyz_t xyyz;
		xyzx_t xyzx;
		xyzy_t xyzy;
		xyzz_t xyzz;
		xzxx_t xzxx;
		xzxy_t xzxy;
		xzxz_t xzxz;
		xzyx_t xzyx;
		xzyy_t xzyy;
		xzyz_t xzyz;
		xzzx_t xzzx;
		xzzy_t xzzy;
		xzzz_t xzzz;
		yxxx_t yxxx;
		yxxy_t yxxy;
		yxxz_t yxxz;
		yxyx_t yxyx;
		yxyy_t yxyy;
		yxyz_t yxyz;
		yxzx_t yxzx;
		yxzy_t yxzy;
		yxzz_t yxzz;
		yyxx_t yyxx;
		yyxy_t yyxy;
		yyxz_t yyxz;
		yyyx_t yyyx;
		yyyy_t yyyy;
		yyyz_t yyyz;
		yyzx_t yyzx;
		yyzy_t yyzy;
		yyzz_t yyzz;
		yzxx_t yzxx;
		yzxy_t yzxy;
		yzxz_t yzxz;
		yzyx_t yzyx;
		yzyy_t yzyy;
		yzyz_t yzyz;
		yzzx_t yzzx;
		yzzy_t yzzy;
		yzzz_t yzzz;
		zxxx_t zxxx;
		zxxy_t zxxy;
		zxxz_t zxxz;
		zxyx_t zxyx;
		zxyy_t zxyy;
		zxyz_t zxyz;
		zxzx_t zxzx;
		zxzy_t zxzy;
		zxzz_t zxzz;
		zyxx_t zyxx;
		zyxy_t zyxy;
		zyxz_t zyxz;
		zyyx_t zyyx;
		zyyy_t zyyy;
		zyyz_t zyyz;
		zyzx_t zyzx;
		zyzy_t zyzy;
		zyzz_t zyzz;
		zzxx_t zzxx;
		zzxy_t zzxy;
		zzxz_t zzxz;
		zzyx_t zzyx;
		zzyy_t zzyy;
		zzyz_t zzyz;
		zzzx_t zzzx;
		zzzy_t zzzy;
		zzzz_t zzzz;

		x_t r;
		y_t g;
		z_t b;
		xx_t rr;
		xy_t rg;
		xz_t rb;
		yx_t gr;
		yy_t gg;
		yz_t gb;
		zx_t br;
		zy_t bg;
		zz_t bb;
		xxx_t rrr;
		xxy_t rrg;
		xxz_t rrb;
		xyx_t rgr;
		xyy_t rgg;
		xyz_t rgb;
		xzx_t rbr;
		xzy_t rbg;
		xzz_t rbb;
		yxx_t grr;
		yxy_t grg;
		yxz_t grb;
		yyx_t ggr;
		yyy_t ggg;
		yyz_t ggb;
		yzx_t gbr;
		yzy_t gbg;
		yzz_t gbb;
		zxx_t brr;
		zxy_t brg;
		zxz_t brb;
		zyx_t bgr;
		zyy_t bgg;
		zyz_t bgb;
		zzx_t bbr;
		zzy_t bbg;
		zzz_t bbb;
		xxxx_t rrrr;
		xxxy_t rrrg;
		xxxz_t rrrb;
		xxyx_t rrgr;
		xxyy_t rrgg;
		xxyz_t rrgb;
		xxzx_t rrbr;
		xxzy_t rrbg;
		xxzz_t rrbb;
		xyxx_t rgrr;
		xyxy_t rgrg;
		xyxz_t rgrb;
		xyyx_t rggr;
		xyyy_t rggg;
		xyyz_t rggb;
		xyzx_t rgbr;
		xyzy_t rgbg;
		xyzz_t rgbb;
		xzxx_t rbrr;
		xzxy_t rbrg;
		xzxz_t rbrb;
		xzyx_t rbgr;
		xzyy_t rbgg;
		xzyz_t rbgb;
		xzzx_t rbbr;
		xzzy_t rbbg;
		xzzz_t rbbb;
		yxxx_t grrr;
		yxxy_t grrg;
		yxxz_t grrb;
		yxyx_t grgr;
		yxyy_t grgg;
		yxyz_t grgb;
		yxzx_t grbr;
		yxzy_t grbg;
		yxzz_t grbb;
		yyxx_t ggrr;
		yyxy_t ggrg;
		yyxz_t ggrb;
		yyyx_t gggr;
		yyyy_t gggg;
		yyyz_t gggb;
		yyzx_t ggbr;
		yyzy_t ggbg;
		yyzz_t ggbb;
		yzxx_t gbrr;
		yzxy_t gbrg;
		yzxz_t gbrb;
		yzyx_t gbgr;
		yzyy_t gbgg;
		yzyz_t gbgb;
		yzzx_t gbbr;
		yzzy_t gbbg;
		yzzz_t gbbb;
		zxxx_t brrr;
		zxxy_t brrg;
		zxxz_t brrb;
		zxyx_t brgr;
		zxyy_t brgg;
		zxyz_t brgb;
		zxzx_t brbr;
		zxzy_t brbg;
		zxzz_t brbb;
		zyxx_t bgrr;
		zyxy_t bgrg;
		zyxz_t bgrb;
		zyyx_t bggr;
		zyyy_t bggg;
		zyyz_t bggb;
		zyzx_t bgbr;
		zyzy_t bgbg;
		zyzz_t bgbb;
		zzxx_t bbrr;
		zzxy_t bbrg;
		zzxz_t bbrb;
		zzyx_t bbgr;
		zzyy_t bbgg;
		zzyz_t bbgb;
		zzzx_t bbbr;
		zzzy_t bbbg;
		zzzz_t bbbb;

		x_t s;
		y_t t;
		z_t p;
		xx_t ss;
		xy_t st;
		xz_t sp;
		yx_t ts;
		yy_t tt;
		yz_t tp;
		zx_t ps;
		zy_t pt;
		zz_t pp;
		xxx_t sss;
		xxy_t sst;
		xxz_t ssp;
		xyx_t sts;
		xyy_t stt;
		xyz_t stp;
		xzx_t sps;
		xzy_t spt;
		xzz_t spp;
		yxx_t tss;
		yxy_t tst;
		yxz_t tsp;
		yyx_t tts;
		yyy_t ttt;
		yyz_t ttp;
		yzx_t tps;
		yzy_t tpt;
		yzz_t tpp;
		zxx_t pss;
		zxy_t pst;
		zxz_t psp;
		zyx_t pts;
		zyy_t ptt;
		zyz_t ptp;
		zzx_t pps;
		zzy_t ppt;
		zzz_t ppp;
		xxxx_t ssss;
		xxxy_t ssst;
		xxxz_t sssp;
		xxyx_t ssts;
		xxyy_t sstt;
		xxyz_t sstp;
		xxzx_t ssps;
		xxzy_t sspt;
		xxzz_t sspp;
		xyxx_t stss;
		xyxy_t stst;
		xyxz_t stsp;
		xyyx_t stts;
		xyyy_t sttt;
		xyyz_t sttp;
		xyzx_t stps;
		xyzy_t stpt;
		xyzz_t stpp;
		xzxx_t spss;
		xzxy_t spst;
		xzxz_t spsp;
		xzyx_t spts;
		xzyy_t sptt;
		xzyz_t sptp;
		xzzx_t spps;
		xzzy_t sppt;
		xzzz_t sppp;
		yxxx_t tsss;
		yxxy_t tsst;
		yxxz_t tssp;
		yxyx_t tsts;
		yxyy_t tstt;
		yxyz_t tstp;
		yxzx_t tsps;
		yxzy_t tspt;
		yxzz_t tspp;
		yyxx_t ttss;
		yyxy_t ttst;
		yyxz_t ttsp;
		yyyx_t ttts;
		yyyy_t tttt;
		yyyz_t tttp;
		yyzx_t ttps;
		yyzy_t ttpt;
		yyzz_t ttpp;
		yzxx_t tpss;
		yzxy_t tpst;
		yzxz_t tpsp;
		yzyx_t tpts;
		yzyy_t tptt;
		yzyz_t tptp;
		yzzx_t tpps;
		yzzy_t tppt;
		yzzz_t tppp;
		zxxx_t psss;
		zxxy_t psst;
		zxxz_t pssp;
		zxyx_t psts;
		zxyy_t pstt;
		zxyz_t pstp;
		zxzx_t psps;
		zxzy_t pspt;
		zxzz_t pspp;
		zyxx_t ptss;
		zyxy_t ptst;
		zyxz_t ptsp;
		zyyx_t ptts;
		zyyy_t pttt;
		zyyz_t pttp;
		zyzx_t ptps;
		zyzy_t ptpt;
		zyzz_t ptpp;
		zzxx_t ppss;
		zzxy_t ppst;
		zzxz_t ppsp;
		zzyx_t ppts;
		zzyy_t pptt;
		zzyz_t pptp;
		zzzx_t ppps;
		zzzy_t pppt;
		zzzz_t pppp;
	};
#endif

	vec3();
	vec3(float value);
	vec3(float _x, float _y, float _z);
	vec3(const vec2& v, float _z);
	vec3(float _x, const vec2& v);
	vec3(const vec3& v);
	vec3(const vec4& v);
	vec3(vec3&& v);
	vec3& operator =(const vec3& v);
	vec3& operator =(vec3&& v);

	operator dvec3()const;
	operator uvec3()const;
	operator ivec3()const;
	operator bvec3()const;

	float* data()const;

	float& operator [](uint i);
	float  operator [](uint i)const;

	vec3 operator +()const;
	vec3 operator -()const;

	vec3& operator +=(const vec3& v);
	vec3& operator -=(const vec3& v);
	vec3& operator *=(const vec3& v);
	vec3& operator /=(const vec3& v);

	vec3& operator +=(float value);
	vec3& operator -=(float value);
	vec3& operator *=(float value);
	vec3& operator /=(float value);

	friend vec3 operator +(const vec3& v1, const vec3& v2);
	friend vec3 operator -(const vec3& v1, const vec3& v2);
	friend vec3 operator *(const vec3& v1, const vec3& v2);
	friend vec3 operator /(const vec3& v1, const vec3& v2);

	friend vec3 operator +(const vec3& v1, const uvec3& v2);
	friend vec3 operator -(const vec3& v1, const uvec3& v2);
	friend vec3 operator *(const vec3& v1, const uvec3& v2);
	friend vec3 operator /(const vec3& v1, const uvec3& v2);

	friend vec3 operator +(const uvec3& v1, const vec3& v2);
	friend vec3 operator -(const uvec3& v1, const vec3& v2);
	friend vec3 operator *(const uvec3& v1, const vec3& v2);
	friend vec3 operator /(const uvec3& v1, const vec3& v2);

	friend vec3 operator +(const vec3& v1, const ivec3& v2);
	friend vec3 operator -(const vec3& v1, const ivec3& v2);
	friend vec3 operator *(const vec3& v1, const ivec3& v2);
	friend vec3 operator /(const vec3& v1, const ivec3& v2);

	friend vec3 operator +(const ivec3& v1, const vec3& v2);
	friend vec3 operator -(const ivec3& v1, const vec3& v2);
	friend vec3 operator *(const ivec3& v1, const vec3& v2);
	friend vec3 operator /(const ivec3& v1, const vec3& v2);

	friend dvec3 operator +(double value, const vec3& v2);
	friend dvec3 operator -(double value, const vec3& v2);
	friend dvec3 operator *(double value, const vec3& v2);
	friend dvec3 operator /(double value, const vec3& v2);

	friend dvec3 operator +(const vec3& v2, double value);
	friend dvec3 operator -(const vec3& v2, double value);
	friend dvec3 operator *(const vec3& v2, double value);
	friend dvec3 operator /(const vec3& v2, double value);

	friend vec3 operator +(float value, const vec3& v2);
	friend vec3 operator -(float value, const vec3& v2);
	friend vec3 operator *(float value, const vec3& v2);
	friend vec3 operator /(float value, const vec3& v2);

	friend vec3 operator +(const vec3& v2, float value);
	friend vec3 operator -(const vec3& v2, float value);
	friend vec3 operator *(const vec3& v2, float value);
	friend vec3 operator /(const vec3& v2, float value);

	friend vec3 operator +(uint value, const vec3& v2);
	friend vec3 operator -(uint value, const vec3& v2);
	friend vec3 operator *(uint value, const vec3& v2);
	friend vec3 operator /(uint value, const vec3& v2);

	friend vec3 operator +(const vec3& v2, uint value);
	friend vec3 operator -(const vec3& v2, uint value);
	friend vec3 operator *(const vec3& v2, uint value);
	friend vec3 operator /(const vec3& v2, uint value);

	friend vec3 operator +(int value, const vec3& v2);
	friend vec3 operator -(int value, const vec3& v2);
	friend vec3 operator *(int value, const vec3& v2);
	friend vec3 operator /(int value, const vec3& v2);

	friend vec3 operator +(const vec3& v2, int value);
	friend vec3 operator -(const vec3& v2, int value);
	friend vec3 operator *(const vec3& v2, int value);
	friend vec3 operator /(const vec3& v2, int value);

	friend ostream& operator <<(ostream& out, const vec3& v);
};

struct vec4
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;

#ifdef ENABLE_SWIZZLE
private:
	DEFINE_FLOAT_REFERENCE(vec4, x)
	DEFINE_FLOAT_REFERENCE(vec4, y)
	DEFINE_FLOAT_REFERENCE(vec4, z)
	DEFINE_FLOAT_REFERENCE(vec4, w)
	DEFINE_VEC2_REFERENCE(vec4, x, x)
	DEFINE_VEC2_REFERENCE(vec4, x, y)
	DEFINE_VEC2_REFERENCE(vec4, x, z)
	DEFINE_VEC2_REFERENCE(vec4, x, w)
	DEFINE_VEC2_REFERENCE(vec4, y, x)
	DEFINE_VEC2_REFERENCE(vec4, y, y)
	DEFINE_VEC2_REFERENCE(vec4, y, z)
	DEFINE_VEC2_REFERENCE(vec4, y, w)
	DEFINE_VEC2_REFERENCE(vec4, z, x)
	DEFINE_VEC2_REFERENCE(vec4, z, y)
	DEFINE_VEC2_REFERENCE(vec4, z, z)
	DEFINE_VEC2_REFERENCE(vec4, z, w)
	DEFINE_VEC2_REFERENCE(vec4, w, x)
	DEFINE_VEC2_REFERENCE(vec4, w, y)
	DEFINE_VEC2_REFERENCE(vec4, w, z)
	DEFINE_VEC2_REFERENCE(vec4, w, w)
	DEFINE_VEC3_REFERENCE(vec4, x, x, x)
	DEFINE_VEC3_REFERENCE(vec4, x, x, y)
	DEFINE_VEC3_REFERENCE(vec4, x, x, z)
	DEFINE_VEC3_REFERENCE(vec4, x, x, w)
	DEFINE_VEC3_REFERENCE(vec4, x, y, x)
	DEFINE_VEC3_REFERENCE(vec4, x, y, y)
	DEFINE_VEC3_REFERENCE(vec4, x, y, z)
	DEFINE_VEC3_REFERENCE(vec4, x, y, w)
	DEFINE_VEC3_REFERENCE(vec4, x, z, x)
	DEFINE_VEC3_REFERENCE(vec4, x, z, y)
	DEFINE_VEC3_REFERENCE(vec4, x, z, z)
	DEFINE_VEC3_REFERENCE(vec4, x, z, w)
	DEFINE_VEC3_REFERENCE(vec4, x, w, x)
	DEFINE_VEC3_REFERENCE(vec4, x, w, y)
	DEFINE_VEC3_REFERENCE(vec4, x, w, z)
	DEFINE_VEC3_REFERENCE(vec4, x, w, w)
	DEFINE_VEC3_REFERENCE(vec4, y, x, x)
	DEFINE_VEC3_REFERENCE(vec4, y, x, y)
	DEFINE_VEC3_REFERENCE(vec4, y, x, z)
	DEFINE_VEC3_REFERENCE(vec4, y, x, w)
	DEFINE_VEC3_REFERENCE(vec4, y, y, x)
	DEFINE_VEC3_REFERENCE(vec4, y, y, y)
	DEFINE_VEC3_REFERENCE(vec4, y, y, z)
	DEFINE_VEC3_REFERENCE(vec4, y, y, w)
	DEFINE_VEC3_REFERENCE(vec4, y, z, x)
	DEFINE_VEC3_REFERENCE(vec4, y, z, y)
	DEFINE_VEC3_REFERENCE(vec4, y, z, z)
	DEFINE_VEC3_REFERENCE(vec4, y, z, w)
	DEFINE_VEC3_REFERENCE(vec4, y, w, x)
	DEFINE_VEC3_REFERENCE(vec4, y, w, y)
	DEFINE_VEC3_REFERENCE(vec4, y, w, z)
	DEFINE_VEC3_REFERENCE(vec4, y, w, w)
	DEFINE_VEC3_REFERENCE(vec4, z, x, x)
	DEFINE_VEC3_REFERENCE(vec4, z, x, y)
	DEFINE_VEC3_REFERENCE(vec4, z, x, z)
	DEFINE_VEC3_REFERENCE(vec4, z, x, w)
	DEFINE_VEC3_REFERENCE(vec4, z, y, x)
	DEFINE_VEC3_REFERENCE(vec4, z, y, y)
	DEFINE_VEC3_REFERENCE(vec4, z, y, z)
	DEFINE_VEC3_REFERENCE(vec4, z, y, w)
	DEFINE_VEC3_REFERENCE(vec4, z, z, x)
	DEFINE_VEC3_REFERENCE(vec4, z, z, y)
	DEFINE_VEC3_REFERENCE(vec4, z, z, z)
	DEFINE_VEC3_REFERENCE(vec4, z, z, w)
	DEFINE_VEC3_REFERENCE(vec4, z, w, x)
	DEFINE_VEC3_REFERENCE(vec4, z, w, y)
	DEFINE_VEC3_REFERENCE(vec4, z, w, z)
	DEFINE_VEC3_REFERENCE(vec4, z, w, w)
	DEFINE_VEC3_REFERENCE(vec4, w, x, x)
	DEFINE_VEC3_REFERENCE(vec4, w, x, y)
	DEFINE_VEC3_REFERENCE(vec4, w, x, z)
	DEFINE_VEC3_REFERENCE(vec4, w, x, w)
	DEFINE_VEC3_REFERENCE(vec4, w, y, x)
	DEFINE_VEC3_REFERENCE(vec4, w, y, y)
	DEFINE_VEC3_REFERENCE(vec4, w, y, z)
	DEFINE_VEC3_REFERENCE(vec4, w, y, w)
	DEFINE_VEC3_REFERENCE(vec4, w, z, x)
	DEFINE_VEC3_REFERENCE(vec4, w, z, y)
	DEFINE_VEC3_REFERENCE(vec4, w, z, z)
	DEFINE_VEC3_REFERENCE(vec4, w, z, w)
	DEFINE_VEC3_REFERENCE(vec4, w, w, x)
	DEFINE_VEC3_REFERENCE(vec4, w, w, y)
	DEFINE_VEC3_REFERENCE(vec4, w, w, z)
	DEFINE_VEC3_REFERENCE(vec4, w, w, w)
	DEFINE_VEC4_REFERENCE(vec4, x, x, x, x)
	DEFINE_VEC4_REFERENCE(vec4, x, x, x, y)
	DEFINE_VEC4_REFERENCE(vec4, x, x, x, z)
	DEFINE_VEC4_REFERENCE(vec4, x, x, x, w)
	DEFINE_VEC4_REFERENCE(vec4, x, x, y, x)
	DEFINE_VEC4_REFERENCE(vec4, x, x, y, y)
	DEFINE_VEC4_REFERENCE(vec4, x, x, y, z)
	DEFINE_VEC4_REFERENCE(vec4, x, x, y, w)
	DEFINE_VEC4_REFERENCE(vec4, x, x, z, x)
	DEFINE_VEC4_REFERENCE(vec4, x, x, z, y)
	DEFINE_VEC4_REFERENCE(vec4, x, x, z, z)
	DEFINE_VEC4_REFERENCE(vec4, x, x, z, w)
	DEFINE_VEC4_REFERENCE(vec4, x, x, w, x)
	DEFINE_VEC4_REFERENCE(vec4, x, x, w, y)
	DEFINE_VEC4_REFERENCE(vec4, x, x, w, z)
	DEFINE_VEC4_REFERENCE(vec4, x, x, w, w)
	DEFINE_VEC4_REFERENCE(vec4, x, y, x, x)
	DEFINE_VEC4_REFERENCE(vec4, x, y, x, y)
	DEFINE_VEC4_REFERENCE(vec4, x, y, x, z)
	DEFINE_VEC4_REFERENCE(vec4, x, y, x, w)
	DEFINE_VEC4_REFERENCE(vec4, x, y, y, x)
	DEFINE_VEC4_REFERENCE(vec4, x, y, y, y)
	DEFINE_VEC4_REFERENCE(vec4, x, y, y, z)
	DEFINE_VEC4_REFERENCE(vec4, x, y, y, w)
	DEFINE_VEC4_REFERENCE(vec4, x, y, z, x)
	DEFINE_VEC4_REFERENCE(vec4, x, y, z, y)
	DEFINE_VEC4_REFERENCE(vec4, x, y, z, z)
	DEFINE_VEC4_REFERENCE(vec4, x, y, z, w)
	DEFINE_VEC4_REFERENCE(vec4, x, y, w, x)
	DEFINE_VEC4_REFERENCE(vec4, x, y, w, y)
	DEFINE_VEC4_REFERENCE(vec4, x, y, w, z)
	DEFINE_VEC4_REFERENCE(vec4, x, y, w, w)
	DEFINE_VEC4_REFERENCE(vec4, x, z, x, x)
	DEFINE_VEC4_REFERENCE(vec4, x, z, x, y)
	DEFINE_VEC4_REFERENCE(vec4, x, z, x, z)
	DEFINE_VEC4_REFERENCE(vec4, x, z, x, w)
	DEFINE_VEC4_REFERENCE(vec4, x, z, y, x)
	DEFINE_VEC4_REFERENCE(vec4, x, z, y, y)
	DEFINE_VEC4_REFERENCE(vec4, x, z, y, z)
	DEFINE_VEC4_REFERENCE(vec4, x, z, y, w)
	DEFINE_VEC4_REFERENCE(vec4, x, z, z, x)
	DEFINE_VEC4_REFERENCE(vec4, x, z, z, y)
	DEFINE_VEC4_REFERENCE(vec4, x, z, z, z)
	DEFINE_VEC4_REFERENCE(vec4, x, z, z, w)
	DEFINE_VEC4_REFERENCE(vec4, x, z, w, x)
	DEFINE_VEC4_REFERENCE(vec4, x, z, w, y)
	DEFINE_VEC4_REFERENCE(vec4, x, z, w, z)
	DEFINE_VEC4_REFERENCE(vec4, x, z, w, w)
	DEFINE_VEC4_REFERENCE(vec4, x, w, x, x)
	DEFINE_VEC4_REFERENCE(vec4, x, w, x, y)
	DEFINE_VEC4_REFERENCE(vec4, x, w, x, z)
	DEFINE_VEC4_REFERENCE(vec4, x, w, x, w)
	DEFINE_VEC4_REFERENCE(vec4, x, w, y, x)
	DEFINE_VEC4_REFERENCE(vec4, x, w, y, y)
	DEFINE_VEC4_REFERENCE(vec4, x, w, y, z)
	DEFINE_VEC4_REFERENCE(vec4, x, w, y, w)
	DEFINE_VEC4_REFERENCE(vec4, x, w, z, x)
	DEFINE_VEC4_REFERENCE(vec4, x, w, z, y)
	DEFINE_VEC4_REFERENCE(vec4, x, w, z, z)
	DEFINE_VEC4_REFERENCE(vec4, x, w, z, w)
	DEFINE_VEC4_REFERENCE(vec4, x, w, w, x)
	DEFINE_VEC4_REFERENCE(vec4, x, w, w, y)
	DEFINE_VEC4_REFERENCE(vec4, x, w, w, z)
	DEFINE_VEC4_REFERENCE(vec4, x, w, w, w)
	DEFINE_VEC4_REFERENCE(vec4, y, x, x, x)
	DEFINE_VEC4_REFERENCE(vec4, y, x, x, y)
	DEFINE_VEC4_REFERENCE(vec4, y, x, x, z)
	DEFINE_VEC4_REFERENCE(vec4, y, x, x, w)
	DEFINE_VEC4_REFERENCE(vec4, y, x, y, x)
	DEFINE_VEC4_REFERENCE(vec4, y, x, y, y)
	DEFINE_VEC4_REFERENCE(vec4, y, x, y, z)
	DEFINE_VEC4_REFERENCE(vec4, y, x, y, w)
	DEFINE_VEC4_REFERENCE(vec4, y, x, z, x)
	DEFINE_VEC4_REFERENCE(vec4, y, x, z, y)
	DEFINE_VEC4_REFERENCE(vec4, y, x, z, z)
	DEFINE_VEC4_REFERENCE(vec4, y, x, z, w)
	DEFINE_VEC4_REFERENCE(vec4, y, x, w, x)
	DEFINE_VEC4_REFERENCE(vec4, y, x, w, y)
	DEFINE_VEC4_REFERENCE(vec4, y, x, w, z)
	DEFINE_VEC4_REFERENCE(vec4, y, x, w, w)
	DEFINE_VEC4_REFERENCE(vec4, y, y, x, x)
	DEFINE_VEC4_REFERENCE(vec4, y, y, x, y)
	DEFINE_VEC4_REFERENCE(vec4, y, y, x, z)
	DEFINE_VEC4_REFERENCE(vec4, y, y, x, w)
	DEFINE_VEC4_REFERENCE(vec4, y, y, y, x)
	DEFINE_VEC4_REFERENCE(vec4, y, y, y, y)
	DEFINE_VEC4_REFERENCE(vec4, y, y, y, z)
	DEFINE_VEC4_REFERENCE(vec4, y, y, y, w)
	DEFINE_VEC4_REFERENCE(vec4, y, y, z, x)
	DEFINE_VEC4_REFERENCE(vec4, y, y, z, y)
	DEFINE_VEC4_REFERENCE(vec4, y, y, z, z)
	DEFINE_VEC4_REFERENCE(vec4, y, y, z, w)
	DEFINE_VEC4_REFERENCE(vec4, y, y, w, x)
	DEFINE_VEC4_REFERENCE(vec4, y, y, w, y)
	DEFINE_VEC4_REFERENCE(vec4, y, y, w, z)
	DEFINE_VEC4_REFERENCE(vec4, y, y, w, w)
	DEFINE_VEC4_REFERENCE(vec4, y, z, x, x)
	DEFINE_VEC4_REFERENCE(vec4, y, z, x, y)
	DEFINE_VEC4_REFERENCE(vec4, y, z, x, z)
	DEFINE_VEC4_REFERENCE(vec4, y, z, x, w)
	DEFINE_VEC4_REFERENCE(vec4, y, z, y, x)
	DEFINE_VEC4_REFERENCE(vec4, y, z, y, y)
	DEFINE_VEC4_REFERENCE(vec4, y, z, y, z)
	DEFINE_VEC4_REFERENCE(vec4, y, z, y, w)
	DEFINE_VEC4_REFERENCE(vec4, y, z, z, x)
	DEFINE_VEC4_REFERENCE(vec4, y, z, z, y)
	DEFINE_VEC4_REFERENCE(vec4, y, z, z, z)
	DEFINE_VEC4_REFERENCE(vec4, y, z, z, w)
	DEFINE_VEC4_REFERENCE(vec4, y, z, w, x)
	DEFINE_VEC4_REFERENCE(vec4, y, z, w, y)
	DEFINE_VEC4_REFERENCE(vec4, y, z, w, z)
	DEFINE_VEC4_REFERENCE(vec4, y, z, w, w)
	DEFINE_VEC4_REFERENCE(vec4, y, w, x, x)
	DEFINE_VEC4_REFERENCE(vec4, y, w, x, y)
	DEFINE_VEC4_REFERENCE(vec4, y, w, x, z)
	DEFINE_VEC4_REFERENCE(vec4, y, w, x, w)
	DEFINE_VEC4_REFERENCE(vec4, y, w, y, x)
	DEFINE_VEC4_REFERENCE(vec4, y, w, y, y)
	DEFINE_VEC4_REFERENCE(vec4, y, w, y, z)
	DEFINE_VEC4_REFERENCE(vec4, y, w, y, w)
	DEFINE_VEC4_REFERENCE(vec4, y, w, z, x)
	DEFINE_VEC4_REFERENCE(vec4, y, w, z, y)
	DEFINE_VEC4_REFERENCE(vec4, y, w, z, z)
	DEFINE_VEC4_REFERENCE(vec4, y, w, z, w)
	DEFINE_VEC4_REFERENCE(vec4, y, w, w, x)
	DEFINE_VEC4_REFERENCE(vec4, y, w, w, y)
	DEFINE_VEC4_REFERENCE(vec4, y, w, w, z)
	DEFINE_VEC4_REFERENCE(vec4, y, w, w, w)
	DEFINE_VEC4_REFERENCE(vec4, z, x, x, x)
	DEFINE_VEC4_REFERENCE(vec4, z, x, x, y)
	DEFINE_VEC4_REFERENCE(vec4, z, x, x, z)
	DEFINE_VEC4_REFERENCE(vec4, z, x, x, w)
	DEFINE_VEC4_REFERENCE(vec4, z, x, y, x)
	DEFINE_VEC4_REFERENCE(vec4, z, x, y, y)
	DEFINE_VEC4_REFERENCE(vec4, z, x, y, z)
	DEFINE_VEC4_REFERENCE(vec4, z, x, y, w)
	DEFINE_VEC4_REFERENCE(vec4, z, x, z, x)
	DEFINE_VEC4_REFERENCE(vec4, z, x, z, y)
	DEFINE_VEC4_REFERENCE(vec4, z, x, z, z)
	DEFINE_VEC4_REFERENCE(vec4, z, x, z, w)
	DEFINE_VEC4_REFERENCE(vec4, z, x, w, x)
	DEFINE_VEC4_REFERENCE(vec4, z, x, w, y)
	DEFINE_VEC4_REFERENCE(vec4, z, x, w, z)
	DEFINE_VEC4_REFERENCE(vec4, z, x, w, w)
	DEFINE_VEC4_REFERENCE(vec4, z, y, x, x)
	DEFINE_VEC4_REFERENCE(vec4, z, y, x, y)
	DEFINE_VEC4_REFERENCE(vec4, z, y, x, z)
	DEFINE_VEC4_REFERENCE(vec4, z, y, x, w)
	DEFINE_VEC4_REFERENCE(vec4, z, y, y, x)
	DEFINE_VEC4_REFERENCE(vec4, z, y, y, y)
	DEFINE_VEC4_REFERENCE(vec4, z, y, y, z)
	DEFINE_VEC4_REFERENCE(vec4, z, y, y, w)
	DEFINE_VEC4_REFERENCE(vec4, z, y, z, x)
	DEFINE_VEC4_REFERENCE(vec4, z, y, z, y)
	DEFINE_VEC4_REFERENCE(vec4, z, y, z, z)
	DEFINE_VEC4_REFERENCE(vec4, z, y, z, w)
	DEFINE_VEC4_REFERENCE(vec4, z, y, w, x)
	DEFINE_VEC4_REFERENCE(vec4, z, y, w, y)
	DEFINE_VEC4_REFERENCE(vec4, z, y, w, z)
	DEFINE_VEC4_REFERENCE(vec4, z, y, w, w)
	DEFINE_VEC4_REFERENCE(vec4, z, z, x, x)
	DEFINE_VEC4_REFERENCE(vec4, z, z, x, y)
	DEFINE_VEC4_REFERENCE(vec4, z, z, x, z)
	DEFINE_VEC4_REFERENCE(vec4, z, z, x, w)
	DEFINE_VEC4_REFERENCE(vec4, z, z, y, x)
	DEFINE_VEC4_REFERENCE(vec4, z, z, y, y)
	DEFINE_VEC4_REFERENCE(vec4, z, z, y, z)
	DEFINE_VEC4_REFERENCE(vec4, z, z, y, w)
	DEFINE_VEC4_REFERENCE(vec4, z, z, z, x)
	DEFINE_VEC4_REFERENCE(vec4, z, z, z, y)
	DEFINE_VEC4_REFERENCE(vec4, z, z, z, z)
	DEFINE_VEC4_REFERENCE(vec4, z, z, z, w)
	DEFINE_VEC4_REFERENCE(vec4, z, z, w, x)
	DEFINE_VEC4_REFERENCE(vec4, z, z, w, y)
	DEFINE_VEC4_REFERENCE(vec4, z, z, w, z)
	DEFINE_VEC4_REFERENCE(vec4, z, z, w, w)
	DEFINE_VEC4_REFERENCE(vec4, z, w, x, x)
	DEFINE_VEC4_REFERENCE(vec4, z, w, x, y)
	DEFINE_VEC4_REFERENCE(vec4, z, w, x, z)
	DEFINE_VEC4_REFERENCE(vec4, z, w, x, w)
	DEFINE_VEC4_REFERENCE(vec4, z, w, y, x)
	DEFINE_VEC4_REFERENCE(vec4, z, w, y, y)
	DEFINE_VEC4_REFERENCE(vec4, z, w, y, z)
	DEFINE_VEC4_REFERENCE(vec4, z, w, y, w)
	DEFINE_VEC4_REFERENCE(vec4, z, w, z, x)
	DEFINE_VEC4_REFERENCE(vec4, z, w, z, y)
	DEFINE_VEC4_REFERENCE(vec4, z, w, z, z)
	DEFINE_VEC4_REFERENCE(vec4, z, w, z, w)
	DEFINE_VEC4_REFERENCE(vec4, z, w, w, x)
	DEFINE_VEC4_REFERENCE(vec4, z, w, w, y)
	DEFINE_VEC4_REFERENCE(vec4, z, w, w, z)
	DEFINE_VEC4_REFERENCE(vec4, z, w, w, w)
	DEFINE_VEC4_REFERENCE(vec4, w, x, x, x)
	DEFINE_VEC4_REFERENCE(vec4, w, x, x, y)
	DEFINE_VEC4_REFERENCE(vec4, w, x, x, z)
	DEFINE_VEC4_REFERENCE(vec4, w, x, x, w)
	DEFINE_VEC4_REFERENCE(vec4, w, x, y, x)
	DEFINE_VEC4_REFERENCE(vec4, w, x, y, y)
	DEFINE_VEC4_REFERENCE(vec4, w, x, y, z)
	DEFINE_VEC4_REFERENCE(vec4, w, x, y, w)
	DEFINE_VEC4_REFERENCE(vec4, w, x, z, x)
	DEFINE_VEC4_REFERENCE(vec4, w, x, z, y)
	DEFINE_VEC4_REFERENCE(vec4, w, x, z, z)
	DEFINE_VEC4_REFERENCE(vec4, w, x, z, w)
	DEFINE_VEC4_REFERENCE(vec4, w, x, w, x)
	DEFINE_VEC4_REFERENCE(vec4, w, x, w, y)
	DEFINE_VEC4_REFERENCE(vec4, w, x, w, z)
	DEFINE_VEC4_REFERENCE(vec4, w, x, w, w)
	DEFINE_VEC4_REFERENCE(vec4, w, y, x, x)
	DEFINE_VEC4_REFERENCE(vec4, w, y, x, y)
	DEFINE_VEC4_REFERENCE(vec4, w, y, x, z)
	DEFINE_VEC4_REFERENCE(vec4, w, y, x, w)
	DEFINE_VEC4_REFERENCE(vec4, w, y, y, x)
	DEFINE_VEC4_REFERENCE(vec4, w, y, y, y)
	DEFINE_VEC4_REFERENCE(vec4, w, y, y, z)
	DEFINE_VEC4_REFERENCE(vec4, w, y, y, w)
	DEFINE_VEC4_REFERENCE(vec4, w, y, z, x)
	DEFINE_VEC4_REFERENCE(vec4, w, y, z, y)
	DEFINE_VEC4_REFERENCE(vec4, w, y, z, z)
	DEFINE_VEC4_REFERENCE(vec4, w, y, z, w)
	DEFINE_VEC4_REFERENCE(vec4, w, y, w, x)
	DEFINE_VEC4_REFERENCE(vec4, w, y, w, y)
	DEFINE_VEC4_REFERENCE(vec4, w, y, w, z)
	DEFINE_VEC4_REFERENCE(vec4, w, y, w, w)
	DEFINE_VEC4_REFERENCE(vec4, w, z, x, x)
	DEFINE_VEC4_REFERENCE(vec4, w, z, x, y)
	DEFINE_VEC4_REFERENCE(vec4, w, z, x, z)
	DEFINE_VEC4_REFERENCE(vec4, w, z, x, w)
	DEFINE_VEC4_REFERENCE(vec4, w, z, y, x)
	DEFINE_VEC4_REFERENCE(vec4, w, z, y, y)
	DEFINE_VEC4_REFERENCE(vec4, w, z, y, z)
	DEFINE_VEC4_REFERENCE(vec4, w, z, y, w)
	DEFINE_VEC4_REFERENCE(vec4, w, z, z, x)
	DEFINE_VEC4_REFERENCE(vec4, w, z, z, y)
	DEFINE_VEC4_REFERENCE(vec4, w, z, z, z)
	DEFINE_VEC4_REFERENCE(vec4, w, z, z, w)
	DEFINE_VEC4_REFERENCE(vec4, w, z, w, x)
	DEFINE_VEC4_REFERENCE(vec4, w, z, w, y)
	DEFINE_VEC4_REFERENCE(vec4, w, z, w, z)
	DEFINE_VEC4_REFERENCE(vec4, w, z, w, w)
	DEFINE_VEC4_REFERENCE(vec4, w, w, x, x)
	DEFINE_VEC4_REFERENCE(vec4, w, w, x, y)
	DEFINE_VEC4_REFERENCE(vec4, w, w, x, z)
	DEFINE_VEC4_REFERENCE(vec4, w, w, x, w)
	DEFINE_VEC4_REFERENCE(vec4, w, w, y, x)
	DEFINE_VEC4_REFERENCE(vec4, w, w, y, y)
	DEFINE_VEC4_REFERENCE(vec4, w, w, y, z)
	DEFINE_VEC4_REFERENCE(vec4, w, w, y, w)
	DEFINE_VEC4_REFERENCE(vec4, w, w, z, x)
	DEFINE_VEC4_REFERENCE(vec4, w, w, z, y)
	DEFINE_VEC4_REFERENCE(vec4, w, w, z, z)
	DEFINE_VEC4_REFERENCE(vec4, w, w, z, w)
	DEFINE_VEC4_REFERENCE(vec4, w, w, w, x)
	DEFINE_VEC4_REFERENCE(vec4, w, w, w, y)
	DEFINE_VEC4_REFERENCE(vec4, w, w, w, z)
	DEFINE_VEC4_REFERENCE(vec4, w, w, w, w)

public:
	union
	{
		vec4* parent;
		xx_t xx;
		xy_t xy;
		xz_t xz;
		xw_t xw;
		yx_t yx;
		yy_t yy;
		yz_t yz;
		yw_t yw;
		zx_t zx;
		zy_t zy;
		zz_t zz;
		zw_t zw;
		wx_t wx;
		wy_t wy;
		wz_t wz;
		ww_t ww;
		xxx_t xxx;
		xxy_t xxy;
		xxz_t xxz;
		xxw_t xxw;
		xyx_t xyx;
		xyy_t xyy;
		xyz_t xyz;
		xyw_t xyw;
		xzx_t xzx;
		xzy_t xzy;
		xzz_t xzz;
		xzw_t xzw;
		xwx_t xwx;
		xwy_t xwy;
		xwz_t xwz;
		xww_t xww;
		yxx_t yxx;
		yxy_t yxy;
		yxz_t yxz;
		yxw_t yxw;
		yyx_t yyx;
		yyy_t yyy;
		yyz_t yyz;
		yyw_t yyw;
		yzx_t yzx;
		yzy_t yzy;
		yzz_t yzz;
		yzw_t yzw;
		ywx_t ywx;
		ywy_t ywy;
		ywz_t ywz;
		yww_t yww;
		zxx_t zxx;
		zxy_t zxy;
		zxz_t zxz;
		zxw_t zxw;
		zyx_t zyx;
		zyy_t zyy;
		zyz_t zyz;
		zyw_t zyw;
		zzx_t zzx;
		zzy_t zzy;
		zzz_t zzz;
		zzw_t zzw;
		zwx_t zwx;
		zwy_t zwy;
		zwz_t zwz;
		zww_t zww;
		wxx_t wxx;
		wxy_t wxy;
		wxz_t wxz;
		wxw_t wxw;
		wyx_t wyx;
		wyy_t wyy;
		wyz_t wyz;
		wyw_t wyw;
		wzx_t wzx;
		wzy_t wzy;
		wzz_t wzz;
		wzw_t wzw;
		wwx_t wwx;
		wwy_t wwy;
		wwz_t wwz;
		www_t www;
		xxxx_t xxxx;
		xxxy_t xxxy;
		xxxz_t xxxz;
		xxxw_t xxxw;
		xxyx_t xxyx;
		xxyy_t xxyy;
		xxyz_t xxyz;
		xxyw_t xxyw;
		xxzx_t xxzx;
		xxzy_t xxzy;
		xxzz_t xxzz;
		xxzw_t xxzw;
		xxwx_t xxwx;
		xxwy_t xxwy;
		xxwz_t xxwz;
		xxww_t xxww;
		xyxx_t xyxx;
		xyxy_t xyxy;
		xyxz_t xyxz;
		xyxw_t xyxw;
		xyyx_t xyyx;
		xyyy_t xyyy;
		xyyz_t xyyz;
		xyyw_t xyyw;
		xyzx_t xyzx;
		xyzy_t xyzy;
		xyzz_t xyzz;
		xyzw_t xyzw;
		xywx_t xywx;
		xywy_t xywy;
		xywz_t xywz;
		xyww_t xyww;
		xzxx_t xzxx;
		xzxy_t xzxy;
		xzxz_t xzxz;
		xzxw_t xzxw;
		xzyx_t xzyx;
		xzyy_t xzyy;
		xzyz_t xzyz;
		xzyw_t xzyw;
		xzzx_t xzzx;
		xzzy_t xzzy;
		xzzz_t xzzz;
		xzzw_t xzzw;
		xzwx_t xzwx;
		xzwy_t xzwy;
		xzwz_t xzwz;
		xzww_t xzww;
		xwxx_t xwxx;
		xwxy_t xwxy;
		xwxz_t xwxz;
		xwxw_t xwxw;
		xwyx_t xwyx;
		xwyy_t xwyy;
		xwyz_t xwyz;
		xwyw_t xwyw;
		xwzx_t xwzx;
		xwzy_t xwzy;
		xwzz_t xwzz;
		xwzw_t xwzw;
		xwwx_t xwwx;
		xwwy_t xwwy;
		xwwz_t xwwz;
		xwww_t xwww;
		yxxx_t yxxx;
		yxxy_t yxxy;
		yxxz_t yxxz;
		yxxw_t yxxw;
		yxyx_t yxyx;
		yxyy_t yxyy;
		yxyz_t yxyz;
		yxyw_t yxyw;
		yxzx_t yxzx;
		yxzy_t yxzy;
		yxzz_t yxzz;
		yxzw_t yxzw;
		yxwx_t yxwx;
		yxwy_t yxwy;
		yxwz_t yxwz;
		yxww_t yxww;
		yyxx_t yyxx;
		yyxy_t yyxy;
		yyxz_t yyxz;
		yyxw_t yyxw;
		yyyx_t yyyx;
		yyyy_t yyyy;
		yyyz_t yyyz;
		yyyw_t yyyw;
		yyzx_t yyzx;
		yyzy_t yyzy;
		yyzz_t yyzz;
		yyzw_t yyzw;
		yywx_t yywx;
		yywy_t yywy;
		yywz_t yywz;
		yyww_t yyww;
		yzxx_t yzxx;
		yzxy_t yzxy;
		yzxz_t yzxz;
		yzxw_t yzxw;
		yzyx_t yzyx;
		yzyy_t yzyy;
		yzyz_t yzyz;
		yzyw_t yzyw;
		yzzx_t yzzx;
		yzzy_t yzzy;
		yzzz_t yzzz;
		yzzw_t yzzw;
		yzwx_t yzwx;
		yzwy_t yzwy;
		yzwz_t yzwz;
		yzww_t yzww;
		ywxx_t ywxx;
		ywxy_t ywxy;
		ywxz_t ywxz;
		ywxw_t ywxw;
		ywyx_t ywyx;
		ywyy_t ywyy;
		ywyz_t ywyz;
		ywyw_t ywyw;
		ywzx_t ywzx;
		ywzy_t ywzy;
		ywzz_t ywzz;
		ywzw_t ywzw;
		ywwx_t ywwx;
		ywwy_t ywwy;
		ywwz_t ywwz;
		ywww_t ywww;
		zxxx_t zxxx;
		zxxy_t zxxy;
		zxxz_t zxxz;
		zxxw_t zxxw;
		zxyx_t zxyx;
		zxyy_t zxyy;
		zxyz_t zxyz;
		zxyw_t zxyw;
		zxzx_t zxzx;
		zxzy_t zxzy;
		zxzz_t zxzz;
		zxzw_t zxzw;
		zxwx_t zxwx;
		zxwy_t zxwy;
		zxwz_t zxwz;
		zxww_t zxww;
		zyxx_t zyxx;
		zyxy_t zyxy;
		zyxz_t zyxz;
		zyxw_t zyxw;
		zyyx_t zyyx;
		zyyy_t zyyy;
		zyyz_t zyyz;
		zyyw_t zyyw;
		zyzx_t zyzx;
		zyzy_t zyzy;
		zyzz_t zyzz;
		zyzw_t zyzw;
		zywx_t zywx;
		zywy_t zywy;
		zywz_t zywz;
		zyww_t zyww;
		zzxx_t zzxx;
		zzxy_t zzxy;
		zzxz_t zzxz;
		zzxw_t zzxw;
		zzyx_t zzyx;
		zzyy_t zzyy;
		zzyz_t zzyz;
		zzyw_t zzyw;
		zzzx_t zzzx;
		zzzy_t zzzy;
		zzzz_t zzzz;
		zzzw_t zzzw;
		zzwx_t zzwx;
		zzwy_t zzwy;
		zzwz_t zzwz;
		zzww_t zzww;
		zwxx_t zwxx;
		zwxy_t zwxy;
		zwxz_t zwxz;
		zwxw_t zwxw;
		zwyx_t zwyx;
		zwyy_t zwyy;
		zwyz_t zwyz;
		zwyw_t zwyw;
		zwzx_t zwzx;
		zwzy_t zwzy;
		zwzz_t zwzz;
		zwzw_t zwzw;
		zwwx_t zwwx;
		zwwy_t zwwy;
		zwwz_t zwwz;
		zwww_t zwww;
		wxxx_t wxxx;
		wxxy_t wxxy;
		wxxz_t wxxz;
		wxxw_t wxxw;
		wxyx_t wxyx;
		wxyy_t wxyy;
		wxyz_t wxyz;
		wxyw_t wxyw;
		wxzx_t wxzx;
		wxzy_t wxzy;
		wxzz_t wxzz;
		wxzw_t wxzw;
		wxwx_t wxwx;
		wxwy_t wxwy;
		wxwz_t wxwz;
		wxww_t wxww;
		wyxx_t wyxx;
		wyxy_t wyxy;
		wyxz_t wyxz;
		wyxw_t wyxw;
		wyyx_t wyyx;
		wyyy_t wyyy;
		wyyz_t wyyz;
		wyyw_t wyyw;
		wyzx_t wyzx;
		wyzy_t wyzy;
		wyzz_t wyzz;
		wyzw_t wyzw;
		wywx_t wywx;
		wywy_t wywy;
		wywz_t wywz;
		wyww_t wyww;
		wzxx_t wzxx;
		wzxy_t wzxy;
		wzxz_t wzxz;
		wzxw_t wzxw;
		wzyx_t wzyx;
		wzyy_t wzyy;
		wzyz_t wzyz;
		wzyw_t wzyw;
		wzzx_t wzzx;
		wzzy_t wzzy;
		wzzz_t wzzz;
		wzzw_t wzzw;
		wzwx_t wzwx;
		wzwy_t wzwy;
		wzwz_t wzwz;
		wzww_t wzww;
		wwxx_t wwxx;
		wwxy_t wwxy;
		wwxz_t wwxz;
		wwxw_t wwxw;
		wwyx_t wwyx;
		wwyy_t wwyy;
		wwyz_t wwyz;
		wwyw_t wwyw;
		wwzx_t wwzx;
		wwzy_t wwzy;
		wwzz_t wwzz;
		wwzw_t wwzw;
		wwwx_t wwwx;
		wwwy_t wwwy;
		wwwz_t wwwz;
		wwww_t wwww;

		x_t r;
		y_t g;
		z_t b;
		w_t a;
		xx_t rr;
		xy_t rg;
		xz_t rb;
		xw_t ra;
		yx_t gr;
		yy_t gg;
		yz_t gb;
		yw_t ga;
		zx_t br;
		zy_t bg;
		zz_t bb;
		zw_t ba;
		wx_t ar;
		wy_t ag;
		wz_t ab;
		ww_t aa;
		xxx_t rrr;
		xxy_t rrg;
		xxz_t rrb;
		xxw_t rra;
		xyx_t rgr;
		xyy_t rgg;
		xyz_t rgb;
		xyw_t rga;
		xzx_t rbr;
		xzy_t rbg;
		xzz_t rbb;
		xzw_t rba;
		xwx_t rar;
		xwy_t rag;
		xwz_t rab;
		xww_t raa;
		yxx_t grr;
		yxy_t grg;
		yxz_t grb;
		yxw_t gra;
		yyx_t ggr;
		yyy_t ggg;
		yyz_t ggb;
		yyw_t gga;
		yzx_t gbr;
		yzy_t gbg;
		yzz_t gbb;
		yzw_t gba;
		ywx_t gar;
		ywy_t gag;
		ywz_t gab;
		yww_t gaa;
		zxx_t brr;
		zxy_t brg;
		zxz_t brb;
		zxw_t bra;
		zyx_t bgr;
		zyy_t bgg;
		zyz_t bgb;
		zyw_t bga;
		zzx_t bbr;
		zzy_t bbg;
		zzz_t bbb;
		zzw_t bba;
		zwx_t bar;
		zwy_t bag;
		zwz_t bab;
		zww_t baa;
		wxx_t arr;
		wxy_t arg;
		wxz_t arb;
		wxw_t ara;
		wyx_t agr;
		wyy_t agg;
		wyz_t agb;
		wyw_t aga;
		wzx_t abr;
		wzy_t abg;
		wzz_t abb;
		wzw_t aba;
		wwx_t aar;
		wwy_t aag;
		wwz_t aab;
		www_t aaa;
		xxxx_t rrrr;
		xxxy_t rrrg;
		xxxz_t rrrb;
		xxxw_t rrra;
		xxyx_t rrgr;
		xxyy_t rrgg;
		xxyz_t rrgb;
		xxyw_t rrga;
		xxzx_t rrbr;
		xxzy_t rrbg;
		xxzz_t rrbb;
		xxzw_t rrba;
		xxwx_t rrar;
		xxwy_t rrag;
		xxwz_t rrab;
		xxww_t rraa;
		xyxx_t rgrr;
		xyxy_t rgrg;
		xyxz_t rgrb;
		xyxw_t rgra;
		xyyx_t rggr;
		xyyy_t rggg;
		xyyz_t rggb;
		xyyw_t rgga;
		xyzx_t rgbr;
		xyzy_t rgbg;
		xyzz_t rgbb;
		xyzw_t rgba;
		xywx_t rgar;
		xywy_t rgag;
		xywz_t rgab;
		xyww_t rgaa;
		xzxx_t rbrr;
		xzxy_t rbrg;
		xzxz_t rbrb;
		xzxw_t rbra;
		xzyx_t rbgr;
		xzyy_t rbgg;
		xzyz_t rbgb;
		xzyw_t rbga;
		xzzx_t rbbr;
		xzzy_t rbbg;
		xzzz_t rbbb;
		xzzw_t rbba;
		xzwx_t rbar;
		xzwy_t rbag;
		xzwz_t rbab;
		xzww_t rbaa;
		xwxx_t rarr;
		xwxy_t rarg;
		xwxz_t rarb;
		xwxw_t rara;
		xwyx_t ragr;
		xwyy_t ragg;
		xwyz_t ragb;
		xwyw_t raga;
		xwzx_t rabr;
		xwzy_t rabg;
		xwzz_t rabb;
		xwzw_t raba;
		xwwx_t raar;
		xwwy_t raag;
		xwwz_t raab;
		xwww_t raaa;
		yxxx_t grrr;
		yxxy_t grrg;
		yxxz_t grrb;
		yxxw_t grra;
		yxyx_t grgr;
		yxyy_t grgg;
		yxyz_t grgb;
		yxyw_t grga;
		yxzx_t grbr;
		yxzy_t grbg;
		yxzz_t grbb;
		yxzw_t grba;
		yxwx_t grar;
		yxwy_t grag;
		yxwz_t grab;
		yxww_t graa;
		yyxx_t ggrr;
		yyxy_t ggrg;
		yyxz_t ggrb;
		yyxw_t ggra;
		yyyx_t gggr;
		yyyy_t gggg;
		yyyz_t gggb;
		yyyw_t ggga;
		yyzx_t ggbr;
		yyzy_t ggbg;
		yyzz_t ggbb;
		yyzw_t ggba;
		yywx_t ggar;
		yywy_t ggag;
		yywz_t ggab;
		yyww_t ggaa;
		yzxx_t gbrr;
		yzxy_t gbrg;
		yzxz_t gbrb;
		yzxw_t gbra;
		yzyx_t gbgr;
		yzyy_t gbgg;
		yzyz_t gbgb;
		yzyw_t gbga;
		yzzx_t gbbr;
		yzzy_t gbbg;
		yzzz_t gbbb;
		yzzw_t gbba;
		yzwx_t gbar;
		yzwy_t gbag;
		yzwz_t gbab;
		yzww_t gbaa;
		ywxx_t garr;
		ywxy_t garg;
		ywxz_t garb;
		ywxw_t gara;
		ywyx_t gagr;
		ywyy_t gagg;
		ywyz_t gagb;
		ywyw_t gaga;
		ywzx_t gabr;
		ywzy_t gabg;
		ywzz_t gabb;
		ywzw_t gaba;
		ywwx_t gaar;
		ywwy_t gaag;
		ywwz_t gaab;
		ywww_t gaaa;
		zxxx_t brrr;
		zxxy_t brrg;
		zxxz_t brrb;
		zxxw_t brra;
		zxyx_t brgr;
		zxyy_t brgg;
		zxyz_t brgb;
		zxyw_t brga;
		zxzx_t brbr;
		zxzy_t brbg;
		zxzz_t brbb;
		zxzw_t brba;
		zxwx_t brar;
		zxwy_t brag;
		zxwz_t brab;
		zxww_t braa;
		zyxx_t bgrr;
		zyxy_t bgrg;
		zyxz_t bgrb;
		zyxw_t bgra;
		zyyx_t bggr;
		zyyy_t bggg;
		zyyz_t bggb;
		zyyw_t bgga;
		zyzx_t bgbr;
		zyzy_t bgbg;
		zyzz_t bgbb;
		zyzw_t bgba;
		zywx_t bgar;
		zywy_t bgag;
		zywz_t bgab;
		zyww_t bgaa;
		zzxx_t bbrr;
		zzxy_t bbrg;
		zzxz_t bbrb;
		zzxw_t bbra;
		zzyx_t bbgr;
		zzyy_t bbgg;
		zzyz_t bbgb;
		zzyw_t bbga;
		zzzx_t bbbr;
		zzzy_t bbbg;
		zzzz_t bbbb;
		zzzw_t bbba;
		zzwx_t bbar;
		zzwy_t bbag;
		zzwz_t bbab;
		zzww_t bbaa;
		zwxx_t barr;
		zwxy_t barg;
		zwxz_t barb;
		zwxw_t bara;
		zwyx_t bagr;
		zwyy_t bagg;
		zwyz_t bagb;
		zwyw_t baga;
		zwzx_t babr;
		zwzy_t babg;
		zwzz_t babb;
		zwzw_t baba;
		zwwx_t baar;
		zwwy_t baag;
		zwwz_t baab;
		zwww_t baaa;
		wxxx_t arrr;
		wxxy_t arrg;
		wxxz_t arrb;
		wxxw_t arra;
		wxyx_t argr;
		wxyy_t argg;
		wxyz_t argb;
		wxyw_t arga;
		wxzx_t arbr;
		wxzy_t arbg;
		wxzz_t arbb;
		wxzw_t arba;
		wxwx_t arar;
		wxwy_t arag;
		wxwz_t arab;
		wxww_t araa;
		wyxx_t agrr;
		wyxy_t agrg;
		wyxz_t agrb;
		wyxw_t agra;
		wyyx_t aggr;
		wyyy_t aggg;
		wyyz_t aggb;
		wyyw_t agga;
		wyzx_t agbr;
		wyzy_t agbg;
		wyzz_t agbb;
		wyzw_t agba;
		wywx_t agar;
		wywy_t agag;
		wywz_t agab;
		wyww_t agaa;
		wzxx_t abrr;
		wzxy_t abrg;
		wzxz_t abrb;
		wzxw_t abra;
		wzyx_t abgr;
		wzyy_t abgg;
		wzyz_t abgb;
		wzyw_t abga;
		wzzx_t abbr;
		wzzy_t abbg;
		wzzz_t abbb;
		wzzw_t abba;
		wzwx_t abar;
		wzwy_t abag;
		wzwz_t abab;
		wzww_t abaa;
		wwxx_t aarr;
		wwxy_t aarg;
		wwxz_t aarb;
		wwxw_t aara;
		wwyx_t aagr;
		wwyy_t aagg;
		wwyz_t aagb;
		wwyw_t aaga;
		wwzx_t aabr;
		wwzy_t aabg;
		wwzz_t aabb;
		wwzw_t aaba;
		wwwx_t aaar;
		wwwy_t aaag;
		wwwz_t aaab;
		wwww_t aaaa;

		x_t s;
		y_t t;
		z_t p;
		w_t q;
		xx_t ss;
		xy_t st;
		xz_t sp;
		xw_t sq;
		yx_t ts;
		yy_t tt;
		yz_t tp;
		yw_t tq;
		zx_t ps;
		zy_t pt;
		zz_t pp;
		zw_t pq;
		wx_t qs;
		wy_t qt;
		wz_t qp;
		ww_t qq;
		xxx_t sss;
		xxy_t sst;
		xxz_t ssp;
		xxw_t ssq;
		xyx_t sts;
		xyy_t stt;
		xyz_t stp;
		xyw_t stq;
		xzx_t sps;
		xzy_t spt;
		xzz_t spp;
		xzw_t spq;
		xwx_t sqs;
		xwy_t sqt;
		xwz_t sqp;
		xww_t sqq;
		yxx_t tss;
		yxy_t tst;
		yxz_t tsp;
		yxw_t tsq;
		yyx_t tts;
		yyy_t ttt;
		yyz_t ttp;
		yyw_t ttq;
		yzx_t tps;
		yzy_t tpt;
		yzz_t tpp;
		yzw_t tpq;
		ywx_t tqs;
		ywy_t tqt;
		ywz_t tqp;
		yww_t tqq;
		zxx_t pss;
		zxy_t pst;
		zxz_t psp;
		zxw_t psq;
		zyx_t pts;
		zyy_t ptt;
		zyz_t ptp;
		zyw_t ptq;
		zzx_t pps;
		zzy_t ppt;
		zzz_t ppp;
		zzw_t ppq;
		zwx_t pqs;
		zwy_t pqt;
		zwz_t pqp;
		zww_t pqq;
		wxx_t qss;
		wxy_t qst;
		wxz_t qsp;
		wxw_t qsq;
		wyx_t qts;
		wyy_t qtt;
		wyz_t qtp;
		wyw_t qtq;
		wzx_t qps;
		wzy_t qpt;
		wzz_t qpp;
		wzw_t qpq;
		wwx_t qqs;
		wwy_t qqt;
		wwz_t qqp;
		www_t qqq;
		xxxx_t ssss;
		xxxy_t ssst;
		xxxz_t sssp;
		xxxw_t sssq;
		xxyx_t ssts;
		xxyy_t sstt;
		xxyz_t sstp;
		xxyw_t sstq;
		xxzx_t ssps;
		xxzy_t sspt;
		xxzz_t sspp;
		xxzw_t sspq;
		xxwx_t ssqs;
		xxwy_t ssqt;
		xxwz_t ssqp;
		xxww_t ssqq;
		xyxx_t stss;
		xyxy_t stst;
		xyxz_t stsp;
		xyxw_t stsq;
		xyyx_t stts;
		xyyy_t sttt;
		xyyz_t sttp;
		xyyw_t sttq;
		xyzx_t stps;
		xyzy_t stpt;
		xyzz_t stpp;
		xyzw_t stpq;
		xywx_t stqs;
		xywy_t stqt;
		xywz_t stqp;
		xyww_t stqq;
		xzxx_t spss;
		xzxy_t spst;
		xzxz_t spsp;
		xzxw_t spsq;
		xzyx_t spts;
		xzyy_t sptt;
		xzyz_t sptp;
		xzyw_t sptq;
		xzzx_t spps;
		xzzy_t sppt;
		xzzz_t sppp;
		xzzw_t sppq;
		xzwx_t spqs;
		xzwy_t spqt;
		xzwz_t spqp;
		xzww_t spqq;
		xwxx_t sqss;
		xwxy_t sqst;
		xwxz_t sqsp;
		xwxw_t sqsq;
		xwyx_t sqts;
		xwyy_t sqtt;
		xwyz_t sqtp;
		xwyw_t sqtq;
		xwzx_t sqps;
		xwzy_t sqpt;
		xwzz_t sqpp;
		xwzw_t sqpq;
		xwwx_t sqqs;
		xwwy_t sqqt;
		xwwz_t sqqp;
		xwww_t sqqq;
		yxxx_t tsss;
		yxxy_t tsst;
		yxxz_t tssp;
		yxxw_t tssq;
		yxyx_t tsts;
		yxyy_t tstt;
		yxyz_t tstp;
		yxyw_t tstq;
		yxzx_t tsps;
		yxzy_t tspt;
		yxzz_t tspp;
		yxzw_t tspq;
		yxwx_t tsqs;
		yxwy_t tsqt;
		yxwz_t tsqp;
		yxww_t tsqq;
		yyxx_t ttss;
		yyxy_t ttst;
		yyxz_t ttsp;
		yyxw_t ttsq;
		yyyx_t ttts;
		yyyy_t tttt;
		yyyz_t tttp;
		yyyw_t tttq;
		yyzx_t ttps;
		yyzy_t ttpt;
		yyzz_t ttpp;
		yyzw_t ttpq;
		yywx_t ttqs;
		yywy_t ttqt;
		yywz_t ttqp;
		yyww_t ttqq;
		yzxx_t tpss;
		yzxy_t tpst;
		yzxz_t tpsp;
		yzxw_t tpsq;
		yzyx_t tpts;
		yzyy_t tptt;
		yzyz_t tptp;
		yzyw_t tptq;
		yzzx_t tpps;
		yzzy_t tppt;
		yzzz_t tppp;
		yzzw_t tppq;
		yzwx_t tpqs;
		yzwy_t tpqt;
		yzwz_t tpqp;
		yzww_t tpqq;
		ywxx_t tqss;
		ywxy_t tqst;
		ywxz_t tqsp;
		ywxw_t tqsq;
		ywyx_t tqts;
		ywyy_t tqtt;
		ywyz_t tqtp;
		ywyw_t tqtq;
		ywzx_t tqps;
		ywzy_t tqpt;
		ywzz_t tqpp;
		ywzw_t tqpq;
		ywwx_t tqqs;
		ywwy_t tqqt;
		ywwz_t tqqp;
		ywww_t tqqq;
		zxxx_t psss;
		zxxy_t psst;
		zxxz_t pssp;
		zxxw_t pssq;
		zxyx_t psts;
		zxyy_t pstt;
		zxyz_t pstp;
		zxyw_t pstq;
		zxzx_t psps;
		zxzy_t pspt;
		zxzz_t pspp;
		zxzw_t pspq;
		zxwx_t psqs;
		zxwy_t psqt;
		zxwz_t psqp;
		zxww_t psqq;
		zyxx_t ptss;
		zyxy_t ptst;
		zyxz_t ptsp;
		zyxw_t ptsq;
		zyyx_t ptts;
		zyyy_t pttt;
		zyyz_t pttp;
		zyyw_t pttq;
		zyzx_t ptps;
		zyzy_t ptpt;
		zyzz_t ptpp;
		zyzw_t ptpq;
		zywx_t ptqs;
		zywy_t ptqt;
		zywz_t ptqp;
		zyww_t ptqq;
		zzxx_t ppss;
		zzxy_t ppst;
		zzxz_t ppsp;
		zzxw_t ppsq;
		zzyx_t ppts;
		zzyy_t pptt;
		zzyz_t pptp;
		zzyw_t pptq;
		zzzx_t ppps;
		zzzy_t pppt;
		zzzz_t pppp;
		zzzw_t pppq;
		zzwx_t ppqs;
		zzwy_t ppqt;
		zzwz_t ppqp;
		zzww_t ppqq;
		zwxx_t pqss;
		zwxy_t pqst;
		zwxz_t pqsp;
		zwxw_t pqsq;
		zwyx_t pqts;
		zwyy_t pqtt;
		zwyz_t pqtp;
		zwyw_t pqtq;
		zwzx_t pqps;
		zwzy_t pqpt;
		zwzz_t pqpp;
		zwzw_t pqpq;
		zwwx_t pqqs;
		zwwy_t pqqt;
		zwwz_t pqqp;
		zwww_t pqqq;
		wxxx_t qsss;
		wxxy_t qsst;
		wxxz_t qssp;
		wxxw_t qssq;
		wxyx_t qsts;
		wxyy_t qstt;
		wxyz_t qstp;
		wxyw_t qstq;
		wxzx_t qsps;
		wxzy_t qspt;
		wxzz_t qspp;
		wxzw_t qspq;
		wxwx_t qsqs;
		wxwy_t qsqt;
		wxwz_t qsqp;
		wxww_t qsqq;
		wyxx_t qtss;
		wyxy_t qtst;
		wyxz_t qtsp;
		wyxw_t qtsq;
		wyyx_t qtts;
		wyyy_t qttt;
		wyyz_t qttp;
		wyyw_t qttq;
		wyzx_t qtps;
		wyzy_t qtpt;
		wyzz_t qtpp;
		wyzw_t qtpq;
		wywx_t qtqs;
		wywy_t qtqt;
		wywz_t qtqp;
		wyww_t qtqq;
		wzxx_t qpss;
		wzxy_t qpst;
		wzxz_t qpsp;
		wzxw_t qpsq;
		wzyx_t qpts;
		wzyy_t qptt;
		wzyz_t qptp;
		wzyw_t qptq;
		wzzx_t qpps;
		wzzy_t qppt;
		wzzz_t qppp;
		wzzw_t qppq;
		wzwx_t qpqs;
		wzwy_t qpqt;
		wzwz_t qpqp;
		wzww_t qpqq;
		wwxx_t qqss;
		wwxy_t qqst;
		wwxz_t qqsp;
		wwxw_t qqsq;
		wwyx_t qqts;
		wwyy_t qqtt;
		wwyz_t qqtp;
		wwyw_t qqtq;
		wwzx_t qqps;
		wwzy_t qqpt;
		wwzz_t qqpp;
		wwzw_t qqpq;
		wwwx_t qqqs;
		wwwy_t qqqt;
		wwwz_t qqqp;
		wwww_t qqqq;
	};
#endif
	
	vec4();
	vec4(float value);
	vec4(float _x, float _y, float _z, float _w);
	vec4(const vec2& v, float _z, float _w);
	vec4(float _x, const vec2& v, float _w);
	vec4(float _x, float _y, const vec2& v);
	vec4(const vec2& v1, const vec2& v2);
	vec4(const vec3& v, float _w);
	vec4(float _x, const vec3& v);
	vec4(const vec4& v);
	vec4(vec4&& v);
	vec4& operator =(const vec4& v);
	vec4& operator =(vec4&& v);

	operator dvec4()const;
	operator uvec4()const;
	operator ivec4()const;
	operator bvec4()const;

	float* data()const;

	float& operator [](uint i);
	float  operator [](uint i)const;

	vec4 operator +()const;
	vec4 operator -()const;

	vec4& operator +=(const vec4& v);
	vec4& operator -=(const vec4& v);
	vec4& operator *=(const vec4& v);
	vec4& operator /=(const vec4& v);

	vec4& operator +=(float value);
	vec4& operator -=(float value);
	vec4& operator *=(float value);
	vec4& operator /=(float value);

	friend vec4 operator +(const vec4& v1, const vec4& v2);
	friend vec4 operator -(const vec4& v1, const vec4& v2);
	friend vec4 operator *(const vec4& v1, const vec4& v2);
	friend vec4 operator /(const vec4& v1, const vec4& v2);

	friend vec4 operator +(const vec4& v1, const uvec4& v2);
	friend vec4 operator -(const vec4& v1, const uvec4& v2);
	friend vec4 operator *(const vec4& v1, const uvec4& v2);
	friend vec4 operator /(const vec4& v1, const uvec4& v2);

	friend vec4 operator +(const uvec4& v1, const vec4& v2);
	friend vec4 operator -(const uvec4& v1, const vec4& v2);
	friend vec4 operator *(const uvec4& v1, const vec4& v2);
	friend vec4 operator /(const uvec4& v1, const vec4& v2);

	friend vec4 operator +(const vec4& v1, const ivec4& v2);
	friend vec4 operator -(const vec4& v1, const ivec4& v2);
	friend vec4 operator *(const vec4& v1, const ivec4& v2);
	friend vec4 operator /(const vec4& v1, const ivec4& v2);

	friend vec4 operator +(const ivec4& v1, const vec4& v2);
	friend vec4 operator -(const ivec4& v1, const vec4& v2);
	friend vec4 operator *(const ivec4& v1, const vec4& v2);
	friend vec4 operator /(const ivec4& v1, const vec4& v2);

	friend dvec4 operator +(double value, const vec4& v2);
	friend dvec4 operator -(double value, const vec4& v2);
	friend dvec4 operator *(double value, const vec4& v2);
	friend dvec4 operator /(double value, const vec4& v2);

	friend dvec4 operator +(const vec4& v2, double value);
	friend dvec4 operator -(const vec4& v2, double value);
	friend dvec4 operator *(const vec4& v2, double value);
	friend dvec4 operator /(const vec4& v2, double value);

	friend vec4 operator +(float value, const vec4& v2);
	friend vec4 operator -(float value, const vec4& v2);
	friend vec4 operator *(float value, const vec4& v2);
	friend vec4 operator /(float value, const vec4& v2);

	friend vec4 operator +(const vec4& v2, float value);
	friend vec4 operator -(const vec4& v2, float value);
	friend vec4 operator *(const vec4& v2, float value);
	friend vec4 operator /(const vec4& v2, float value);

	friend vec4 operator +(uint value, const vec4& v2);
	friend vec4 operator -(uint value, const vec4& v2);
	friend vec4 operator *(uint value, const vec4& v2);
	friend vec4 operator /(uint value, const vec4& v2);

	friend vec4 operator +(const vec4& v2, uint value);
	friend vec4 operator -(const vec4& v2, uint value);
	friend vec4 operator *(const vec4& v2, uint value);
	friend vec4 operator /(const vec4& v2, uint value);

	friend vec4 operator +(int value, const vec4& v2);
	friend vec4 operator -(int value, const vec4& v2);
	friend vec4 operator *(int value, const vec4& v2);
	friend vec4 operator /(int value, const vec4& v2);

	friend vec4 operator +(const vec4& v2, int value);
	friend vec4 operator -(const vec4& v2, int value);
	friend vec4 operator *(const vec4& v2, int value);
	friend vec4 operator /(const vec4& v2, int value);

	friend ostream& operator <<(ostream& out, const vec4& v);
};

#endif