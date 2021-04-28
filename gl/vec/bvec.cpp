#include "dvec.h"
#include "fvec.h"
#include "uvec.h"
#include "ivec.h"
#include "bvec.h"
#include "vec_process.h"
#include "exception.h"

#ifdef ENABLE_SWIZZLE
#define IMPLEMENT_BOOL_REFERENCE(vec_type, e1) \
vec_type::e1##_t::operator bool()const\
{\
	return parent->e1;\
}\
bool& vec_type::e1##_t::operator =(bool value)\
{\
	parent->e1 = value;\
	return parent->e1;\
}\

#define IMPLEMENT_BVEC2_REFERENCE(vec_type, e1, e2) \
vec_type::e1##e2##_t::operator bvec2()\
{\
	return bvec2(parent->e1, parent->e2);\
}\
bool vec_type::e1##e2##_t::operator [](uint i)const\
{\
	switch(i)\
	{\
		case 0: return parent->e1; break;\
		case 1: return parent->e2; break;\
		default: throw IndexError(i, 0, 1);\
	}\
}\
bool& vec_type::e1##e2##_t::operator [](uint i)\
{\
	switch(i)\
	{\
		case 0: return parent->e1; break;\
		case 1: return parent->e2; break;\
		default: throw IndexError(i, 0, 1);\
	}\
}\
vec_type& vec_type::e1##e2##_t::operator =(const bvec2& v)\
{\
	if((void*)(&v) != (void*)(parent))\
	{\
		parent->e1 = v.x;\
		parent->e2 = v.y;\
	}\
	else\
	{\
		bool x = v.x, y = v.y;\
		parent->e1 = x;\
		parent->e2 = y;\
	}\
	return *parent;\
}\
vec_type& vec_type::e1##e2##_t::operator =(bvec2&& v)\
{\
	if((void*)(&v) != (void*)(parent))\
	{\
		parent->e1 = move(v.x);\
		parent->e2 = move(v.y);\
	}\
	else\
	{\
		bool x = v.x, y = v.y;\
		parent->e1 = x;\
		parent->e2 = y;\
	}\
	return *parent;\
}

#define IMPLEMENT_BVEC3_REFERENCE(vec_type, e1, e2, e3) \
vec_type::e1##e2##e3##_t::operator bvec3()\
{\
	return bvec3(parent->e1, parent->e2, parent->e3);\
}\
bool vec_type::e1##e2##e3##_t::operator [](uint i)const\
{\
	switch(i)\
	{\
		case 0: return parent->e1; break;\
		case 1: return parent->e2; break;\
		case 2: return parent->e3; break;\
		default: throw IndexError(i, 0, 2);\
	}\
}\
bool& vec_type::e1##e2##e3##_t::operator [](uint i)\
{\
	switch(i)\
	{\
		case 0: return parent->e1; break;\
		case 1: return parent->e2; break;\
		case 2: return parent->e3; break;\
		default: throw IndexError(i, 0, 2);\
	}\
}\
vec_type& vec_type::e1##e2##e3##_t::operator =(const bvec3& v)\
{\
	if((void*)(&v) != (void*)(parent))\
	{\
		parent->e1 = v.x;\
		parent->e2 = v.y;\
		parent->e3 = v.z;\
	}\
	else\
	{\
		bool x = v.x, y = v.y, z = v.z;\
		parent->e1 = x;\
		parent->e2 = y;\
		parent->e3 = z;\
	}\
	return *parent;\
}\
vec_type& vec_type::e1##e2##e3##_t::operator =(bvec3&& v)\
{\
	if((void*)(&v) != (void*)(parent))\
	{\
		parent->e1 = move(v.x);\
		parent->e2 = move(v.y);\
		parent->e3 = move(v.z);\
	}\
	else\
	{\
		bool x = v.x, y = v.y, z = v.z;\
		parent->e1 = x;\
		parent->e2 = y;\
		parent->e3 = z;\
	}\
	return *parent;\
}

#define IMPLEMENT_BVEC4_REFERENCE(vec_type, e1, e2, e3, e4) \
vec_type::e1##e2##e3##e4##_t::operator bvec4()\
{\
	return bvec4(parent->e1, parent->e2, parent->e3, parent->e4);\
}\
bool vec_type::e1##e2##e3##e4##_t::operator [](uint i)const\
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
bool& vec_type::e1##e2##e3##e4##_t::operator [](uint i)\
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
vec_type& vec_type::e1##e2##e3##e4##_t::operator =(const bvec4& v)\
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
		bool x = v.x, y = v.y, z = v.z, w = v.w;\
		parent->e1 = x;\
		parent->e2 = y;\
		parent->e3 = z;\
		parent->e4 = w;\
	}\
	return *parent;\
}\
vec_type& vec_type::e1##e2##e3##e4##_t::operator =(bvec4&& v)\
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
		bool x = v.x, y = v.y, z = v.z, w = v.w;\
		parent->e1 = x;\
		parent->e2 = y;\
		parent->e3 = z;\
		parent->e4 = w;\
	}\
	return *parent;\
}

IMPLEMENT_BOOL_REFERENCE(bvec2, x)
IMPLEMENT_BOOL_REFERENCE(bvec2, y)
IMPLEMENT_BVEC2_REFERENCE(bvec2, x, x)
IMPLEMENT_BVEC2_REFERENCE(bvec2, x, y)
IMPLEMENT_BVEC2_REFERENCE(bvec2, y, x)
IMPLEMENT_BVEC2_REFERENCE(bvec2, y, y)
IMPLEMENT_BVEC3_REFERENCE(bvec2, x, x, x)
IMPLEMENT_BVEC3_REFERENCE(bvec2, x, x, y)
IMPLEMENT_BVEC3_REFERENCE(bvec2, x, y, x)
IMPLEMENT_BVEC3_REFERENCE(bvec2, x, y, y)
IMPLEMENT_BVEC3_REFERENCE(bvec2, y, x, x)
IMPLEMENT_BVEC3_REFERENCE(bvec2, y, x, y)
IMPLEMENT_BVEC3_REFERENCE(bvec2, y, y, x)
IMPLEMENT_BVEC3_REFERENCE(bvec2, y, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec2, x, x, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec2, x, x, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec2, x, x, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec2, x, x, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec2, x, y, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec2, x, y, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec2, x, y, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec2, x, y, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec2, y, x, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec2, y, x, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec2, y, x, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec2, y, x, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec2, y, y, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec2, y, y, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec2, y, y, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec2, y, y, y, y)

IMPLEMENT_BOOL_REFERENCE(bvec3, x)
IMPLEMENT_BOOL_REFERENCE(bvec3, y)
IMPLEMENT_BOOL_REFERENCE(bvec3, z)
IMPLEMENT_BVEC2_REFERENCE(bvec3, x, x)
IMPLEMENT_BVEC2_REFERENCE(bvec3, x, y)
IMPLEMENT_BVEC2_REFERENCE(bvec3, x, z)
IMPLEMENT_BVEC2_REFERENCE(bvec3, y, x)
IMPLEMENT_BVEC2_REFERENCE(bvec3, y, y)
IMPLEMENT_BVEC2_REFERENCE(bvec3, y, z)
IMPLEMENT_BVEC2_REFERENCE(bvec3, z, x)
IMPLEMENT_BVEC2_REFERENCE(bvec3, z, y)
IMPLEMENT_BVEC2_REFERENCE(bvec3, z, z)
IMPLEMENT_BVEC3_REFERENCE(bvec3, x, x, x)
IMPLEMENT_BVEC3_REFERENCE(bvec3, x, x, y)
IMPLEMENT_BVEC3_REFERENCE(bvec3, x, x, z)
IMPLEMENT_BVEC3_REFERENCE(bvec3, x, y, x)
IMPLEMENT_BVEC3_REFERENCE(bvec3, x, y, y)
IMPLEMENT_BVEC3_REFERENCE(bvec3, x, y, z)
IMPLEMENT_BVEC3_REFERENCE(bvec3, x, z, x)
IMPLEMENT_BVEC3_REFERENCE(bvec3, x, z, y)
IMPLEMENT_BVEC3_REFERENCE(bvec3, x, z, z)
IMPLEMENT_BVEC3_REFERENCE(bvec3, y, x, x)
IMPLEMENT_BVEC3_REFERENCE(bvec3, y, x, y)
IMPLEMENT_BVEC3_REFERENCE(bvec3, y, x, z)
IMPLEMENT_BVEC3_REFERENCE(bvec3, y, y, x)
IMPLEMENT_BVEC3_REFERENCE(bvec3, y, y, y)
IMPLEMENT_BVEC3_REFERENCE(bvec3, y, y, z)
IMPLEMENT_BVEC3_REFERENCE(bvec3, y, z, x)
IMPLEMENT_BVEC3_REFERENCE(bvec3, y, z, y)
IMPLEMENT_BVEC3_REFERENCE(bvec3, y, z, z)
IMPLEMENT_BVEC3_REFERENCE(bvec3, z, x, x)
IMPLEMENT_BVEC3_REFERENCE(bvec3, z, x, y)
IMPLEMENT_BVEC3_REFERENCE(bvec3, z, x, z)
IMPLEMENT_BVEC3_REFERENCE(bvec3, z, y, x)
IMPLEMENT_BVEC3_REFERENCE(bvec3, z, y, y)
IMPLEMENT_BVEC3_REFERENCE(bvec3, z, y, z)
IMPLEMENT_BVEC3_REFERENCE(bvec3, z, z, x)
IMPLEMENT_BVEC3_REFERENCE(bvec3, z, z, y)
IMPLEMENT_BVEC3_REFERENCE(bvec3, z, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, x, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, x, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, x, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, x, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, x, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, x, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, x, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, x, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, x, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, y, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, y, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, y, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, y, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, y, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, y, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, y, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, y, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, y, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, z, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, z, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, z, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, z, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, z, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, z, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, z, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, z, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, x, z, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, x, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, x, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, x, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, x, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, x, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, x, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, x, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, x, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, x, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, y, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, y, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, y, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, y, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, y, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, y, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, y, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, y, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, y, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, z, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, z, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, z, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, z, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, z, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, z, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, z, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, z, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, y, z, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, x, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, x, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, x, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, x, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, x, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, x, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, x, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, x, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, x, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, y, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, y, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, y, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, y, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, y, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, y, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, y, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, y, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, y, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, z, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, z, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, z, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, z, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, z, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, z, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, z, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, z, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec3, z, z, z, z)

IMPLEMENT_BOOL_REFERENCE(bvec4, x)
IMPLEMENT_BOOL_REFERENCE(bvec4, y)
IMPLEMENT_BOOL_REFERENCE(bvec4, z)
IMPLEMENT_BOOL_REFERENCE(bvec4, w)
IMPLEMENT_BVEC2_REFERENCE(bvec4, x, x)
IMPLEMENT_BVEC2_REFERENCE(bvec4, x, y)
IMPLEMENT_BVEC2_REFERENCE(bvec4, x, z)
IMPLEMENT_BVEC2_REFERENCE(bvec4, x, w)
IMPLEMENT_BVEC2_REFERENCE(bvec4, y, x)
IMPLEMENT_BVEC2_REFERENCE(bvec4, y, y)
IMPLEMENT_BVEC2_REFERENCE(bvec4, y, z)
IMPLEMENT_BVEC2_REFERENCE(bvec4, y, w)
IMPLEMENT_BVEC2_REFERENCE(bvec4, z, x)
IMPLEMENT_BVEC2_REFERENCE(bvec4, z, y)
IMPLEMENT_BVEC2_REFERENCE(bvec4, z, z)
IMPLEMENT_BVEC2_REFERENCE(bvec4, z, w)
IMPLEMENT_BVEC2_REFERENCE(bvec4, w, x)
IMPLEMENT_BVEC2_REFERENCE(bvec4, w, y)
IMPLEMENT_BVEC2_REFERENCE(bvec4, w, z)
IMPLEMENT_BVEC2_REFERENCE(bvec4, w, w)
IMPLEMENT_BVEC3_REFERENCE(bvec4, x, x, x)
IMPLEMENT_BVEC3_REFERENCE(bvec4, x, x, y)
IMPLEMENT_BVEC3_REFERENCE(bvec4, x, x, z)
IMPLEMENT_BVEC3_REFERENCE(bvec4, x, x, w)
IMPLEMENT_BVEC3_REFERENCE(bvec4, x, y, x)
IMPLEMENT_BVEC3_REFERENCE(bvec4, x, y, y)
IMPLEMENT_BVEC3_REFERENCE(bvec4, x, y, z)
IMPLEMENT_BVEC3_REFERENCE(bvec4, x, y, w)
IMPLEMENT_BVEC3_REFERENCE(bvec4, x, z, x)
IMPLEMENT_BVEC3_REFERENCE(bvec4, x, z, y)
IMPLEMENT_BVEC3_REFERENCE(bvec4, x, z, z)
IMPLEMENT_BVEC3_REFERENCE(bvec4, x, z, w)
IMPLEMENT_BVEC3_REFERENCE(bvec4, x, w, x)
IMPLEMENT_BVEC3_REFERENCE(bvec4, x, w, y)
IMPLEMENT_BVEC3_REFERENCE(bvec4, x, w, z)
IMPLEMENT_BVEC3_REFERENCE(bvec4, x, w, w)
IMPLEMENT_BVEC3_REFERENCE(bvec4, y, x, x)
IMPLEMENT_BVEC3_REFERENCE(bvec4, y, x, y)
IMPLEMENT_BVEC3_REFERENCE(bvec4, y, x, z)
IMPLEMENT_BVEC3_REFERENCE(bvec4, y, x, w)
IMPLEMENT_BVEC3_REFERENCE(bvec4, y, y, x)
IMPLEMENT_BVEC3_REFERENCE(bvec4, y, y, y)
IMPLEMENT_BVEC3_REFERENCE(bvec4, y, y, z)
IMPLEMENT_BVEC3_REFERENCE(bvec4, y, y, w)
IMPLEMENT_BVEC3_REFERENCE(bvec4, y, z, x)
IMPLEMENT_BVEC3_REFERENCE(bvec4, y, z, y)
IMPLEMENT_BVEC3_REFERENCE(bvec4, y, z, z)
IMPLEMENT_BVEC3_REFERENCE(bvec4, y, z, w)
IMPLEMENT_BVEC3_REFERENCE(bvec4, y, w, x)
IMPLEMENT_BVEC3_REFERENCE(bvec4, y, w, y)
IMPLEMENT_BVEC3_REFERENCE(bvec4, y, w, z)
IMPLEMENT_BVEC3_REFERENCE(bvec4, y, w, w)
IMPLEMENT_BVEC3_REFERENCE(bvec4, z, x, x)
IMPLEMENT_BVEC3_REFERENCE(bvec4, z, x, y)
IMPLEMENT_BVEC3_REFERENCE(bvec4, z, x, z)
IMPLEMENT_BVEC3_REFERENCE(bvec4, z, x, w)
IMPLEMENT_BVEC3_REFERENCE(bvec4, z, y, x)
IMPLEMENT_BVEC3_REFERENCE(bvec4, z, y, y)
IMPLEMENT_BVEC3_REFERENCE(bvec4, z, y, z)
IMPLEMENT_BVEC3_REFERENCE(bvec4, z, y, w)
IMPLEMENT_BVEC3_REFERENCE(bvec4, z, z, x)
IMPLEMENT_BVEC3_REFERENCE(bvec4, z, z, y)
IMPLEMENT_BVEC3_REFERENCE(bvec4, z, z, z)
IMPLEMENT_BVEC3_REFERENCE(bvec4, z, z, w)
IMPLEMENT_BVEC3_REFERENCE(bvec4, z, w, x)
IMPLEMENT_BVEC3_REFERENCE(bvec4, z, w, y)
IMPLEMENT_BVEC3_REFERENCE(bvec4, z, w, z)
IMPLEMENT_BVEC3_REFERENCE(bvec4, z, w, w)
IMPLEMENT_BVEC3_REFERENCE(bvec4, w, x, x)
IMPLEMENT_BVEC3_REFERENCE(bvec4, w, x, y)
IMPLEMENT_BVEC3_REFERENCE(bvec4, w, x, z)
IMPLEMENT_BVEC3_REFERENCE(bvec4, w, x, w)
IMPLEMENT_BVEC3_REFERENCE(bvec4, w, y, x)
IMPLEMENT_BVEC3_REFERENCE(bvec4, w, y, y)
IMPLEMENT_BVEC3_REFERENCE(bvec4, w, y, z)
IMPLEMENT_BVEC3_REFERENCE(bvec4, w, y, w)
IMPLEMENT_BVEC3_REFERENCE(bvec4, w, z, x)
IMPLEMENT_BVEC3_REFERENCE(bvec4, w, z, y)
IMPLEMENT_BVEC3_REFERENCE(bvec4, w, z, z)
IMPLEMENT_BVEC3_REFERENCE(bvec4, w, z, w)
IMPLEMENT_BVEC3_REFERENCE(bvec4, w, w, x)
IMPLEMENT_BVEC3_REFERENCE(bvec4, w, w, y)
IMPLEMENT_BVEC3_REFERENCE(bvec4, w, w, z)
IMPLEMENT_BVEC3_REFERENCE(bvec4, w, w, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, x, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, x, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, x, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, x, x, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, x, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, x, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, x, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, x, y, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, x, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, x, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, x, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, x, z, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, x, w, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, x, w, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, x, w, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, x, w, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, y, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, y, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, y, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, y, x, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, y, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, y, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, y, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, y, y, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, y, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, y, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, y, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, y, z, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, y, w, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, y, w, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, y, w, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, y, w, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, z, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, z, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, z, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, z, x, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, z, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, z, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, z, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, z, y, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, z, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, z, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, z, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, z, z, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, z, w, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, z, w, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, z, w, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, z, w, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, w, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, w, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, w, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, w, x, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, w, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, w, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, w, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, w, y, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, w, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, w, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, w, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, w, z, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, w, w, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, w, w, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, w, w, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, x, w, w, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, x, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, x, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, x, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, x, x, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, x, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, x, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, x, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, x, y, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, x, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, x, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, x, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, x, z, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, x, w, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, x, w, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, x, w, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, x, w, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, y, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, y, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, y, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, y, x, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, y, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, y, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, y, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, y, y, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, y, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, y, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, y, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, y, z, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, y, w, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, y, w, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, y, w, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, y, w, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, z, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, z, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, z, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, z, x, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, z, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, z, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, z, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, z, y, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, z, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, z, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, z, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, z, z, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, z, w, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, z, w, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, z, w, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, z, w, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, w, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, w, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, w, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, w, x, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, w, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, w, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, w, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, w, y, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, w, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, w, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, w, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, w, z, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, w, w, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, w, w, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, w, w, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, y, w, w, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, x, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, x, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, x, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, x, x, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, x, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, x, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, x, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, x, y, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, x, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, x, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, x, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, x, z, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, x, w, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, x, w, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, x, w, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, x, w, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, y, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, y, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, y, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, y, x, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, y, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, y, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, y, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, y, y, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, y, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, y, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, y, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, y, z, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, y, w, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, y, w, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, y, w, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, y, w, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, z, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, z, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, z, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, z, x, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, z, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, z, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, z, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, z, y, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, z, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, z, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, z, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, z, z, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, z, w, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, z, w, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, z, w, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, z, w, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, w, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, w, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, w, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, w, x, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, w, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, w, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, w, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, w, y, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, w, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, w, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, w, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, w, z, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, w, w, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, w, w, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, w, w, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, z, w, w, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, x, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, x, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, x, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, x, x, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, x, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, x, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, x, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, x, y, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, x, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, x, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, x, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, x, z, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, x, w, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, x, w, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, x, w, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, x, w, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, y, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, y, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, y, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, y, x, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, y, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, y, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, y, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, y, y, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, y, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, y, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, y, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, y, z, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, y, w, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, y, w, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, y, w, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, y, w, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, z, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, z, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, z, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, z, x, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, z, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, z, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, z, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, z, y, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, z, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, z, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, z, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, z, z, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, z, w, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, z, w, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, z, w, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, z, w, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, w, x, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, w, x, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, w, x, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, w, x, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, w, y, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, w, y, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, w, y, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, w, y, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, w, z, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, w, z, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, w, z, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, w, z, w)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, w, w, x)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, w, w, y)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, w, w, z)
IMPLEMENT_BVEC4_REFERENCE(bvec4, w, w, w, w)
#endif

bvec2::bvec2()
#ifdef ENABLE_SWIZZLE
 : parent(this)
#endif
{}

bvec2::bvec2(bool value) : x(value), y(value)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

bvec2::bvec2(bool _x, bool _y) : x(_x), y(_y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

bvec2::bvec2(const bvec3& v) : x(v.x), y(v.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

bvec2::bvec2(const bvec4& v) : x(v.x), y(v.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

bvec2::bvec2(const bvec2& v) : x(v.x), y(v.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

bvec2::bvec2(bvec2&& v) : x(move(v.x)), y(move(v.y))
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}


bool* bvec2::data()const
{
	return (bool*)this;
}

bvec2& bvec2::operator =(const bvec2& v)
{
	if(this != &v)
	{
		x = v.x;
		y = v.y;
	}

	return *this;
}

bvec2& bvec2::operator =(bvec2&& v)
{
	if(this != &v)
	{
		x = move(v.x);
		y = move(v.y);
	}

	return *this;
}

bvec2::operator dvec2()const
{
	return dvec2(x, y);
}

bvec2::operator vec2()const
{
	return vec2(x, y);
}

bvec2::operator uvec2()const
{
	return uvec2(x, y);
}

bvec2::operator ivec2()const
{
	return ivec2(x, y);
}

bool& bvec2::operator [](uint i)
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

bool bvec2::operator [](uint i)const
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

bvec2 bvec2::operator !()const
{
	return bvec2(!x, !y);
}

bvec2& bvec2::operator &=(const bvec2& v)
{
	x = (x && v.x);
	y = (y && v.y);

	return *this;
}

bvec2& bvec2::operator |=(const bvec2& v)
{
	x = (x || v.x);
	y = (y || v.y);

	return *this;
}

bvec2 operator &(const bvec2& v1, const bvec2& v2)
{
	return bvec2(v1.x && v2.x, v1.y && v2.y);
}

bvec2 operator |(const bvec2& v1, const bvec2& v2)
{
	return bvec2(v1.x || v2.x, v1.y && v2.y);
}

ostream& operator <<(ostream& out, const bvec2& v)
{
	out << "bvec2(" << v.x << ", " << v.y << ")";
	return out;
}

bvec3::bvec3()
#ifdef ENABLE_SWIZZLE
 : parent(this)
#endif
{}

bvec3::bvec3(bool value) : x(value), y(value), z(value)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

bvec3::bvec3(bool _x, bool _y, bool _z) : x(_x), y(_y), z(_z)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

bvec3::bvec3(const bvec2& v, bool _z) : x(v.x), y(v.y), z(_z)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

bvec3::bvec3(bool _x, const bvec2& v) : x(_x), y(v.x), z(v.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

bvec3::bvec3(const bvec4& v) : x(v.x), y(v.y), z(v.z)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

bvec3::bvec3(const bvec3& v) : x(v.x), y(v.y), z(v.z)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

bvec3::bvec3(bvec3&& v) : x(move(v.x)), y(move(v.y)), z(move(v.z))
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}


bool* bvec3::data()const
{
	return (bool*)this;
}

bvec3& bvec3::operator =(const bvec3& v)
{
	if(this != &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	return *this;
}

bvec3& bvec3::operator =(bvec3&& v)
{
	if(this != &v)
	{
		x = move(v.x);
		y = move(v.y);
		z = move(v.z);
	}

	return *this;
}

bvec3::operator dvec3()const
{
	return dvec3(x, y, z);
}

bvec3::operator vec3()const
{
	return vec3(x, y, z);
}

bvec3::operator uvec3()const
{
	return uvec3(x, y, z);
}

bvec3::operator ivec3()const
{
	return ivec3(x, y, z);
}

bool& bvec3::operator [](uint i)
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

bool bvec3::operator [](uint i)const
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

bvec3 bvec3::operator !()const
{
	return bvec3(!x, !y, !z);
}

bvec3& bvec3::operator &=(const bvec3& v)
{
	x = (x && v.x);
	y = (y && v.y);
	z = (z && v.z);

	return *this;
}

bvec3& bvec3::operator |=(const bvec3& v)
{
	x = (x || v.x);
	y = (y || v.y);
	z = (z || v.z);

	return *this;
}

bvec3 operator &(const bvec3& v1, const bvec3& v2)
{
	return bvec3(v1.x && v2.x, v1.y && v2.y, v1.z && v2.z);
}

bvec3 operator |(const bvec3& v1, const bvec3& v2)
{
	return bvec3(v1.x || v2.x, v1.y || v2.y, v1.z || v2.z);
}

ostream& operator <<(ostream& out, const bvec3& v)
{
	out << "bvec3(" << v.x << ", " << v.y << ", " << v.z << ")";
	return out;
}

bvec4::bvec4()
#ifdef ENABLE_SWIZZLE
 : parent(this)
#endif
{}

bvec4::bvec4(bool value) : x(value), y(value), z(value), w(value)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

bvec4::bvec4(bool _x, bool _y, bool _z, bool _w) : x(_x), y(_y), z(_z), w(_w)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

bvec4::bvec4(const bvec2& v, bool _z, bool _w) : x(v.x), y(v.y), z(_z), w(_w)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

bvec4::bvec4(bool _x, const bvec2& v, bool _w) : x(_x), y(v.x), z(v.y), w(_w)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

bvec4::bvec4(bool _x, bool _y, const bvec2& v) : x(_x), y(_y), z(v.x), w(v.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

bvec4::bvec4(const bvec2& v1, const bvec2& v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

bvec4::bvec4(const bvec3& v, bool _w) : x(v.x), y(v.y), z(v.z), w(_w)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

bvec4::bvec4(bool _x, const bvec3& v) : x(_x), y(v.x), z(v.y), w(v.z)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

bvec4::bvec4(const bvec4& v) : x(v.x), y(v.y), z(v.z), w(v.w)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

bvec4::bvec4(bvec4&& v) : x(move(v.x)), y(move(v.y)), z(move(v.z)), w(move(v.w))
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}


bool* bvec4::data()const
{
	return (bool*)this;
}

bvec4& bvec4::operator =(const bvec4& v)
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

bvec4& bvec4::operator =(bvec4&& v)
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

bvec4::operator dvec4()const
{
	return dvec4(x, y, z, w);
}

bvec4::operator vec4()const
{
	return vec4(x, y, z, w);
}

bvec4::operator uvec4()const
{
	return uvec4(x, y, z, w);
}

bvec4::operator ivec4()const
{
	return ivec4(x, y, z, w);
}

bool& bvec4::operator [](uint i)
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

bool bvec4::operator [](uint i)const
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

bvec4 bvec4::operator !()const
{
	return bvec4(!x, !y, !z, !w);
}

bvec4& bvec4::operator &=(const bvec4& v)
{
	x = (x && v.x);
	y = (y && v.y);
	z = (z && v.z);
	w = (w && v.w);

	return *this;
}

bvec4& bvec4::operator |=(const bvec4& v)
{
	x = (x || v.x);
	y = (y || v.y);
	z = (z || v.z);
	w = (w || v.w);

	return *this;
}

bvec4 operator &(const bvec4& v1, const bvec4& v2)
{
	return bvec4(v1.x && v2.x, v1.y && v2.y, v1.z && v2.z, v1.w && v2.w);
}

bvec4 operator |(const bvec4& v1, const bvec4& v2)
{
	return bvec4(v1.x || v2.x, v1.y || v2.y, v1.z || v2.z, v1.w || v2.w);
}

ostream& operator <<(ostream& out, const bvec4& v)
{
	out << "bvec4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
	return out;
}