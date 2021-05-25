#ifndef __GLASS_COMMON__
#define __GLASS_COMMON__

#include <map>

#ifndef __gl_h_
#include "third_party/glad/glad/glad.h"
#endif

typedef unsigned int uint;
typedef unsigned char byte;

struct GLSL
{
	struct TypeInfo
	{
		uint size;
		uint align_size;
		uint glsl_size;

		TypeInfo(uint _size = 0, uint _align_size = 0, uint _glsl_size = 0) :
		size(_size), align_size(_align_size), glsl_size(_glsl_size) {}
	};
	static std::map<std::string, TypeInfo> built_in_types;
};

#endif