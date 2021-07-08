#include <iostream>

#include "glass/common.h"
#include "glass/BO"
#include "glass/utils/exceptions.h"

using namespace std;

unordered_map<BO::BufferType, uint> BO::active_BO;

bool BO::empty()const
{
	return (self == NULL || self->id == 0 || self->n_sources == 0);
}

BO::Instance* BO::instance()
{
	if(self == NULL)
	{
		self = new Instance();
		if(self == NULL)
		{
			throw glass::RuntimeError("Failed to allocate memory!");
		}
		self->n_sources++;
	}

	return self;
}

void BO::init()
{
	if(instance()->id == 0)
	{
		switch(buffer_type)
		{
		case VERTEX:
		case ELEMENT:
		case UNIFORM:
			glGenBuffers(1, &(instance()->id));
			break;
		case FRAME:
			glGenFramebuffers(1, &(instance()->id));
			break;
		case RENDER:
			glGenRenderbuffers(1, &(instance()->id));
			break;
		}
		
		if(instance()->id == 0)
		{
			throw glass::RuntimeError("Failed to create " + type_str());
		}
	#ifdef _DEBUG
		cout << "constructing " << type_str() << " " << id() << endl;
	#endif
	}
	else if(instance()->n_sources == 0)
	{
		throw glass::DeveloperError(type_str() + " " + str::str(id()) + " has already been destructed.");
	}
}

void BO::del()
{
	if(self == NULL)
	{
		return;
	}

	if(instance()->id == 0)
	{		
		if(instance()->n_sources > 0)
		{
			instance()->n_sources--;
		}

		if(instance()->n_sources == 0)
		{
			delete self;
		}

		self = NULL;
		return;
	}

	instance()->n_sources--;
	if(instance()->n_sources == 0) // Not empty: id != 0
	{
	#ifdef _DEBUG
		cout << "destructing " << type_str() << " " << id() << endl;
	#endif
		unbind();
		switch(buffer_type)
		{
			case VERTEX:
			case ELEMENT:
			case UNIFORM:
				glDeleteBuffers(1, &(instance()->id));
				break;
			case FRAME:
				glDeleteFramebuffers(1, &(instance()->id));
				break;
			case RENDER:
				glDeleteRenderbuffers(1, &(instance()->id));
				break;
		}
		delete self;
	}

	self = NULL;
}

BO::BO(BufferType _type):
buffer_type(_type) {}

BO::BO(const BO& bo):
buffer_type(bo.buffer_type),
self(bo.self)
{
	if(self != NULL)
	{
		self->n_sources++;
	}
}

BO::BO(BO&& bo) :
buffer_type(move(bo.buffer_type)),
self(move(bo.self))
{
	bo.self = NULL;
}

BO& BO::operator =(const BO& bo)
{
	if(this != &bo && self != bo.self)
	{
		del();

		self = bo.self;
		buffer_type = bo.buffer_type;

		if(self != NULL)
		{
			self->n_sources++;
		}
	}
	return *this;
}

BO& BO::operator =(BO&& bo)
{
	if(this != &bo)
	{
		if(self != bo.self)
		{
			del();
		}

		self = move(bo.self);
		buffer_type = move(bo.buffer_type);

		bo.self = NULL;
	}
	return *this;
}

BO::~BO()
{
	del();
}

bool BO::isBind()const
{
	return (!empty() && active_BO[buffer_type] == self->id);
}

uint BO::n_sources()const
{
	return (self == NULL ? 0 : self->n_sources);
}

void BO::bind()
{
	bool should_apply_malloc = (id() == 0 && size() != 0);

	init();
	switch(buffer_type)
	{
	case VERTEX:
	case ELEMENT:
	case UNIFORM:
		glBindBuffer(buffer_type, instance()->id);
		break;
	case FRAME:
		glBindFramebuffer(buffer_type, instance()->id);
		break;
	case RENDER:
		glBindRenderbuffer(buffer_type, instance()->id);
		break;
	}
	
	active_BO[buffer_type] = instance()->id;
	if(should_apply_malloc)
	{
		glBufferData(buffer_type, instance()->size, NULL, instance()->mem_type);
	}
}

void BO::unbind()const
{
	if(active_BO.count(buffer_type) && active_BO[buffer_type] == id())
	{
		switch(buffer_type)
		{
		case VERTEX:
		case ELEMENT:
		case UNIFORM:
			glBindBuffer(buffer_type, 0);
			break;
		case FRAME:
			glBindFramebuffer(buffer_type, 0);
			break;
		case RENDER:
			glBindRenderbuffer(buffer_type, 0);
			break;
		}

		active_BO[buffer_type] = 0;
	}
}

size_t BO::size()const
{
	return (self == NULL ? 0 : self->size);
}

uint BO::id()const
{
	return (self == NULL ? 0 : self->id);
}

BO::BufferType BO::type()const
{
	return buffer_type;
}

string BO::type_str()const
{
	switch(buffer_type)
	{
		case VERTEX: return "VBO";
		case ELEMENT: return "EBO";
		case UNIFORM: return "UBO";
		case FRAME: return "FBO";
		case RENDER: return "RBO";
		default: return "VBO";
	}
}

BO::MemoryType BO::memType()const
{
	return (self == NULL ? STATIC : self->mem_type);
}

void BO::malloc(size_t n_bytes, MemoryType mem_type)
{
	if(id() != 0)
	{
		bind();
		glBufferData(buffer_type, n_bytes, NULL, mem_type);
	}

	instance()->size = n_bytes;
	instance()->mem_type = mem_type;
}

void BO::realloc(size_t n_bytes, MemoryType mem_type)
{
	if(empty())
	{
		malloc(n_bytes, mem_type);
		return;
	}

	size_t data_size = std::min(n_bytes, size());
	void* data_temp = ::malloc(data_size);
	if(data_temp == NULL)
	{
		throw glass::MemoryError("Failed to allocate memory!");
	}
	
	void* pointer = this->mapBuffer();
	::memcpy(data_temp, pointer, data_size);
	this->unMapBuffer();
	this->malloc(n_bytes, mem_type);
	pointer = this->mapBuffer();
	::memcpy(pointer, data_temp, data_size);
	this->unMapBuffer();
	::free(data_temp);
}

void BO::calloc(size_t n_elements, size_t sizeof_element, MemoryType mem_type)
{
	size_t n_bytes = n_elements * sizeof_element;
	malloc(n_bytes, mem_type);
}

void BO::memcpy(void* ptr_value, size_t n_bytes, MemoryType mem_type)
{
	bind();
	glBufferData(buffer_type, n_bytes, ptr_value, mem_type);
	instance()->size = n_bytes;
	instance()->mem_type = mem_type;
}

void BO::free()
{
	instance()->size = 0;
	if(empty())
	{
		return;
	}

#ifdef _DEBUG
	cout << "destructing " << type_str() << " " << id() << endl;
#endif
	unbind();
	glDeleteBuffers(1, &(instance()->id));
	instance()->id = 0;
}

void* BO::mapBuffer()
{
	if(size() == 0)
	{
		return NULL;
	}
	
	bind();
	void* pointer = glMapBuffer(buffer_type, GL_READ_WRITE);
	instance()->is_mapped = true;
	return pointer;
}

bool BO::unMapBuffer()
{
	if(empty())
	{
		return false;
	}
	if(instance()->is_mapped == false)
	{
		return false;
	}

	bind();
	bool result = glUnmapBuffer(buffer_type);
	instance()->is_mapped = false;
	return result;
}
