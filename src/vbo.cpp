#include <iostream>

#include "glass/common.h"
#include "glass/VBO"
#include "glass/utils/exceptions.h"
#include "glass/utils/helper.h"

using namespace std;

multiset<uint> VBO::existing_VBOs;
uint VBO::active_VBO = 0;

void VBO::init()
{
	if(_id == 0)
	{
		glGenBuffers(1, &_id);
	#ifdef _DEBUG
		cout << "constructing VBO " << _id << endl;
	#endif
		if(_id == 0)
		{
			throw glass::MemoryError("Failed to create VBO!");
		}
		existing_VBOs.insert(_id);
	}
	else if(existing_VBOs.count(_id) == 0)
	{
		throw glass::RuntimeError("VBO " + str::str(_id) + " has already been destructed.");
	}
}

void VBO::del()
{
	uint count = existing_VBOs.count(_id);
	if(count > 0)
	{
		multiset_pop(existing_VBOs, _id);
		if(count == 1)
		{
		#ifdef _DEBUG
			cout << "destructing VBO " << _id << endl;
		#endif
			unbind();
			glDeleteBuffers(1, &_id);
		}
	}
}

VBO::VBO() {}

VBO::VBO(const VBO& vbo) :
_id(vbo._id),
_size(vbo._size)
{
	if(existing_VBOs.count(_id) > 0)
	{
		existing_VBOs.insert(_id);
	}
}

VBO::VBO(VBO&& vbo) :
_id(move(vbo._id)),
_size(move(vbo._size))
{
	vbo._id = 0;
	vbo._size = 0;
}

VBO& VBO::operator =(const VBO& vbo)
{
	if(this != &vbo && _id != vbo._id)
	{
		del();

		_id = vbo._id;
		_size = vbo._size;

		if(existing_VBOs.count(_id) > 0)
		{
			existing_VBOs.insert(_id);
		}
	}
	return *this;
}

VBO& VBO::operator =(VBO&& vbo)
{
	if(this != &vbo)
	{
		del();

		_id = move(vbo._id);
		_size = move(vbo._size);

		vbo._id = 0;
		vbo._size = 0;
	}
	return *this;
}

bool VBO::isBind()const
{
	return (existing_VBOs.count(_id) > 0 && active_VBO == _id);
}

VBO::~VBO()
{
	del();
}

void VBO::clear()
{
	if(empty())
	{
		return;
	}

	unbind();
	existing_VBOs.erase(_id);
	#ifdef _DEBUG
		cout << "destructing VBO " << _id << endl;
	#endif
	glDeleteBuffers(1, &_id);
	_id = 0;
	_size = 0;
}

void VBO::bind()
{
	init();
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	active_VBO = _id;
}

void VBO::unbind()const
{
	if(active_VBO == _id)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		active_VBO = 0;
	}
}

void VBO::bufferData(void* ptr_value, uint n_bytes)
{
	if(n_bytes == 0)
	{
		return;
	}

	bind();
	glBufferData(GL_ARRAY_BUFFER, n_bytes, ptr_value, GL_STATIC_DRAW);
	_size = n_bytes;
}

bool VBO::empty()const
{
	return (existing_VBOs.count(_id) == 0 || _size == 0);
}

uint VBO::size()const
{
	return _size;
}

uint VBO::id()const
{
	return _id;
}

void VBO::malloc(uint n_bytes)
{
	bind();
	glBufferData(GL_ARRAY_BUFFER, n_bytes, NULL, GL_STATIC_DRAW);
	_size = n_bytes;
}

void* VBO::ptr()
{
	if(_size == 0)
	{
		return NULL;
	}
	
	bind();
	return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}

bool VBO::apply()
{
	bind();
	return glUnmapBuffer(GL_ARRAY_BUFFER);
}

VBO VBO::clone()
{
	VBO vbo;
	if(empty())
	{
		return vbo;
	}

	vbo.malloc(_size);
	bind();
	glBindBuffer(GL_COPY_WRITE_BUFFER, vbo._id);
	glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, _size);
	return vbo;
}