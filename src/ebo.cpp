#include "glass/EBO"

using namespace std;

EBO::EBO() : BO(BO::ELEMENT) {}

EBO::EBO(const EBO& ebo) : BO(ebo) {}

EBO::EBO(EBO&& ebo) : BO(move(ebo)) {}

EBO& EBO::operator =(const EBO& ebo)
{
	if(this != &ebo && self != ebo.self)
	{
		return static_cast<EBO&>(BO::operator=(ebo));
	}
	else
	{
		return *this;
	}
}

EBO& EBO::operator =(EBO&& ebo)
{
	if(this != &ebo)
	{
		return static_cast<EBO&>(BO::operator=(move(ebo)));
	}
	else
	{
		return *this;
	}
}

void EBO::setIndices(const vector<uint>& indices, MemoryType mem_type)
{
	this->memcpy((void*)(indices.data()), indices.size() * sizeof(uint), mem_type);
}

void EBO::setIndices(const vector<uvec3>& indices, MemoryType mem_type)
{
	this->malloc(indices.size() * GLSL::built_in_types["uvec3"].glsl_size, mem_type);
	byte* pointer = (byte*)mapBuffer();
    for(uint i = 0; i < indices.size(); i++)
	{
		::memcpy((void*)pointer, (void*)(&(indices[i])), GLSL::built_in_types["uvec3"].glsl_size);
		pointer += GLSL::built_in_types["uvec3"].glsl_size;
	}
	unMapBuffer();
}
