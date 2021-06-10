#include "glass/utils/vec/dvec.h"
#include "glass/utils/vec/fvec.h"
#include "glass/utils/vec/uvec.h"
#include "glass/utils/vec/ivec.h"
#include "glass/utils/vec/bvec.h"
#include "glass/utils/vec/vec_process.h"
#include "glass/utils/exceptions.h"

using namespace std;

#ifdef ENABLE_SWIZZLE
#define IMPLEMENT_UINT_REFERENCE(vec_type, e1) \
vec_type::e1##_t::operator uint()const\
{\
	return parent->e1;\
}\
uint& vec_type::e1##_t::operator =(uint value)\
{\
	parent->e1 = value;\
	return parent->e1;\
}\

#define IMPLEMENT_UVEC2_REFERENCE(vec_type, e1, e2) \
vec_type::e1##e2##_t::operator uvec2()\
{\
	return uvec2(parent->e1, parent->e2);\
}\
uint vec_type::e1##e2##_t::operator [](uint i)const\
{\
	switch(i)\
	{\
		case 0: return parent->e1; break;\
		case 1: return parent->e2; break;\
		default: throw glass::IndexError(i, 0, 1);\
	}\
}\
uint& vec_type::e1##e2##_t::operator [](uint i)\
{\
	switch(i)\
	{\
		case 0: return parent->e1; break;\
		case 1: return parent->e2; break;\
		default: throw glass::IndexError(i, 0, 1);\
	}\
}\
vec_type& vec_type::e1##e2##_t::operator =(const uvec2& v)\
{\
	if((void*)(&v) != (void*)(parent))\
	{\
		parent->e1 = v.x;\
		parent->e2 = v.y;\
	}\
	else\
	{\
		uint x = v.x, y = v.y;\
		parent->e1 = x;\
		parent->e2 = y;\
	}\
	return *parent;\
}\
vec_type& vec_type::e1##e2##_t::operator =(uvec2&& v)\
{\
	if((void*)(&v) != (void*)(parent))\
	{\
		parent->e1 = move(v.x);\
		parent->e2 = move(v.y);\
	}\
	else\
	{\
		uint x = v.x, y = v.y;\
		parent->e1 = x;\
		parent->e2 = y;\
	}\
	return *parent;\
}

#define IMPLEMENT_UVEC3_REFERENCE(vec_type, e1, e2, e3) \
vec_type::e1##e2##e3##_t::operator uvec3()\
{\
	return uvec3(parent->e1, parent->e2, parent->e3);\
}\
uint vec_type::e1##e2##e3##_t::operator [](uint i)const\
{\
	switch(i)\
	{\
		case 0: return parent->e1; break;\
		case 1: return parent->e2; break;\
		case 2: return parent->e3; break;\
		default: throw glass::IndexError(i, 0, 2);\
	}\
}\
uint& vec_type::e1##e2##e3##_t::operator [](uint i)\
{\
	switch(i)\
	{\
		case 0: return parent->e1; break;\
		case 1: return parent->e2; break;\
		case 2: return parent->e3; break;\
		default: throw glass::IndexError(i, 0, 2);\
	}\
}\
vec_type& vec_type::e1##e2##e3##_t::operator =(const uvec3& v)\
{\
	if((void*)(&v) != (void*)(parent))\
	{\
		parent->e1 = v.x;\
		parent->e2 = v.y;\
		parent->e3 = v.z;\
	}\
	else\
	{\
		uint x = v.x, y = v.y, z = v.z;\
		parent->e1 = x;\
		parent->e2 = y;\
		parent->e3 = z;\
	}\
	return *parent;\
}\
vec_type& vec_type::e1##e2##e3##_t::operator =(uvec3&& v)\
{\
	if((void*)(&v) != (void*)(parent))\
	{\
		parent->e1 = move(v.x);\
		parent->e2 = move(v.y);\
		parent->e3 = move(v.z);\
	}\
	else\
	{\
		uint x = v.x, y = v.y, z = v.z;\
		parent->e1 = x;\
		parent->e2 = y;\
		parent->e3 = z;\
	}\
	return *parent;\
}

#define IMPLEMENT_UVEC4_REFERENCE(vec_type, e1, e2, e3, e4) \
vec_type::e1##e2##e3##e4##_t::operator uvec4()\
{\
	return uvec4(parent->e1, parent->e2, parent->e3, parent->e4);\
}\
uint vec_type::e1##e2##e3##e4##_t::operator [](uint i)const\
{\
	switch(i)\
	{\
		case 0: return parent->e1; break;\
		case 1: return parent->e2; break;\
		case 2: return parent->e3; break;\
		case 3: return parent->e4; break;\
		default: throw glass::IndexError(i, 0, 3);\
	}\
}\
uint& vec_type::e1##e2##e3##e4##_t::operator [](uint i)\
{\
	switch(i)\
	{\
		case 0: return parent->e1; break;\
		case 1: return parent->e2; break;\
		case 2: return parent->e3; break;\
		case 3: return parent->e4; break;\
		default: throw glass::IndexError(i, 0, 3);\
	}\
}\
vec_type& vec_type::e1##e2##e3##e4##_t::operator =(const uvec4& v)\
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
		uint x = v.x, y = v.y, z = v.z, w = v.w;\
		parent->e1 = x;\
		parent->e2 = y;\
		parent->e3 = z;\
		parent->e4 = w;\
	}\
	return *parent;\
}\
vec_type& vec_type::e1##e2##e3##e4##_t::operator =(uvec4&& v)\
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
		uint x = v.x, y = v.y, z = v.z, w = v.w;\
		parent->e1 = x;\
		parent->e2 = y;\
		parent->e3 = z;\
		parent->e4 = w;\
	}\
	return *parent;\
}

IMPLEMENT_UINT_REFERENCE(uvec2, x)
IMPLEMENT_UINT_REFERENCE(uvec2, y)
IMPLEMENT_UVEC2_REFERENCE(uvec2, x, x)
IMPLEMENT_UVEC2_REFERENCE(uvec2, x, y)
IMPLEMENT_UVEC2_REFERENCE(uvec2, y, x)
IMPLEMENT_UVEC2_REFERENCE(uvec2, y, y)
IMPLEMENT_UVEC3_REFERENCE(uvec2, x, x, x)
IMPLEMENT_UVEC3_REFERENCE(uvec2, x, x, y)
IMPLEMENT_UVEC3_REFERENCE(uvec2, x, y, x)
IMPLEMENT_UVEC3_REFERENCE(uvec2, x, y, y)
IMPLEMENT_UVEC3_REFERENCE(uvec2, y, x, x)
IMPLEMENT_UVEC3_REFERENCE(uvec2, y, x, y)
IMPLEMENT_UVEC3_REFERENCE(uvec2, y, y, x)
IMPLEMENT_UVEC3_REFERENCE(uvec2, y, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec2, x, x, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec2, x, x, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec2, x, x, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec2, x, x, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec2, x, y, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec2, x, y, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec2, x, y, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec2, x, y, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec2, y, x, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec2, y, x, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec2, y, x, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec2, y, x, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec2, y, y, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec2, y, y, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec2, y, y, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec2, y, y, y, y)

IMPLEMENT_UINT_REFERENCE(uvec3, x)
IMPLEMENT_UINT_REFERENCE(uvec3, y)
IMPLEMENT_UINT_REFERENCE(uvec3, z)
IMPLEMENT_UVEC2_REFERENCE(uvec3, x, x)
IMPLEMENT_UVEC2_REFERENCE(uvec3, x, y)
IMPLEMENT_UVEC2_REFERENCE(uvec3, x, z)
IMPLEMENT_UVEC2_REFERENCE(uvec3, y, x)
IMPLEMENT_UVEC2_REFERENCE(uvec3, y, y)
IMPLEMENT_UVEC2_REFERENCE(uvec3, y, z)
IMPLEMENT_UVEC2_REFERENCE(uvec3, z, x)
IMPLEMENT_UVEC2_REFERENCE(uvec3, z, y)
IMPLEMENT_UVEC2_REFERENCE(uvec3, z, z)
IMPLEMENT_UVEC3_REFERENCE(uvec3, x, x, x)
IMPLEMENT_UVEC3_REFERENCE(uvec3, x, x, y)
IMPLEMENT_UVEC3_REFERENCE(uvec3, x, x, z)
IMPLEMENT_UVEC3_REFERENCE(uvec3, x, y, x)
IMPLEMENT_UVEC3_REFERENCE(uvec3, x, y, y)
IMPLEMENT_UVEC3_REFERENCE(uvec3, x, y, z)
IMPLEMENT_UVEC3_REFERENCE(uvec3, x, z, x)
IMPLEMENT_UVEC3_REFERENCE(uvec3, x, z, y)
IMPLEMENT_UVEC3_REFERENCE(uvec3, x, z, z)
IMPLEMENT_UVEC3_REFERENCE(uvec3, y, x, x)
IMPLEMENT_UVEC3_REFERENCE(uvec3, y, x, y)
IMPLEMENT_UVEC3_REFERENCE(uvec3, y, x, z)
IMPLEMENT_UVEC3_REFERENCE(uvec3, y, y, x)
IMPLEMENT_UVEC3_REFERENCE(uvec3, y, y, y)
IMPLEMENT_UVEC3_REFERENCE(uvec3, y, y, z)
IMPLEMENT_UVEC3_REFERENCE(uvec3, y, z, x)
IMPLEMENT_UVEC3_REFERENCE(uvec3, y, z, y)
IMPLEMENT_UVEC3_REFERENCE(uvec3, y, z, z)
IMPLEMENT_UVEC3_REFERENCE(uvec3, z, x, x)
IMPLEMENT_UVEC3_REFERENCE(uvec3, z, x, y)
IMPLEMENT_UVEC3_REFERENCE(uvec3, z, x, z)
IMPLEMENT_UVEC3_REFERENCE(uvec3, z, y, x)
IMPLEMENT_UVEC3_REFERENCE(uvec3, z, y, y)
IMPLEMENT_UVEC3_REFERENCE(uvec3, z, y, z)
IMPLEMENT_UVEC3_REFERENCE(uvec3, z, z, x)
IMPLEMENT_UVEC3_REFERENCE(uvec3, z, z, y)
IMPLEMENT_UVEC3_REFERENCE(uvec3, z, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, x, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, x, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, x, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, x, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, x, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, x, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, x, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, x, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, x, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, y, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, y, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, y, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, y, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, y, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, y, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, y, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, y, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, y, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, z, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, z, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, z, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, z, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, z, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, z, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, z, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, z, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, x, z, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, x, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, x, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, x, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, x, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, x, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, x, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, x, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, x, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, x, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, y, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, y, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, y, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, y, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, y, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, y, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, y, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, y, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, y, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, z, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, z, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, z, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, z, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, z, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, z, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, z, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, z, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, y, z, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, x, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, x, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, x, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, x, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, x, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, x, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, x, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, x, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, x, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, y, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, y, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, y, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, y, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, y, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, y, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, y, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, y, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, y, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, z, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, z, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, z, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, z, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, z, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, z, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, z, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, z, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec3, z, z, z, z)

IMPLEMENT_UINT_REFERENCE(uvec4, x)
IMPLEMENT_UINT_REFERENCE(uvec4, y)
IMPLEMENT_UINT_REFERENCE(uvec4, z)
IMPLEMENT_UINT_REFERENCE(uvec4, w)
IMPLEMENT_UVEC2_REFERENCE(uvec4, x, x)
IMPLEMENT_UVEC2_REFERENCE(uvec4, x, y)
IMPLEMENT_UVEC2_REFERENCE(uvec4, x, z)
IMPLEMENT_UVEC2_REFERENCE(uvec4, x, w)
IMPLEMENT_UVEC2_REFERENCE(uvec4, y, x)
IMPLEMENT_UVEC2_REFERENCE(uvec4, y, y)
IMPLEMENT_UVEC2_REFERENCE(uvec4, y, z)
IMPLEMENT_UVEC2_REFERENCE(uvec4, y, w)
IMPLEMENT_UVEC2_REFERENCE(uvec4, z, x)
IMPLEMENT_UVEC2_REFERENCE(uvec4, z, y)
IMPLEMENT_UVEC2_REFERENCE(uvec4, z, z)
IMPLEMENT_UVEC2_REFERENCE(uvec4, z, w)
IMPLEMENT_UVEC2_REFERENCE(uvec4, w, x)
IMPLEMENT_UVEC2_REFERENCE(uvec4, w, y)
IMPLEMENT_UVEC2_REFERENCE(uvec4, w, z)
IMPLEMENT_UVEC2_REFERENCE(uvec4, w, w)
IMPLEMENT_UVEC3_REFERENCE(uvec4, x, x, x)
IMPLEMENT_UVEC3_REFERENCE(uvec4, x, x, y)
IMPLEMENT_UVEC3_REFERENCE(uvec4, x, x, z)
IMPLEMENT_UVEC3_REFERENCE(uvec4, x, x, w)
IMPLEMENT_UVEC3_REFERENCE(uvec4, x, y, x)
IMPLEMENT_UVEC3_REFERENCE(uvec4, x, y, y)
IMPLEMENT_UVEC3_REFERENCE(uvec4, x, y, z)
IMPLEMENT_UVEC3_REFERENCE(uvec4, x, y, w)
IMPLEMENT_UVEC3_REFERENCE(uvec4, x, z, x)
IMPLEMENT_UVEC3_REFERENCE(uvec4, x, z, y)
IMPLEMENT_UVEC3_REFERENCE(uvec4, x, z, z)
IMPLEMENT_UVEC3_REFERENCE(uvec4, x, z, w)
IMPLEMENT_UVEC3_REFERENCE(uvec4, x, w, x)
IMPLEMENT_UVEC3_REFERENCE(uvec4, x, w, y)
IMPLEMENT_UVEC3_REFERENCE(uvec4, x, w, z)
IMPLEMENT_UVEC3_REFERENCE(uvec4, x, w, w)
IMPLEMENT_UVEC3_REFERENCE(uvec4, y, x, x)
IMPLEMENT_UVEC3_REFERENCE(uvec4, y, x, y)
IMPLEMENT_UVEC3_REFERENCE(uvec4, y, x, z)
IMPLEMENT_UVEC3_REFERENCE(uvec4, y, x, w)
IMPLEMENT_UVEC3_REFERENCE(uvec4, y, y, x)
IMPLEMENT_UVEC3_REFERENCE(uvec4, y, y, y)
IMPLEMENT_UVEC3_REFERENCE(uvec4, y, y, z)
IMPLEMENT_UVEC3_REFERENCE(uvec4, y, y, w)
IMPLEMENT_UVEC3_REFERENCE(uvec4, y, z, x)
IMPLEMENT_UVEC3_REFERENCE(uvec4, y, z, y)
IMPLEMENT_UVEC3_REFERENCE(uvec4, y, z, z)
IMPLEMENT_UVEC3_REFERENCE(uvec4, y, z, w)
IMPLEMENT_UVEC3_REFERENCE(uvec4, y, w, x)
IMPLEMENT_UVEC3_REFERENCE(uvec4, y, w, y)
IMPLEMENT_UVEC3_REFERENCE(uvec4, y, w, z)
IMPLEMENT_UVEC3_REFERENCE(uvec4, y, w, w)
IMPLEMENT_UVEC3_REFERENCE(uvec4, z, x, x)
IMPLEMENT_UVEC3_REFERENCE(uvec4, z, x, y)
IMPLEMENT_UVEC3_REFERENCE(uvec4, z, x, z)
IMPLEMENT_UVEC3_REFERENCE(uvec4, z, x, w)
IMPLEMENT_UVEC3_REFERENCE(uvec4, z, y, x)
IMPLEMENT_UVEC3_REFERENCE(uvec4, z, y, y)
IMPLEMENT_UVEC3_REFERENCE(uvec4, z, y, z)
IMPLEMENT_UVEC3_REFERENCE(uvec4, z, y, w)
IMPLEMENT_UVEC3_REFERENCE(uvec4, z, z, x)
IMPLEMENT_UVEC3_REFERENCE(uvec4, z, z, y)
IMPLEMENT_UVEC3_REFERENCE(uvec4, z, z, z)
IMPLEMENT_UVEC3_REFERENCE(uvec4, z, z, w)
IMPLEMENT_UVEC3_REFERENCE(uvec4, z, w, x)
IMPLEMENT_UVEC3_REFERENCE(uvec4, z, w, y)
IMPLEMENT_UVEC3_REFERENCE(uvec4, z, w, z)
IMPLEMENT_UVEC3_REFERENCE(uvec4, z, w, w)
IMPLEMENT_UVEC3_REFERENCE(uvec4, w, x, x)
IMPLEMENT_UVEC3_REFERENCE(uvec4, w, x, y)
IMPLEMENT_UVEC3_REFERENCE(uvec4, w, x, z)
IMPLEMENT_UVEC3_REFERENCE(uvec4, w, x, w)
IMPLEMENT_UVEC3_REFERENCE(uvec4, w, y, x)
IMPLEMENT_UVEC3_REFERENCE(uvec4, w, y, y)
IMPLEMENT_UVEC3_REFERENCE(uvec4, w, y, z)
IMPLEMENT_UVEC3_REFERENCE(uvec4, w, y, w)
IMPLEMENT_UVEC3_REFERENCE(uvec4, w, z, x)
IMPLEMENT_UVEC3_REFERENCE(uvec4, w, z, y)
IMPLEMENT_UVEC3_REFERENCE(uvec4, w, z, z)
IMPLEMENT_UVEC3_REFERENCE(uvec4, w, z, w)
IMPLEMENT_UVEC3_REFERENCE(uvec4, w, w, x)
IMPLEMENT_UVEC3_REFERENCE(uvec4, w, w, y)
IMPLEMENT_UVEC3_REFERENCE(uvec4, w, w, z)
IMPLEMENT_UVEC3_REFERENCE(uvec4, w, w, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, x, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, x, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, x, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, x, x, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, x, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, x, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, x, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, x, y, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, x, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, x, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, x, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, x, z, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, x, w, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, x, w, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, x, w, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, x, w, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, y, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, y, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, y, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, y, x, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, y, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, y, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, y, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, y, y, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, y, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, y, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, y, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, y, z, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, y, w, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, y, w, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, y, w, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, y, w, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, z, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, z, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, z, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, z, x, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, z, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, z, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, z, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, z, y, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, z, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, z, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, z, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, z, z, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, z, w, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, z, w, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, z, w, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, z, w, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, w, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, w, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, w, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, w, x, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, w, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, w, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, w, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, w, y, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, w, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, w, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, w, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, w, z, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, w, w, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, w, w, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, w, w, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, x, w, w, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, x, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, x, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, x, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, x, x, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, x, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, x, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, x, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, x, y, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, x, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, x, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, x, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, x, z, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, x, w, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, x, w, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, x, w, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, x, w, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, y, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, y, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, y, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, y, x, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, y, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, y, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, y, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, y, y, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, y, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, y, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, y, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, y, z, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, y, w, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, y, w, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, y, w, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, y, w, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, z, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, z, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, z, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, z, x, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, z, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, z, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, z, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, z, y, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, z, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, z, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, z, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, z, z, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, z, w, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, z, w, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, z, w, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, z, w, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, w, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, w, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, w, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, w, x, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, w, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, w, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, w, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, w, y, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, w, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, w, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, w, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, w, z, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, w, w, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, w, w, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, w, w, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, y, w, w, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, x, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, x, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, x, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, x, x, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, x, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, x, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, x, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, x, y, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, x, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, x, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, x, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, x, z, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, x, w, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, x, w, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, x, w, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, x, w, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, y, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, y, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, y, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, y, x, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, y, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, y, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, y, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, y, y, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, y, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, y, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, y, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, y, z, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, y, w, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, y, w, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, y, w, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, y, w, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, z, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, z, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, z, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, z, x, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, z, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, z, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, z, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, z, y, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, z, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, z, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, z, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, z, z, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, z, w, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, z, w, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, z, w, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, z, w, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, w, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, w, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, w, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, w, x, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, w, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, w, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, w, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, w, y, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, w, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, w, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, w, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, w, z, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, w, w, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, w, w, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, w, w, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, z, w, w, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, x, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, x, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, x, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, x, x, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, x, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, x, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, x, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, x, y, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, x, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, x, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, x, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, x, z, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, x, w, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, x, w, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, x, w, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, x, w, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, y, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, y, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, y, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, y, x, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, y, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, y, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, y, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, y, y, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, y, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, y, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, y, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, y, z, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, y, w, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, y, w, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, y, w, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, y, w, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, z, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, z, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, z, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, z, x, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, z, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, z, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, z, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, z, y, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, z, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, z, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, z, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, z, z, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, z, w, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, z, w, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, z, w, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, z, w, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, w, x, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, w, x, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, w, x, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, w, x, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, w, y, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, w, y, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, w, y, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, w, y, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, w, z, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, w, z, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, w, z, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, w, z, w)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, w, w, x)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, w, w, y)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, w, w, z)
IMPLEMENT_UVEC4_REFERENCE(uvec4, w, w, w, w)
#endif

uvec2::uvec2()
#ifdef ENABLE_SWIZZLE
: parent(this)
#endif
{}

uvec2::uvec2(uint value) : x(value), y(value)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

uvec2::uvec2(uint _x, uint _y) : x(_x), y(_y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

uvec2::uvec2(const uvec3& v) : x(v.x), y(v.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

uvec2::uvec2(const uvec4& v) : x(v.x), y(v.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

uvec2::uvec2(const uvec2& v) : x(v.x), y(v.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

uvec2::uvec2(uvec2&& v) : x(move(v.x)), y(move(v.y))
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}


uint* uvec2::data()const
{
	return (uint*)this;
}

uvec2& uvec2::operator =(const uvec2& v)
{
	if(this != &v)
	{
		x = v.x;
		y = v.y;
	}

	return *this;
}

uvec2& uvec2::operator =(uvec2&& v)
{
	if(this != &v)
	{
		x = move(v.x);
		y = move(v.y);
	}

	return *this;
}

uvec2::operator dvec2()const
{
	return dvec2(x, y);
}

uvec2::operator vec2()const
{
	return vec2(x, y);
}

uvec2::operator ivec2()const
{
	return ivec2(x, y);
}

uvec2::operator bvec2()const
{
	return bvec2(x, y);
}

uint& uvec2::operator [](uint i)
{
	switch(i)
	{
		case 0: return x;
		case 1: return y;
		default:
		{
			throw glass::IndexError(i, 0, 1);\
		}
	}
}

uint uvec2::operator [](uint i)const
{
	switch(i)
	{
		case 0: return x;
		case 1: return y;
		default:
		{
			throw glass::IndexError(i, 0, 1);\
		}
	}
}

uvec2 uvec2::operator +()const
{
	return *this;
}

uvec2 uvec2::operator -()const
{
	return uvec2(-x, -y);
}

uvec2& uvec2::operator +=(const uvec2& v)
{
	x += v.x;
	y += v.y;

	return *this;
}

uvec2& uvec2::operator -=(const uvec2& v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}

uvec2& uvec2::operator *=(const uvec2& v)
{
	x *= v.x;
	y *= v.y;

	return *this;
}

uvec2& uvec2::operator /=(const uvec2& v)
{
	if(v.x == 0)
	{
		throw glass::ZeroDivisionError("Error when ivec2 v1 /= ivec2 v2: v2.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw glass::ZeroDivisionError("Error when ivec2 v1 /= ivec2 v2: v2.y is 0 now.");
	}

	x /= v.x;
	y /= v.y;

	return *this;
}

uvec2& uvec2::operator +=(uint value)
{
	x += value;
	y += value;

	return *this;
}

uvec2& uvec2::operator -=(uint value)
{
	x -= value;
	y -= value;

	return *this;
}

uvec2& uvec2::operator *=(uint value)
{
	x *= value;
	y *= value;

	return *this;
}

uvec2& uvec2::operator /=(uint value)
{
	if(value == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec2 v /= float value: value is 0 now.");
	}

	x /= value;
	y /= value;

	return *this;
}

uvec2& uvec2::operator %=(uint value)
{
	if(value == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec2 v %= float value: value is 0 now.");
	}

	x %= value;
	y %= value;

	return *this;
}

uvec2 operator +(const uvec2& v1, const uvec2& v2)
{
	return uvec2(v1.x + v2.x, v1.y + v2.y);
}

uvec2 operator -(const uvec2& v1, const uvec2& v2)
{
	return uvec2(v1.x - v2.x, v1.y - v2.y);
}

uvec2 operator *(const uvec2& v1, const uvec2& v2)
{
	return uvec2(v1.x * v2.x, v1.y * v2.y);
}

uvec2 operator /(const uvec2& v1, const uvec2& v2)
{
	if(v2.x == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec2 v1 / uvec2 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec2 v1 / uvec2 v2: v2.y is 0 now.");
	}
	return uvec2(v1.x / v2.x, v1.y / v2.y);
}

uvec2 operator %(const uvec2& v1, const uvec2& v2)
{
	if(v2.x == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec2 v1 %% uvec2 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec2 v1 %% uvec2 v2: v2.y is 0 now.");
	}
	return uvec2(v1.x % v2.x, v1.y % v2.y);
}

uvec2 operator +(const uvec2& v1, const ivec2& v2)
{
	return uvec2(v1.x + v2.x, v1.y + v2.y);
}

uvec2 operator -(const uvec2& v1, const ivec2& v2)
{
	return uvec2(v1.x - v2.x, v1.y - v2.y);
}

uvec2 operator *(const uvec2& v1, const ivec2& v2)
{
	return uvec2(v1.x * v2.x, v1.y * v2.y);
}

uvec2 operator /(const uvec2& v1, const ivec2& v2)
{
	if(v2.x == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec2 v1 / ivec2 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec2 v1 / ivec2 v2: v2.y is 0 now.");
	}
	return uvec2(v1.x / v2.x, v1.y / v2.y);
}

uvec2 operator %(const uvec2& v1, const ivec2& v2)
{
	if(v2.x == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec2 v1 %% ivec2 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec2 v1 %% ivec2 v2: v2.y is 0 now.");
	}
	return uvec2(v1.x % v2.x, v1.y % v2.y);
}

uvec2 operator +(const ivec2& v1, const uvec2& v2)
{
	return uvec2(v1.x + v2.x, v1.y + v2.y);
}

uvec2 operator -(const ivec2& v1, const uvec2& v2)
{
	return uvec2(v1.x - v2.x, v1.y - v2.y);
}

uvec2 operator *(const ivec2& v1, const uvec2& v2)
{
	return uvec2(v1.x * v2.x, v1.y * v2.y);
}

uvec2 operator /(const ivec2& v1, const uvec2& v2)
{
	if(v2.x == 0)
	{
		throw glass::ZeroDivisionError("Error when ivec2 v1 / uvec2 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw glass::ZeroDivisionError("Error when ivec2 v1 / uvec2 v2: v2.y is 0 now.");
	}
	return uvec2(v1.x / v2.x, v1.y / v2.y);
}

uvec2 operator %(const ivec2& v1, const uvec2& v2)
{
	if(v2.x == 0)
	{
		throw glass::ZeroDivisionError("Error when ivec2 v1 %% uvec2 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw glass::ZeroDivisionError("Error when ivec2 v1 %% uvec2 v2: v2.y is 0 now.");
	}
	return uvec2(v1.x % v2.x, v1.y % v2.y);
}

dvec2 operator +(const uvec2& v, double value)
{
	return dvec2(v.x + value, v.y + value);
}

dvec2 operator -(const uvec2& v, double value)
{
	return dvec2(v.x - value, v.y - value);
}

dvec2 operator *(const uvec2& v, double value)
{
	return dvec2(v.x * value, v.y * value);
}

dvec2 operator /(const uvec2& v, double value)
{
	if(is_zero(value))
	{
		throw glass::ZeroDivisionError("Error when uvec2 v / double value: value is 0 now.");
	}
	return dvec2(v.x / value, v.y / value);
}

dvec2 operator +(double value, const uvec2& v)
{
	return dvec2(value + v.x, value + v.y);
}

dvec2 operator -(double value, const uvec2& v)
{
	return dvec2(value - v.x, value - v.y);
}

dvec2 operator *(double value, const uvec2& v)
{
	return dvec2(value * v.x, value * v.y);
}

dvec2 operator /(double value, const uvec2& v)
{
	if(v.x == 0)
	{
		throw glass::ZeroDivisionError("Error when double value / uvec2 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw glass::ZeroDivisionError("Error when double value / uvec2 v: v.y is 0 now.");
	}
	return dvec2(value / v.x, value / v.y);
}

vec2 operator +(const uvec2& v, float value)
{
	return vec2(v.x + value, v.y + value);
}

vec2 operator -(const uvec2& v, float value)
{
	return vec2(v.x - value, v.y - value);
}

vec2 operator *(const uvec2& v, float value)
{
	return vec2(v.x * value, v.y * value);
}

vec2 operator /(const uvec2& v, float value)
{
	if(is_zero(value))
	{
		throw glass::ZeroDivisionError("Error when vec2 v / float value: value is 0 now.");
	}
	return vec2(v.x / value, v.y / value);
}

vec2 operator +(float value, const uvec2& v)
{
	return vec2(value + v.x, value + v.y);
}

vec2 operator -(float value, const uvec2& v)
{
	return vec2(value - v.x, value - v.y);
}

vec2 operator *(float value, const uvec2& v)
{
	return vec2(value * v.x, value * v.y);
}

vec2 operator /(float value, const uvec2& v)
{
	if(v.x == 0)
	{
		throw glass::ZeroDivisionError("Error when float value / vec2 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw glass::ZeroDivisionError("Error when float value / vec2 v: v.y is 0 now.");
	}
	return vec2(value / v.x, value / v.y);
}

uvec2 operator +(const uvec2& v, uint value)
{
	return uvec2(v.x + value, v.y + value);
}

uvec2 operator -(const uvec2& v, uint value)
{
	return uvec2(v.x - value, v.y - value);
}

uvec2 operator *(const uvec2& v, uint value)
{
	return uvec2(v.x * value, v.y * value);
}

uvec2 operator /(const uvec2& v, uint value)
{
	if(value == 0)
	{
		throw glass::ZeroDivisionError("Error when vec2 v / uint value: value is 0 now.");
	}
	return uvec2(v.x / value, v.y / value);
}

uvec2 operator %(const uvec2& v, uint value)
{
	if(value == 0)
	{
		throw glass::ZeroDivisionError("Error when vec2 v %% uint value: value is 0 now.");
	}
	return uvec2(v.x % value, v.y % value);
}

uvec2 operator +(uint value, const uvec2& v)
{
	return uvec2(value + v.x, value + v.y);
}

uvec2 operator -(uint value, const uvec2& v)
{
	return uvec2(value - v.x, value - v.y);
}

uvec2 operator *(uint value, const uvec2& v)
{
	return uvec2(value * v.x, value * v.y);
}

uvec2 operator /(uint value, const uvec2& v)
{
	if(v.x == 0)
	{
		throw glass::ZeroDivisionError("Error when uint value / uvec2 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw glass::ZeroDivisionError("Error when uint value / uvec2 v: v.y is 0 now.");
	}
	return uvec2(value / v.x, value / v.y);
}

uvec2 operator %(uint value, const uvec2& v)
{
	if(v.x == 0)
	{
		throw glass::ZeroDivisionError("Error when uint value %% uvec2 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw glass::ZeroDivisionError("Error when uint value %% uvec2 v: v.y is 0 now.");
	}
	return uvec2(value % v.x, value % v.y);
}

uvec2 operator +(const uvec2& v, int value)
{
	return uvec2(v.x + value, v.y + value);
}

uvec2 operator -(const uvec2& v, int value)
{
	return uvec2(v.x - value, v.y - value);
}

uvec2 operator *(const uvec2& v, int value)
{
	return uvec2(v.x * value, v.y * value);
}

uvec2 operator /(const uvec2& v, int value)
{
	if(value == 0)
	{
		throw glass::ZeroDivisionError("Error when vec2 v / int value: value is 0 now.");
	}
	return uvec2(v.x / value, v.y / value);
}

uvec2 operator %(const uvec2& v, int value)
{
	if(value == 0)
	{
		throw glass::ZeroDivisionError("Error when vec2 v %% int value: value is 0 now.");
	}
	return uvec2(v.x % value, v.y % value);
}

uvec2 operator +(int value, const uvec2& v)
{
	return uvec2(value + v.x, value + v.y);
}

uvec2 operator -(int value, const uvec2& v)
{
	return uvec2(value - v.x, value - v.y);
}

uvec2 operator *(int value, const uvec2& v)
{
	return uvec2(value * v.x, value * v.y);
}

uvec2 operator /(int value, const uvec2& v)
{
	if(v.x == 0)
	{
		throw glass::ZeroDivisionError("Error when int value / uvec2 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw glass::ZeroDivisionError("Error when int value / uvec2 v: v.y is 0 now.");
	}
	return uvec2(value / v.x, value / v.y);
}

uvec2 operator %(int value, const uvec2& v)
{
	if(v.x == 0)
	{
		throw glass::ZeroDivisionError("Error when int value %% uvec2 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw glass::ZeroDivisionError("Error when int value %% uvec2 v: v.y is 0 now.");
	}
	return uvec2(value % v.x, value % v.y);
}

ostream& operator <<(ostream& out, const uvec2& v)
{
	out << "uvec2(" << v.x << ", " << v.y << ")";
	return out;
}

uvec3::uvec3()
#ifdef ENABLE_SWIZZLE
: parent(this)
#endif
{}

uvec3::uvec3(uint value) : x(value), y(value), z(value)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

uvec3::uvec3(uint _x, uint _y, uint _z) : x(_x), y(_y), z(_z)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

uvec3::uvec3(const uvec2& v, uint _z) : x(v.x), y(v.y), z(_z)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

uvec3::uvec3(uint _x, const uvec2& v) : x(_x), y(v.x), z(v.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

uvec3::uvec3(const uvec4& v) : x(v.x), y(v.y), z(v.z)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

uvec3::uvec3(const uvec3& v) : x(v.x), y(v.y), z(v.z)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

uvec3::uvec3(uvec3&& v) : x(move(v.x)), y(move(v.y)), z(move(v.z))
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}


uint* uvec3::data()const
{
	return (uint*)this;
}

uvec3& uvec3::operator =(const uvec3& v)
{
	if(this != &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	return *this;
}

uvec3& uvec3::operator =(uvec3&& v)
{
	if(this != &v)
	{
		x = move(v.x);
		y = move(v.y);
		z = move(v.z);
	}

	return *this;
}

uvec3::operator dvec3()const
{
	return dvec3(x, y, z);
}

uvec3::operator vec3()const
{
	return vec3(x, y, z);
}

uvec3::operator ivec3()const
{
	return ivec3(x, y, z);
}

uvec3::operator bvec3()const
{
	return bvec3(x, y, z);
}

uint& uvec3::operator [](uint i)
{
	switch(i)
	{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default:
		{
			throw glass::IndexError(i, 0, 2);\
		}
	}
}

uint uvec3::operator [](uint i)const
{
	switch(i)
	{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default:
		{
			throw glass::IndexError(i, 0, 2);\
		}
	}
}

uvec3 uvec3::operator +()const
{
	return *this;
}

uvec3 uvec3::operator -()const
{
	return uvec3(-x, -y, -z);
}

uvec3& uvec3::operator +=(const uvec3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

uvec3& uvec3::operator -=(const uvec3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

uvec3& uvec3::operator *=(const uvec3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}

uvec3& uvec3::operator /=(const uvec3& v)
{
	if(v.x == 0)
	{
		throw glass::ZeroDivisionError("Error using uvec3 v1 /= uvec3 v2:\nv2.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw glass::ZeroDivisionError("Error using uvec3 v1 /= uvec3 v2:\nv2.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw glass::ZeroDivisionError("Error using uvec3 v1 /= uvec3 v2:\nv2.z is 0 now.");
	}

	x /= v.x;
	y /= v.y;
	z /= v.z;

	return *this;
}

uvec3& uvec3::operator %=(const uvec3& v)
{
	if(v.x == 0)
	{
		throw glass::ZeroDivisionError("Error using uvec3 v1 %= uvec3 v2:\nv2.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw glass::ZeroDivisionError("Error using uvec3 v1 %= uvec3 v2:\nv2.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw glass::ZeroDivisionError("Error using uvec3 v1 %= uvec3 v2:\nv2.z is 0 now.");
	}

	x %= v.x;
	y %= v.y;
	z %= v.z;

	return *this;
}

uvec3& uvec3::operator +=(uint value)
{
	x += value;
	y += value;
	z += value;

	return *this;
}

uvec3& uvec3::operator -=(uint value)
{
	x -= value;
	y -= value;
	z -= value;

	return *this;
}

uvec3& uvec3::operator *=(uint value)
{
	x *= value;
	y *= value;
	z *= value;

	return *this;
}

uvec3& uvec3::operator /=(uint value)
{
	if(value == 0)
	{
		throw glass::ZeroDivisionError("Error using vec3 v /= int value:\nvalue is 0 now.");
	}
	x /= value;
	y /= value;
	z /= value;

	return *this;
}

uvec3& uvec3::operator %=(uint value)
{
	if(value == 0)
	{
		throw glass::ZeroDivisionError("Error using uvec3 v %= int value:\nvalue is 0 now.");
	}
	x %= value;
	y %= value;
	z %= value;

	return *this;
}

uvec3 operator +(const uvec3& v1, const uvec3& v2)
{
	return uvec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

uvec3 operator -(const uvec3& v1, const uvec3& v2)
{
	return uvec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

uvec3 operator *(const uvec3& v1, const uvec3& v2)
{
	return uvec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

uvec3 operator /(const uvec3& v1, const uvec3& v2)
{
	if(v2.x == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec3 v1 / uvec3 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec3 v1 / uvec3 v2: v2.y is 0 now.");
	}
	if(v2.z == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec3 v1 / uvec3 v2: v2.z is 0 now.");
	}
	return uvec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

uvec3 operator %(const uvec3& v1, const uvec3& v2)
{
	if(v2.x == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec3 v1 %% uvec3 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec3 v1 %% uvec3 v2: v2.y is 0 now.");
	}
	if(v2.z == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec3 v1 %% uvec3 v2: v2.z is 0 now.");
	}
	return uvec3(v1.x % v2.x, v1.y % v2.y, v1.z % v2.z);
}

uvec3 operator +(const uvec3& v1, const ivec3& v2)
{
	return uvec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

uvec3 operator -(const uvec3& v1, const ivec3& v2)
{
	return uvec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

uvec3 operator *(const uvec3& v1, const ivec3& v2)
{
	return uvec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

uvec3 operator /(const uvec3& v1, const ivec3& v2)
{
	if(v2.x == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec3 v1 / uvec3 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec3 v1 / uvec3 v2: v2.y is 0 now.");
	}
	if(v2.z == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec3 v1 / uvec3 v2: v2.z is 0 now.");
	}
	return uvec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

uvec3 operator %(const uvec3& v1, const ivec3& v2)
{
	if(v2.x == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec3 v1 %% uvec3 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec3 v1 %% uvec3 v2: v2.y is 0 now.");
	}
	if(v2.z == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec3 v1 %% uvec3 v2: v2.z is 0 now.");
	}
	return uvec3(v1.x % v2.x, v1.y % v2.y, v1.z % v2.z);
}

uvec3 operator +(const ivec3& v1, const uvec3& v2)
{
	return uvec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

uvec3 operator -(const ivec3& v1, const uvec3& v2)
{
	return uvec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

uvec3 operator *(const ivec3& v1, const uvec3& v2)
{
	return uvec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

uvec3 operator /(const ivec3& v1, const uvec3& v2)
{
	if(v2.x == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec3 v1 / uvec3 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec3 v1 / uvec3 v2: v2.y is 0 now.");
	}
	if(v2.z == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec3 v1 / uvec3 v2: v2.z is 0 now.");
	}
	return uvec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

uvec3 operator %(const ivec3& v1, const uvec3& v2)
{
	if(v2.x == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec3 v1 %% uvec3 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec3 v1 %% uvec3 v2: v2.y is 0 now.");
	}
	if(v2.z == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec3 v1 %% uvec3 v2: v2.z is 0 now.");
	}
	return uvec3(v1.x % v2.x, v1.y % v2.y, v1.z % v2.z);
}

dvec3 operator +(const uvec3& v, double value)
{
	return dvec3(v.x + value, v.y + value, v.z + value);
}

dvec3 operator -(const uvec3& v, double value)
{
	return dvec3(v.x - value, v.y - value, v.z - value);
}

dvec3 operator *(const uvec3& v, double value)
{
	return dvec3(v.x * value, v.y * value, v.z * value);
}

dvec3 operator /(const uvec3& v, double value)
{
	if(is_zero(value))
	{
		throw glass::ZeroDivisionError("Error when vec3 v / double value: value is 0 now.");
	}
	return dvec3(v.x / value, v.y / value, v.z / value);
}

dvec3 operator +(double value, const uvec3& v)
{
	return dvec3(value + v.x, value + v.y, value + v.z);
}

dvec3 operator -(double value, const uvec3& v)
{
	return dvec3(value - v.x, value - v.y, value - v.z);
}

dvec3 operator *(double value, const uvec3& v)
{
	return dvec3(value * v.x, value * v.y, value * v.z);
}

dvec3 operator /(double value, const uvec3& v)
{
	if(v.x == 0)
	{
		throw glass::ZeroDivisionError("Error when double value / uvec3 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw glass::ZeroDivisionError("Error when double value / uvec3 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw glass::ZeroDivisionError("Error when double value / uvec3 v: v.z is 0 now.");
	}
	return dvec3(value / v.x, value / v.y, value / v.z);
}

vec3 operator +(const uvec3& v, float value)
{
	return vec3(v.x + value, v.y + value, v.z + value);
}

vec3 operator -(const uvec3& v, float value)
{
	return vec3(v.x - value, v.y - value, v.z - value);
}

vec3 operator *(const uvec3& v, float value)
{
	return vec3(v.x * value, v.y * value, v.z * value);
}

vec3 operator /(const uvec3& v, float value)
{
	if(is_zero(value))
	{
		throw glass::ZeroDivisionError("Error when vec3 v / float value: value is 0 now.");
	}
	return vec3(v.x / value, v.y / value, v.z / value);
}

vec3 operator +(float value, const uvec3& v)
{
	return vec3(value + v.x, value + v.y, value + v.z);
}

vec3 operator -(float value, const uvec3& v)
{
	return vec3(value - v.x, value - v.y, value - v.z);
}

vec3 operator *(float value, const uvec3& v)
{
	return vec3(value * v.x, value * v.y, value * v.z);
}

vec3 operator /(float value, const uvec3& v)
{
	if(v.x == 0)
	{
		throw glass::ZeroDivisionError("Error when float value / uvec3 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw glass::ZeroDivisionError("Error when float value / uvec3 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw glass::ZeroDivisionError("Error when float value / uvec3 v: v.z is 0 now.");
	}
	return vec3(value / v.x, value / v.y, value / v.z);
}

uvec3 operator +(const uvec3& v, uint value)
{
	return uvec3(v.x + value, v.y + value, v.z + value);
}

uvec3 operator -(const uvec3& v, uint value)
{
	return uvec3(v.x - value, v.y - value, v.z - value);
}

uvec3 operator *(const uvec3& v, uint value)
{
	return uvec3(v.x * value, v.y * value, v.z * value);
}

uvec3 operator /(const uvec3& v, uint value)
{
	if(value == 0)
	{
		throw glass::ZeroDivisionError("Error when vec3 v / uint value: value is 0 now.");
	}
	return uvec3(v.x / value, v.y / value, v.z / value);
}

uvec3 operator %(const uvec3& v, uint value)
{
	if(value == 0)
	{
		throw glass::ZeroDivisionError("Error when vec3 v %% uint value: value is 0 now.");
	}
	return uvec3(v.x % value, v.y % value, v.z % value);
}

uvec3 operator +(uint value, const uvec3& v)
{
	return uvec3(value + v.x, value + v.y, value + v.z);
}

uvec3 operator -(uint value, const uvec3& v)
{
	return uvec3(value - v.x, value - v.y, value - v.z);
}

uvec3 operator *(uint value, const uvec3& v)
{
	return uvec3(value * v.x, value * v.y, value * v.z);
}

uvec3 operator /(uint value, const uvec3& v)
{
	if(v.x == 0)
	{
		throw glass::ZeroDivisionError("Error when uint value / uvec3 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw glass::ZeroDivisionError("Error when uint value / uvec3 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw glass::ZeroDivisionError("Error when uint value / uvec3 v: v.z is 0 now.");
	}
	return uvec3(value / v.x, value / v.y, value / v.z);
}

uvec3 operator %(uint value, const uvec3& v)
{
	if(v.x == 0)
	{
		throw glass::ZeroDivisionError("Error when uint value %% uvec3 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw glass::ZeroDivisionError("Error when uint value %% uvec3 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw glass::ZeroDivisionError("Error when uint value %% uvec3 v: v.z is 0 now.");
	}
	return uvec3(value % v.x, value % v.y, value % v.z);
}

uvec3 operator +(const uvec3& v, int value)
{
	return uvec3(v.x + value, v.y + value, v.z + value);
}

uvec3 operator -(const uvec3& v, int value)
{
	return uvec3(v.x - value, v.y - value, v.z - value);
}

uvec3 operator *(const uvec3& v, int value)
{
	return uvec3(v.x * value, v.y * value, v.z * value);
}

uvec3 operator /(const uvec3& v, int value)
{
	if(value == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec3 v / int value: value is 0 now.");
	}
	return uvec3(v.x / value, v.y / value, v.z / value);
}

uvec3 operator %(const uvec3& v, int value)
{
	if(value == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec3 v %% int value: value is 0 now.");
	}
	return uvec3(v.x % value, v.y % value, v.z % value);
}

uvec3 operator +(int value, const uvec3& v)
{
	return uvec3(value + v.x, value + v.y, value + v.z);
}

uvec3 operator -(int value, const uvec3& v)
{
	return uvec3(value - v.x, value - v.y, value - v.z);
}

uvec3 operator *(int value, const uvec3& v)
{
	return uvec3(value * v.x, value * v.y, value * v.z);
}

uvec3 operator /(int value, const uvec3& v)
{
	if(v.x == 0)
	{
		throw glass::ZeroDivisionError("Error when int value / uvec3 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw glass::ZeroDivisionError("Error when int value / uvec3 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw glass::ZeroDivisionError("Error when int value / uvec3 v: v.z is 0 now.");
	}
	return uvec3(value / v.x, value / v.y, value / v.z);
}

uvec3 operator %(int value, const uvec3& v)
{
	if(v.x == 0)
	{
		throw glass::ZeroDivisionError("Error when int value %% uvec3 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw glass::ZeroDivisionError("Error when int value %% uvec3 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw glass::ZeroDivisionError("Error when int value %% uvec3 v: v.z is 0 now.");
	}
	return uvec3(value % v.x, value % v.y, value % v.z);
}

ostream& operator <<(ostream& out, const uvec3& v)
{
	out << "uvec3(" << v.x << ", " << v.y << ", " << v.z << ")";
	return out;
}

uvec4::uvec4()
#ifdef ENABLE_SWIZZLE
: parent(this)
#endif
{}

uvec4::uvec4(uint value) : x(value), y(value), z(value), w(value)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

uvec4::uvec4(uint _x, uint _y, uint _z, uint _w) : x(_x), y(_y), z(_z), w(_w)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

uvec4::uvec4(const uvec2& v, uint _z, uint _w) : x(v.x), y(v.y), z(_z), w(_w)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

uvec4::uvec4(uint _x, const uvec2& v, uint _w) : x(_x), y(v.x), z(v.y), w(_w)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

uvec4::uvec4(uint _x, uint _y, const uvec2& v) : x(_x), y(_y), z(v.x), w(v.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

uvec4::uvec4(const uvec2& v1, const uvec2& v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

uvec4::uvec4(const uvec3& v, uint _w) : x(v.x), y(v.y), z(v.z), w(_w)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

uvec4::uvec4(uint _x, const uvec3& v) : x(_x), y(v.x), z(v.y), w(v.z)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

uvec4::uvec4(const uvec4& v) : x(v.x), y(v.y), z(v.z), w(v.w)
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}

uvec4::uvec4(uvec4&& v) : x(move(v.x)), y(move(v.y)), z(move(v.z)), w(move(v.w))
#ifdef ENABLE_SWIZZLE
, parent(this)
#endif
{}


uint* uvec4::data()const
{
	return (uint*)this;
}

uvec4& uvec4::operator =(const uvec4& v)
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

uvec4& uvec4::operator =(uvec4&& v)
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

uvec4::operator dvec4()const
{
	return dvec4(x, y, z, w);
}

uvec4::operator vec4()const
{
	return uvec4(x, y, z, w);
}

uvec4::operator ivec4()const
{
	return ivec4(x, y, z, w);
}

uvec4::operator bvec4()const
{
	return bvec4(x, y, z, w);
}

uint& uvec4::operator [](uint i)
{
	switch(i)
	{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		default:
		{
			throw glass::IndexError(i, 0, 3);
		}
	}
}

uint uvec4::operator [](uint i)const
{
	switch(i)
	{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		default:
		{
			throw glass::IndexError(i, 0, 3);
		}
	}
}

uvec4 uvec4::operator +()const
{
	return *this;
}

uvec4 uvec4::operator -()const
{
	return uvec4(-x, -y, -z, -w);
}

uvec4& uvec4::operator +=(const uvec4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;

	return *this;
}

uvec4& uvec4::operator -=(const uvec4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;

	return *this;
}

uvec4& uvec4::operator *=(const uvec4& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;

	return *this;
}

uvec4& uvec4::operator /=(const uvec4& v)
{
	if(v.x == 0)
	{
		throw glass::ZeroDivisionError("Error using uvec4 v1 /= uvec4 v2:\nv2.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw glass::ZeroDivisionError("Error using uvec4 v1 /= uvec4 v2:\nv2.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw glass::ZeroDivisionError("Error using uvec4 v1 /= uvec4 v2:\nv2.z is 0 now.");
	}
	if(v.w == 0)
	{
		throw glass::ZeroDivisionError("Error using uvec4 v1 /= uvec4 v2:\nv2.w is 0 now.");
	}

	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;

	return *this;
}

uvec4& uvec4::operator %=(const uvec4& v)
{
	if(v.x == 0)
	{
		throw glass::ZeroDivisionError("Error using uvec4 v1 %= uvec4 v2:\nv2.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw glass::ZeroDivisionError("Error using uvec4 v1 %= uvec4 v2:\nv2.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw glass::ZeroDivisionError("Error using uvec4 v1 %= uvec4 v2:\nv2.z is 0 now.");
	}
	if(v.w == 0)
	{
		throw glass::ZeroDivisionError("Error using uvec4 v1 %= uvec4 v2:\nv2.w is 0 now.");
	}

	x %= v.x;
	y %= v.y;
	z %= v.z;
	w %= v.w;

	return *this;
}

uvec4& uvec4::operator +=(uint value)
{
	x += value;
	y += value;
	z += value;
	w += value;

	return *this;
}

uvec4& uvec4::operator -=(uint value)
{
	x -= value;
	y -= value;
	z -= value;
	w -= value;

	return *this;
}

uvec4& uvec4::operator *=(uint value)
{
	x *= value;
	y *= value;
	z *= value;
	w *= value;

	return *this;
}

uvec4& uvec4::operator /=(uint value)
{
	if(value == 0)
	{
		throw glass::ZeroDivisionError("Error using uvec4 v1 /= int value:\nvalue is 0 now.");
	}

	x /= value;
	y /= value;
	z /= value;
	w /= value;

	return *this;
}

uvec4& uvec4::operator %=(uint value)
{
	if(value == 0)
	{
		throw glass::ZeroDivisionError("Error using uvec4 v1 %= int value:\nvalue is 0 now.");
	}

	x %= value;
	y %= value;
	z %= value;
	w %= value;

	return *this;
}

uvec4 operator +(const uvec4& v1, const uvec4& v2)
{
	return uvec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

uvec4 operator -(const uvec4& v1, const uvec4& v2)
{
	return uvec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

uvec4 operator *(const uvec4& v1, const uvec4& v2)
{
	return uvec4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}

uvec4 operator /(const uvec4& v1, const uvec4& v2)
{
	if(v2.x == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 / uvec4 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 / uvec4 v2: v2.y is 0 now.");
	}
	if(v2.z == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 / uvec4 v2: v2.z is 0 now.");
	}
	if(v2.w == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 / uvec4 v2: v2.w is 0 now.");
	}
	return uvec4(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
}

uvec4 operator %(const uvec4& v1, const uvec4& v2)
{
	if(v2.x == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 %% uvec4 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 %% uvec4 v2: v2.y is 0 now.");
	}
	if(v2.z == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 %% uvec4 v2: v2.z is 0 now.");
	}
	if(v2.w == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 %% uvec4 v2: v2.w is 0 now.");
	}
	return uvec4(v1.x % v2.x, v1.y % v2.y, v1.z % v2.z, v1.w % v2.w);
}

uvec4 operator +(const uvec4& v1, const ivec4& v2)
{
	return uvec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

uvec4 operator -(const uvec4& v1, const ivec4& v2)
{
	return uvec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

uvec4 operator *(const uvec4& v1, const ivec4& v2)
{
	return uvec4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}

uvec4 operator /(const uvec4& v1, const ivec4& v2)
{
	if(v2.x == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 / uvec4 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 / uvec4 v2: v2.y is 0 now.");
	}
	if(v2.z == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 / uvec4 v2: v2.z is 0 now.");
	}
	if(v2.w == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 / uvec4 v2: v2.w is 0 now.");
	}
	return uvec4(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
}

uvec4 operator %(const uvec4& v1, const ivec4& v2)
{
	if(v2.x == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 %% uvec4 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 %% uvec4 v2: v2.y is 0 now.");
	}
	if(v2.z == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 %% uvec4 v2: v2.z is 0 now.");
	}
	if(v2.w == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 %% uvec4 v2: v2.w is 0 now.");
	}
	return uvec4(v1.x % v2.x, v1.y % v2.y, v1.z % v2.z, v1.w % v2.w);
}

uvec4 operator +(const ivec4& v1, const uvec4& v2)
{
	return uvec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

uvec4 operator -(const ivec4& v1, const uvec4& v2)
{
	return uvec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

uvec4 operator *(const ivec4& v1, const uvec4& v2)
{
	return uvec4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}

uvec4 operator /(const ivec4& v1, const uvec4& v2)
{
	if(v2.x == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 / uvec4 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 / uvec4 v2: v2.y is 0 now.");
	}
	if(v2.z == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 / uvec4 v2: v2.z is 0 now.");
	}
	if(v2.w == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 / uvec4 v2: v2.w is 0 now.");
	}
	return uvec4(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
}

uvec4 operator %(const ivec4& v1, const uvec4& v2)
{
	if(v2.x == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 %% uvec4 v2: v2.x is 0 now.");
	}
	if(v2.y == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 %% uvec4 v2: v2.y is 0 now.");
	}
	if(v2.z == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 %% uvec4 v2: v2.z is 0 now.");
	}
	if(v2.w == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v1 %% uvec4 v2: v2.w is 0 now.");
	}
	return uvec4(v1.x % v2.x, v1.y % v2.y, v1.z % v2.z, v1.w % v2.w);
}

dvec4 operator +(const uvec4& v, double value)
{
	return dvec4(v.x + value, v.y + value, v.z + value, v.w + value);
}

dvec4 operator -(const uvec4& v, double value)
{
	return dvec4(v.x - value, v.y - value, v.z - value, v.w - value);
}

dvec4 operator *(const uvec4& v, double value)
{
	return dvec4(v.x * value, v.y * value, v.z * value, v.w * value);
}

dvec4 operator /(const uvec4& v, double value)
{
	if(is_zero(value))
	{
		throw glass::ZeroDivisionError("Error when uvec4 v / double value: value is 0 now.");
	}
	return dvec4(v.x / value, v.y / value, v.z / value, v.w / value);
}

dvec4 operator +(double value, const uvec4& v)
{
	return dvec4(value + v.x, value + v.y, value + v.z, value + v.w);
}

dvec4 operator -(double value, const uvec4& v)
{
	return dvec4(value - v.x, value - v.y, value - v.z, value - v.w);
}

dvec4 operator *(double value, const uvec4& v)
{
	return dvec4(value * v.x, value * v.y, value * v.z, value * v.w);
}

dvec4 operator /(double value, const uvec4& v)
{
	if(v.x == 0)
	{
		throw glass::ZeroDivisionError("Error when double value / uvec4 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw glass::ZeroDivisionError("Error when double value / uvec4 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw glass::ZeroDivisionError("Error when double value / uvec4 v: v.z is 0 now.");
	}
	if(v.w == 0)
	{
		throw glass::ZeroDivisionError("Error when double value / uvec4 v: v.w is 0 now.");
	}
	return dvec4(value / v.x, value / v.y, value / v.z, value / v.w);
}

vec4 operator +(const uvec4& v, float value)
{
	return vec4(v.x + value, v.y + value, v.z + value, v.w + value);
}

vec4 operator -(const uvec4& v, float value)
{
	return vec4(v.x - value, v.y - value, v.z - value, v.w - value);
}

vec4 operator *(const uvec4& v, float value)
{
	return vec4(v.x * value, v.y * value, v.z * value, v.w * value);
}

vec4 operator /(const uvec4& v, float value)
{
	if(is_zero(value))
	{
		throw glass::ZeroDivisionError("Error when uvec4 v / float value: value is 0 now.");
	}
	return vec4(v.x / value, v.y / value, v.z / value, v.w / value);
}

vec4 operator +(float value, const uvec4& v)
{
	return vec4(value + v.x, value + v.y, value + v.z, value + v.w);
}

vec4 operator -(float value, const uvec4& v)
{
	return vec4(value - v.x, value - v.y, value - v.z, value - v.w);
}

vec4 operator *(float value, const uvec4& v)
{
	return vec4(value * v.x, value * v.y, value * v.z, value * v.w);
}

vec4 operator /(float value, const uvec4& v)
{
	if(v.x == 0)
	{
		throw glass::ZeroDivisionError("Error when float value / uvec4 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw glass::ZeroDivisionError("Error when float value / uvec4 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw glass::ZeroDivisionError("Error when float value / uvec4 v: v.z is 0 now.");
	}
	if(v.w == 0)
	{
		throw glass::ZeroDivisionError("Error when float value / uvec4 v: v.w is 0 now.");
	}
	return vec4(value / v.x, value / v.y, value / v.z, value / v.w);
}

uvec4 operator +(const uvec4& v, uint value)
{
	return uvec4(v.x + value, v.y + value, v.z + value, v.w + value);
}

uvec4 operator -(const uvec4& v, uint value)
{
	return uvec4(v.x - value, v.y - value, v.z - value, v.w - value);
}

uvec4 operator *(const uvec4& v, uint value)
{
	return uvec4(v.x * value, v.y * value, v.z * value, v.w * value);
}

uvec4 operator /(const uvec4& v, uint value)
{
	if(value == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v / uint value: value is 0 now.");
	}
	return uvec4(v.x / value, v.y / value, v.z / value, v.w / value);
}

uvec4 operator %(const uvec4& v, uint value)
{
	if(value == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v %% uint value: value is 0 now.");
	}
	return uvec4(v.x % value, v.y % value, v.z % value, v.w % value);
}

uvec4 operator +(uint value, const uvec4& v)
{
	return uvec4(value + v.x, value + v.y, value + v.z, value + v.w);
}

uvec4 operator -(uint value, const uvec4& v)
{
	return uvec4(value - v.x, value - v.y, value - v.z, value - v.w);
}

uvec4 operator *(uint value, const uvec4& v)
{
	return uvec4(value * v.x, value * v.y, value * v.z, value * v.w);
}

uvec4 operator /(uint value, const uvec4& v)
{
	if(v.x == 0)
	{
		throw glass::ZeroDivisionError("Error when uint value / uvec4 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw glass::ZeroDivisionError("Error when uint value / uvec4 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw glass::ZeroDivisionError("Error when uint value / uvec4 v: v.z is 0 now.");
	}
	if(v.w == 0)
	{
		throw glass::ZeroDivisionError("Error when uint value / uvec4 v: v.w is 0 now.");
	}
	return uvec4(value / v.x, value / v.y, value / v.z, value / v.w);
}

uvec4 operator %(uint value, const uvec4& v)
{
	if(v.x == 0)
	{
		throw glass::ZeroDivisionError("Error when uint value %% uvec4 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw glass::ZeroDivisionError("Error when uint value %% uvec4 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw glass::ZeroDivisionError("Error when uint value %% uvec4 v: v.z is 0 now.");
	}
	if(v.w == 0)
	{
		throw glass::ZeroDivisionError("Error when uint value %% uvec4 v: v.w is 0 now.");
	}
	return uvec4(value % v.x, value % v.y, value % v.z, value % v.w);
}

uvec4 operator +(const uvec4& v, int value)
{
	return uvec4(v.x + value, v.y + value, v.z + value, v.w + value);
}

uvec4 operator -(const uvec4& v, int value)
{
	return uvec4(v.x - value, v.y - value, v.z - value, v.w - value);
}

uvec4 operator *(const uvec4& v, int value)
{
	return uvec4(v.x * value, v.y * value, v.z * value, v.w * value);
}

uvec4 operator /(const uvec4& v, int value)
{
	if(value == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v / int value: value is 0 now.");
	}
	return uvec4(v.x / value, v.y / value, v.z / value, v.w / value);
}

uvec4 operator %(const uvec4& v, int value)
{
	if(value == 0)
	{
		throw glass::ZeroDivisionError("Error when uvec4 v %% int value: value is 0 now.");
	}
	return uvec4(v.x % value, v.y % value, v.z % value, v.w % value);
}

uvec4 operator +(int value, const uvec4& v)
{
	return uvec4(value + v.x, value + v.y, value + v.z, value + v.w);
}

uvec4 operator -(int value, const uvec4& v)
{
	return uvec4(value - v.x, value - v.y, value - v.z, value - v.w);
}

uvec4 operator *(int value, const uvec4& v)
{
	return uvec4(value * v.x, value * v.y, value * v.z, value * v.w);
}

uvec4 operator /(int value, const uvec4& v)
{
	if(v.x == 0)
	{
		throw glass::ZeroDivisionError("Error when int value / uvec4 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw glass::ZeroDivisionError("Error when int value / uvec4 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw glass::ZeroDivisionError("Error when int value / uvec4 v: v.z is 0 now.");
	}
	if(v.w == 0)
	{
		throw glass::ZeroDivisionError("Error when int value / uvec4 v: v.w is 0 now.");
	}
	return uvec4(value / v.x, value / v.y, value / v.z, value / v.w);
}

uvec4 operator %(int value, const uvec4& v)
{
	if(v.x == 0)
	{
		throw glass::ZeroDivisionError("Error when int value %% uvec4 v: v.x is 0 now.");
	}
	if(v.y == 0)
	{
		throw glass::ZeroDivisionError("Error when int value %% uvec4 v: v.y is 0 now.");
	}
	if(v.z == 0)
	{
		throw glass::ZeroDivisionError("Error when int value %% uvec4 v: v.z is 0 now.");
	}
	if(v.w == 0)
	{
		throw glass::ZeroDivisionError("Error when int value %% uvec4 v: v.w is 0 now.");
	}
	return uvec4(value % v.x, value % v.y, value % v.z, value % v.w);
}

ostream& operator <<(ostream& out, const uvec4& v)
{
	out << "uvec4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
	return out;
}