#include "dvec.h"
#include "fvec.h"
#include "uvec.h"
#include "ivec.h"
#include "bvec.h"
#include "vec_process.h"
#include "exception.h"

#ifdef ENABLE_SWIZZLE
#define IMPLEMENT_INT_REFERENCE(vec_type, e1) \
vec_type::e1##_t::operator int()const\
{\
	return parent->e1;\
}\
int& vec_type::e1##_t::operator =(int value)\
{\
	parent->e1 = value;\
	return parent->e1;\
}\

#define IMPLEMENT_IVEC2_REFERENCE(vec_type, e1, e2) \
vec_type::e1##e2##_t::operator ivec2()\
{\
	return ivec2(parent->e1, parent->e2);\
}\
int vec_type::e1##e2##_t::operator [](uint i)const\
{\
	switch(i)\
	{\
		case 0: return parent->e1; break;\
		case 1: return parent->e2; break;\
		default: throw IndexError(i, 0, 1);\
	}\
}\
int& vec_type::e1##e2##_t::operator [](uint i)\
{\
	switch(i)\
	{\
		case 0: return parent->e1; break;\
		case 1: return parent->e2; break;\
		default: throw IndexError(i, 0, 1);\
	}\
}\
vec_type& vec_type::e1##e2##_t::operator =(const ivec2& v)\
{\
	if((void*)(&v) != (void*)(parent))\
	{\
		parent->e1 = v.x;\
		parent->e2 = v.y;\
	}\
	else\
	{\
		int x = v.x, y = v.y;\
		parent->e1 = x;\
		parent->e2 = y;\
	}\
	return *parent;\
}\
vec_type& vec_type::e1##e2##_t::operator =(ivec2&& v)\
{\
	if((void*)(&v) != (void*)(parent))\
	{\
		parent->e1 = move(v.x);\
		parent->e2 = move(v.y);\
	}\
	else\
	{\
		int x = v.x, y = v.y;\
		parent->e1 = x;\
		parent->e2 = y;\
	}\
	return *parent;\
}

#define IMPLEMENT_IVEC3_REFERENCE(vec_type, e1, e2, e3) \
vec_type::e1##e2##e3##_t::operator ivec3()\
{\
	return ivec3(parent->e1, parent->e2, parent->e3);\
}\
int vec_type::e1##e2##e3##_t::operator [](uint i)const\
{\
	switch(i)\
	{\
		case 0: return parent->e1; break;\
		case 1: return parent->e2; break;\
		case 2: return parent->e3; break;\
		default: throw IndexError(i, 0, 2);\
	}\
}\
int& vec_type::e1##e2##e3##_t::operator [](uint i)\
{\
	switch(i)\
	{\
		case 0: return parent->e1; break;\
		case 1: return parent->e2; break;\
		case 2: return parent->e3; break;\
		default: throw IndexError(i, 0, 2);\
	}\
}\
vec_type& vec_type::e1##e2##e3##_t::operator =(const ivec3& v)\
{\
	if((void*)(&v) != (void*)(parent))\
	{\
		parent->e1 = v.x;\
		parent->e2 = v.y;\
		parent->e3 = v.z;\
	}\
	else\
	{\
		int x = v.x, y = v.y, z = v.z;\
		parent->e1 = x;\
		parent->e2 = y;\
		parent->e3 = z;\
	}\
	return *parent;\
}\
vec_type& vec_type::e1##e2##e3##_t::operator =(ivec3&& v)\
{\
	if((void*)(&v) != (void*)(parent))\
	{\
		parent->e1 = move(v.x);\
		parent->e2 = move(v.y);\
		parent->e3 = move(v.z);\
	}\
	else\
	{\
		int x = v.x, y = v.y, z = v.z;\
		parent->e1 = x;\
		parent->e2 = y;\
		parent->e3 = z;\
	}\
	return *parent;\
}

#define IMPLEMENT_IVEC4_REFERENCE(vec_type, e1, e2, e3, e4) \
vec_type::e1##e2##e3##e4##_t::operator ivec4()\
{\
	return ivec4(parent->e1, parent->e2, parent->e3, parent->e4);\
}\
int vec_type::e1##e2##e3##e4##_t::operator [](uint i)const\
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
int& vec_type::e1##e2##e3##e4##_t::operator [](uint i)\
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
vec_type& vec_type::e1##e2##e3##e4##_t::operator =(const ivec4& v)\
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
		int x = v.x, y = v.y, z = v.z, w = v.w;\
		parent->e1 = x;\
		parent->e2 = y;\
		parent->e3 = z;\
		parent->e4 = w;\
	}\
	return *parent;\
}\
vec_type& vec_type::e1##e2##e3##e4##_t::operator =(ivec4&& v)\
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
		int x = v.x, y = v.y, z = v.z, w = v.w;\
		parent->e1 = x;\
		parent->e2 = y;\
		parent->e3 = z;\
		parent->e4 = w;\
	}\
	return *parent;\
}

IMPLEMENT_INT_REFERENCE(ivec2, x)
IMPLEMENT_INT_REFERENCE(ivec2, y)
IMPLEMENT_IVEC2_REFERENCE(ivec2, x, x)
IMPLEMENT_IVEC2_REFERENCE(ivec2, x, y)
IMPLEMENT_IVEC2_REFERENCE(ivec2, y, x)
IMPLEMENT_IVEC2_REFERENCE(ivec2, y, y)
IMPLEMENT_IVEC3_REFERENCE(ivec2, x, x, x)
IMPLEMENT_IVEC3_REFERENCE(ivec2, x, x, y)
IMPLEMENT_IVEC3_REFERENCE(ivec2, x, y, x)
IMPLEMENT_IVEC3_REFERENCE(ivec2, x, y, y)
IMPLEMENT_IVEC3_REFERENCE(ivec2, y, x, x)
IMPLEMENT_IVEC3_REFERENCE(ivec2, y, x, y)
IMPLEMENT_IVEC3_REFERENCE(ivec2, y, y, x)
IMPLEMENT_IVEC3_REFERENCE(ivec2, y, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec2, x, x, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec2, x, x, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec2, x, x, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec2, x, x, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec2, x, y, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec2, x, y, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec2, x, y, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec2, x, y, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec2, y, x, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec2, y, x, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec2, y, x, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec2, y, x, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec2, y, y, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec2, y, y, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec2, y, y, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec2, y, y, y, y)

IMPLEMENT_INT_REFERENCE(ivec3, x)
IMPLEMENT_INT_REFERENCE(ivec3, y)
IMPLEMENT_INT_REFERENCE(ivec3, z)
IMPLEMENT_IVEC2_REFERENCE(ivec3, x, x)
IMPLEMENT_IVEC2_REFERENCE(ivec3, x, y)
IMPLEMENT_IVEC2_REFERENCE(ivec3, x, z)
IMPLEMENT_IVEC2_REFERENCE(ivec3, y, x)
IMPLEMENT_IVEC2_REFERENCE(ivec3, y, y)
IMPLEMENT_IVEC2_REFERENCE(ivec3, y, z)
IMPLEMENT_IVEC2_REFERENCE(ivec3, z, x)
IMPLEMENT_IVEC2_REFERENCE(ivec3, z, y)
IMPLEMENT_IVEC2_REFERENCE(ivec3, z, z)
IMPLEMENT_IVEC3_REFERENCE(ivec3, x, x, x)
IMPLEMENT_IVEC3_REFERENCE(ivec3, x, x, y)
IMPLEMENT_IVEC3_REFERENCE(ivec3, x, x, z)
IMPLEMENT_IVEC3_REFERENCE(ivec3, x, y, x)
IMPLEMENT_IVEC3_REFERENCE(ivec3, x, y, y)
IMPLEMENT_IVEC3_REFERENCE(ivec3, x, y, z)
IMPLEMENT_IVEC3_REFERENCE(ivec3, x, z, x)
IMPLEMENT_IVEC3_REFERENCE(ivec3, x, z, y)
IMPLEMENT_IVEC3_REFERENCE(ivec3, x, z, z)
IMPLEMENT_IVEC3_REFERENCE(ivec3, y, x, x)
IMPLEMENT_IVEC3_REFERENCE(ivec3, y, x, y)
IMPLEMENT_IVEC3_REFERENCE(ivec3, y, x, z)
IMPLEMENT_IVEC3_REFERENCE(ivec3, y, y, x)
IMPLEMENT_IVEC3_REFERENCE(ivec3, y, y, y)
IMPLEMENT_IVEC3_REFERENCE(ivec3, y, y, z)
IMPLEMENT_IVEC3_REFERENCE(ivec3, y, z, x)
IMPLEMENT_IVEC3_REFERENCE(ivec3, y, z, y)
IMPLEMENT_IVEC3_REFERENCE(ivec3, y, z, z)
IMPLEMENT_IVEC3_REFERENCE(ivec3, z, x, x)
IMPLEMENT_IVEC3_REFERENCE(ivec3, z, x, y)
IMPLEMENT_IVEC3_REFERENCE(ivec3, z, x, z)
IMPLEMENT_IVEC3_REFERENCE(ivec3, z, y, x)
IMPLEMENT_IVEC3_REFERENCE(ivec3, z, y, y)
IMPLEMENT_IVEC3_REFERENCE(ivec3, z, y, z)
IMPLEMENT_IVEC3_REFERENCE(ivec3, z, z, x)
IMPLEMENT_IVEC3_REFERENCE(ivec3, z, z, y)
IMPLEMENT_IVEC3_REFERENCE(ivec3, z, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, x, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, x, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, x, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, x, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, x, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, x, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, x, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, x, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, x, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, y, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, y, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, y, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, y, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, y, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, y, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, y, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, y, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, y, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, z, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, z, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, z, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, z, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, z, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, z, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, z, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, z, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, x, z, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, x, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, x, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, x, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, x, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, x, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, x, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, x, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, x, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, x, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, y, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, y, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, y, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, y, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, y, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, y, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, y, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, y, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, y, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, z, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, z, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, z, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, z, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, z, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, z, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, z, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, z, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, y, z, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, x, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, x, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, x, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, x, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, x, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, x, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, x, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, x, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, x, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, y, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, y, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, y, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, y, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, y, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, y, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, y, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, y, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, y, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, z, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, z, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, z, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, z, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, z, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, z, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, z, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, z, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec3, z, z, z, z)

IMPLEMENT_INT_REFERENCE(ivec4, x)
IMPLEMENT_INT_REFERENCE(ivec4, y)
IMPLEMENT_INT_REFERENCE(ivec4, z)
IMPLEMENT_INT_REFERENCE(ivec4, w)
IMPLEMENT_IVEC2_REFERENCE(ivec4, x, x)
IMPLEMENT_IVEC2_REFERENCE(ivec4, x, y)
IMPLEMENT_IVEC2_REFERENCE(ivec4, x, z)
IMPLEMENT_IVEC2_REFERENCE(ivec4, x, w)
IMPLEMENT_IVEC2_REFERENCE(ivec4, y, x)
IMPLEMENT_IVEC2_REFERENCE(ivec4, y, y)
IMPLEMENT_IVEC2_REFERENCE(ivec4, y, z)
IMPLEMENT_IVEC2_REFERENCE(ivec4, y, w)
IMPLEMENT_IVEC2_REFERENCE(ivec4, z, x)
IMPLEMENT_IVEC2_REFERENCE(ivec4, z, y)
IMPLEMENT_IVEC2_REFERENCE(ivec4, z, z)
IMPLEMENT_IVEC2_REFERENCE(ivec4, z, w)
IMPLEMENT_IVEC2_REFERENCE(ivec4, w, x)
IMPLEMENT_IVEC2_REFERENCE(ivec4, w, y)
IMPLEMENT_IVEC2_REFERENCE(ivec4, w, z)
IMPLEMENT_IVEC2_REFERENCE(ivec4, w, w)
IMPLEMENT_IVEC3_REFERENCE(ivec4, x, x, x)
IMPLEMENT_IVEC3_REFERENCE(ivec4, x, x, y)
IMPLEMENT_IVEC3_REFERENCE(ivec4, x, x, z)
IMPLEMENT_IVEC3_REFERENCE(ivec4, x, x, w)
IMPLEMENT_IVEC3_REFERENCE(ivec4, x, y, x)
IMPLEMENT_IVEC3_REFERENCE(ivec4, x, y, y)
IMPLEMENT_IVEC3_REFERENCE(ivec4, x, y, z)
IMPLEMENT_IVEC3_REFERENCE(ivec4, x, y, w)
IMPLEMENT_IVEC3_REFERENCE(ivec4, x, z, x)
IMPLEMENT_IVEC3_REFERENCE(ivec4, x, z, y)
IMPLEMENT_IVEC3_REFERENCE(ivec4, x, z, z)
IMPLEMENT_IVEC3_REFERENCE(ivec4, x, z, w)
IMPLEMENT_IVEC3_REFERENCE(ivec4, x, w, x)
IMPLEMENT_IVEC3_REFERENCE(ivec4, x, w, y)
IMPLEMENT_IVEC3_REFERENCE(ivec4, x, w, z)
IMPLEMENT_IVEC3_REFERENCE(ivec4, x, w, w)
IMPLEMENT_IVEC3_REFERENCE(ivec4, y, x, x)
IMPLEMENT_IVEC3_REFERENCE(ivec4, y, x, y)
IMPLEMENT_IVEC3_REFERENCE(ivec4, y, x, z)
IMPLEMENT_IVEC3_REFERENCE(ivec4, y, x, w)
IMPLEMENT_IVEC3_REFERENCE(ivec4, y, y, x)
IMPLEMENT_IVEC3_REFERENCE(ivec4, y, y, y)
IMPLEMENT_IVEC3_REFERENCE(ivec4, y, y, z)
IMPLEMENT_IVEC3_REFERENCE(ivec4, y, y, w)
IMPLEMENT_IVEC3_REFERENCE(ivec4, y, z, x)
IMPLEMENT_IVEC3_REFERENCE(ivec4, y, z, y)
IMPLEMENT_IVEC3_REFERENCE(ivec4, y, z, z)
IMPLEMENT_IVEC3_REFERENCE(ivec4, y, z, w)
IMPLEMENT_IVEC3_REFERENCE(ivec4, y, w, x)
IMPLEMENT_IVEC3_REFERENCE(ivec4, y, w, y)
IMPLEMENT_IVEC3_REFERENCE(ivec4, y, w, z)
IMPLEMENT_IVEC3_REFERENCE(ivec4, y, w, w)
IMPLEMENT_IVEC3_REFERENCE(ivec4, z, x, x)
IMPLEMENT_IVEC3_REFERENCE(ivec4, z, x, y)
IMPLEMENT_IVEC3_REFERENCE(ivec4, z, x, z)
IMPLEMENT_IVEC3_REFERENCE(ivec4, z, x, w)
IMPLEMENT_IVEC3_REFERENCE(ivec4, z, y, x)
IMPLEMENT_IVEC3_REFERENCE(ivec4, z, y, y)
IMPLEMENT_IVEC3_REFERENCE(ivec4, z, y, z)
IMPLEMENT_IVEC3_REFERENCE(ivec4, z, y, w)
IMPLEMENT_IVEC3_REFERENCE(ivec4, z, z, x)
IMPLEMENT_IVEC3_REFERENCE(ivec4, z, z, y)
IMPLEMENT_IVEC3_REFERENCE(ivec4, z, z, z)
IMPLEMENT_IVEC3_REFERENCE(ivec4, z, z, w)
IMPLEMENT_IVEC3_REFERENCE(ivec4, z, w, x)
IMPLEMENT_IVEC3_REFERENCE(ivec4, z, w, y)
IMPLEMENT_IVEC3_REFERENCE(ivec4, z, w, z)
IMPLEMENT_IVEC3_REFERENCE(ivec4, z, w, w)
IMPLEMENT_IVEC3_REFERENCE(ivec4, w, x, x)
IMPLEMENT_IVEC3_REFERENCE(ivec4, w, x, y)
IMPLEMENT_IVEC3_REFERENCE(ivec4, w, x, z)
IMPLEMENT_IVEC3_REFERENCE(ivec4, w, x, w)
IMPLEMENT_IVEC3_REFERENCE(ivec4, w, y, x)
IMPLEMENT_IVEC3_REFERENCE(ivec4, w, y, y)
IMPLEMENT_IVEC3_REFERENCE(ivec4, w, y, z)
IMPLEMENT_IVEC3_REFERENCE(ivec4, w, y, w)
IMPLEMENT_IVEC3_REFERENCE(ivec4, w, z, x)
IMPLEMENT_IVEC3_REFERENCE(ivec4, w, z, y)
IMPLEMENT_IVEC3_REFERENCE(ivec4, w, z, z)
IMPLEMENT_IVEC3_REFERENCE(ivec4, w, z, w)
IMPLEMENT_IVEC3_REFERENCE(ivec4, w, w, x)
IMPLEMENT_IVEC3_REFERENCE(ivec4, w, w, y)
IMPLEMENT_IVEC3_REFERENCE(ivec4, w, w, z)
IMPLEMENT_IVEC3_REFERENCE(ivec4, w, w, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, x, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, x, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, x, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, x, x, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, x, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, x, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, x, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, x, y, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, x, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, x, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, x, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, x, z, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, x, w, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, x, w, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, x, w, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, x, w, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, y, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, y, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, y, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, y, x, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, y, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, y, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, y, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, y, y, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, y, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, y, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, y, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, y, z, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, y, w, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, y, w, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, y, w, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, y, w, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, z, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, z, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, z, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, z, x, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, z, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, z, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, z, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, z, y, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, z, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, z, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, z, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, z, z, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, z, w, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, z, w, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, z, w, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, z, w, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, w, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, w, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, w, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, w, x, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, w, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, w, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, w, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, w, y, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, w, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, w, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, w, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, w, z, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, w, w, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, w, w, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, w, w, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, x, w, w, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, x, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, x, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, x, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, x, x, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, x, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, x, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, x, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, x, y, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, x, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, x, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, x, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, x, z, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, x, w, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, x, w, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, x, w, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, x, w, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, y, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, y, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, y, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, y, x, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, y, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, y, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, y, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, y, y, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, y, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, y, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, y, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, y, z, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, y, w, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, y, w, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, y, w, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, y, w, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, z, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, z, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, z, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, z, x, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, z, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, z, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, z, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, z, y, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, z, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, z, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, z, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, z, z, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, z, w, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, z, w, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, z, w, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, z, w, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, w, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, w, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, w, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, w, x, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, w, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, w, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, w, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, w, y, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, w, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, w, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, w, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, w, z, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, w, w, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, w, w, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, w, w, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, y, w, w, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, x, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, x, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, x, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, x, x, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, x, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, x, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, x, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, x, y, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, x, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, x, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, x, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, x, z, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, x, w, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, x, w, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, x, w, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, x, w, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, y, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, y, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, y, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, y, x, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, y, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, y, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, y, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, y, y, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, y, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, y, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, y, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, y, z, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, y, w, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, y, w, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, y, w, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, y, w, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, z, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, z, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, z, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, z, x, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, z, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, z, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, z, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, z, y, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, z, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, z, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, z, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, z, z, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, z, w, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, z, w, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, z, w, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, z, w, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, w, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, w, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, w, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, w, x, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, w, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, w, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, w, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, w, y, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, w, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, w, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, w, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, w, z, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, w, w, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, w, w, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, w, w, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, z, w, w, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, x, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, x, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, x, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, x, x, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, x, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, x, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, x, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, x, y, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, x, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, x, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, x, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, x, z, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, x, w, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, x, w, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, x, w, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, x, w, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, y, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, y, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, y, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, y, x, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, y, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, y, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, y, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, y, y, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, y, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, y, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, y, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, y, z, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, y, w, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, y, w, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, y, w, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, y, w, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, z, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, z, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, z, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, z, x, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, z, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, z, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, z, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, z, y, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, z, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, z, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, z, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, z, z, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, z, w, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, z, w, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, z, w, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, z, w, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, w, x, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, w, x, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, w, x, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, w, x, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, w, y, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, w, y, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, w, y, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, w, y, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, w, z, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, w, z, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, w, z, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, w, z, w)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, w, w, x)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, w, w, y)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, w, w, z)
IMPLEMENT_IVEC4_REFERENCE(ivec4, w, w, w, w)
#endif

ivec2::ivec2()
#ifdef ENABLE_SWIZZLE
 : parent(this)
#endif
{}
ivec2::ivec2(int value) : x(value), y(value)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

ivec2::ivec2(int _x, int _y) : x(_x), y(_y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

ivec2::ivec2(const ivec3& v) : x(v.x), y(v.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

ivec2::ivec2(const ivec4& v) : x(v.x), y(v.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

ivec2::ivec2(const ivec2& v) : x(v.x), y(v.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

ivec2::ivec2(ivec2&& v) : x(move(v.x)), y(move(v.y))
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}


int* ivec2::data()const
{
	return (int*)this;
}

ivec2& ivec2::operator =(const ivec2& v)
{
	if(this != &v)
	{
		x = v.x;
		y = v.y;
	}

	return *this;
}

ivec2& ivec2::operator =(ivec2&& v)
{
	if(this != &v)
	{
		x = move(v.x);
		y = move(v.y);
	}

	return *this;
}

ivec2::operator dvec2()const
{
	return dvec2(x, y);
}

ivec2::operator vec2()const
{
	return vec2(x, y);
}

ivec2::operator uvec2()const
{
	return uvec2(x, y);
}

ivec2::operator bvec2()const
{
	return bvec2(x, y);
}

int& ivec2::operator [](uint i)
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

int ivec2::operator [](uint i)const
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

ivec2 ivec2::operator +()const
{
	return *this;
}

ivec2 ivec2::operator -()const
{
	return ivec2(-x, -y);
}

ivec2& ivec2::operator +=(const ivec2& v)
{
	x += v.x;
	y += v.y;

	return *this;
}

ivec2& ivec2::operator -=(const ivec2& v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}

ivec2& ivec2::operator *=(const ivec2& v)
{
	x *= v.x;
	y *= v.y;

	return *this;
}

ivec2& ivec2::operator /=(const ivec2& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error when ivec2 v1 /= ivec2 v2: v2.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error when ivec2 v1 /= ivec2 v2: v2.y is 0 now.");
	}

	x /= v.x;
	y /= v.y;

	return *this;
}

ivec2& ivec2::operator %=(const ivec2& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error when ivec2 v1 %= ivec2 v2: v2.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error when ivec2 v1 %= ivec2 v2: v2.y is 0 now.");
	}

	x %= v.x;
	y %= v.y;

	return *this;
}

ivec2& ivec2::operator +=(int value)
{
	x += value;
	y += value;

	return *this;
}

ivec2& ivec2::operator -=(int value)
{
	x -= value;
	y -= value;

	return *this;
}

ivec2& ivec2::operator *=(int value)
{
	x *= value;
	y *= value;

	return *this;
}

ivec2& ivec2::operator /=(int value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error when ivec2 v /= int value: value is 0 now.");
	}

	x /= value;
	y /= value;

	return *this;
}

ivec2& ivec2::operator %=(int value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error when ivec2 v %= int value: value is 0 now.");
	}

	x %= value;
	y %= value;

	return *this;
}

ivec2 operator +(const ivec2& v1, const ivec2& v2)
{
	return ivec2(v1.x + v2.x, v1.y + v2.y);
}

ivec2 operator -(const ivec2& v1, const ivec2& v2)
{
	return ivec2(v1.x - v2.x, v1.y - v2.y);
}

ivec2 operator *(const ivec2& v1, const ivec2& v2)
{
	return ivec2(v1.x * v2.x, v1.y * v2.y);
}

ivec2 operator /(const ivec2& v1, const ivec2& v2)
{
	if(v2.x == 0)
	{
		throw ZeroDivisionError("Error when ivec2 v1 / ivec2 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw ZeroDivisionError("Error when ivec2 v1 / ivec2 v2: v2.y is 0 now.");
	}
	return ivec2(v1.x / v2.x, v1.y / v2.y);
}

ivec2 operator %(const ivec2& v1, const ivec2& v2)
{
	if(v2.x == 0)
	{
		throw ZeroDivisionError("Error when ivec2 v1 %% ivec2 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw ZeroDivisionError("Error when ivec2 v1 %% ivec2 v2: v2.y is 0 now.");
	}
	return ivec2(v1.x % v2.x, v1.y % v2.y);
}

dvec2 operator +(const ivec2& v, double value)
{
	return dvec2(v.x + value, v.y + value);
}

dvec2 operator -(const ivec2& v, double value)
{
	return dvec2(v.x - value, v.y - value);
}

dvec2 operator *(const ivec2& v, double value)
{
	return dvec2(v.x * value, v.y * value);
}

dvec2 operator /(const ivec2& v, double value)
{
	if(is_zero(value))
	{
		throw ZeroDivisionError("Error when ivec2 v / double value: value is 0 now.");
	}
	return dvec2(v.x / value, v.y / value);
}

dvec2 operator +(double value, const ivec2& v)
{
	return dvec2(value + v.x, value + v.y);
}

dvec2 operator -(double value, const ivec2& v)
{
	return dvec2(value - v.x, value - v.y);
}

dvec2 operator *(double value, const ivec2& v)
{
	return dvec2(value * v.x, value * v.y);
}

dvec2 operator /(double value, const ivec2& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error when double value / ivec2 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error when double value / ivec2 v: v.y is 0 now.");
	}
	return dvec2(value / v.x, value / v.y);
}

vec2 operator +(const ivec2& v, float value)
{
	return vec2(v.x + value, v.y + value);
}

vec2 operator -(const ivec2& v, float value)
{
	return vec2(v.x - value, v.y - value);
}

vec2 operator *(const ivec2& v, float value)
{
	return vec2(v.x * value, v.y * value);
}

vec2 operator /(const ivec2& v, float value)
{
	if(is_zero(value))
	{
		throw ZeroDivisionError("Error when vec2 v / float value: value is 0 now.");
	}
	return vec2(v.x / value, v.y / value);
}

vec2 operator +(float value, const ivec2& v)
{
	return vec2(value + v.x, value + v.y);
}

vec2 operator -(float value, const ivec2& v)
{
	return vec2(value - v.x, value - v.y);
}

vec2 operator *(float value, const ivec2& v)
{
	return vec2(value * v.x, value * v.y);
}

vec2 operator /(float value, const ivec2& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error when float value / vec2 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error when float value / vec2 v: v.y is 0 now.");
	}
	return vec2(value / v.x, value / v.y);
}

uvec2 operator +(const ivec2& v, uint value)
{
	return uvec2(v.x + value, v.y + value);
}

uvec2 operator -(const ivec2& v, uint value)
{
	return uvec2(v.x - value, v.y - value);
}

uvec2 operator *(const ivec2& v, uint value)
{
	return uvec2(v.x * value, v.y * value);
}

uvec2 operator /(const ivec2& v, uint value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error when vec2 v / uint value: value is 0 now.");
	}
	return uvec2(v.x / value, v.y / value);
}

uvec2 operator %(const ivec2& v, uint value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error when vec2 v %% uint value: value is 0 now.");
	}
	return uvec2(v.x % value, v.y % value);
}

uvec2 operator +(uint value, const ivec2& v)
{
	return uvec2(value + v.x, value + v.y);
}

uvec2 operator -(uint value, const ivec2& v)
{
	return uvec2(value - v.x, value - v.y);
}

uvec2 operator *(uint value, const ivec2& v)
{
	return uvec2(value * v.x, value * v.y);
}

uvec2 operator /(uint value, const ivec2& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error when uint value / vec2 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error when uint value / vec2 v: v.y is 0 now.");
	}
	return uvec2(value / v.x, value / v.y);
}

uvec2 operator %(uint value, const ivec2& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error when uint value %% vec2 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error when uint value %% vec2 v: v.y is 0 now.");
	}
	return uvec2(value % v.x, value % v.y);
}

ivec2 operator +(const ivec2& v, int value)
{
	return ivec2(v.x + value, v.y + value);
}

ivec2 operator -(const ivec2& v, int value)
{
	return ivec2(v.x - value, v.y - value);
}

ivec2 operator *(const ivec2& v, int value)
{
	return ivec2(v.x * value, v.y * value);
}

ivec2 operator /(const ivec2& v, int value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error when vec2 v / int value: value is 0 now.");
	}
	return ivec2(v.x / value, v.y / value);
}

ivec2 operator %(const ivec2& v, int value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error when vec2 v %% int value: value is 0 now.");
	}
	return ivec2(v.x % value, v.y % value);
}

ivec2 operator +(int value, const ivec2& v)
{
	return ivec2(value + v.x, value + v.y);
}

ivec2 operator -(int value, const ivec2& v)
{
	return ivec2(value - v.x, value - v.y);
}

ivec2 operator *(int value, const ivec2& v)
{
	return ivec2(value * v.x, value * v.y);
}

ivec2 operator /(int value, const ivec2& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error when int value / ivec2 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error when int value / ivec2 v: v.y is 0 now.");
	}
	return ivec2(value / v.x, value / v.y);
}

ivec2 operator %(int value, const ivec2& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error when int value %% ivec2 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error when int value %% ivec2 v: v.y is 0 now.");
	}
	return ivec2(value % v.x, value % v.y);
}

ostream& operator <<(ostream& out, const ivec2& v)
{
	out << "ivec2(" << v.x << ", " << v.y << ")";
	return out;
}

ivec3::ivec3()
#ifdef ENABLE_SWIZZLE
 : parent(this)
#endif
{}

ivec3::ivec3(int value) : x(value), y(value), z(value)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

ivec3::ivec3(int _x, int _y, int _z) : x(_x), y(_y), z(_z)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

ivec3::ivec3(const ivec2& v, int _z) : x(v.x), y(v.y), z(_z)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

ivec3::ivec3(int _x, const ivec2& v) : x(_x), y(v.x), z(v.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

ivec3::ivec3(const ivec4& v) : x(v.x), y(v.y), z(v.z)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

ivec3::ivec3(const ivec3& v) : x(v.x), y(v.y), z(v.z)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

ivec3::ivec3(ivec3&& v) : x(move(v.x)), y(move(v.y)), z(move(v.z))
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}


int* ivec3::data()const
{
	return (int*)this;
}

ivec3& ivec3::operator =(const ivec3& v)
{
	if(this != &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	return *this;
}

ivec3& ivec3::operator =(ivec3&& v)
{
	if(this != &v)
	{
		x = move(v.x);
		y = move(v.y);
		z = move(v.z);
	}

	return *this;
}

ivec3::operator dvec3()const
{
	return dvec3(x, y, z);
}

ivec3::operator vec3()const
{
	return vec3(x, y, z);
}

ivec3::operator uvec3()const
{
	return uvec3(x, y, z);
}

ivec3::operator bvec3()const
{
	return bvec3(x, y, z);
}

int& ivec3::operator [](uint i)
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

int ivec3::operator [](uint i)const
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

ivec3 ivec3::operator +()const
{
	return *this;
}

ivec3 ivec3::operator -()const
{
	return ivec3(-x, -y, -z);
}

ivec3& ivec3::operator +=(const ivec3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

ivec3& ivec3::operator -=(const ivec3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

ivec3& ivec3::operator *=(const ivec3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}

ivec3& ivec3::operator /=(const ivec3& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error using ivec3 v1 /= ivec3 v2:\nv2.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error using ivec3 v1 /= ivec3 v2:\nv2.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw ZeroDivisionError("Error using ivec3 v1 /= ivec3 v2:\nv2.z is 0 now.");
	}

	x /= v.x;
	y /= v.y;
	z /= v.z;

	return *this;
}

ivec3& ivec3::operator %=(const ivec3& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error using ivec3 v1 %= ivec3 v2:\nv2.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error using ivec3 v1 %= ivec3 v2:\nv2.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw ZeroDivisionError("Error using ivec3 v1 %= ivec3 v2:\nv2.z is 0 now.");
	}

	x %= v.x;
	y %= v.y;
	z %= v.z;

	return *this;
}

ivec3& ivec3::operator +=(int value)
{
	x += value;
	y += value;
	z += value;

	return *this;
}

ivec3& ivec3::operator -=(int value)
{
	x -= value;
	y -= value;
	z -= value;

	return *this;
}

ivec3& ivec3::operator *=(int value)
{
	x *= value;
	y *= value;
	z *= value;

	return *this;
}

ivec3& ivec3::operator /=(int value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error using vec3 v /= int value:\nvalue is 0 now.");
	}
	x /= value;
	y /= value;
	z /= value;

	return *this;
}

ivec3& ivec3::operator %=(int value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error using vec3 v %= int value:\nvalue is 0 now.");
	}
	x %= value;
	y %= value;
	z %= value;

	return *this;
}

ivec3 operator +(const ivec3& v1, const ivec3& v2)
{
	return ivec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

ivec3 operator -(const ivec3& v1, const ivec3& v2)
{
	return ivec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

ivec3 operator *(const ivec3& v1, const ivec3& v2)
{
	return ivec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

ivec3 operator /(const ivec3& v1, const ivec3& v2)
{
	if(v2.x == 0)
	{
		throw ZeroDivisionError("Error when ivec3 v1 / ivec3 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw ZeroDivisionError("Error when ivec3 v1 / ivec3 v2: v2.y is 0 now.");
	}
	if(v2.z == 0)
	{
		throw ZeroDivisionError("Error when ivec3 v1 / ivec3 v2: v2.z is 0 now.");
	}
	return ivec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

ivec3 operator %(const ivec3& v1, const ivec3& v2)
{
	if(v2.x == 0)
	{
		throw ZeroDivisionError("Error when ivec3 v1 %% ivec3 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw ZeroDivisionError("Error when ivec3 v1 %% ivec3 v2: v2.y is 0 now.");
	}
	if(v2.z == 0)
	{
		throw ZeroDivisionError("Error when ivec3 v1 %% ivec3 v2: v2.z is 0 now.");
	}
	return ivec3(v1.x % v2.x, v1.y % v2.y, v1.z % v2.z);
}

dvec3 operator +(const ivec3& v, double value)
{
	return dvec3(v.x + value, v.y + value, v.z + value);
}

dvec3 operator -(const ivec3& v, double value)
{
	return dvec3(v.x - value, v.y - value, v.z - value);
}

dvec3 operator *(const ivec3& v, double value)
{
	return dvec3(v.x * value, v.y * value, v.z * value);
}

dvec3 operator /(const ivec3& v, double value)
{
	if(is_zero(value))
	{
		throw ZeroDivisionError("Error when vec3 v / double value: value is 0 now.");
	}
	return dvec3(v.x / value, v.y / value, v.z / value);
}

dvec3 operator +(double value, const ivec3& v)
{
	return dvec3(value + v.x, value + v.y, value + v.z);
}

dvec3 operator -(double value, const ivec3& v)
{
	return dvec3(value - v.x, value - v.y, value - v.z);
}

dvec3 operator *(double value, const ivec3& v)
{
	return dvec3(value * v.x, value * v.y, value * v.z);
}

dvec3 operator /(double value, const ivec3& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error when double value / ivec3 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error when double value / ivec3 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw ZeroDivisionError("Error when double value / ivec3 v: v.z is 0 now.");
	}
	return dvec3(value / v.x, value / v.y, value / v.z);
}

vec3 operator +(const ivec3& v, float value)
{
	return vec3(v.x + value, v.y + value, v.z + value);
}

vec3 operator -(const ivec3& v, float value)
{
	return vec3(v.x - value, v.y - value, v.z - value);
}

vec3 operator *(const ivec3& v, float value)
{
	return vec3(v.x * value, v.y * value, v.z * value);
}

vec3 operator /(const ivec3& v, float value)
{
	if(is_zero(value))
	{
		throw ZeroDivisionError("Error when vec3 v / float value: value is 0 now.");
	}
	return vec3(v.x / value, v.y / value, v.z / value);
}

vec3 operator +(float value, const ivec3& v)
{
	return vec3(value + v.x, value + v.y, value + v.z);
}

vec3 operator -(float value, const ivec3& v)
{
	return vec3(value - v.x, value - v.y, value - v.z);
}

vec3 operator *(float value, const ivec3& v)
{
	return vec3(value * v.x, value * v.y, value * v.z);
}

vec3 operator /(float value, const ivec3& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error when float value / ivec3 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error when float value / ivec3 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw ZeroDivisionError("Error when float value / ivec3 v: v.z is 0 now.");
	}
	return vec3(value / v.x, value / v.y, value / v.z);
}

uvec3 operator +(const ivec3& v, uint value)
{
	return uvec3(v.x + value, v.y + value, v.z + value);
}

uvec3 operator -(const ivec3& v, uint value)
{
	return uvec3(v.x - value, v.y - value, v.z - value);
}

uvec3 operator *(const ivec3& v, uint value)
{
	return uvec3(v.x * value, v.y * value, v.z * value);
}

uvec3 operator /(const ivec3& v, uint value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error when vec3 v / uint value: value is 0 now.");
	}
	return uvec3(v.x / value, v.y / value, v.z / value);
}

uvec3 operator %(const ivec3& v, uint value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error when vec3 v %% uint value: value is 0 now.");
	}
	return uvec3(v.x % value, v.y % value, v.z % value);
}

uvec3 operator +(uint value, const ivec3& v)
{
	return uvec3(value + v.x, value + v.y, value + v.z);
}

uvec3 operator -(uint value, const ivec3& v)
{
	return uvec3(value - v.x, value - v.y, value - v.z);
}

uvec3 operator *(uint value, const ivec3& v)
{
	return uvec3(value * v.x, value * v.y, value * v.z);
}

uvec3 operator /(uint value, const ivec3& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error when uint value / ivec3 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error when uint value / ivec3 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw ZeroDivisionError("Error when uint value / ivec3 v: v.z is 0 now.");
	}
	return uvec3(value / v.x, value / v.y, value / v.z);
}

uvec3 operator %(uint value, const ivec3& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error when uint value %% ivec3 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error when uint value %% ivec3 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw ZeroDivisionError("Error when uint value %% ivec3 v: v.z is 0 now.");
	}
	return uvec3(value % v.x, value % v.y, value % v.z);
}

ivec3 operator +(const ivec3& v, int value)
{
	return ivec3(v.x + value, v.y + value, v.z + value);
}

ivec3 operator -(const ivec3& v, int value)
{
	return ivec3(v.x - value, v.y - value, v.z - value);
}

ivec3 operator *(const ivec3& v, int value)
{
	return ivec3(v.x * value, v.y * value, v.z * value);
}

ivec3 operator /(const ivec3& v, int value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error when ivec3 v / int value: value is 0 now.");
	}
	return ivec3(v.x / value, v.y / value, v.z / value);
}

ivec3 operator %(const ivec3& v, int value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error when ivec3 v %% int value: value is 0 now.");
	}
	return ivec3(v.x % value, v.y % value, v.z % value);
}

ivec3 operator +(int value, const ivec3& v)
{
	return ivec3(value + v.x, value + v.y, value + v.z);
}

ivec3 operator -(int value, const ivec3& v)
{
	return ivec3(value - v.x, value - v.y, value - v.z);
}

ivec3 operator *(int value, const ivec3& v)
{
	return ivec3(value * v.x, value * v.y, value * v.z);
}

ivec3 operator /(int value, const ivec3& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error when int value / ivec3 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error when int value / ivec3 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw ZeroDivisionError("Error when int value / ivec3 v: v.z is 0 now.");
	}
	return ivec3(value / v.x, value / v.y, value / v.z);
}

ivec3 operator %(int value, const ivec3& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error when int value %% ivec3 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error when int value %% ivec3 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw ZeroDivisionError("Error when int value %% ivec3 v: v.z is 0 now.");
	}
	return ivec3(value % v.x, value % v.y, value % v.z);
}

ivec4::ivec4()
#ifdef ENABLE_SWIZZLE
 : parent(this)
#endif
{}

ivec4::ivec4(int value) : x(value), y(value), z(value), w(value)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

ivec4::ivec4(int _x, int _y, int _z, int _w) : x(_x), y(_y), z(_z), w(_w)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

ivec4::ivec4(const ivec2& v, int _z, int _w) : x(v.x), y(v.y), z(_z), w(_w)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

ivec4::ivec4(int _x, const ivec2& v, int _w) : x(_x), y(v.x), z(v.y), w(_w)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

ivec4::ivec4(int _x, int _y, const ivec2& v) : x(_x), y(_y), z(v.x), w(v.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

ivec4::ivec4(const ivec2& v1, const ivec2& v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

ivec4::ivec4(const ivec3& v, int _w) : x(v.x), y(v.y), z(v.z), w(_w)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

ivec4::ivec4(int _x, const ivec3& v) : x(_x), y(v.x), z(v.y), w(v.z)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

ivec4::ivec4(const ivec4& v) : x(v.x), y(v.y), z(v.z), w(v.w)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

ivec4::ivec4(ivec4&& v) : x(move(v.x)), y(move(v.y)), z(move(v.z)), w(move(v.w))
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}


int* ivec4::data()const
{
	return (int*)this;
}

ivec4& ivec4::operator =(const ivec4& v)
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

ivec4& ivec4::operator =(ivec4&& v)
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

ivec4::operator dvec4()const
{
	return dvec4(x, y, z, w);
}

ivec4::operator vec4()const
{
	return vec4(x, y, z, w);
}

ivec4::operator uvec4()const
{
	return uvec4(x, y, z, w);
}

ivec4::operator bvec4()const
{
	return bvec4(x, y, z, w);
}

int& ivec4::operator [](uint i)
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

int ivec4::operator [](uint i)const
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

ivec4 ivec4::operator +()const
{
	return *this;
}

ivec4 ivec4::operator -()const
{
	return ivec4(-x, -y, -z, -w);
}

ivec4& ivec4::operator +=(const ivec4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;

	return *this;
}

ivec4& ivec4::operator -=(const ivec4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;

	return *this;
}

ivec4& ivec4::operator *=(const ivec4& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;

	return *this;
}

ivec4& ivec4::operator /=(const ivec4& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error using ivec4 v1 /= ivec4 v2:\nv2.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error using ivec4 v1 /= ivec4 v2:\nv2.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw ZeroDivisionError("Error using ivec4 v1 /= ivec4 v2:\nv2.z is 0 now.");
	}
	if(v.w == 0)
	{
		throw ZeroDivisionError("Error using ivec4 v1 /= ivec4 v2:\nv2.w is 0 now.");
	}

	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;

	return *this;
}

ivec4& ivec4::operator %=(const ivec4& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error using ivec4 v1 %= ivec4 v2:\nv2.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error using ivec4 v1 %= ivec4 v2:\nv2.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw ZeroDivisionError("Error using ivec4 v1 %= ivec4 v2:\nv2.z is 0 now.");
	}
	if(v.w == 0)
	{
		throw ZeroDivisionError("Error using ivec4 v1 %= ivec4 v2:\nv2.w is 0 now.");
	}

	x %= v.x;
	y %= v.y;
	z %= v.z;
	w %= v.w;

	return *this;
}

ivec4& ivec4::operator +=(int value)
{
	x += value;
	y += value;
	z += value;
	w += value;

	return *this;
}

ivec4& ivec4::operator -=(int value)
{
	x -= value;
	y -= value;
	z -= value;
	w -= value;

	return *this;
}

ivec4& ivec4::operator *=(int value)
{
	x *= value;
	y *= value;
	z *= value;
	w *= value;

	return *this;
}

ivec4& ivec4::operator /=(int value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error using ivec4 v1 /= int value:\nvalue is 0 now.");
	}

	x /= value;
	y /= value;
	z /= value;
	w /= value;

	return *this;
}

ivec4& ivec4::operator %=(int value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error using ivec4 v1 %= int value:\nvalue is 0 now.");
	}

	x %= value;
	y %= value;
	z %= value;
	w %= value;

	return *this;
}

ivec4 operator +(const ivec4& v1, const ivec4& v2)
{
	return ivec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

ivec4 operator -(const ivec4& v1, const ivec4& v2)
{
	return ivec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

ivec4 operator *(const ivec4& v1, const ivec4& v2)
{
	return ivec4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}

ivec4 operator /(const ivec4& v1, const ivec4& v2)
{
	if(v2.x == 0)
	{
		throw ZeroDivisionError("Error when ivec4 v1 / ivec4 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw ZeroDivisionError("Error when ivec4 v1 / ivec4 v2: v2.y is 0 now.");
	}
	if(v2.z == 0)
	{
		throw ZeroDivisionError("Error when ivec4 v1 / ivec4 v2: v2.z is 0 now.");
	}
	if(v2.w == 0)
	{
		throw ZeroDivisionError("Error when ivec4 v1 / ivec4 v2: v2.w is 0 now.");
	}
	return ivec4(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
}

ivec4 operator %(const ivec4& v1, const ivec4& v2)
{
	if(v2.x == 0)
	{
		throw ZeroDivisionError("Error when ivec4 v1 %% ivec4 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw ZeroDivisionError("Error when ivec4 v1 %% ivec4 v2: v2.y is 0 now.");
	}
	if(v2.z == 0)
	{
		throw ZeroDivisionError("Error when ivec4 v1 %% ivec4 v2: v2.z is 0 now.");
	}
	if(v2.w == 0)
	{
		throw ZeroDivisionError("Error when ivec4 v1 %% ivec4 v2: v2.w is 0 now.");
	}
	return ivec4(v1.x % v2.x, v1.y % v2.y, v1.z % v2.z, v1.w % v2.w);
}

dvec4 operator +(const ivec4& v, double value)
{
	return dvec4(v.x + value, v.y + value, v.z + value, v.w + value);
}

dvec4 operator -(const ivec4& v, double value)
{
	return dvec4(v.x - value, v.y - value, v.z - value, v.w - value);
}

dvec4 operator *(const ivec4& v, double value)
{
	return dvec4(v.x * value, v.y * value, v.z * value, v.w * value);
}

dvec4 operator /(const ivec4& v, double value)
{
	if(is_zero(value))
	{
		throw ZeroDivisionError("Error when ivec4 v / double value: value is 0 now.");
	}
	return dvec4(v.x / value, v.y / value, v.z / value, v.w / value);
}

dvec4 operator +(double value, const ivec4& v)
{
	return dvec4(value + v.x, value + v.y, value + v.z, value + v.w);
}

dvec4 operator -(double value, const ivec4& v)
{
	return dvec4(value - v.x, value - v.y, value - v.z, value - v.w);
}

dvec4 operator *(double value, const ivec4& v)
{
	return dvec4(value * v.x, value * v.y, value * v.z, value * v.w);
}

dvec4 operator /(double value, const ivec4& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error when double value / ivec4 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error when double value / ivec4 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw ZeroDivisionError("Error when double value / ivec4 v: v.z is 0 now.");
	}
	if(v.w == 0)
	{
		throw ZeroDivisionError("Error when double value / ivec4 v: v.w is 0 now.");
	}
	return dvec4(value / v.x, value / v.y, value / v.z, value / v.w);
}

vec4 operator +(const ivec4& v, float value)
{
	return vec4(v.x + value, v.y + value, v.z + value, v.w + value);
}

vec4 operator -(const ivec4& v, float value)
{
	return vec4(v.x - value, v.y - value, v.z - value, v.w - value);
}

vec4 operator *(const ivec4& v, float value)
{
	return vec4(v.x * value, v.y * value, v.z * value, v.w * value);
}

vec4 operator /(const ivec4& v, float value)
{
	if(is_zero(value))
	{
		throw ZeroDivisionError("Error when ivec4 v / float value: value is 0 now.");
	}
	return vec4(v.x / value, v.y / value, v.z / value, v.w / value);
}

vec4 operator +(float value, const ivec4& v)
{
	return vec4(value + v.x, value + v.y, value + v.z, value + v.w);
}

vec4 operator -(float value, const ivec4& v)
{
	return vec4(value - v.x, value - v.y, value - v.z, value - v.w);
}

vec4 operator *(float value, const ivec4& v)
{
	return vec4(value * v.x, value * v.y, value * v.z, value * v.w);
}

vec4 operator /(float value, const ivec4& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error when float value / ivec4 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error when float value / ivec4 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw ZeroDivisionError("Error when float value / ivec4 v: v.z is 0 now.");
	}
	if(v.w == 0)
	{
		throw ZeroDivisionError("Error when float value / ivec4 v: v.w is 0 now.");
	}
	return vec4(value / v.x, value / v.y, value / v.z, value / v.w);
}

uvec4 operator +(const ivec4& v, uint value)
{
	return uvec4(v.x + value, v.y + value, v.z + value, v.w + value);
}

uvec4 operator -(const ivec4& v, uint value)
{
	return uvec4(v.x - value, v.y - value, v.z - value, v.w - value);
}

uvec4 operator *(const ivec4& v, uint value)
{
	return uvec4(v.x * value, v.y * value, v.z * value, v.w * value);
}

uvec4 operator /(const ivec4& v, uint value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error when ivec4 v / uint value: value is 0 now.");
	}
	return uvec4(v.x / value, v.y / value, v.z / value, v.w / value);
}

uvec4 operator %(const ivec4& v, uint value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error when ivec4 v %% uint value: value is 0 now.");
	}
	return uvec4(v.x % value, v.y % value, v.z % value, v.w % value);
}

uvec4 operator +(uint value, const ivec4& v)
{
	return uvec4(value + v.x, value + v.y, value + v.z, value + v.w);
}

uvec4 operator -(uint value, const ivec4& v)
{
	return uvec4(value - v.x, value - v.y, value - v.z, value - v.w);
}

uvec4 operator *(uint value, const ivec4& v)
{
	return uvec4(value * v.x, value * v.y, value * v.z, value * v.w);
}

uvec4 operator /(uint value, const ivec4& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error when uint value / ivec4 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error when uint value / ivec4 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw ZeroDivisionError("Error when uint value / ivec4 v: v.z is 0 now.");
	}
	if(v.w == 0)
	{
		throw ZeroDivisionError("Error when uint value / ivec4 v: v.w is 0 now.");
	}
	return uvec4(value / v.x, value / v.y, value / v.z, value / v.w);
}

uvec4 operator %(uint value, const ivec4& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error when uint value %% ivec4 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error when uint value %% ivec4 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw ZeroDivisionError("Error when uint value %% ivec4 v: v.z is 0 now.");
	}
	if(v.w == 0)
	{
		throw ZeroDivisionError("Error when uint value %% ivec4 v: v.w is 0 now.");
	}
	return uvec4(value % v.x, value % v.y, value % v.z, value % v.w);
}

ivec4 operator +(const ivec4& v, int value)
{
	return ivec4(v.x + value, v.y + value, v.z + value, v.w + value);
}

ivec4 operator -(const ivec4& v, int value)
{
	return ivec4(v.x - value, v.y - value, v.z - value, v.w - value);
}

ivec4 operator *(const ivec4& v, int value)
{
	return ivec4(v.x * value, v.y * value, v.z * value, v.w * value);
}

ivec4 operator /(const ivec4& v, int value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error when ivec4 v / int value: value is 0 now.");
	}
	return ivec4(v.x / value, v.y / value, v.z / value, v.w / value);
}

ivec4 operator %(const ivec4& v, int value)
{
	if(value == 0)
	{
		throw ZeroDivisionError("Error when ivec4 v %% int value: value is 0 now.");
	}
	return ivec4(v.x % value, v.y % value, v.z % value, v.w % value);
}

ivec4 operator +(int value, const ivec4& v)
{
	return ivec4(value + v.x, value + v.y, value + v.z, value + v.w);
}

ivec4 operator -(int value, const ivec4& v)
{
	return ivec4(value - v.x, value - v.y, value - v.z, value - v.w);
}

ivec4 operator *(int value, const ivec4& v)
{
	return ivec4(value * v.x, value * v.y, value * v.z, value * v.w);
}

ivec4 operator /(int value, const ivec4& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error when int value / ivec4 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error when int value / ivec4 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw ZeroDivisionError("Error when int value / ivec4 v: v.z is 0 now.");
	}
	if(v.w == 0)
	{
		throw ZeroDivisionError("Error when int value / ivec4 v: v.w is 0 now.");
	}
	return ivec4(value / v.x, value / v.y, value / v.z, value / v.w);
}

ivec4 operator %(int value, const ivec4& v)
{
	if(v.x == 0)
	{
		throw ZeroDivisionError("Error when int value %% ivec4 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw ZeroDivisionError("Error when int value %% ivec4 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw ZeroDivisionError("Error when int value %% ivec4 v: v.z is 0 now.");
	}
	if(v.w == 0)
	{
		throw ZeroDivisionError("Error when int value %% ivec4 v: v.w is 0 now.");
	}
	return ivec4(value % v.x, value % v.y, value % v.z, value % v.w);
}

ostream& operator <<(ostream& out, const ivec4& v)
{
	out << "ivec4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
	return out;
}