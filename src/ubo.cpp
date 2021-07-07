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
    for(uint i = 0; i < length; i++)
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

bool UBO::is_poll_init = false;
Counter UBO::binding_points_poll;

UBO::Reference::Reference(UBO* _parent, const string& _name) :
parent(_parent),
name(_name) {}

string UBO::Reference::type()const
{
	return parent->userData<UBO_Instance>()->block[name].type;
}

uint UBO::Reference::size()const
{
	return parent->userData<UBO_Instance>()->block[name].size;
}

int UBO::Reference::offset()const
{
	return parent->userData<UBO_Instance>()->block[name].location;
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

	delWithUserData<UBO_Instance>();
}

UBO::UBO(): BO(UNIFORM) {}

UBO::UBO(const UBO& ubo) : BO(ubo) {}

UBO::UBO(UBO&& ubo) : BO(move(ubo)) {}

UBO::~UBO()
{
	this->del();
}

void UBO::bind()
{
	BO::bind();
}

void UBO::copy(const UBO& ubo)
{
	if(this != &ubo && self != ubo.self)
	{
		del();

		self = ubo.self;

		userData<UBO_Instance>()->binding_point = ubo.bindingPoint();
		binding_points_poll.borrow(ubo.bindingPoint());
	}
}

UBO& UBO::operator =(UBO&& ubo)
{
	if(this != &ubo)
	{
		if(!(userData<UBO_Instance>()->block.empty()) && userData<UBO_Instance>()->block != ubo.userData<UBO_Instance>()->block)
		{
			throw glass::RuntimeError("Different structure uniform block cannot assign to each other.");
		}

		BO::operator=(std::move(ubo));
	}
	return *this;
}

UBO::Reference UBO::operator [](const string& member_name)
{
	if(userData<UBO_Instance>()->block.empty())
	{
		throw glass::RuntimeError("Please call UBO::setStructure(Uniform::Block) before assign value!");
	}

	return Reference(this, member_name);
}

uint UBO::bindingPoint()
{
	return userData<UBO_Instance>()->binding_point;
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
	userData<UBO_Instance>()->binding_point = point;
	glBindBufferBase(GL_UNIFORM_BUFFER, point, id());
}

void UBO::setStructure(const Uniform::BlockMap::Reference& block)
{
	this->malloc(block.size());

	bind(binding_points_poll.get());
	userData<UBO_Instance>()->block = block;
}

bool UBO::is_atom(const string& name)
{
	return (userData<UBO_Instance>()->block.contains(name) && userData<UBO_Instance>()->block[name].atoms.size() == 1 && userData<UBO_Instance>()->block[name].atoms[0] == name);
}

#define SET_PREPARE(TYPENAME) \
if(!(userData<UBO_Instance>()->block.contains(name)))\
{\
	throw glass::KeyError("Member " + name + " is not defined in uniform block " + userData<UBO_Instance>()->block.name);\
}\
if(userData<UBO_Instance>()->block[name].type != #TYPENAME)\
{\
	throw glass::TypeError(string("Cannot convert ") + #TYPENAME + " to " + userData<UBO_Instance>()->block[name].type);\
}\
if(size() != userData<UBO_Instance>()->block.size())\
{\
	this->malloc(userData<UBO_Instance>()->block.size());\
	bind(binding_points_poll.get());\
}\
bind();

#define DEFINE_SET_THING(TYPENAME) \
void UBO::set_##TYPENAME(const string& name, const TYPENAME& value)\
{\
	SET_PREPARE(TYPENAME)\
	glBufferSubData(GL_UNIFORM_BUFFER, userData<UBO_Instance>()->block[name].location, GLSL::built_in_types[#TYPENAME].glsl_size, (void*)(&value));\
	unbind();\
}

#define DEFINE_SET_MAT(TYPENAME) \
void UBO::set_##TYPENAME(const string& name, TYPENAME value)\
{\
	SET_PREPARE(TYPENAME)\
	glBufferSubData(GL_UNIFORM_BUFFER, userData<UBO_Instance>()->block[name].location, GLSL::built_in_types[#TYPENAME].glsl_size, value.data());\
	unbind();\
}

void UBO::set_bool(const string& name, bool value)
{
	SET_PREPARE(bool);

	int int_value = (int)value;
	glBufferSubData(GL_UNIFORM_BUFFER, userData<UBO_Instance>()->block[name].location, GLSL::built_in_types["bool"].glsl_size, (void*)(&int_value));
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
	if(userData<UBO_Instance>()->block.empty())
	{
		throw glass::RuntimeError("Please call UBO::setStructure(Uniform::Block) before assign value!");
	}

	userData<UBO_Instance>()->block.parent->refresh();

	if(!userData<UBO_Instance>()->block.contains(name))
	{
		throw glass::KeyError(name + " is not a member of uniform block " + userData<UBO_Instance>()->block.name);
	}
	if(is_atom(name))
	{
		set_atom(name, ptr_value);
		return (void*)((unsigned char*)ptr_value + GLSL::built_in_types[userData<UBO_Instance>()->block[name].type].size);
	}

	std::string type = userData<UBO_Instance>()->block[name].type;
	std::string basetype = str::base_type(type);
	if(userData<UBO_Instance>()->block.parent->defined_structs.count(basetype) == 0 ||
	   userData<UBO_Instance>()->block.parent->defined_structs[basetype].host_hash == 0)
	{
		throw glass::TypeError("Please call Uniform::define<" + basetype + ">() before assign value!");
	}

	userData<UBO_Instance>()->block.parent->please_define(type);

	unsigned char* ptr = (unsigned char*)(ptr_value);
	for(std::string atom : userData<UBO_Instance>()->block[name].atoms)
	{
		ptr += userData<UBO_Instance>()->block[atom].offset;
		set_atom(atom, (void*)(ptr));
		ptr += userData<UBO_Instance>()->block[atom].size + userData<UBO_Instance>()->block[atom].padding_size;
	}

	return (void*)ptr;
}
