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

multiset<uint> UBO::existing_UBOs;
uint UBO::active_UBO = 0;
bool UBO::is_poll_init = false;
Counter UBO::binding_points_poll;

UBO::Reference::Reference(UBO* _parent, const string& _name) :
parent(_parent),
name(_name) {}

string UBO::Reference::type()const
{
	return parent->block[name].type;
}

uint UBO::Reference::size()const
{
	return parent->block[name].size;
}

int UBO::Reference::offset()const
{
	return parent->block[name].location;
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

	if(_id == 0)
	{
		glGenBuffers(1, &_id);
		if(_id == 0)
		{
			throw glass::RuntimeError("Failed to create UBO.");
		}
	#ifdef _DEBUG
		cout << "constructing UBO " << _id << endl;
	#endif
		existing_UBOs.insert(_id);
	}
	else if(existing_UBOs.count(_id) == 0)
	{
		throw glass::RuntimeError("UBO " + str::str(_id) + " has already been destructed.");
	}
}

void UBO::del()
{
	uint count = existing_UBOs.count(_id);
	if(count > 0)
	{
		multiset_pop(existing_UBOs, _id);
		if(count == 1)
		{
		#ifdef _DEBUG
			cout << "destructing UBO " << _id << endl;
		#endif
			unbind();
			glDeleteBuffers(1, &_id);
		}
	}
}

UBO::UBO() {}

UBO::UBO(const UBO& ubo) :
_id(ubo._id),
_size(ubo._size),
_binding_point(ubo._binding_point),
block(ubo.block)
{
	if(existing_UBOs.count(_id) > 0)
	{
		existing_UBOs.insert(_id);
	}
}

UBO::UBO(UBO&& ubo) :
_id(move(ubo._id)),
_size(move(ubo._size)),
_binding_point(move(ubo._binding_point)),
block(move(ubo.block))
{
	ubo._id = 0;
}

void UBO::copy(const UBO& ubo)
{
	if(this != &ubo && _id != ubo._id)
	{
		del();

		_id = ubo._id;
		_size = ubo._size;
		block = ubo.block;

		binding_points_poll.put_back(_binding_point);
		_binding_point = ubo._binding_point;
		binding_points_poll.borrow(_binding_point);

		if(existing_UBOs.count(_id) > 0)
		{
			existing_UBOs.insert(_id);
		}
	}
}

UBO& UBO::operator =(UBO&& ubo)
{
	if(this != &ubo)
	{
		if(!block.empty() && block != ubo.block)
		{
			throw glass::RuntimeError("Different structure uniform block cannot assign to each other.");
		}

		del();
		_id = move(ubo._id);
		_size = move(ubo._size);

		block = move(ubo.block);
		binding_points_poll.put_back(_binding_point);
		_binding_point = move(ubo._binding_point);

		ubo._id = 0;
	}
	return *this;
}

UBO::Reference UBO::operator [](const string& member_name)
{
	if(block.empty())
	{
		throw glass::RuntimeError("Please call UBO::setStructure(Uniform::Block) before assign value!");
	}

	return Reference(this, member_name);
}

UBO::~UBO()
{
	del();
}

uint UBO::id()const
{
	return _id;
}

uint UBO::size()const
{
	return _size;
}

uint UBO::binding_point()const
{
	return _binding_point;
}

void UBO::bind(uint point)
{
	if(_binding_point == point)
	{
		return;
	}

	binding_points_poll.put_back(_binding_point);
	binding_points_poll.borrow(point);

	_binding_point = point;
	bind();
	glBindBufferBase(GL_UNIFORM_BUFFER, point, _id);
}

void UBO::bind()
{
	init();
	glBindBuffer(GL_UNIFORM_BUFFER, _id);
	active_UBO = _id;
}

void UBO::unbind()const
{
	if(active_UBO == _id)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		active_UBO = 0;
	}
}

bool UBO::isBind()const
{
	return (existing_UBOs.count(_id) > 0 && active_UBO == _id);
}

bool UBO::empty()const
{
	return (existing_UBOs.count(_id) == 0 || _size == 0);
}

void UBO::malloc(uint n_bytes)
{
	bind();
	glBufferData(GL_UNIFORM_BUFFER, n_bytes, NULL, GL_STATIC_DRAW);
	unbind();
	_size = n_bytes;
}

void UBO::setStructure(const Uniform::BlockMap::Reference& _block)
{
	this->malloc(_block.size());
	block = _block;
	bind(binding_points_poll.get());
}

bool UBO::is_atom(const string& name)
{
	return (block.contains(name) && block[name].atoms.size() == 1 && block[name].atoms[0] == name);
}

#define SET_PREPARE(TYPENAME) \
if(!(block.contains(name)))\
{\
	throw glass::KeyError("Member " + name + " is not defined in uniform block " + block.name);\
}\
if(block[name].type != #TYPENAME)\
{\
	throw glass::TypeError(string("Cannot convert ") + #TYPENAME + " to " + block[name].type);\
}\
if(_size != block.size())\
{\
	this->malloc(block.size());\
	bind(binding_points_poll.get());\
}\
bind();

#define DEFINE_SET_THING(TYPENAME) \
void UBO::set_##TYPENAME(const string& name, const TYPENAME& value)\
{\
	SET_PREPARE(TYPENAME)\
	glBufferSubData(GL_UNIFORM_BUFFER, block[name].location, GLSL::built_in_types[#TYPENAME].glsl_size, (void*)(&value));\
	unbind();\
}

#define DEFINE_SET_MAT(TYPENAME) \
void UBO::set_##TYPENAME(const string& name, TYPENAME value)\
{\
	SET_PREPARE(TYPENAME)\
	glBufferSubData(GL_UNIFORM_BUFFER, block[name].location, GLSL::built_in_types[#TYPENAME].glsl_size, value.data());\
	unbind();\
}

void UBO::set_bool(const string& name, bool value)
{
	SET_PREPARE(bool);

	int int_value = (int)value;
	glBufferSubData(GL_UNIFORM_BUFFER, block[name].location, GLSL::built_in_types["bool"].glsl_size, (void*)(&int_value));
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
	if(block.empty())
	{
		throw glass::RuntimeError("Please call UBO::setStructure(Uniform::Block) before assign value!");
	}

	block.parent->refresh();

	if(!block.contains(name))
	{
		throw glass::KeyError(name + " is not a member of uniform block " + block.name);
	}
	if(is_atom(name))
	{
		set_atom(name, ptr_value);
		return (void*)((unsigned char*)ptr_value + GLSL::built_in_types[block[name].type].size);
	}

	std::string type = block[name].type;
	std::string basetype = str::base_type(type);
	if(block.parent->defined_structs.count(basetype) == 0 ||
	   block.parent->defined_structs[basetype].host_hash == 0)
	{
		throw glass::TypeError("Please call Uniform::define<" + basetype + ">() before assign value!");
	}

	block.parent->please_define(type);

	unsigned char* ptr = (unsigned char*)(ptr_value);
	for(std::string atom : block[name].atoms)
	{
		ptr += block[atom].offset;
		set_atom(atom, (void*)(ptr));
		ptr += block[atom].size + block[atom].padding_size;
	}

	return (void*)ptr;
}