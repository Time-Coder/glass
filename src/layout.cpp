#include "glass/common.h"
#include "glass/Layout"

using namespace std;

Layout::Layout(){}

GLVector& Layout::operator ()(uint location)
{
	if(defined_layouts.count(location) == 0)
	{
		defined_layouts[location] = GLVector();
	}
	defined_layouts[location].location = location;
	defined_layouts[location].parent = this;
	return defined_layouts[location];
}

const GLVector& Layout::operator ()(uint location)const
{
	if(defined_layouts.count(location) == 0)
	{
		throw glass::KeyError("layout location " + str::str(location) + " is not defined");
	}
	return defined_layouts.at(location);
}

void Layout::order(const vector<uvec3>& _order_mat)
{
	order_mat = _order_mat;
}

void Layout::apply()
{
	if(is_applied || defined_layouts.empty())
	{
		return;
	}

	uint common_length = defined_layouts.begin()->second.size();
	uint data_size = 0;
	for(auto it = defined_layouts.begin(); it != defined_layouts.end(); it++)
	{
		if(it->second.size() != common_length)
		{
			throw glass::RuntimeError(string("Each layout must have the same length.\n") + 
				               "But layout(location = " + str::str(defined_layouts.begin()->first) + ") is of length " + str::str(common_length) + ",\n" + 
				               "    layout(location = " + str::str(it->first) + ") is of length " + str::str(it->second.size()) + ".");
		}
		data_size += common_length * GLSL::built_in_types[it->second.dtype()].glsl_size;
	}
	vbo.malloc(data_size);
	byte* ptr = (byte*)(vbo.mapBuffer());

	for(auto it = defined_layouts.begin(); it != defined_layouts.end(); it++)
	{
		string dtype = it->second.dtype();
		if(dtype == "bool" || dtype == "int" || dtype == "uint" || dtype == "float" || dtype == "double")
		{
			uint size = common_length*GLSL::built_in_types[dtype].glsl_size;
			memcpy((void*)ptr, it->second.ptr(0), size);
			ptr += size;
		}
		else if(dtype == "dvec2" || dtype == "dvec3" || dtype == "dvec4" ||
			    dtype == "vec2"  || dtype == "vec3" || dtype == "vec4" ||
			    dtype == "uvec2" || dtype == "uvec3" || dtype == "uvec4" ||
			    dtype == "ivec2" || dtype == "ivec3" || dtype == "ivec4" ||
			    dtype == "bvec2" || dtype == "bvec3" || dtype == "bvec4")
		{
            for(uint i = 0; i < common_length; i++)
			{
				memcpy((void*)ptr, it->second.ptr(i), GLSL::built_in_types[dtype].glsl_size);
				ptr += GLSL::built_in_types[dtype].glsl_size;
			}
		}
		else if(dtype == "mat2")
		{
            for(uint i = 0; i < common_length; i++)
			{
				mat2* ptr_mat2 = (mat2*)(it->second.ptr(i));
				memcpy((void*)ptr, (void*)(&((*ptr_mat2)[0])), 2*sizeof(float));
				ptr += 2*sizeof(float);
				memcpy((void*)ptr, (void*)(&((*ptr_mat2)[1])), 2*sizeof(float));
				ptr += 2*sizeof(float);
			}
		}
		else if(dtype == "mat3")
		{
            for(uint i = 0; i < common_length; i++)
			{
				mat3* ptr_mat3 = (mat3*)(it->second.ptr(i));
				memcpy((void*)ptr, (void*)(&((*ptr_mat3)[0])), 3*sizeof(float));
				ptr += 3*sizeof(float);
				memcpy((void*)ptr, (void*)(&((*ptr_mat3)[1])), 3*sizeof(float));
				ptr += 3*sizeof(float);
				memcpy((void*)ptr, (void*)(&((*ptr_mat3)[2])), 3*sizeof(float));
				ptr += 3*sizeof(float);
			}
		}
		else if(dtype == "mat4")
		{
            for(uint i = 0; i < common_length; i++)
			{
				mat4* ptr_mat4 = (mat4*)(it->second.ptr(i));
				memcpy((void*)ptr, (void*)(&((*ptr_mat4)[0])), 4*sizeof(float));
				ptr += 4*sizeof(float);
				memcpy((void*)ptr, (void*)(&((*ptr_mat4)[1])), 4*sizeof(float));
				ptr += 4*sizeof(float);
				memcpy((void*)ptr, (void*)(&((*ptr_mat4)[2])), 4*sizeof(float));
				ptr += 4*sizeof(float);
				memcpy((void*)ptr, (void*)(&((*ptr_mat4)[3])), 4*sizeof(float));
				ptr += 4*sizeof(float);
			}
		}
	}
	vbo.unMapBuffer();

	uint offset = 0;
	for(auto it = defined_layouts.begin(); it != defined_layouts.end(); it++)
	{
		string dtype = it->second.dtype();
		uint location = it->first;
		uint stripe = GLSL::built_in_types[dtype].glsl_size;
		if(dtype == "bool") vao[location](1, GL_BOOL, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "uint") vao[location](1, GL_UNSIGNED_INT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "int") vao[location](1, GL_INT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "float") vao[location](1, GL_FLOAT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "double") vao[location](1, GL_DOUBLE, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "bvec2") vao[location](2, GL_BOOL, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "bvec3") vao[location](3, GL_BOOL, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "bvec4") vao[location](4, GL_BOOL, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "ivec2") vao[location](2, GL_INT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "ivec3") vao[location](3, GL_INT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "ivec4") vao[location](4, GL_INT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "uvec2") vao[location](2, GL_UNSIGNED_INT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "uvec3") vao[location](3, GL_UNSIGNED_INT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "uvec4") vao[location](4, GL_UNSIGNED_INT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "vec2") vao[location](2, GL_FLOAT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "vec3") vao[location](3, GL_FLOAT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "vec4") vao[location](4, GL_FLOAT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "dvec2") vao[location](2, GL_DOUBLE, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "dvec3") vao[location](3, GL_DOUBLE, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "dvec4") vao[location](4, GL_DOUBLE, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "mat2") vao[location](4, GL_FLOAT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "mat3") vao[location](9, GL_FLOAT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "mat4") vao[location](16, GL_FLOAT, GL_FALSE, stripe, offset).interp(vbo);
		
		offset += common_length * GLSL::built_in_types[dtype].glsl_size;
	}
	if(!order_mat.empty())
	{
		ebo.setIndices(order_mat);
		vao.setEBO(ebo);
	}
	is_applied = true;
}

void Layout::draw(VAO::DrawType type)
{
	if(defined_layouts.empty())
	{
		return;
	}

	apply();
	if(vao.ebo().empty())
	{
		vao.drawArrays(defined_layouts.begin()->second.size(), type);
	}
	else
	{
		vao.drawElements(type);
	}
}

bool Layout::empty()const
{
	return defined_layouts.empty();
}
