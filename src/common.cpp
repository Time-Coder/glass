#include <list>

#include "glass/common.h"
#include "glass/mat"
#include "glass/sampler2D"
#include "glass/samplerCube"

using namespace std;

map<string, GLSL::TypeInfo> GLSL::built_in_types = 
{
	{"int", TypeInfo(sizeof(int), alignof(int), sizeof(int))},
	{"uint", TypeInfo(sizeof(uint), alignof(uint), sizeof(uint))},
	{"bool", TypeInfo(sizeof(bool), alignof(bool), 4)},
	{"float", TypeInfo(sizeof(float), alignof(float), sizeof(float))},
	{"double", TypeInfo(sizeof(double), alignof(double), sizeof(double))},

	{"vec2", TypeInfo(sizeof(vec2), alignof(vec2), 2*sizeof(float))},
	{"vec3", TypeInfo(sizeof(vec3), alignof(vec3), 3*sizeof(float))},
	{"vec4", TypeInfo(sizeof(vec4), alignof(vec4), 4*sizeof(float))},

	{"ivec2", TypeInfo(sizeof(ivec2), alignof(ivec2), 2*sizeof(int))},
	{"ivec3", TypeInfo(sizeof(ivec3), alignof(ivec3), 3*sizeof(int))},
	{"ivec4", TypeInfo(sizeof(ivec4), alignof(ivec4), 4*sizeof(int))},

	{"uvec2", TypeInfo(sizeof(uvec2), alignof(uvec2), 2*sizeof(uint))},
	{"uvec3", TypeInfo(sizeof(uvec3), alignof(uvec3), 3*sizeof(uint))},
	{"uvec4", TypeInfo(sizeof(uvec4), alignof(uvec4), 4*sizeof(uint))},

	{"bvec2", TypeInfo(sizeof(bvec2), alignof(bvec2), 2*4)},
	{"bvec3", TypeInfo(sizeof(bvec3), alignof(bvec3), 3*4)},
	{"bvec4", TypeInfo(sizeof(bvec4), alignof(bvec4), 4*4)},

	{"dvec2", TypeInfo(sizeof(dvec2), alignof(dvec2), 2*sizeof(double))},
	{"dvec3", TypeInfo(sizeof(dvec3), alignof(dvec3), 3*sizeof(double))},
	{"dvec4", TypeInfo(sizeof(dvec4), alignof(dvec4), 4*sizeof(double))},

	{"mat2", TypeInfo(sizeof(mat2), alignof(mat2), 4*sizeof(float))},
	{"mat3", TypeInfo(sizeof(mat3), alignof(mat3), 9*sizeof(float))},
	{"mat4", TypeInfo(sizeof(mat4), alignof(mat4), 16*sizeof(float))},
	{"sampler2D", TypeInfo(sizeof(sampler2D), alignof(sampler2D), 0)},
	{"samplerCube", TypeInfo(sizeof(samplerCube), alignof(samplerCube), 0)}
};