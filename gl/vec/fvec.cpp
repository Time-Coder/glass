#include "dvec.h"
#include "fvec.h"
#include "uvec.h"
#include "ivec.h"
#include "bvec.h"
#include "vec_process.h"
#include "exception.h"

#ifdef ENABLE_SWIZZLE
#define IMPLEMENT_FLOAT_REFERENCE(vec_type, e1) \
vec_type::e1##_t::operator float()const\
{\
	return parent->e1;\
}\
float& vec_type::e1##_t::operator =(float value)\
{\
	parent->e1 = value;\
	return parent->e1;\
}\

#define IMPLEMENT_VEC2_REFERENCE(vec_type, e1, e2) \
vec_type::e1##e2##_t::operator vec2()\
{\
	return vec2(parent->e1, parent->e2);\
}\
float vec_type::e1##e2##_t::operator [](uint i)const\
{\
	switch(i)\
	{\
		case 0: return parent->e1; break;\
		case 1: return parent->e2; break;\
		default: throw IndexError(i, 0, 1);\
	}\
}\
float& vec_type::e1##e2##_t::operator [](uint i)\
{\
	switch(i)\
	{\
		case 0: return parent->e1; break;\
		case 1: return parent->e2; break;\
		default: throw IndexError(i, 0, 1);\
	}\
}\
vec_type& vec_type::e1##e2##_t::operator =(const vec2& v)\
{\
	if((void*)(&v) != (void*)(parent))\
	{\
		parent->e1 = v.x;\
		parent->e2 = v.y;\
	}\
	else\
	{\
		float x = v.x, y = v.y;\
		parent->e1 = x;\
		parent->e2 = y;\
	}\
	return *parent;\
}\
vec_type& vec_type::e1##e2##_t::operator =(vec2&& v)\
{\
	if((void*)(&v) != (void*)(parent))\
	{\
		parent->e1 = move(v.x);\
		parent->e2 = move(v.y);\
	}\
	else\
	{\
		float x = v.x, y = v.y;\
		parent->e1 = x;\
		parent->e2 = y;\
	}\
	return *parent;\
}

#define IMPLEMENT_VEC3_REFERENCE(vec_type, e1, e2, e3) \
vec_type::e1##e2##e3##_t::operator vec3()\
{\
	return vec3(parent->e1, parent->e2, parent->e3);\
}\
float vec_type::e1##e2##e3##_t::operator [](uint i)const\
{\
	switch(i)\
	{\
		case 0: return parent->e1; break;\
		case 1: return parent->e2; break;\
		case 2: return parent->e3; break;\
		default: throw IndexError(i, 0, 2);\
	}\
}\
float& vec_type::e1##e2##e3##_t::operator [](uint i)\
{\
	switch(i)\
	{\
		case 0: return parent->e1; break;\
		case 1: return parent->e2; break;\
		case 2: return parent->e3; break;\
		default: throw IndexError(i, 0, 2);\
	}\
}\
vec_type& vec_type::e1##e2##e3##_t::operator =(const vec3& v)\
{\
	if((void*)(&v) != (void*)(parent))\
	{\
		parent->e1 = v.x;\
		parent->e2 = v.y;\
		parent->e3 = v.z;\
	}\
	else\
	{\
		float x = v.x, y = v.y, z = v.z;\
		parent->e1 = x;\
		parent->e2 = y;\
		parent->e3 = z;\
	}\
	return *parent;\
}\
vec_type& vec_type::e1##e2##e3##_t::operator =(vec3&& v)\
{\
	if((void*)(&v) != (void*)(parent))\
	{\
		parent->e1 = move(v.x);\
		parent->e2 = move(v.y);\
		parent->e3 = move(v.z);\
	}\
	else\
	{\
		float x = v.x, y = v.y, z = v.z;\
		parent->e1 = x;\
		parent->e2 = y;\
		parent->e3 = z;\
	}\
	return *parent;\
}

#define IMPLEMENT_VEC4_REFERENCE(vec_type, e1, e2, e3, e4) \
vec_type::e1##e2##e3##e4##_t::operator vec4()\
{\
	return vec4(parent->e1, parent->e2, parent->e3, parent->e4);\
}\
float vec_type::e1##e2##e3##e4##_t::operator [](uint i)const\
{\
	switch(i)\
	{\
		case 0: return parent->e1; break;\
		case 1: return parent->e2; break;\
		case 2: return parent->e3; break;\
		case 3: return parent->e4; break;\
		default: throw IndexError(i, 0, 3);\
	}\
}\
float& vec_type::e1##e2##e3##e4##_t::operator [](uint i)\
{\
	switch(i)\
	{\
		case 0: return parent->e1; break;\
		case 1: return parent->e2; break;\
		case 2: return parent->e3; break;\
		case 3: return parent->e4; break;\
		default: throw IndexError(i, 0, 3);\
	}\
}\
vec_type& vec_type::e1##e2##e3##e4##_t::operator =(const vec4& v)\
{\
	if((void*)(&v) != (void*)(parent))\
	{\
		parent->e1 = v.x;\
		parent->e2 = v.y;\
		parent->e3 = v.z;\
		parent->e4 = v.w;\
	}\
	else\
	{\
		float x = v.x, y = v.y, z = v.z, w = v.w;\
		parent->e1 = x;\
		parent->e2 = y;\
		parent->e3 = z;\
		parent->e4 = w;\
	}\
	return *parent;\
}\
vec_type& vec_type::e1##e2##e3##e4##_t::operator =(vec4&& v)\
{\
	if((void*)(&v) != (void*)(parent))\
	{\
		parent->e1 = move(v.x);\
		parent->e2 = move(v.y);\
		parent->e3 = move(v.z);\
		parent->e4 = move(v.w);\
	}\
	else\
	{\
		float x = v.x, y = v.y, z = v.z, w = v.w;\
		parent->e1 = x;\
		parent->e2 = y;\
		parent->e3 = z;\
		parent->e4 = w;\
	}\
	return *parent;\
}

IMPLEMENT_FLOAT_REFERENCE(vec2, x)
IMPLEMENT_FLOAT_REFERENCE(vec2, y)
IMPLEMENT_VEC2_REFERENCE(vec2, x, x)
IMPLEMENT_VEC2_REFERENCE(vec2, x, y)
IMPLEMENT_VEC2_REFERENCE(vec2, y, x)
IMPLEMENT_VEC2_REFERENCE(vec2, y, y)
IMPLEMENT_VEC3_REFERENCE(vec2, x, x, x)
IMPLEMENT_VEC3_REFERENCE(vec2, x, x, y)
IMPLEMENT_VEC3_REFERENCE(vec2, x, y, x)
IMPLEMENT_VEC3_REFERENCE(vec2, x, y, y)
IMPLEMENT_VEC3_REFERENCE(vec2, y, x, x)
IMPLEMENT_VEC3_REFERENCE(vec2, y, x, y)
IMPLEMENT_VEC3_REFERENCE(vec2, y, y, x)
IMPLEMENT_VEC3_REFERENCE(vec2, y, y, y)
IMPLEMENT_VEC4_REFERENCE(vec2, x, x, x, x)
IMPLEMENT_VEC4_REFERENCE(vec2, x, x, x, y)
IMPLEMENT_VEC4_REFERENCE(vec2, x, x, y, x)
IMPLEMENT_VEC4_REFERENCE(vec2, x, x, y, y)
IMPLEMENT_VEC4_REFERENCE(vec2, x, y, x, x)
IMPLEMENT_VEC4_REFERENCE(vec2, x, y, x, y)
IMPLEMENT_VEC4_REFERENCE(vec2, x, y, y, x)
IMPLEMENT_VEC4_REFERENCE(vec2, x, y, y, y)
IMPLEMENT_VEC4_REFERENCE(vec2, y, x, x, x)
IMPLEMENT_VEC4_REFERENCE(vec2, y, x, x, y)
IMPLEMENT_VEC4_REFERENCE(vec2, y, x, y, x)
IMPLEMENT_VEC4_REFERENCE(vec2, y, x, y, y)
IMPLEMENT_VEC4_REFERENCE(vec2, y, y, x, x)
IMPLEMENT_VEC4_REFERENCE(vec2, y, y, x, y)
IMPLEMENT_VEC4_REFERENCE(vec2, y, y, y, x)
IMPLEMENT_VEC4_REFERENCE(vec2, y, y, y, y)

IMPLEMENT_FLOAT_REFERENCE(vec3, x)
IMPLEMENT_FLOAT_REFERENCE(vec3, y)
IMPLEMENT_FLOAT_REFERENCE(vec3, z)
IMPLEMENT_VEC2_REFERENCE(vec3, x, x)
IMPLEMENT_VEC2_REFERENCE(vec3, x, y)
IMPLEMENT_VEC2_REFERENCE(vec3, x, z)
IMPLEMENT_VEC2_REFERENCE(vec3, y, x)
IMPLEMENT_VEC2_REFERENCE(vec3, y, y)
IMPLEMENT_VEC2_REFERENCE(vec3, y, z)
IMPLEMENT_VEC2_REFERENCE(vec3, z, x)
IMPLEMENT_VEC2_REFERENCE(vec3, z, y)
IMPLEMENT_VEC2_REFERENCE(vec3, z, z)
IMPLEMENT_VEC3_REFERENCE(vec3, x, x, x)
IMPLEMENT_VEC3_REFERENCE(vec3, x, x, y)
IMPLEMENT_VEC3_REFERENCE(vec3, x, x, z)
IMPLEMENT_VEC3_REFERENCE(vec3, x, y, x)
IMPLEMENT_VEC3_REFERENCE(vec3, x, y, y)
IMPLEMENT_VEC3_REFERENCE(vec3, x, y, z)
IMPLEMENT_VEC3_REFERENCE(vec3, x, z, x)
IMPLEMENT_VEC3_REFERENCE(vec3, x, z, y)
IMPLEMENT_VEC3_REFERENCE(vec3, x, z, z)
IMPLEMENT_VEC3_REFERENCE(vec3, y, x, x)
IMPLEMENT_VEC3_REFERENCE(vec3, y, x, y)
IMPLEMENT_VEC3_REFERENCE(vec3, y, x, z)
IMPLEMENT_VEC3_REFERENCE(vec3, y, y, x)
IMPLEMENT_VEC3_REFERENCE(vec3, y, y, y)
IMPLEMENT_VEC3_REFERENCE(vec3, y, y, z)
IMPLEMENT_VEC3_REFERENCE(vec3, y, z, x)
IMPLEMENT_VEC3_REFERENCE(vec3, y, z, y)
IMPLEMENT_VEC3_REFERENCE(vec3, y, z, z)
IMPLEMENT_VEC3_REFERENCE(vec3, z, x, x)
IMPLEMENT_VEC3_REFERENCE(vec3, z, x, y)
IMPLEMENT_VEC3_REFERENCE(vec3, z, x, z)
IMPLEMENT_VEC3_REFERENCE(vec3, z, y, x)
IMPLEMENT_VEC3_REFERENCE(vec3, z, y, y)
IMPLEMENT_VEC3_REFERENCE(vec3, z, y, z)
IMPLEMENT_VEC3_REFERENCE(vec3, z, z, x)
IMPLEMENT_VEC3_REFERENCE(vec3, z, z, y)
IMPLEMENT_VEC3_REFERENCE(vec3, z, z, z)
IMPLEMENT_VEC4_REFERENCE(vec3, x, x, x, x)
IMPLEMENT_VEC4_REFERENCE(vec3, x, x, x, y)
IMPLEMENT_VEC4_REFERENCE(vec3, x, x, x, z)
IMPLEMENT_VEC4_REFERENCE(vec3, x, x, y, x)
IMPLEMENT_VEC4_REFERENCE(vec3, x, x, y, y)
IMPLEMENT_VEC4_REFERENCE(vec3, x, x, y, z)
IMPLEMENT_VEC4_REFERENCE(vec3, x, x, z, x)
IMPLEMENT_VEC4_REFERENCE(vec3, x, x, z, y)
IMPLEMENT_VEC4_REFERENCE(vec3, x, x, z, z)
IMPLEMENT_VEC4_REFERENCE(vec3, x, y, x, x)
IMPLEMENT_VEC4_REFERENCE(vec3, x, y, x, y)
IMPLEMENT_VEC4_REFERENCE(vec3, x, y, x, z)
IMPLEMENT_VEC4_REFERENCE(vec3, x, y, y, x)
IMPLEMENT_VEC4_REFERENCE(vec3, x, y, y, y)
IMPLEMENT_VEC4_REFERENCE(vec3, x, y, y, z)
IMPLEMENT_VEC4_REFERENCE(vec3, x, y, z, x)
IMPLEMENT_VEC4_REFERENCE(vec3, x, y, z, y)
IMPLEMENT_VEC4_REFERENCE(vec3, x, y, z, z)
IMPLEMENT_VEC4_REFERENCE(vec3, x, z, x, x)
IMPLEMENT_VEC4_REFERENCE(vec3, x, z, x, y)
IMPLEMENT_VEC4_REFERENCE(vec3, x, z, x, z)
IMPLEMENT_VEC4_REFERENCE(vec3, x, z, y, x)
IMPLEMENT_VEC4_REFERENCE(vec3, x, z, y, y)
IMPLEMENT_VEC4_REFERENCE(vec3, x, z, y, z)
IMPLEMENT_VEC4_REFERENCE(vec3, x, z, z, x)
IMPLEMENT_VEC4_REFERENCE(vec3, x, z, z, y)
IMPLEMENT_VEC4_REFERENCE(vec3, x, z, z, z)
IMPLEMENT_VEC4_REFERENCE(vec3, y, x, x, x)
IMPLEMENT_VEC4_REFERENCE(vec3, y, x, x, y)
IMPLEMENT_VEC4_REFERENCE(vec3, y, x, x, z)
IMPLEMENT_VEC4_REFERENCE(vec3, y, x, y, x)
IMPLEMENT_VEC4_REFERENCE(vec3, y, x, y, y)
IMPLEMENT_VEC4_REFERENCE(vec3, y, x, y, z)
IMPLEMENT_VEC4_REFERENCE(vec3, y, x, z, x)
IMPLEMENT_VEC4_REFERENCE(vec3, y, x, z, y)
IMPLEMENT_VEC4_REFERENCE(vec3, y, x, z, z)
IMPLEMENT_VEC4_REFERENCE(vec3, y, y, x, x)
IMPLEMENT_VEC4_REFERENCE(vec3, y, y, x, y)
IMPLEMENT_VEC4_REFERENCE(vec3, y, y, x, z)
IMPLEMENT_VEC4_REFERENCE(vec3, y, y, y, x)
IMPLEMENT_VEC4_REFERENCE(vec3, y, y, y, y)
IMPLEMENT_VEC4_REFERENCE(vec3, y, y, y, z)
IMPLEMENT_VEC4_REFERENCE(vec3, y, y, z, x)
IMPLEMENT_VEC4_REFERENCE(vec3, y, y, z, y)
IMPLEMENT_VEC4_REFERENCE(vec3, y, y, z, z)
IMPLEMENT_VEC4_REFERENCE(vec3, y, z, x, x)
IMPLEMENT_VEC4_REFERENCE(vec3, y, z, x, y)
IMPLEMENT_VEC4_REFERENCE(vec3, y, z, x, z)
IMPLEMENT_VEC4_REFERENCE(vec3, y, z, y, x)
IMPLEMENT_VEC4_REFERENCE(vec3, y, z, y, y)
IMPLEMENT_VEC4_REFERENCE(vec3, y, z, y, z)
IMPLEMENT_VEC4_REFERENCE(vec3, y, z, z, x)
IMPLEMENT_VEC4_REFERENCE(vec3, y, z, z, y)
IMPLEMENT_VEC4_REFERENCE(vec3, y, z, z, z)
IMPLEMENT_VEC4_REFERENCE(vec3, z, x, x, x)
IMPLEMENT_VEC4_REFERENCE(vec3, z, x, x, y)
IMPLEMENT_VEC4_REFERENCE(vec3, z, x, x, z)
IMPLEMENT_VEC4_REFERENCE(vec3, z, x, y, x)
IMPLEMENT_VEC4_REFERENCE(vec3, z, x, y, y)
IMPLEMENT_VEC4_REFERENCE(vec3, z, x, y, z)
IMPLEMENT_VEC4_REFERENCE(vec3, z, x, z, x)
IMPLEMENT_VEC4_REFERENCE(vec3, z, x, z, y)
IMPLEMENT_VEC4_REFERENCE(vec3, z, x, z, z)
IMPLEMENT_VEC4_REFERENCE(vec3, z, y, x, x)
IMPLEMENT_VEC4_REFERENCE(vec3, z, y, x, y)
IMPLEMENT_VEC4_REFERENCE(vec3, z, y, x, z)
IMPLEMENT_VEC4_REFERENCE(vec3, z, y, y, x)
IMPLEMENT_VEC4_REFERENCE(vec3, z, y, y, y)
IMPLEMENT_VEC4_REFERENCE(vec3, z, y, y, z)
IMPLEMENT_VEC4_REFERENCE(vec3, z, y, z, x)
IMPLEMENT_VEC4_REFERENCE(vec3, z, y, z, y)
IMPLEMENT_VEC4_REFERENCE(vec3, z, y, z, z)
IMPLEMENT_VEC4_REFERENCE(vec3, z, z, x, x)
IMPLEMENT_VEC4_REFERENCE(vec3, z, z, x, y)
IMPLEMENT_VEC4_REFERENCE(vec3, z, z, x, z)
IMPLEMENT_VEC4_REFERENCE(vec3, z, z, y, x)
IMPLEMENT_VEC4_REFERENCE(vec3, z, z, y, y)
IMPLEMENT_VEC4_REFERENCE(vec3, z, z, y, z)
IMPLEMENT_VEC4_REFERENCE(vec3, z, z, z, x)
IMPLEMENT_VEC4_REFERENCE(vec3, z, z, z, y)
IMPLEMENT_VEC4_REFERENCE(vec3, z, z, z, z)

IMPLEMENT_FLOAT_REFERENCE(vec4, x)
IMPLEMENT_FLOAT_REFERENCE(vec4, y)
IMPLEMENT_FLOAT_REFERENCE(vec4, z)
IMPLEMENT_FLOAT_REFERENCE(vec4, w)
IMPLEMENT_VEC2_REFERENCE(vec4, x, x)
IMPLEMENT_VEC2_REFERENCE(vec4, x, y)
IMPLEMENT_VEC2_REFERENCE(vec4, x, z)
IMPLEMENT_VEC2_REFERENCE(vec4, x, w)
IMPLEMENT_VEC2_REFERENCE(vec4, y, x)
IMPLEMENT_VEC2_REFERENCE(vec4, y, y)
IMPLEMENT_VEC2_REFERENCE(vec4, y, z)
IMPLEMENT_VEC2_REFERENCE(vec4, y, w)
IMPLEMENT_VEC2_REFERENCE(vec4, z, x)
IMPLEMENT_VEC2_REFERENCE(vec4, z, y)
IMPLEMENT_VEC2_REFERENCE(vec4, z, z)
IMPLEMENT_VEC2_REFERENCE(vec4, z, w)
IMPLEMENT_VEC2_REFERENCE(vec4, w, x)
IMPLEMENT_VEC2_REFERENCE(vec4, w, y)
IMPLEMENT_VEC2_REFERENCE(vec4, w, z)
IMPLEMENT_VEC2_REFERENCE(vec4, w, w)
IMPLEMENT_VEC3_REFERENCE(vec4, x, x, x)
IMPLEMENT_VEC3_REFERENCE(vec4, x, x, y)
IMPLEMENT_VEC3_REFERENCE(vec4, x, x, z)
IMPLEMENT_VEC3_REFERENCE(vec4, x, x, w)
IMPLEMENT_VEC3_REFERENCE(vec4, x, y, x)
IMPLEMENT_VEC3_REFERENCE(vec4, x, y, y)
IMPLEMENT_VEC3_REFERENCE(vec4, x, y, z)
IMPLEMENT_VEC3_REFERENCE(vec4, x, y, w)
IMPLEMENT_VEC3_REFERENCE(vec4, x, z, x)
IMPLEMENT_VEC3_REFERENCE(vec4, x, z, y)
IMPLEMENT_VEC3_REFERENCE(vec4, x, z, z)
IMPLEMENT_VEC3_REFERENCE(vec4, x, z, w)
IMPLEMENT_VEC3_REFERENCE(vec4, x, w, x)
IMPLEMENT_VEC3_REFERENCE(vec4, x, w, y)
IMPLEMENT_VEC3_REFERENCE(vec4, x, w, z)
IMPLEMENT_VEC3_REFERENCE(vec4, x, w, w)
IMPLEMENT_VEC3_REFERENCE(vec4, y, x, x)
IMPLEMENT_VEC3_REFERENCE(vec4, y, x, y)
IMPLEMENT_VEC3_REFERENCE(vec4, y, x, z)
IMPLEMENT_VEC3_REFERENCE(vec4, y, x, w)
IMPLEMENT_VEC3_REFERENCE(vec4, y, y, x)
IMPLEMENT_VEC3_REFERENCE(vec4, y, y, y)
IMPLEMENT_VEC3_REFERENCE(vec4, y, y, z)
IMPLEMENT_VEC3_REFERENCE(vec4, y, y, w)
IMPLEMENT_VEC3_REFERENCE(vec4, y, z, x)
IMPLEMENT_VEC3_REFERENCE(vec4, y, z, y)
IMPLEMENT_VEC3_REFERENCE(vec4, y, z, z)
IMPLEMENT_VEC3_REFERENCE(vec4, y, z, w)
IMPLEMENT_VEC3_REFERENCE(vec4, y, w, x)
IMPLEMENT_VEC3_REFERENCE(vec4, y, w, y)
IMPLEMENT_VEC3_REFERENCE(vec4, y, w, z)
IMPLEMENT_VEC3_REFERENCE(vec4, y, w, w)
IMPLEMENT_VEC3_REFERENCE(vec4, z, x, x)
IMPLEMENT_VEC3_REFERENCE(vec4, z, x, y)
IMPLEMENT_VEC3_REFERENCE(vec4, z, x, z)
IMPLEMENT_VEC3_REFERENCE(vec4, z, x, w)
IMPLEMENT_VEC3_REFERENCE(vec4, z, y, x)
IMPLEMENT_VEC3_REFERENCE(vec4, z, y, y)
IMPLEMENT_VEC3_REFERENCE(vec4, z, y, z)
IMPLEMENT_VEC3_REFERENCE(vec4, z, y, w)
IMPLEMENT_VEC3_REFERENCE(vec4, z, z, x)
IMPLEMENT_VEC3_REFERENCE(vec4, z, z, y)
IMPLEMENT_VEC3_REFERENCE(vec4, z, z, z)
IMPLEMENT_VEC3_REFERENCE(vec4, z, z, w)
IMPLEMENT_VEC3_REFERENCE(vec4, z, w, x)
IMPLEMENT_VEC3_REFERENCE(vec4, z, w, y)
IMPLEMENT_VEC3_REFERENCE(vec4, z, w, z)
IMPLEMENT_VEC3_REFERENCE(vec4, z, w, w)
IMPLEMENT_VEC3_REFERENCE(vec4, w, x, x)
IMPLEMENT_VEC3_REFERENCE(vec4, w, x, y)
IMPLEMENT_VEC3_REFERENCE(vec4, w, x, z)
IMPLEMENT_VEC3_REFERENCE(vec4, w, x, w)
IMPLEMENT_VEC3_REFERENCE(vec4, w, y, x)
IMPLEMENT_VEC3_REFERENCE(vec4, w, y, y)
IMPLEMENT_VEC3_REFERENCE(vec4, w, y, z)
IMPLEMENT_VEC3_REFERENCE(vec4, w, y, w)
IMPLEMENT_VEC3_REFERENCE(vec4, w, z, x)
IMPLEMENT_VEC3_REFERENCE(vec4, w, z, y)
IMPLEMENT_VEC3_REFERENCE(vec4, w, z, z)
IMPLEMENT_VEC3_REFERENCE(vec4, w, z, w)
IMPLEMENT_VEC3_REFERENCE(vec4, w, w, x)
IMPLEMENT_VEC3_REFERENCE(vec4, w, w, y)
IMPLEMENT_VEC3_REFERENCE(vec4, w, w, z)
IMPLEMENT_VEC3_REFERENCE(vec4, w, w, w)
IMPLEMENT_VEC4_REFERENCE(vec4, x, x, x, x)
IMPLEMENT_VEC4_REFERENCE(vec4, x, x, x, y)
IMPLEMENT_VEC4_REFERENCE(vec4, x, x, x, z)
IMPLEMENT_VEC4_REFERENCE(vec4, x, x, x, w)
IMPLEMENT_VEC4_REFERENCE(vec4, x, x, y, x)
IMPLEMENT_VEC4_REFERENCE(vec4, x, x, y, y)
IMPLEMENT_VEC4_REFERENCE(vec4, x, x, y, z)
IMPLEMENT_VEC4_REFERENCE(vec4, x, x, y, w)
IMPLEMENT_VEC4_REFERENCE(vec4, x, x, z, x)
IMPLEMENT_VEC4_REFERENCE(vec4, x, x, z, y)
IMPLEMENT_VEC4_REFERENCE(vec4, x, x, z, z)
IMPLEMENT_VEC4_REFERENCE(vec4, x, x, z, w)
IMPLEMENT_VEC4_REFERENCE(vec4, x, x, w, x)
IMPLEMENT_VEC4_REFERENCE(vec4, x, x, w, y)
IMPLEMENT_VEC4_REFERENCE(vec4, x, x, w, z)
IMPLEMENT_VEC4_REFERENCE(vec4, x, x, w, w)
IMPLEMENT_VEC4_REFERENCE(vec4, x, y, x, x)
IMPLEMENT_VEC4_REFERENCE(vec4, x, y, x, y)
IMPLEMENT_VEC4_REFERENCE(vec4, x, y, x, z)
IMPLEMENT_VEC4_REFERENCE(vec4, x, y, x, w)
IMPLEMENT_VEC4_REFERENCE(vec4, x, y, y, x)
IMPLEMENT_VEC4_REFERENCE(vec4, x, y, y, y)
IMPLEMENT_VEC4_REFERENCE(vec4, x, y, y, z)
IMPLEMENT_VEC4_REFERENCE(vec4, x, y, y, w)
IMPLEMENT_VEC4_REFERENCE(vec4, x, y, z, x)
IMPLEMENT_VEC4_REFERENCE(vec4, x, y, z, y)
IMPLEMENT_VEC4_REFERENCE(vec4, x, y, z, z)
IMPLEMENT_VEC4_REFERENCE(vec4, x, y, z, w)
IMPLEMENT_VEC4_REFERENCE(vec4, x, y, w, x)
IMPLEMENT_VEC4_REFERENCE(vec4, x, y, w, y)
IMPLEMENT_VEC4_REFERENCE(vec4, x, y, w, z)
IMPLEMENT_VEC4_REFERENCE(vec4, x, y, w, w)
IMPLEMENT_VEC4_REFERENCE(vec4, x, z, x, x)
IMPLEMENT_VEC4_REFERENCE(vec4, x, z, x, y)
IMPLEMENT_VEC4_REFERENCE(vec4, x, z, x, z)
IMPLEMENT_VEC4_REFERENCE(vec4, x, z, x, w)
IMPLEMENT_VEC4_REFERENCE(vec4, x, z, y, x)
IMPLEMENT_VEC4_REFERENCE(vec4, x, z, y, y)
IMPLEMENT_VEC4_REFERENCE(vec4, x, z, y, z)
IMPLEMENT_VEC4_REFERENCE(vec4, x, z, y, w)
IMPLEMENT_VEC4_REFERENCE(vec4, x, z, z, x)
IMPLEMENT_VEC4_REFERENCE(vec4, x, z, z, y)
IMPLEMENT_VEC4_REFERENCE(vec4, x, z, z, z)
IMPLEMENT_VEC4_REFERENCE(vec4, x, z, z, w)
IMPLEMENT_VEC4_REFERENCE(vec4, x, z, w, x)
IMPLEMENT_VEC4_REFERENCE(vec4, x, z, w, y)
IMPLEMENT_VEC4_REFERENCE(vec4, x, z, w, z)
IMPLEMENT_VEC4_REFERENCE(vec4, x, z, w, w)
IMPLEMENT_VEC4_REFERENCE(vec4, x, w, x, x)
IMPLEMENT_VEC4_REFERENCE(vec4, x, w, x, y)
IMPLEMENT_VEC4_REFERENCE(vec4, x, w, x, z)
IMPLEMENT_VEC4_REFERENCE(vec4, x, w, x, w)
IMPLEMENT_VEC4_REFERENCE(vec4, x, w, y, x)
IMPLEMENT_VEC4_REFERENCE(vec4, x, w, y, y)
IMPLEMENT_VEC4_REFERENCE(vec4, x, w, y, z)
IMPLEMENT_VEC4_REFERENCE(vec4, x, w, y, w)
IMPLEMENT_VEC4_REFERENCE(vec4, x, w, z, x)
IMPLEMENT_VEC4_REFERENCE(vec4, x, w, z, y)
IMPLEMENT_VEC4_REFERENCE(vec4, x, w, z, z)
IMPLEMENT_VEC4_REFERENCE(vec4, x, w, z, w)
IMPLEMENT_VEC4_REFERENCE(vec4, x, w, w, x)
IMPLEMENT_VEC4_REFERENCE(vec4, x, w, w, y)
IMPLEMENT_VEC4_REFERENCE(vec4, x, w, w, z)
IMPLEMENT_VEC4_REFERENCE(vec4, x, w, w, w)
IMPLEMENT_VEC4_REFERENCE(vec4, y, x, x, x)
IMPLEMENT_VEC4_REFERENCE(vec4, y, x, x, y)
IMPLEMENT_VEC4_REFERENCE(vec4, y, x, x, z)
IMPLEMENT_VEC4_REFERENCE(vec4, y, x, x, w)
IMPLEMENT_VEC4_REFERENCE(vec4, y, x, y, x)
IMPLEMENT_VEC4_REFERENCE(vec4, y, x, y, y)
IMPLEMENT_VEC4_REFERENCE(vec4, y, x, y, z)
IMPLEMENT_VEC4_REFERENCE(vec4, y, x, y, w)
IMPLEMENT_VEC4_REFERENCE(vec4, y, x, z, x)
IMPLEMENT_VEC4_REFERENCE(vec4, y, x, z, y)
IMPLEMENT_VEC4_REFERENCE(vec4, y, x, z, z)
IMPLEMENT_VEC4_REFERENCE(vec4, y, x, z, w)
IMPLEMENT_VEC4_REFERENCE(vec4, y, x, w, x)
IMPLEMENT_VEC4_REFERENCE(vec4, y, x, w, y)
IMPLEMENT_VEC4_REFERENCE(vec4, y, x, w, z)
IMPLEMENT_VEC4_REFERENCE(vec4, y, x, w, w)
IMPLEMENT_VEC4_REFERENCE(vec4, y, y, x, x)
IMPLEMENT_VEC4_REFERENCE(vec4, y, y, x, y)
IMPLEMENT_VEC4_REFERENCE(vec4, y, y, x, z)
IMPLEMENT_VEC4_REFERENCE(vec4, y, y, x, w)
IMPLEMENT_VEC4_REFERENCE(vec4, y, y, y, x)
IMPLEMENT_VEC4_REFERENCE(vec4, y, y, y, y)
IMPLEMENT_VEC4_REFERENCE(vec4, y, y, y, z)
IMPLEMENT_VEC4_REFERENCE(vec4, y, y, y, w)
IMPLEMENT_VEC4_REFERENCE(vec4, y, y, z, x)
IMPLEMENT_VEC4_REFERENCE(vec4, y, y, z, y)
IMPLEMENT_VEC4_REFERENCE(vec4, y, y, z, z)
IMPLEMENT_VEC4_REFERENCE(vec4, y, y, z, w)
IMPLEMENT_VEC4_REFERENCE(vec4, y, y, w, x)
IMPLEMENT_VEC4_REFERENCE(vec4, y, y, w, y)
IMPLEMENT_VEC4_REFERENCE(vec4, y, y, w, z)
IMPLEMENT_VEC4_REFERENCE(vec4, y, y, w, w)
IMPLEMENT_VEC4_REFERENCE(vec4, y, z, x, x)
IMPLEMENT_VEC4_REFERENCE(vec4, y, z, x, y)
IMPLEMENT_VEC4_REFERENCE(vec4, y, z, x, z)
IMPLEMENT_VEC4_REFERENCE(vec4, y, z, x, w)
IMPLEMENT_VEC4_REFERENCE(vec4, y, z, y, x)
IMPLEMENT_VEC4_REFERENCE(vec4, y, z, y, y)
IMPLEMENT_VEC4_REFERENCE(vec4, y, z, y, z)
IMPLEMENT_VEC4_REFERENCE(vec4, y, z, y, w)
IMPLEMENT_VEC4_REFERENCE(vec4, y, z, z, x)
IMPLEMENT_VEC4_REFERENCE(vec4, y, z, z, y)
IMPLEMENT_VEC4_REFERENCE(vec4, y, z, z, z)
IMPLEMENT_VEC4_REFERENCE(vec4, y, z, z, w)
IMPLEMENT_VEC4_REFERENCE(vec4, y, z, w, x)
IMPLEMENT_VEC4_REFERENCE(vec4, y, z, w, y)
IMPLEMENT_VEC4_REFERENCE(vec4, y, z, w, z)
IMPLEMENT_VEC4_REFERENCE(vec4, y, z, w, w)
IMPLEMENT_VEC4_REFERENCE(vec4, y, w, x, x)
IMPLEMENT_VEC4_REFERENCE(vec4, y, w, x, y)
IMPLEMENT_VEC4_REFERENCE(vec4, y, w, x, z)
IMPLEMENT_VEC4_REFERENCE(vec4, y, w, x, w)
IMPLEMENT_VEC4_REFERENCE(vec4, y, w, y, x)
IMPLEMENT_VEC4_REFERENCE(vec4, y, w, y, y)
IMPLEMENT_VEC4_REFERENCE(vec4, y, w, y, z)
IMPLEMENT_VEC4_REFERENCE(vec4, y, w, y, w)
IMPLEMENT_VEC4_REFERENCE(vec4, y, w, z, x)
IMPLEMENT_VEC4_REFERENCE(vec4, y, w, z, y)
IMPLEMENT_VEC4_REFERENCE(vec4, y, w, z, z)
IMPLEMENT_VEC4_REFERENCE(vec4, y, w, z, w)
IMPLEMENT_VEC4_REFERENCE(vec4, y, w, w, x)
IMPLEMENT_VEC4_REFERENCE(vec4, y, w, w, y)
IMPLEMENT_VEC4_REFERENCE(vec4, y, w, w, z)
IMPLEMENT_VEC4_REFERENCE(vec4, y, w, w, w)
IMPLEMENT_VEC4_REFERENCE(vec4, z, x, x, x)
IMPLEMENT_VEC4_REFERENCE(vec4, z, x, x, y)
IMPLEMENT_VEC4_REFERENCE(vec4, z, x, x, z)
IMPLEMENT_VEC4_REFERENCE(vec4, z, x, x, w)
IMPLEMENT_VEC4_REFERENCE(vec4, z, x, y, x)
IMPLEMENT_VEC4_REFERENCE(vec4, z, x, y, y)
IMPLEMENT_VEC4_REFERENCE(vec4, z, x, y, z)
IMPLEMENT_VEC4_REFERENCE(vec4, z, x, y, w)
IMPLEMENT_VEC4_REFERENCE(vec4, z, x, z, x)
IMPLEMENT_VEC4_REFERENCE(vec4, z, x, z, y)
IMPLEMENT_VEC4_REFERENCE(vec4, z, x, z, z)
IMPLEMENT_VEC4_REFERENCE(vec4, z, x, z, w)
IMPLEMENT_VEC4_REFERENCE(vec4, z, x, w, x)
IMPLEMENT_VEC4_REFERENCE(vec4, z, x, w, y)
IMPLEMENT_VEC4_REFERENCE(vec4, z, x, w, z)
IMPLEMENT_VEC4_REFERENCE(vec4, z, x, w, w)
IMPLEMENT_VEC4_REFERENCE(vec4, z, y, x, x)
IMPLEMENT_VEC4_REFERENCE(vec4, z, y, x, y)
IMPLEMENT_VEC4_REFERENCE(vec4, z, y, x, z)
IMPLEMENT_VEC4_REFERENCE(vec4, z, y, x, w)
IMPLEMENT_VEC4_REFERENCE(vec4, z, y, y, x)
IMPLEMENT_VEC4_REFERENCE(vec4, z, y, y, y)
IMPLEMENT_VEC4_REFERENCE(vec4, z, y, y, z)
IMPLEMENT_VEC4_REFERENCE(vec4, z, y, y, w)
IMPLEMENT_VEC4_REFERENCE(vec4, z, y, z, x)
IMPLEMENT_VEC4_REFERENCE(vec4, z, y, z, y)
IMPLEMENT_VEC4_REFERENCE(vec4, z, y, z, z)
IMPLEMENT_VEC4_REFERENCE(vec4, z, y, z, w)
IMPLEMENT_VEC4_REFERENCE(vec4, z, y, w, x)
IMPLEMENT_VEC4_REFERENCE(vec4, z, y, w, y)
IMPLEMENT_VEC4_REFERENCE(vec4, z, y, w, z)
IMPLEMENT_VEC4_REFERENCE(vec4, z, y, w, w)
IMPLEMENT_VEC4_REFERENCE(vec4, z, z, x, x)
IMPLEMENT_VEC4_REFERENCE(vec4, z, z, x, y)
IMPLEMENT_VEC4_REFERENCE(vec4, z, z, x, z)
IMPLEMENT_VEC4_REFERENCE(vec4, z, z, x, w)
IMPLEMENT_VEC4_REFERENCE(vec4, z, z, y, x)
IMPLEMENT_VEC4_REFERENCE(vec4, z, z, y, y)
IMPLEMENT_VEC4_REFERENCE(vec4, z, z, y, z)
IMPLEMENT_VEC4_REFERENCE(vec4, z, z, y, w)
IMPLEMENT_VEC4_REFERENCE(vec4, z, z, z, x)
IMPLEMENT_VEC4_REFERENCE(vec4, z, z, z, y)
IMPLEMENT_VEC4_REFERENCE(vec4, z, z, z, z)
IMPLEMENT_VEC4_REFERENCE(vec4, z, z, z, w)
IMPLEMENT_VEC4_REFERENCE(vec4, z, z, w, x)
IMPLEMENT_VEC4_REFERENCE(vec4, z, z, w, y)
IMPLEMENT_VEC4_REFERENCE(vec4, z, z, w, z)
IMPLEMENT_VEC4_REFERENCE(vec4, z, z, w, w)
IMPLEMENT_VEC4_REFERENCE(vec4, z, w, x, x)
IMPLEMENT_VEC4_REFERENCE(vec4, z, w, x, y)
IMPLEMENT_VEC4_REFERENCE(vec4, z, w, x, z)
IMPLEMENT_VEC4_REFERENCE(vec4, z, w, x, w)
IMPLEMENT_VEC4_REFERENCE(vec4, z, w, y, x)
IMPLEMENT_VEC4_REFERENCE(vec4, z, w, y, y)
IMPLEMENT_VEC4_REFERENCE(vec4, z, w, y, z)
IMPLEMENT_VEC4_REFERENCE(vec4, z, w, y, w)
IMPLEMENT_VEC4_REFERENCE(vec4, z, w, z, x)
IMPLEMENT_VEC4_REFERENCE(vec4, z, w, z, y)
IMPLEMENT_VEC4_REFERENCE(vec4, z, w, z, z)
IMPLEMENT_VEC4_REFERENCE(vec4, z, w, z, w)
IMPLEMENT_VEC4_REFERENCE(vec4, z, w, w, x)
IMPLEMENT_VEC4_REFERENCE(vec4, z, w, w, y)
IMPLEMENT_VEC4_REFERENCE(vec4, z, w, w, z)
IMPLEMENT_VEC4_REFERENCE(vec4, z, w, w, w)
IMPLEMENT_VEC4_REFERENCE(vec4, w, x, x, x)
IMPLEMENT_VEC4_REFERENCE(vec4, w, x, x, y)
IMPLEMENT_VEC4_REFERENCE(vec4, w, x, x, z)
IMPLEMENT_VEC4_REFERENCE(vec4, w, x, x, w)
IMPLEMENT_VEC4_REFERENCE(vec4, w, x, y, x)
IMPLEMENT_VEC4_REFERENCE(vec4, w, x, y, y)
IMPLEMENT_VEC4_REFERENCE(vec4, w, x, y, z)
IMPLEMENT_VEC4_REFERENCE(vec4, w, x, y, w)
IMPLEMENT_VEC4_REFERENCE(vec4, w, x, z, x)
IMPLEMENT_VEC4_REFERENCE(vec4, w, x, z, y)
IMPLEMENT_VEC4_REFERENCE(vec4, w, x, z, z)
IMPLEMENT_VEC4_REFERENCE(vec4, w, x, z, w)
IMPLEMENT_VEC4_REFERENCE(vec4, w, x, w, x)
IMPLEMENT_VEC4_REFERENCE(vec4, w, x, w, y)
IMPLEMENT_VEC4_REFERENCE(vec4, w, x, w, z)
IMPLEMENT_VEC4_REFERENCE(vec4, w, x, w, w)
IMPLEMENT_VEC4_REFERENCE(vec4, w, y, x, x)
IMPLEMENT_VEC4_REFERENCE(vec4, w, y, x, y)
IMPLEMENT_VEC4_REFERENCE(vec4, w, y, x, z)
IMPLEMENT_VEC4_REFERENCE(vec4, w, y, x, w)
IMPLEMENT_VEC4_REFERENCE(vec4, w, y, y, x)
IMPLEMENT_VEC4_REFERENCE(vec4, w, y, y, y)
IMPLEMENT_VEC4_REFERENCE(vec4, w, y, y, z)
IMPLEMENT_VEC4_REFERENCE(vec4, w, y, y, w)
IMPLEMENT_VEC4_REFERENCE(vec4, w, y, z, x)
IMPLEMENT_VEC4_REFERENCE(vec4, w, y, z, y)
IMPLEMENT_VEC4_REFERENCE(vec4, w, y, z, z)
IMPLEMENT_VEC4_REFERENCE(vec4, w, y, z, w)
IMPLEMENT_VEC4_REFERENCE(vec4, w, y, w, x)
IMPLEMENT_VEC4_REFERENCE(vec4, w, y, w, y)
IMPLEMENT_VEC4_REFERENCE(vec4, w, y, w, z)
IMPLEMENT_VEC4_REFERENCE(vec4, w, y, w, w)
IMPLEMENT_VEC4_REFERENCE(vec4, w, z, x, x)
IMPLEMENT_VEC4_REFERENCE(vec4, w, z, x, y)
IMPLEMENT_VEC4_REFERENCE(vec4, w, z, x, z)
IMPLEMENT_VEC4_REFERENCE(vec4, w, z, x, w)
IMPLEMENT_VEC4_REFERENCE(vec4, w, z, y, x)
IMPLEMENT_VEC4_REFERENCE(vec4, w, z, y, y)
IMPLEMENT_VEC4_REFERENCE(vec4, w, z, y, z)
IMPLEMENT_VEC4_REFERENCE(vec4, w, z, y, w)
IMPLEMENT_VEC4_REFERENCE(vec4, w, z, z, x)
IMPLEMENT_VEC4_REFERENCE(vec4, w, z, z, y)
IMPLEMENT_VEC4_REFERENCE(vec4, w, z, z, z)
IMPLEMENT_VEC4_REFERENCE(vec4, w, z, z, w)
IMPLEMENT_VEC4_REFERENCE(vec4, w, z, w, x)
IMPLEMENT_VEC4_REFERENCE(vec4, w, z, w, y)
IMPLEMENT_VEC4_REFERENCE(vec4, w, z, w, z)
IMPLEMENT_VEC4_REFERENCE(vec4, w, z, w, w)
IMPLEMENT_VEC4_REFERENCE(vec4, w, w, x, x)
IMPLEMENT_VEC4_REFERENCE(vec4, w, w, x, y)
IMPLEMENT_VEC4_REFERENCE(vec4, w, w, x, z)
IMPLEMENT_VEC4_REFERENCE(vec4, w, w, x, w)
IMPLEMENT_VEC4_REFERENCE(vec4, w, w, y, x)
IMPLEMENT_VEC4_REFERENCE(vec4, w, w, y, y)
IMPLEMENT_VEC4_REFERENCE(vec4, w, w, y, z)
IMPLEMENT_VEC4_REFERENCE(vec4, w, w, y, w)
IMPLEMENT_VEC4_REFERENCE(vec4, w, w, z, x)
IMPLEMENT_VEC4_REFERENCE(vec4, w, w, z, y)
IMPLEMENT_VEC4_REFERENCE(vec4, w, w, z, z)
IMPLEMENT_VEC4_REFERENCE(vec4, w, w, z, w)
IMPLEMENT_VEC4_REFERENCE(vec4, w, w, w, x)
IMPLEMENT_VEC4_REFERENCE(vec4, w, w, w, y)
IMPLEMENT_VEC4_REFERENCE(vec4, w, w, w, z)
IMPLEMENT_VEC4_REFERENCE(vec4, w, w, w, w)
#endif

vec2::vec2()
#ifdef ENABLE_SWIZZLE
 : parent(this)
#endif
{}

vec2::vec2(float value) : x(value), y(value)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

vec2::vec2(float _x, float _y) : x(_x), y(_y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

vec2::vec2(const vec3& v) : x(v.x), y(v.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

vec2::vec2(const vec4& v) : x(v.x), y(v.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

vec2::vec2(const vec2& v) : x(v.x), y(v.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

vec2::vec2(vec2&& v) : x(move(v.x)), y(move(v.y))
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}


float* vec2::data()const
{
	return (float*)this;
}

vec2& vec2::operator =(const vec2& v)
{
	if(this != &v)
	{
		x = v.x;
		y = v.y;
	}

	return *this;
}

vec2& vec2::operator =(vec2&& v)
{
	if(this != &v)
	{
		x = move(v.x);
		y = move(v.y);
	}

	return *this;
}

vec2::operator dvec2()const
{
	return dvec2(x, y);
}

vec2::operator uvec2()const
{
	return uvec2(x, y);
}

vec2::operator ivec2()const
{
	return ivec2(x, y);
}

vec2::operator bvec2()const
{
	return bvec2(x, y);
}

float& vec2::operator [](uint i)
{
	switch(i)
	{
		case 0: return x;
		case 1: return y;
		default:
		{
			throw IndexError(i, 0, 1);\
		}
	}
}

float vec2::operator [](uint i)const
{
	switch(i)
	{
		case 0: return x;
		case 1: return y;
		default:
		{
			throw IndexError(i, 0, 1);\
		}
	}
}

vec2 vec2::operator +()const
{
	return *this;
}

vec2 vec2::operator -()const
{
	return vec2(-x, -y);
}

vec2& vec2::operator +=(const vec2& v)
{
	x += v.x;
	y += v.y;

	return *this;
}

vec2& vec2::operator -=(const vec2& v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}

vec2& vec2::operator *=(const vec2& v)
{
	x *= v.x;
	y *= v.y;

	return *this;
}

vec2& vec2::operator /=(const vec2& v)
{
	if(is_zero(v.x))
	{
		throw ZeroDivisionError("Error when vec2 v1 /= vec2 v2: v2.x is 0 now.");
	}
	if(is_zero(v.y))
	{
		throw ZeroDivisionError("Error when vec2 v1 /= vec2 v2: v2.y is 0 now.");
	}

	x /= v.x;
	y /= v.y;

	return *this;
}

vec2& vec2::operator +=(float value)
{
	x += value;
	y += value;

	return *this;
}

vec2& vec2::operator -=(float value)
{
	x -= value;
	y -= value;

	return *this;
}

vec2& vec2::operator *=(float value)
{
	x *= value;
	y *= value;

	return *this;
}

vec2& vec2::operator /=(float value)
{
	if(is_zero(value))
	{
		throw ZeroDivisionError("Error when vec2 v /= float value: value is 0 now.");
	}

	x /= value;
	y /= value;

	return *this;
}

vec2 operator +(const vec2& v1, const vec2& v2)
{
	return vec2(v1.x + v2.x, v1.y + v2.y);
}

vec2 operator -(const vec2& v1, const vec2& v2)
{
	return vec2(v1.x - v2.x, v1.y - v2.y);
}

vec2 operator *(const vec2& v1, const vec2& v2)
{
	return vec2(v1.x * v2.x, v1.y * v2.y);
}

vec2 operator /(const vec2& v1, const vec2& v2)
{
	if(is_zero(v2.x))
	{
		throw ZeroDivisionError("Error when vec2 v1 / vec2 v2: v2.x is 0 now.");
	}
	if(is_zero(v2.y))
	{
		throw ZeroDivisionError("Error when vec2 v1 / vec2 v2: v2.y is 0 now.");
	}
	return vec2(v1.x / v2.x, v1.y / v2.y);
}

vec2 operator +(const vec2& v1, const uvec2& v2)
{
	return vec2(v1.x + v2.x, v1.y + v2.y);
}

vec2 operator -(const vec2& v1, const uvec2& v2)
{
	return vec2(v1.x - v2.x, v1.y - v2.y);
}

vec2 operator *(const vec2& v1, const uvec2& v2)
{
	return vec2(v1.x * v2.x, v1.y * v2.y);
}

vec2 operator /(const vec2& v1, const uvec2& v2)
{
	if(v2.x == 0)
	{
		throw ZeroDivisionError("Error when vec2 v1 / vec2 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw ZeroDivisionError("Error when vec2 v1 / vec2 v2: v2.y is 0 now.");
	}
	return vec2(v1.x / v2.x, v1.y / v2.y);
}

vec2 operator +(const uvec2& v1, const vec2& v2)
{
	return vec2(v1.x + v2.x, v1.y + v2.y);
}

vec2 operator -(const uvec2& v1, const vec2& v2)
{
	return vec2(v1.x - v2.x, v1.y - v2.y);
}

vec2 operator *(const uvec2& v1, const vec2& v2)
{
	return vec2(v1.x * v2.x, v1.y * v2.y);
}

vec2 operator /(const uvec2& v1, const vec2& v2)
{
	if(is_zero(v2.x))
	{
		throw ZeroDivisionError("Error when vec2 v1 / vec2 v2: v2.x is 0 now.");
	}
	if(is_zero(v2.y))
	{
		throw ZeroDivisionError("Error when vec2 v1 / vec2 v2: v2.y is 0 now.");
	}
	return vec2(v1.x / v2.x, v1.y / v2.y);
}

vec2 operator +(const vec2& v1, const ivec2& v2)
{
	return vec2(v1.x + v2.x, v1.y + v2.y);
}

vec2 operator -(const vec2& v1, const ivec2& v2)
{
	return vec2(v1.x - v2.x, v1.y - v2.y);
}

vec2 operator *(const vec2& v1, const ivec2& v2)
{
	return vec2(v1.x * v2.x, v1.y * v2.y);
}

vec2 operator /(const vec2& v1, const ivec2& v2)
{
	if(v2.x == 0)
	{
		throw ZeroDivisionError("Error when vec2 v1 / vec2 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw ZeroDivisionError("Error when vec2 v1 / vec2 v2: v2.y is 0 now.");
	}
	return vec2(v1.x / v2.x, v1.y / v2.y);
}

vec2 operator +(const ivec2& v1, const vec2& v2)
{
	return vec2(v1.x + v2.x, v1.y + v2.y);
}

vec2 operator -(const ivec2& v1, const vec2& v2)
{
	return vec2(v1.x - v2.x, v1.y - v2.y);
}

vec2 operator *(const ivec2& v1, const vec2& v2)
{
	return vec2(v1.x * v2.x, v1.y * v2.y);
}

vec2 operator /(const ivec2& v1, const vec2& v2)
{
	if(is_zero(v2.x))
	{
		throw ZeroDivisionError("Error when vec2 v1 / vec2 v2: v2.x is 0 now.");
	}
	if(is_zero(v2.y))
	{
		throw ZeroDivisionError("Error when vec2 v1 / vec2 v2: v2.y is 0 now.");
	}
	return vec2(v1.x / v2.x, v1.y / v2.y);
}

dvec2 operator +(const vec2& v, double value)
{
	return dvec2(v.x + value, v.y + value);
}

dvec2 operator -(const vec2& v, double value)
{
	return dvec2(v.x - value, v.y - value);
}

dvec2 operator *(const vec2& v, double value)
{
	return dvec2(v.x * value, v.y * value);
}

dvec2 operator /(const vec2& v, double value)
{
	if(is_zero(value))
	{
		throw ZeroDivisionError("Error when vec2 v / double value: value is 0 now.");
	}
	return dvec2(v.x / value, v.y / value);
}

dvec2 operator +(double value, const vec2& v)
{
	return dvec2(value + v.x, value + v.y);
}

dvec2 operator -(double value, const vec2& v)
{
	return dvec2(value - v.x, value - v.y);
}

dvec2 operator *(double value, const vec2& v)
{
	return dvec2(value * v.x, value * v.y);
}

dvec2 operator /(double value, const vec2& v)
{
	if(is_zero(v.x))
	{
		throw ZeroDivisionError("Error when double value / vec2 v: v.x is 0 now.");
	}
	if(is_zero(v.y))
	{
		throw ZeroDivisionError("Error when double value / vec2 v: v.y is 0 now.");
	}
	return dvec2(value / v.x, value / v.y);
}

vec2 operator +(const vec2& v, float value)
{
	return vec2(v.x + value, v.y + value);
}

vec2 operator -(const vec2& v, float value)
{
	return vec2(v.x - value, v.y - value);
}

vec2 operator *(const vec2& v, float value)
{
	return vec2(v.x * value, v.y * value);
}

vec2 operator /(const vec2& v, float value)
{
	if(is_zero(value))
	{
		throw ZeroDivisionError("Error when vec2 v / float value: value is 0 now.");
	}
	return vec2(v.x / value, v.y / value);
}

vec2 operator +(float value, const vec2& v)
{
	return vec2(value + v.x, value + v.y);
}

vec2 operator -(float value, const vec2& v)
{
	return vec2(value - v.x, value - v.y);
}

vec2 operator *(float value, const vec2& v)
{
	return vec2(value * v.x, value * v.y);
}

vec2 operator /(float value, const vec2& v)
{
	if(is_zero(v.x))
	{
		throw ZeroDivisionError("Error when float value / vec2 v: v.x is 0 now.");
	}
	if(is_zero(v.y))
	{
		throw ZeroDivisionError("Error when float value / vec2 v: v.y is 0 now.");
	}
	return vec2(value / v.x, value / v.y);
}

vec2 operator +(const vec2& v, uint value)
{
	return vec2(v.x + value, v.y + value);
}

vec2 operator -(const vec2& v, uint value)
{
	return vec2(v.x - value, v.y - value);
}

vec2 operator *(const vec2& v, uint value)
{
	return vec2(v.x * value, v.y * value);
}

vec2 operator /(const vec2& v, uint value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error when vec2 v / uint value: value is 0 now.");
	}
	return vec2(v.x / value, v.y / value);
}

vec2 operator +(uint value, const vec2& v)
{
	return vec2(value + v.x, value + v.y);
}

vec2 operator -(uint value, const vec2& v)
{
	return vec2(value - v.x, value - v.y);
}

vec2 operator *(uint value, const vec2& v)
{
	return vec2(value * v.x, value * v.y);
}

vec2 operator /(uint value, const vec2& v)
{
	if(is_zero(v.x))
	{
		throw ZeroDivisionError("Error when uint value / vec2 v: v.x is 0 now.");
	}
	if(is_zero(v.y))
	{
		throw ZeroDivisionError("Error when uint value / vec2 v: v.y is 0 now.");
	}
	return vec2(value / v.x, value / v.y);
}

vec2 operator +(const vec2& v, int value)
{
	return vec2(v.x + value, v.y + value);
}

vec2 operator -(const vec2& v, int value)
{
	return vec2(v.x - value, v.y - value);
}

vec2 operator *(const vec2& v, int value)
{
	return vec2(v.x * value, v.y * value);
}

vec2 operator /(const vec2& v, int value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error when vec2 v / int value: value is 0 now.");
	}
	return vec2(v.x / value, v.y / value);
}

vec2 operator +(int value, const vec2& v)
{
	return vec2(value + v.x, value + v.y);
}

vec2 operator -(int value, const vec2& v)
{
	return vec2(value - v.x, value - v.y);
}

vec2 operator *(int value, const vec2& v)
{
	return vec2(value * v.x, value * v.y);
}

vec2 operator /(int value, const vec2& v)
{
	if(is_zero(v.x))
	{
		throw ZeroDivisionError("Error when int value / vec2 v: v.x is 0 now.");
	}
	if(is_zero(v.y))
	{
		throw ZeroDivisionError("Error when int value / vec2 v: v.y is 0 now.");
	}
	return vec2(value / v.x, value / v.y);
}

ostream& operator <<(ostream& out, const vec2& v)
{
	out << "vec2(" << v.x << ", " << v.y << ")";
	return out;
}

vec3::vec3()
#ifdef ENABLE_SWIZZLE
 : parent(this)
#endif
{}

vec3::vec3(float value) : x(value), y(value), z(value)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

vec3::vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

vec3::vec3(const vec2& v, float _z) : x(v.x), y(v.y), z(_z)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

vec3::vec3(float _x, const vec2& v) : x(_x), y(v.x), z(v.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

vec3::vec3(const vec4& v) : x(v.x), y(v.y), z(v.z)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

vec3::vec3(const vec3& v) : x(v.x), y(v.y), z(v.z)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

vec3::vec3(vec3&& v) : x(move(v.x)), y(move(v.y)), z(move(v.z))
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}


float* vec3::data()const
{
	return (float*)this;
}

vec3& vec3::operator =(const vec3& v)
{
	if(this != &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	return *this;
}

vec3& vec3::operator =(vec3&& v)
{
	if(this != &v)
	{
		x = move(v.x);
		y = move(v.y);
		z = move(v.z);
	}

	return *this;
}

vec3::operator dvec3()const
{
	return dvec3(x, y, z);
}

vec3::operator uvec3()const
{
	return uvec3(x, y, z);
}

vec3::operator ivec3()const
{
	return ivec3(x, y, z);
}

vec3::operator bvec3()const
{
	return bvec3(x, y, z);
}

float& vec3::operator [](uint i)
{
	switch(i)
	{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default:
		{
			throw IndexError(i, 0, 2);\
		}
	}
}

float vec3::operator [](uint i)const
{
	switch(i)
	{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default:
		{
			throw IndexError(i, 0, 2);\
		}
	}
}

vec3 vec3::operator +()const
{
	return *this;
}

vec3 vec3::operator -()const
{
	return vec3(-x, -y, -z);
}

vec3& vec3::operator +=(const vec3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

vec3& vec3::operator -=(const vec3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

vec3& vec3::operator *=(const vec3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}

vec3& vec3::operator /=(const vec3& v)
{
	if(is_zero(v.x))
	{
		throw ZeroDivisionError("Error using vec3 v1 /= vec3 v2:\nv2.x is 0 now.");
	}
	if(is_zero(v.y))
	{
		throw ZeroDivisionError("Error using vec3 v1 /= vec3 v2:\nv2.y is 0 now.");
	}
	if(is_zero(v.z))
	{
		throw ZeroDivisionError("Error using vec3 v1 /= vec3 v2:\nv2.z is 0 now.");
	}

	x /= v.x;
	y /= v.y;
	z /= v.z;

	return *this;
}

vec3& vec3::operator +=(float value)
{
	x += value;
	y += value;
	z += value;

	return *this;
}

vec3& vec3::operator -=(float value)
{
	x -= value;
	y -= value;
	z -= value;

	return *this;
}

vec3& vec3::operator *=(float value)
{
	x *= value;
	y *= value;
	z *= value;

	return *this;
}

vec3& vec3::operator /=(float value)
{
	if(is_zero(value))
	{
		throw ZeroDivisionError("Error using vec3 v /= float value:\nvalue is 0 now.");
	}
	x /= value;
	y /= value;
	z /= value;

	return *this;
}

vec3 operator +(const vec3& v1, const vec3& v2)
{
	return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

vec3 operator -(const vec3& v1, const vec3& v2)
{
	return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

vec3 operator *(const vec3& v1, const vec3& v2)
{
	return vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

vec3 operator /(const vec3& v1, const vec3& v2)
{
	if(is_zero(v2.x))
	{
		throw ZeroDivisionError("Error when vec3 v1 / vec3 v2: v2.x is 0 now.");
	}
	if(is_zero(v2.y))
	{
		throw ZeroDivisionError("Error when vec3 v1 / vec3 v2: v2.y is 0 now.");
	}
	if(is_zero(v2.z))
	{
		throw ZeroDivisionError("Error when vec3 v1 / vec3 v2: v2.z is 0 now.");
	}
	return vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

vec3 operator +(const vec3& v1, const uvec3& v2)
{
	return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

vec3 operator -(const vec3& v1, const uvec3& v2)
{
	return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

vec3 operator *(const vec3& v1, const uvec3& v2)
{
	return vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

vec3 operator /(const vec3& v1, const uvec3& v2)
{
	if(v2.x == 0)
	{
		throw ZeroDivisionError("Error when vec3 v1 / vec3 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw ZeroDivisionError("Error when vec3 v1 / vec3 v2: v2.y is 0 now.");
	}
	if(v2.z == 0)
	{
		throw ZeroDivisionError("Error when vec3 v1 / vec3 v2: v2.z is 0 now.");
	}
	return vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

vec3 operator +(const uvec3& v1, const vec3& v2)
{
	return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

vec3 operator -(const uvec3& v1, const vec3& v2)
{
	return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

vec3 operator *(const uvec3& v1, const vec3& v2)
{
	return vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

vec3 operator /(const uvec3& v1, const vec3& v2)
{
	if(is_zero(v2.x))
	{
		throw ZeroDivisionError("Error when vec3 v1 / vec3 v2: v2.x is 0 now.");
	}
	if(is_zero(v2.y))
	{
		throw ZeroDivisionError("Error when vec3 v1 / vec3 v2: v2.y is 0 now.");
	}
	if(is_zero(v2.z))
	{
		throw ZeroDivisionError("Error when vec3 v1 / vec3 v2: v2.z is 0 now.");
	}
	return vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

vec3 operator +(const vec3& v1, const ivec3& v2)
{
	return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

vec3 operator -(const vec3& v1, const ivec3& v2)
{
	return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

vec3 operator *(const vec3& v1, const ivec3& v2)
{
	return vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

vec3 operator /(const vec3& v1, const ivec3& v2)
{
	if(v2.x == 0)
	{
		throw ZeroDivisionError("Error when vec3 v1 / vec3 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw ZeroDivisionError("Error when vec3 v1 / vec3 v2: v2.y is 0 now.");
	}
	if(v2.z == 0)
	{
		throw ZeroDivisionError("Error when vec3 v1 / vec3 v2: v2.z is 0 now.");
	}
	return vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

vec3 operator +(const ivec3& v1, const vec3& v2)
{
	return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

vec3 operator -(const ivec3& v1, const vec3& v2)
{
	return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

vec3 operator *(const ivec3& v1, const vec3& v2)
{
	return vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

vec3 operator /(const ivec3& v1, const vec3& v2)
{
	if(is_zero(v2.x))
	{
		throw ZeroDivisionError("Error when vec3 v1 / vec3 v2: v2.x is 0 now.");
	}
	if(is_zero(v2.y))
	{
		throw ZeroDivisionError("Error when vec3 v1 / vec3 v2: v2.y is 0 now.");
	}
	if(is_zero(v2.z))
	{
		throw ZeroDivisionError("Error when vec3 v1 / vec3 v2: v2.z is 0 now.");
	}
	return vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

dvec3 operator +(const vec3& v, double value)
{
	return dvec3(v.x + value, v.y + value, v.z + value);
}

dvec3 operator -(const vec3& v, double value)
{
	return dvec3(v.x - value, v.y - value, v.z - value);
}

dvec3 operator *(const vec3& v, double value)
{
	return dvec3(v.x * value, v.y * value, v.z * value);
}

dvec3 operator /(const vec3& v, double value)
{
	if(is_zero(value))
	{
		throw ZeroDivisionError("Error when vec3 v / double value: value is 0 now.");
	}
	return dvec3(v.x / value, v.y / value, v.z / value);
}

dvec3 operator +(double value, const vec3& v)
{
	return dvec3(value + v.x, value + v.y, value + v.z);
}

dvec3 operator -(double value, const vec3& v)
{
	return dvec3(value - v.x, value - v.y, value - v.z);
}

dvec3 operator *(double value, const vec3& v)
{
	return dvec3(value * v.x, value * v.y, value * v.z);
}

dvec3 operator /(double value, const vec3& v)
{
	if(is_zero(v.x))
	{
		throw ZeroDivisionError("Error when double value / vec3 v: v.x is 0 now.");
	}
	if(is_zero(v.y))
	{
		throw ZeroDivisionError("Error when double value / vec3 v: v.y is 0 now.");
	}
	if(is_zero(v.z))
	{
		throw ZeroDivisionError("Error when double value / vec3 v: v.z is 0 now.");
	}
	return dvec3(value / v.x, value / v.y, value / v.z);
}

vec3 operator +(const vec3& v, float value)
{
	return vec3(v.x + value, v.y + value, v.z + value);
}

vec3 operator -(const vec3& v, float value)
{
	return vec3(v.x - value, v.y - value, v.z - value);
}

vec3 operator *(const vec3& v, float value)
{
	return vec3(v.x * value, v.y * value, v.z * value);
}

vec3 operator /(const vec3& v, float value)
{
	if(is_zero(value))
	{
		throw ZeroDivisionError("Error when vec3 v / float value: value is 0 now.");
	}
	return vec3(v.x / value, v.y / value, v.z / value);
}

vec3 operator +(float value, const vec3& v)
{
	return vec3(value + v.x, value + v.y, value + v.z);
}

vec3 operator -(float value, const vec3& v)
{
	return vec3(value - v.x, value - v.y, value - v.z);
}

vec3 operator *(float value, const vec3& v)
{
	return vec3(value * v.x, value * v.y, value * v.z);
}

vec3 operator /(float value, const vec3& v)
{
	if(is_zero(v.x))
	{
		throw ZeroDivisionError("Error when float value / vec3 v: v.x is 0 now.");
	}
	if(is_zero(v.y))
	{
		throw ZeroDivisionError("Error when float value / vec3 v: v.y is 0 now.");
	}
	if(is_zero(v.z))
	{
		throw ZeroDivisionError("Error when float value / vec3 v: v.z is 0 now.");
	}
	return vec3(value / v.x, value / v.y, value / v.z);
}

vec3 operator +(const vec3& v, uint value)
{
	return vec3(v.x + value, v.y + value, v.z + value);
}

vec3 operator -(const vec3& v, uint value)
{
	return vec3(v.x - value, v.y - value, v.z - value);
}

vec3 operator *(const vec3& v, uint value)
{
	return vec3(v.x * value, v.y * value, v.z * value);
}

vec3 operator /(const vec3& v, uint value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error when vec3 v / uint value: value is 0 now.");
	}
	return vec3(v.x / value, v.y / value, v.z / value);
}

vec3 operator +(uint value, const vec3& v)
{
	return vec3(value + v.x, value + v.y, value + v.z);
}

vec3 operator -(uint value, const vec3& v)
{
	return vec3(value - v.x, value - v.y, value - v.z);
}

vec3 operator *(uint value, const vec3& v)
{
	return vec3(value * v.x, value * v.y, value * v.z);
}

vec3 operator /(uint value, const vec3& v)
{
	if(is_zero(v.x))
	{
		throw ZeroDivisionError("Error when uint value / vec3 v: v.x is 0 now.");
	}
	if(is_zero(v.y))
	{
		throw ZeroDivisionError("Error when uint value / vec3 v: v.y is 0 now.");
	}
	if(is_zero(v.z))
	{
		throw ZeroDivisionError("Error when uint value / vec3 v: v.z is 0 now.");
	}
	return vec3(value / v.x, value / v.y, value / v.z);
}

vec3 operator +(const vec3& v, int value)
{
	return vec3(v.x + value, v.y + value, v.z + value);
}

vec3 operator -(const vec3& v, int value)
{
	return vec3(v.x - value, v.y - value, v.z - value);
}

vec3 operator *(const vec3& v, int value)
{
	return vec3(v.x * value, v.y * value, v.z * value);
}

vec3 operator /(const vec3& v, int value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error when vec3 v / int value: value is 0 now.");
	}
	return vec3(v.x / value, v.y / value, v.z / value);
}

vec3 operator +(int value, const vec3& v)
{
	return vec3(value + v.x, value + v.y, value + v.z);
}

vec3 operator -(int value, const vec3& v)
{
	return vec3(value - v.x, value - v.y, value - v.z);
}

vec3 operator *(int value, const vec3& v)
{
	return vec3(value * v.x, value * v.y, value * v.z);
}

vec3 operator /(int value, const vec3& v)
{
	if(is_zero(v.x))
	{
		throw ZeroDivisionError("Error when int value / vec3 v: v.x is 0 now.");
	}
	if(is_zero(v.y))
	{
		throw ZeroDivisionError("Error when int value / vec3 v: v.y is 0 now.");
	}
	if(is_zero(v.z))
	{
		throw ZeroDivisionError("Error when int value / vec3 v: v.z is 0 now.");
	}
	return vec3(value / v.x, value / v.y, value / v.z);
}

ostream& operator <<(ostream& out, const vec3& v)
{
	out << "vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
	return out;
}

vec4::vec4()
#ifdef ENABLE_SWIZZLE
 : parent(this)
#endif
{}

vec4::vec4(float value) : x(value), y(value), z(value), w(value)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

vec4::vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

vec4::vec4(const vec2& v, float _z, float _w) : x(v.x), y(v.y), z(_z), w(_w)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

vec4::vec4(float _x, const vec2& v, float _w) : x(_x), y(v.x), z(v.y), w(_w)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

vec4::vec4(float _x, float _y, const vec2& v) : x(_x), y(_y), z(v.x), w(v.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

vec4::vec4(const vec2& v1, const vec2& v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

vec4::vec4(const vec3& v, float _w) : x(v.x), y(v.y), z(v.z), w(_w)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

vec4::vec4(float _x, const vec3& v) : x(_x), y(v.x), z(v.y), w(v.z)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

vec4::vec4(const vec4& v) : x(v.x), y(v.y), z(v.z), w(v.w)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

vec4::vec4(vec4&& v) : x(move(v.x)), y(move(v.y)), z(move(v.z)), w(move(v.w))
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}


float* vec4::data()const
{
	return (float*)this;
}

vec4& vec4::operator =(const vec4& v)
{
	if(this != &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}

	return *this;
}

vec4& vec4::operator =(vec4&& v)
{
	if(this != &v)
	{
		x = move(v.x);
		y = move(v.y);
		z = move(v.z);
		w = move(v.w);
	}

	return *this;
}

vec4::operator dvec4()const
{
	return dvec4(x, y, z, w);
}

vec4::operator uvec4()const
{
	return uvec4(x, y, z, w);
}

vec4::operator ivec4()const
{
	return ivec4(x, y, z, w);
}

vec4::operator bvec4()const
{
	return bvec4(x, y, z, w);
}

float& vec4::operator [](uint i)
{
	switch(i)
	{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		default:
		{
			throw IndexError(i, 0, 3);
		}
	}
}

float vec4::operator [](uint i)const
{
	switch(i)
	{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		default:
		{
			throw IndexError(i, 0, 3);
		}
	}
}

vec4 vec4::operator +()const
{
	return *this;
}

vec4 vec4::operator -()const
{
	return vec4(-x, -y, -z, -w);
}

vec4& vec4::operator +=(const vec4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;

	return *this;
}

vec4& vec4::operator -=(const vec4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;

	return *this;
}

vec4& vec4::operator *=(const vec4& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;

	return *this;
}

vec4& vec4::operator /=(const vec4& v)
{
	if(is_zero(v.x))
	{
		throw ZeroDivisionError("Error using vec4 v1 /= vec4 v2:\nv2.x is 0 now.");
	}
	if(is_zero(v.y))
	{
		throw ZeroDivisionError("Error using vec4 v1 /= vec4 v2:\nv2.y is 0 now.");
	}
	if(is_zero(v.z))
	{
		throw ZeroDivisionError("Error using vec4 v1 /= vec4 v2:\nv2.z is 0 now.");
	}
	if(is_zero(v.w))
	{
		throw ZeroDivisionError("Error using vec4 v1 /= vec4 v2:\nv2.w is 0 now.");
	}

	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;

	return *this;
}

vec4& vec4::operator +=(float value)
{
	x += value;
	y += value;
	z += value;
	w += value;

	return *this;
}

vec4& vec4::operator -=(float value)
{
	x -= value;
	y -= value;
	z -= value;
	w -= value;

	return *this;
}

vec4& vec4::operator *=(float value)
{
	x *= value;
	y *= value;
	z *= value;
	w *= value;

	return *this;
}

vec4& vec4::operator /=(float value)
{
	if(is_zero(value))
	{
		throw ZeroDivisionError("Error using vec4 v1 /= float value:\nvalue is 0 now.");
	}

	x /= value;
	y /= value;
	z /= value;
	w /= value;

	return *this;
}

vec4 operator +(const vec4& v1, const vec4& v2)
{
	return vec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

vec4 operator -(const vec4& v1, const vec4& v2)
{
	return vec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

vec4 operator *(const vec4& v1, const vec4& v2)
{
	return vec4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}

vec4 operator /(const vec4& v1, const vec4& v2)
{
	if(is_zero(v2.x))
	{
		throw ZeroDivisionError("Error when vec4 v1 / vec4 v2: v2.x is 0 now.");
	}
	if(is_zero(v2.y))
	{
		throw ZeroDivisionError("Error when vec4 v1 / vec4 v2: v2.y is 0 now.");
	}
	if(is_zero(v2.z))
	{
		throw ZeroDivisionError("Error when vec4 v1 / vec4 v2: v2.z is 0 now.");
	}
	if(is_zero(v2.w))
	{
		throw ZeroDivisionError("Error when vec4 v1 / vec4 v2: v2.w is 0 now.");
	}
	return vec4(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
}

vec4 operator +(const vec4& v1, const uvec4& v2)
{
	return vec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

vec4 operator -(const vec4& v1, const uvec4& v2)
{
	return vec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

vec4 operator *(const vec4& v1, const uvec4& v2)
{
	return vec4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}

vec4 operator /(const vec4& v1, const uvec4& v2)
{
	if(v2.x == 0)
	{
		throw ZeroDivisionError("Error when vec4 v1 / vec4 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw ZeroDivisionError("Error when vec4 v1 / vec4 v2: v2.y is 0 now.");
	}
	if(v2.z == 0)
	{
		throw ZeroDivisionError("Error when vec4 v1 / vec4 v2: v2.z is 0 now.");
	}
	if(v2.w == 0)
	{
		throw ZeroDivisionError("Error when vec4 v1 / vec4 v2: v2.w is 0 now.");
	}
	return vec4(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w + v2.w);
}

vec4 operator +(const uvec4& v1, const vec4& v2)
{
	return vec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

vec4 operator -(const uvec4& v1, const vec4& v2)
{
	return vec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

vec4 operator *(const uvec4& v1, const vec4& v2)
{
	return vec4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}

vec4 operator /(const uvec4& v1, const vec4& v2)
{
	if(is_zero(v2.x))
	{
		throw ZeroDivisionError("Error when vec4 v1 / vec4 v2: v2.x is 0 now.");
	}
	if(is_zero(v2.y))
	{
		throw ZeroDivisionError("Error when vec4 v1 / vec4 v2: v2.y is 0 now.");
	}
	if(is_zero(v2.z))
	{
		throw ZeroDivisionError("Error when vec4 v1 / vec4 v2: v2.z is 0 now.");
	}
	if(is_zero(v2.w))
	{
		throw ZeroDivisionError("Error when vec4 v1 / vec4 v2: v2.w is 0 now.");
	}
	return vec4(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
}

vec4 operator +(const vec4& v1, const ivec4& v2)
{
	return vec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

vec4 operator -(const vec4& v1, const ivec4& v2)
{
	return vec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

vec4 operator *(const vec4& v1, const ivec4& v2)
{
	return vec4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}

vec4 operator /(const vec4& v1, const ivec4& v2)
{
	if(v2.x == 0)
	{
		throw ZeroDivisionError("Error when vec4 v1 / vec4 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw ZeroDivisionError("Error when vec4 v1 / vec4 v2: v2.y is 0 now.");
	}
	if(v2.z == 0)
	{
		throw ZeroDivisionError("Error when vec4 v1 / vec4 v2: v2.z is 0 now.");
	}
	if(v2.w == 0)
	{
		throw ZeroDivisionError("Error when vec4 v1 / vec4 v2: v2.w is 0 now.");
	}
	return vec4(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
}

vec4 operator +(const ivec4& v1, const vec4& v2)
{
	return vec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

vec4 operator -(const ivec4& v1, const vec4& v2)
{
	return vec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

vec4 operator *(const ivec4& v1, const vec4& v2)
{
	return vec4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}

vec4 operator /(const ivec4& v1, const vec4& v2)
{
	if(is_zero(v2.x))
	{
		throw ZeroDivisionError("Error when vec4 v1 / vec4 v2: v2.x is 0 now.");
	}
	if(is_zero(v2.y))
	{
		throw ZeroDivisionError("Error when vec4 v1 / vec4 v2: v2.y is 0 now.");
	}
	if(is_zero(v2.z))
	{
		throw ZeroDivisionError("Error when vec4 v1 / vec4 v2: v2.z is 0 now.");
	}
	if(is_zero(v2.w))
	{
		throw ZeroDivisionError("Error when vec4 v1 / vec4 v2: v2.w is 0 now.");
	}
	return vec4(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.z);
}

dvec4 operator +(const vec4& v, double value)
{
	return dvec4(v.x + value, v.y + value, v.z + value, v.w + value);
}

dvec4 operator -(const vec4& v, double value)
{
	return dvec4(v.x - value, v.y - value, v.z - value, v.w - value);
}

dvec4 operator *(const vec4& v, double value)
{
	return dvec4(v.x * value, v.y * value, v.z * value, v.w * value);
}

dvec4 operator /(const vec4& v, double value)
{
	if(is_zero(value))
	{
		throw ZeroDivisionError("Error when vec4 v / double value: value is 0 now.");
	}
	return dvec4(v.x / value, v.y / value, v.z / value, v.w / value);
}

dvec4 operator +(double value, const vec4& v)
{
	return dvec4(value + v.x, value + v.y, value + v.z, value + v.w);
}

dvec4 operator -(double value, const vec4& v)
{
	return dvec4(value - v.x, value - v.y, value - v.z, value - v.w);
}

dvec4 operator *(double value, const vec4& v)
{
	return dvec4(value * v.x, value * v.y, value * v.z, value * v.w);
}

dvec4 operator /(double value, const vec4& v)
{
	if(is_zero(v.x))
	{
		throw ZeroDivisionError("Error when double value / vec4 v: v.x is 0 now.");
	}
	if(is_zero(v.y))
	{
		throw ZeroDivisionError("Error when double value / vec4 v: v.y is 0 now.");
	}
	if(is_zero(v.z))
	{
		throw ZeroDivisionError("Error when double value / vec4 v: v.z is 0 now.");
	}
	if(is_zero(v.w))
	{
		throw ZeroDivisionError("Error when double value / vec4 v: v.w is 0 now.");
	}
	return dvec4(value / v.x, value / v.y, value / v.z, value / v.w);
}

vec4 operator +(const vec4& v, float value)
{
	return vec4(v.x + value, v.y + value, v.z + value, v.w + value);
}

vec4 operator -(const vec4& v, float value)
{
	return vec4(v.x - value, v.y - value, v.z - value, v.w - value);
}

vec4 operator *(const vec4& v, float value)
{
	return vec4(v.x * value, v.y * value, v.z * value, v.w * value);
}

vec4 operator /(const vec4& v, float value)
{
	if(is_zero(value))
	{
		throw ZeroDivisionError("Error when vec4 v / float value: value is 0 now.");
	}
	return vec4(v.x / value, v.y / value, v.z / value, v.w / value);
}

vec4 operator +(float value, const vec4& v)
{
	return vec4(value + v.x, value + v.y, value + v.z, value + v.w);
}

vec4 operator -(float value, const vec4& v)
{
	return vec4(value - v.x, value - v.y, value - v.z, value - v.w);
}

vec4 operator *(float value, const vec4& v)
{
	return vec4(value * v.x, value * v.y, value * v.z, value * v.w);
}

vec4 operator /(float value, const vec4& v)
{
	if(is_zero(v.x))
	{
		throw ZeroDivisionError("Error when float value / vec4 v: v.x is 0 now.");
	}
	if(is_zero(v.y))
	{
		throw ZeroDivisionError("Error when float value / vec4 v: v.y is 0 now.");
	}
	if(is_zero(v.z))
	{
		throw ZeroDivisionError("Error when float value / vec4 v: v.z is 0 now.");
	}
	if(is_zero(v.w))
	{
		throw ZeroDivisionError("Error when float value / vec4 v: v.w is 0 now.");
	}
	return vec4(value / v.x, value / v.y, value / v.z, value / v.w);
}

vec4 operator +(const vec4& v, uint value)
{
	return vec4(v.x + value, v.y + value, v.z + value, v.w + value);
}

vec4 operator -(const vec4& v, uint value)
{
	return vec4(v.x - value, v.y - value, v.z - value, v.w - value);
}

vec4 operator *(const vec4& v, uint value)
{
	return vec4(v.x * value, v.y * value, v.z * value, v.w * value);
}

vec4 operator /(const vec4& v, uint value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error when vec4 v / uint value: value is 0 now.");
	}
	return vec4(v.x / value, v.y / value, v.z / value, v.w / value);
}

vec4 operator +(uint value, const vec4& v)
{
	return vec4(value + v.x, value + v.y, value + v.z, value + v.w);
}

vec4 operator -(uint value, const vec4& v)
{
	return vec4(value - v.x, value - v.y, value - v.z, value - v.w);
}

vec4 operator *(uint value, const vec4& v)
{
	return vec4(value * v.x, value * v.y, value * v.z, value * v.w);
}

vec4 operator /(uint value, const vec4& v)
{
	if(is_zero(v.x))
	{
		throw ZeroDivisionError("Error when uint value / vec4 v: v.x is 0 now.");
	}
	if(is_zero(v.y))
	{
		throw ZeroDivisionError("Error when uint value / vec4 v: v.y is 0 now.");
	}
	if(is_zero(v.z))
	{
		throw ZeroDivisionError("Error when uint value / vec4 v: v.z is 0 now.");
	}
	if(is_zero(v.w))
	{
		throw ZeroDivisionError("Error when uint value / vec4 v: v.w is 0 now.");
	}
	return vec4(value / v.x, value / v.y, value / v.z, value / v.w);
}

vec4 operator +(const vec4& v, int value)
{
	return vec4(v.x + value, v.y + value, v.z + value, v.w + value);
}

vec4 operator -(const vec4& v, int value)
{
	return vec4(v.x - value, v.y - value, v.z - value, v.w - value);
}

vec4 operator *(const vec4& v, int value)
{
	return vec4(v.x * value, v.y * value, v.z * value, v.w * value);
}

vec4 operator /(const vec4& v, int value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error when vec4 v / int value: value is 0 now.");
	}
	return vec4(v.x / value, v.y / value, v.z / value, v.w / value);
}

vec4 operator +(int value, const vec4& v)
{
	return vec4(value + v.x, value + v.y, value + v.z, value + v.w);
}

vec4 operator -(int value, const vec4& v)
{
	return vec4(value - v.x, value - v.y, value - v.z, value - v.w);
}

vec4 operator *(int value, const vec4& v)
{
	return vec4(value * v.x, value * v.y, value * v.z, value * v.w);
}

vec4 operator /(int value, const vec4& v)
{
	if(is_zero(v.x))
	{
		throw ZeroDivisionError("Error when int value / vec4 v: v.x is 0 now.");
	}
	if(is_zero(v.y))
	{
		throw ZeroDivisionError("Error when int value / vec4 v: v.y is 0 now.");
	}
	if(is_zero(v.z))
	{
		throw ZeroDivisionError("Error when int value / vec4 v: v.z is 0 now.");
	}
	if(is_zero(v.w))
	{
		throw ZeroDivisionError("Error when int value / vec4 v: v.w is 0 now.");
	}
	return vec4(value / v.x, value / v.y, value / v.z, value / v.w);
}

ostream& operator <<(ostream& out, const vec4& v)
{
	out << "vec4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
	return out;
}