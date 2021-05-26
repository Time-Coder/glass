#include "glass/common.h"
#include "glass/EBO"
#include "glass/utils/helper.h"
#include "glass/utils/exceptions.h"

using namespace std;

multiset<uint> EBO::existing_EBOs;
uint EBO::active_EBO = 0;

void EBO::init()
{
	if(_id == 0)
	{
		glGenBuffers(1, &_id);
		if(_id == 0)
		{
			throw glass::MemoryError("Failed to create EBO!");
		}
	#ifdef _DEBUG
		cout << "constructing EBO " << _id << endl;
	#endif
		existing_EBOs.insert(_id);
	}
	else if(existing_EBOs.count(_id) == 0)
	{
		throw glass::RuntimeError("EBO " + str::str(_id) + " has already been destructed.");
	}
}

void EBO::del()
{
	uint count = existing_EBOs.count(_id);
	if(count > 0)
	{
		multiset_pop(existing_EBOs, _id);
		if(count == 1)
		{
		#ifdef _DEBUG
			cout << "destructing EBO " << _id << endl;
		#endif
			unbind();
			glDeleteBuffers(1, &_id);
		}
	}
}

EBO::EBO() {}

EBO::EBO(const EBO& ebo) :
_id(ebo._id), _size(ebo._size)
{
	if(existing_EBOs.count(_id) > 0)
	{
		existing_EBOs.insert(_id);
	}
}

EBO::EBO(EBO&& ebo) :
_id(move(ebo._id)), _size(move(ebo._size))
{
	ebo._id = 0;
	ebo._size = 0;
}

EBO& EBO::operator =(const EBO& ebo)
{
	if(this != &ebo && _id != ebo._id)
	{
		del();

		_id = ebo._id;
		_size = ebo._size;

		if(existing_EBOs.count(_id) > 0)
		{
			existing_EBOs.insert(_id);
		}
	}
	return *this;
}

EBO& EBO::operator =(EBO&& ebo)
{
	if(this != &ebo)
	{
		if(_id != ebo._id)
		{
			del();
		}

		_id = move(ebo._id);
		_size = move(ebo._size);

		ebo._id = 0;
		ebo._size = 0;
	}
	return *this;
}

EBO::~EBO()
{
	del();
}

uint EBO::id()const
{
	return _id;
}

void EBO::bind()
{
	init();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	active_EBO = _id;
}

void EBO::unbind()const
{
	if(active_EBO == _id)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		active_EBO = 0;
	}
}

void EBO::bufferData(void* ptr_value, uint n_bytes)
{
	clear();
	if(n_bytes == 0)
	{
		return;
	}

	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_bytes, ptr_value, GL_STATIC_DRAW);
	_size = n_bytes/sizeof(uint);
}

bool EBO::bufferData(const vector<uint>& indices)
{
	clear();
	if(indices.size() == 0)
	{
		return false;
	}
	if(indices.size() % 3 != 0)
	{
		throw glass::RuntimeError("EBO's buffer data must can be divided by 3.");
	}

	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), NULL, GL_STATIC_DRAW);
	void* ptr_start = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(ptr_start, (void*)(&(indices[0])), indices.size() * sizeof(uint));
	bool success = glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	unbind();

	_size = indices.size();

	return success;
}

bool EBO::bufferData(const vector<uvec3>& indices)
{
	clear();
	if(indices.size() == 0)
	{
		return false;
	}

	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * GLSL::built_in_types["uvec3"].glsl_size, NULL, GL_STATIC_DRAW);
	void* ptr_start = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	byte* ptr = (byte*)ptr_start;
	for(int i = 0; i < indices.size(); i++)
	{
		memcpy((void*)ptr, (void*)(&(indices[i])), GLSL::built_in_types["uvec3"].glsl_size);
		ptr += GLSL::built_in_types["uvec3"].glsl_size;
	}
	bool success = glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	unbind();

	_size = 3*indices.size();

	return success;
}

bool EBO::empty()const
{
	return (existing_EBOs.count(_id) == 0 || _size == 0);
}

uint EBO::size()const
{
	return _size;
}

bool EBO::isBind()const
{
	return (existing_EBOs.count(_id) > 0 && active_EBO == _id);
}

void EBO::clear()
{
	if(empty())
	{
		return;
	}

	unbind();
	existing_EBOs.erase(_id);
	#ifdef _DEBUG
		cout << "destructing EBO " << _id << endl;
	#endif
	glDeleteBuffers(1, &_id);
	_id = 0;
	_size = 0;
}

void EBO::malloc(uint n_bytes)
{
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_bytes, NULL, GL_STATIC_DRAW);
	_size = n_bytes/sizeof(uint);
}

void* EBO::ptr()
{
	if(empty())
	{
		return NULL;
	}

	bind();
	return glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
}

bool EBO::apply()
{
	bind();
	return glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

EBO EBO::clone()
{
	EBO ebo;
	if(empty())
	{
		return ebo;
	}

	ebo.malloc(_size * sizeof(uint));
	glBindBuffer(GL_COPY_WRITE_BUFFER, ebo._id);

	bind();
	glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, _size * sizeof(uint));
	
	return ebo;
}