template_type = \
[
	"bool",
	"int",
	"uint",
	"float",
	"double",
	"bvec2", "bvec3", "bvec4",
	"ivec2", "ivec3", "ivec4",
	"uvec2", "uvec3", "uvec4",
	"vec2", "vec3", "vec4",
	"dvec2", "dvec3", "dvec4",
	"mat2", "mat3", "mat4",
]

def modify_code(code):
	lines = code.split("\n");
	result = ""
	i = 0;
	while i < len(lines):
		if lines[i].find("template ") == 0:
			for j in range(23):
				result += lines[i+j].replace("DataType", template_type[j]) + "\n"
			i += 23
		else:
			result += lines[i] + "\n"
			i += 1

	return result;

code = open("glvector.cpp").read()
open("glvector.cpp", "w").write(modify_code(code))