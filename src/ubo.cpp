#include "glass/common.h"
#include "glass/UBO"
#include "glass/mat"
#include "glass/utils/type.h"
#include "glass/utils/helper.h"

using namespace std;

void Counter::init(uint _length)
{
	length = _length;
	data = new int[_length];
	if(data == NULL)
	{
		throw glass::MemoryError("Failed to allocate memory.");
	}
	for(int i = 0; i < length; i++)
	{
		data[i] = 0;
	}
}

Counter::Counter(uint _length)
{
	init(_length);
}

Counter::~Counter()
{
	if(data != NULL)
	{
		delete [] data;
		data = NULL;
	}
}

uint Counter::get()const
{
	for(uint i = 1; i < length; i++)
	{
		if(data[i] >= 0)
		{
			return i;
		}
	}
	throw glass::RuntimeError("No available value.");
}

void Counter::borrow(uint value)
{
	if(value >= length)
	{
		throw glass::ValueError("value over range " + str::str(length));
	}
	data[value]--;
}

void Counter::put_back(uint value)
{
	if(value >= length)
	{
		throw glass::ValueError("value over range " + str::str(length));
	}
	data[value]++;
}

unordered_map<uint, UBO::UBO_Instance> UBO::existing_UBOs;
bool UBO::is_poll_init = false;
Counter UBO::binding_points_poll;

UBO::Reference::Reference(UBO* _parent, const string& _name) :
parent(_parent),
name(_name) {}

string UBO::Reference::type()const
{
	return UBO::existing_UBOs[parent->_id].block[name].type;
}

uint UBO::Reference::size()const
{
	return UBO::existing_UBOs[parent->_id].block[name].size;
}

int UBO::Reference::offset()const
{
	return UBO::existing_UBOs[parent->_id].block[name].location;
}

UBO::Reference UBO::Reference::operator [](const string& sub_name)const
{
	return UBO::Reference(parent, name + "." + sub_name);
}

UBO::Reference UBO::Reference::operator [](uint index)const
{
	return UBO::Reference(parent, name + "[" + str::str(index) + "]");
}

bool UBO::Reference::operator ==(const UBO::Reference& ref)const
{
	return (parent == ref.parent && name == ref.name);
}

bool UBO::Reference::operator !=(const UBO::Reference& ref)const
{
	return (parent != ref.parent || name != ref.name);
}

void UBO::init()
{
	if(!is_poll_init)
	{
		int max_blocks = 0;
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &max_blocks);
		binding_points_poll.init(max_blocks);
		is_poll_init = true;
	}

	BO::init();
}

void UBO::del()
{
	if(bindingPoint() != 0)
	{
		binding_points_poll.put_back(bindingPoint());
	}

	BO::del();
}

UBO::UBO(): BO(UNIFORM) {}

UBO::UBO(const UBO& ubo) : BO(ubo) {}

UBO::UBO(UBO&& ubo) : BO(move(ubo)) {}

UBO::~UBO()
{
	del();
}

void UBO::bind()
{
	BO::bind();
}

void UBO::copy(const UBO& ubo)
{
	if(this != &ubo && _id != ubo._id)
	{
		del();

		_id = ubo._id;

		if(existing_UBOs.count(_id))
		{
			existing_UBOs[_id].binding_point = ubo.bindingPoint();
			binding_points_poll.borrow(ubo.bindingPoint());
		}
	}
}

UBO& UBO::operator =(UBO&& ubo)
{
	if(this != &ubo)
	{
		if(!existing_UBOs[_id].block.empty() && existing_UBOs[_id].block != existing_UBOs[ubo._id].block)
		{
			throw glass::RuntimeError("Different structure uniform block cannot assign to each other.");
		}

		BO::operator=(std::move(ubo));
	}
	return *this;
}

UBO::Reference UBO::operator [](const string& member_name)
{
	if(!existing_UBOs.count(_id) || existing_UBOs[_id].block.empty())
	{
		throw glass::RuntimeError("Please call UBO::setStructure(Uniform::Block) before assign value!");
	}

	return Reference(this, member_name);
}

uint UBO::bindingPoint()const
{
	return existing_UBOs.count(_id) ? existing_UBOs[_id].binding_point : 0;
}

void UBO::bind(uint point)
{
	if(bindingPoint() == point)
	{
		return;
	}

	if(bindingPoint() != 0)
	{
		binding_points_poll.put_back(bindingPoint());
	}
	binding_points_poll.borrow(point);

	bind();
	existing_UBOs[_id].binding_point = point;
	glBindBufferBase(GL_UNIFORM_BUFFER, point, _id);
}

void UBO::setStructure(const Uniform::BlockMap::Reference& block)
{
	this->malloc(block.size());

	bind(binding_points_poll.get());
	existing_UBOs[_id].block = block;
}

bool UBO::is_atom(const string& name)
{
	return (existing_UBOs[_id].block.contains(name) && existing_UBOs[_id].block[name].atoms.size() == 1 && existing_UBOs[_id].block[name].atoms[0] == name);
}

#define SET_PREPARE(TYPENAME) \
if(!(existing_UBOs[_id].block.contains(name)))\
{\
	throw glass::KeyError("Member " + name + " is not defined in uniform block " + existing_UBOs[_id].block.name);\
}\
if(existing_UBOs[_id].block[name].type != #TYPENAME)\
{\
	throw glass::TypeError(string("Cannot convert ") + #TYPENAME + " to " + existing_UBOs[_id].block[name].type);\
}\
if(size() != existing_UBOs[_id].block.size())\
{\
	this->malloc(existing_UBOs[_id].block.size());\
	bind(binding_points_poll.get());\
}\
bind();

#define DEFINE_SET_THING(TYPENAME) \
void UBO::set_##TYPENAME(const string& name, const TYPENAME& value)\
{\
	SET_PREPARE(TYPENAME)\
	glBufferSubData(GL_UNIFORM_BUFFER, existing_UBOs[_id].block[name].location, GLSL::built_in_types[#TYPENAME].glsl_size, (void*)(&value));\
	unbind();\
}

#define DEFINE_SET_MAT(TYPENAME) \
void UBO::set_##TYPENAME(const string& name, TYPENAME value)\
{\
	SET_PREPARE(TYPENAME)\
	glBufferSubData(GL_UNIFORM_BUFFER, existing_UBOs[_id].block[name].location, GLSL::built_in_types[#TYPENAME].glsl_size, value.data());\
	unbind();\
}

void UBO::set_bool(const string& name, bool value)
{
	SET_PREPARE(bool);

	int int_value = (int)value;
	glBufferSubData(GL_UNIFORM_BUFFER, existing_UBOs[_id].block[name].location, GLSL::built_in_types["bool"].glsl_size, (void*)(&int_value));
	unbind();
}

DEFINE_SET_THING(uint)
DEFINE_SET_THING(int)
DEFINE_SET_THING(float)
DEFINE_SET_THING(double)
DEFINE_SET_THING(bvec2)
DEFINE_SET_THING(bvec3)
DEFINE_SET_THING(bvec4)
DEFINE_SET_THING(ivec2)
DEFINE_SET_THING(ivec3)
DEFINE_SET_THING(ivec4)
DEFINE_SET_THING(uvec2)
DEFINE_SET_THING(uvec3)
DEFINE_SET_THING(uvec4)
DEFINE_SET_THING(vec2)
DEFINE_SET_THING(vec3)
DEFINE_SET_THING(vec4)
DEFINE_SET_THING(dvec2)
DEFINE_SET_THING(dvec3)
DEFINE_SET_THING(dvec4)
DEFINE_SET_MAT(mat2)
DEFINE_SET_MAT(mat3)
DEFINE_SET_MAT(mat4)

void* UBO::set(const std::string& name, void* ptr_value)
{
	if(existing_UBOs[_id].block.empty())
	{
		throw glass::RuntimeError("Please call UBO::setStructure(Uniform::Block) before assign value!");
	}

	existing_UBOs[_id].block.parent->refresh();

	if(!existing_UBOs[_id].block.contains(name))
	{
		throw glass::KeyError(name + " is not a member of uniform block " + existing_UBOs[_id].block.name);
	}
	if(is_atom(name))
	{
		set_atom(name, ptr_value);
		return (void*)((unsigned char*)ptr_value + GLSL::built_in_types[existing_UBOs[_id].block[name].type].size);
	}

	std::string type = existing_UBOs[_id].block[name].type;
	std::string basetype = str::base_type(type);
	if(existing_UBOs[_id].block.parent->defined_structs.count(basetype) == 0 ||
	   existing_UBOs[_id].block.parent->defined_structs[basetype].host_hash == 0)
	{
		throw glass::TypeError("Please call Uniform::define<" + basetype + ">() before assign value!");
	}

	existing_UBOs[_id].block.parent->please_define(type);

	unsigned char* ptr = (unsigned char*)(ptr_value);
	for(std::string atom : existing_UBOs[_id].block[name].atoms)
	{
		ptr += existing_UBOs[_id].block[atom].offset;
		set_atom(atom, (void*)(ptr));
		ptr += existing_UBOs[_id].block[atom].size + existing_UBOs[_id].block[atom].padding_size;
	}

	return (void*)ptr;
}