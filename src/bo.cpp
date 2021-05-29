#include <iostream>

#include "glass/common.h"
#include "glass/BO"
#include "glass/utils/exceptions.h"

using namespace std;

unordered_map<BO::BufferType, unordered_map<uint, BO::Instance> > BO::existing_BOs;
unordered_map<BO::BufferType, uint> BO::active_BO;

void BO::init()
{
	if(_id == 0)
	{
		switch(_buffer_type)
		{
			case VERTEX:
			case ELEMENT:
			case UNIFORM:
				glGenBuffers(1, &_id);
				break;
			case FRAME:
				glGenFramebuffers(1, &_id);
				break;
			case RENDER:
				glGenRenderbuffers(1, &_id);
				break;
		}
		
		if(_id == 0)
		{
			throw glass::RuntimeError("Failed to create " + type_str());
		}
	#ifdef _DEBUG
		cout << "constructing " << type_str() << " " << _id << endl;
	#endif
		existing_BOs[_buffer_type][_id].n_sources++;
	}
	else if(existing_BOs[_buffer_type].count(_id) == 0 || existing_BOs[_buffer_type][_id].n_sources == 0)
	{
		throw glass::RuntimeError(type_str() + " " + str::str(_id) + " has already been destructed.");
	}
}

void BO::del()
{
	if(existing_BOs[_buffer_type].count(_id) == 0 || existing_BOs[_buffer_type][_id].n_sources == 0)
	{
		_id = 0;
		return;
	}

	uint count = existing_BOs[_buffer_type][_id].n_sources;
	if(count > 0)
	{
		existing_BOs[_buffer_type][_id].n_sources--;
		if(count == 1)
		{
		#ifdef _DEBUG
			cout << "destructing " << type_str() << " " << _id << endl;
		#endif
			unbind();
			existing_BOs[_buffer_type].erase(_id);
			switch(_buffer_type)
			{
				case VERTEX:
				case ELEMENT:
				case UNIFORM:
					glDeleteBuffers(1, &_id);
					break;
				case FRAME:
					glDeleteFramebuffers(1, &_id);
					break;
				case RENDER:
					glDeleteRenderbuffers(1, &_id);
					break;
			}
		}
	}
	_id = 0;
}

BO::BO(BufferType _type):
_buffer_type(_type) {}

BO::BO(const BO& bo) :
_id(bo._id),
_buffer_type(bo._buffer_type)
{
	if(existing_BOs[_buffer_type].count(_id) && existing_BOs[_buffer_type][_id].n_sources > 0)
	{
		existing_BOs[_buffer_type][_id].n_sources++;
	}
}

BO::BO(BO&& bo) :
_id(move(bo._id)),
_buffer_type(move(bo._buffer_type))
{
	bo._id = 0;
}

BO& BO::operator =(const BO& bo)
{
	if(this != &bo && _id != bo._id)
	{
		del();

		_id = bo._id;
		_buffer_type = bo._buffer_type;

		if(existing_BOs[_buffer_type].count(_id) && existing_BOs[_buffer_type][_id].n_sources > 0)
		{
			existing_BOs[_buffer_type][_id].n_sources++;
		}
	}
	return *this;
}

BO& BO::operator =(BO&& bo)
{
	if(this != &bo)
	{
		if(_id != bo._id)
		{
			del();
		}

		_id = move(bo._id);
		_buffer_type = move(bo._buffer_type);

		bo._id = 0;
	}
	return *this;
}

BO::~BO()
{
	del();
}

bool BO::isBind()const
{
	return (_id != 0 && 
		    existing_BOs[_buffer_type].count(_id) && existing_BOs[_buffer_type][_id].n_sources > 0 && 
		    active_BO[_buffer_type] == _id);
}

uint BO::n_sources()const
{
	if(_id == 0)
	{
		return 0;
	}

	return existing_BOs[_buffer_type].count(_id) ? existing_BOs[_buffer_type][_id].n_sources : 0;
}

void BO::bind()
{
	init();
	switch(_buffer_type)
	{
		case VERTEX:
		case ELEMENT:
		case UNIFORM:
			glBindBuffer(_buffer_type, _id);
			break;
		case FRAME:
			glBindFramebuffer(_buffer_type, _id);
			break;
		case RENDER:
			glBindRenderbuffer(_buffer_type, _id);
			break;
	}
	
	active_BO[_buffer_type] = _id;
}

void BO::unbind()const
{
	if(active_BO.count(_buffer_type) && active_BO[_buffer_type] == _id)
	{
		switch(_buffer_type)
		{
			case VERTEX:
			case ELEMENT:
			case UNIFORM:
				glBindBuffer(_buffer_type, 0);
				break;
			case FRAME:
				glBindFramebuffer(_buffer_type, 0);
				break;
			case RENDER:
				glBindRenderbuffer(_buffer_type, 0);
				break;
		}

		active_BO[_buffer_type] = 0;
	}
}

bool BO::empty()const
{
	return (_id == 0 || existing_BOs[_buffer_type].count(_id) == 0 || 
		    existing_BOs[_buffer_type][_id].n_sources == 0 || existing_BOs[_buffer_type][_id].size == 0);
}

uint BO::size()const
{
	return (existing_BOs[_buffer_type].count(_id) ? existing_BOs[_buffer_type][_id].size : 0);
}

uint BO::id()const
{
	return _id;
}

BO::BufferType BO::type()const
{
	return _buffer_type;
}

string BO::type_str()const
{
	switch(_buffer_type)
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
	if(_id == 0 || existing_BOs[_buffer_type].count(_id) == 0)
	{
		return STATIC;
	}

	return existing_BOs[_buffer_type][_id].mem_type;
}

void BO::malloc(uint n_bytes, MemoryType mem_type)
{
	bind();
	glBufferData(_buffer_type, n_bytes, NULL, mem_type);
	existing_BOs[_buffer_type][_id].size = n_bytes;
	existing_BOs[_buffer_type][_id].mem_type = mem_type;
}

void BO::calloc(uint n_elements, uint sizeof_element, MemoryType mem_type)
{
	uint n_bytes = n_elements * sizeof_element;

	bind();
	glBufferData(_buffer_type, n_bytes, NULL, mem_type);
	existing_BOs[_buffer_type][_id].size = n_bytes;
	existing_BOs[_buffer_type][_id].mem_type = mem_type;
}

void BO::memcpy(void* ptr_value, uint n_bytes, MemoryType mem_type)
{
	bind();
	glBufferData(_buffer_type, n_bytes, ptr_value, mem_type);
	existing_BOs[_buffer_type][_id].size = n_bytes;
	existing_BOs[_buffer_type][_id].mem_type = mem_type;
}

void BO::free()
{
	if(empty())
	{
		return;
	}

#ifdef _DEBUG
	cout << "destructing " << type_str() << " " << _id << endl;
#endif
	unbind();
	existing_BOs[_buffer_type].erase(_id);
	glDeleteBuffers(1, &_id);
}

void* BO::ptr()
{
	if(size() == 0)
	{
		return NULL;
	}
	
	bind();
	void* pointer = glMapBuffer(_buffer_type, GL_WRITE_ONLY);
	existing_BOs[_buffer_type][_id].is_mapped = true;
	return pointer;
}

bool BO::apply()
{
	if(empty())
	{
		return false;
	}
	if(existing_BOs[_buffer_type][_id].is_mapped == false)
	{
		return false;
	}

	bind();
	bool result = glUnmapBuffer(_buffer_type);
	existing_BOs[_buffer_type][_id].is_mapped = false;
	return result;
}

BO BO::clone()
{
	BO bo(_buffer_type);
	if(empty())
	{
		return bo;
	}

	bo.malloc(size());
	bind();
	switch(_buffer_type)
	{
		case VERTEX:
		case ELEMENT:
		case UNIFORM:
			glBindBuffer(GL_COPY_WRITE_BUFFER, bo._id);
			break;
		case FRAME:
			glBindFramebuffer(GL_COPY_WRITE_BUFFER, bo._id);
			break;
		case RENDER:
			glBindRenderbuffer(GL_COPY_WRITE_BUFFER, bo._id);
			break;
	}

	glCopyBufferSubData(_buffer_type, GL_COPY_WRITE_BUFFER, 0, 0, size());
	return bo;
}