#include "glass/VAO"
#include "glass/utils/exceptions.h"
#include "glass/utils/helper.h"

using namespace std;

VertexAttribPointer::VertexAttribPointer(uint _location, uint _vertex_size, uint _dtype, uint _should_normalize, uint _stride, uint _offset) : 
location(_location),
vertex_size(_vertex_size),
dtype(_dtype),
should_normalize(_should_normalize),
stride(_stride),
offset(_offset) {}

void VertexAttribPointer::interp(const VBO& _vbo)
{
	if(vertex_size == 0)
	{
		throw glass::Exception("Empty attrib for VBO.");
	}

	vbo = _vbo;
	vbo.bind();
	glVertexAttribPointer(location, 
		                  vertex_size, 
		                  dtype, 
		                  should_normalize,
		                  stride,
		                  (void*)((byte*)NULL + offset));
	glEnableVertexAttribArray(location);
}

void VertexAttribPointer::divisor(uint n)
{
	glVertexAttribDivisor(location, n);
}

VertexAttribPointer& VertexAttribPointer::operator ()(uint _vertex_size, uint _dtype, uint _should_normalize, uint _stride, uint _offset)
{
	vertex_size = _vertex_size;
	dtype = _dtype;
	should_normalize = _should_normalize;
	stride = _stride;
	offset = _offset;
	return *this;
}

bool VertexAttribPointer::empty()const
{
	return (vertex_size == 0 || vbo.empty());
}

VertexAttribPointer& VAO::operator [](uint location)
{
	bind();
	if(!(existing_VAOs[_id].map_VAP.count(location)))
	{
		existing_VAOs[_id].map_VAP[location] = VertexAttribPointer(location, 0, GL_FLOAT, GL_FALSE, 0, 0);
	}
	return existing_VAOs[_id].map_VAP[location];
}

unordered_map<uint, VAO::Instance> VAO::existing_VAOs;
uint VAO::active_VAO = 0;

void VAO::init()
{
	if(_id == 0)
	{
		glGenVertexArrays(1, &_id);
		if(_id == 0)
		{
			throw glass::RuntimeError("Failed to create VAO.");
		}
	#ifdef _DEBUG
		cout << "constructing VAO " << _id << endl;
	#endif
		existing_VAOs[_id].n_sources++;
	}
	else if(existing_VAOs.count(_id) == 0 || existing_VAOs[_id].n_sources == 0)
	{
		throw glass::RuntimeError("VAO " + str::str(_id) + " has been destructed.");
	}
}

void VAO::del()
{
	if(existing_VAOs.count(_id) == 0)
	{
		_id = 0;
		return;
	}

	uint count = existing_VAOs[_id].n_sources;
	if(count > 0)
	{
		existing_VAOs[_id].n_sources--;
		if(count == 1)
		{
		#ifdef _DEBUG
			cout << "destructing VAO " << _id << endl;
		#endif
			unbind();
			existing_VAOs.erase(_id);
			glDeleteVertexArrays(1, &_id);
		}
	}
	_id = 0;
}

VAO::VAO() {}

VAO::VAO(const VAO& vao) :
_id(vao._id)
{
	if(existing_VAOs.count(_id) && existing_VAOs[_id].n_sources > 0)
	{
		existing_VAOs[_id].n_sources++;
	}
}

VAO::VAO(VAO&& vao) :
_id(move(vao._id))
{	
	vao._id = 0;
}

VAO& VAO::operator =(const VAO& vao)
{
	if(this != &vao && _id != vao._id)
	{
		del();

		_id = vao._id;

		if(existing_VAOs.count(_id) && existing_VAOs[_id].n_sources > 0)
		{
			existing_VAOs[_id].n_sources++;
		}
	}
	return *this;
}

VAO& VAO::operator =(VAO&& vao)
{
	if(this != &vao)
	{
		if(_id != vao._id)
		{
			del();
		}

		_id = move(vao._id);
		vao._id = 0;
	}
	return *this;
}

VAO::~VAO()
{
	del();
}

void VAO::bind()
{
	init();
	glBindVertexArray(_id);
	active_VAO = _id;
}

void VAO::unbind()const
{
	if(active_VAO == _id)
	{
		glBindVertexArray(0);
		active_VAO = 0;
	}
}

bool VAO::isBind()const
{
	return (_id != 0 && existing_VAOs.count(_id) && existing_VAOs[_id].n_sources > 0 && active_VAO == _id);
}

void VAO::setEBO(const EBO& _ebo)
{
	bind();
	existing_VAOs[_id].ebo = _ebo;
	existing_VAOs[_id].ebo.bind();
}

bool VAO::empty()const
{
	return (_id == 0 || existing_VAOs.count(_id) == 0 || existing_VAOs[_id].n_sources == 0 || existing_VAOs[_id].map_VAP.empty());
}

void VAO::drawArrays(uint n, DrawType type)
{
	if(empty() || n == 0)
	{
		return;
	}

	bind();
	glDrawArrays(type, 0, n);
}

void VAO::drawElements(DrawType type)
{
	if(empty() || existing_VAOs[_id].ebo.empty())
	{
		return;
	}

	bind();
	glDrawElements(type, existing_VAOs[_id].ebo.size()/sizeof(uint), GL_UNSIGNED_INT, 0);
}

EBO VAO::ebo()const
{
	if(_id == 0 || existing_VAOs.count(_id) == 0)
	{
		return EBO();
	}

	return existing_VAOs[_id].ebo;
}