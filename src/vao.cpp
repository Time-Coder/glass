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
		throw glass::Exception("Empty attrib for VBO!");
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

void VertexAttribPointer::update_policy(uint n)
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
	if(!(map_VAP.count(location)))
	{
		map_VAP[location] = VertexAttribPointer(location, 0, GL_FLOAT, GL_FALSE, 0, 0);
	}
	return map_VAP[location];
}

multiset<uint> VAO::existing_VAOs;
uint VAO::active_VAO = 0;

void VAO::init()
{
	if(_id == 0)
	{
		glGenVertexArrays(1, &_id);
		if(_id == 0)
		{
			throw glass::MemoryError("Failed to create VAO!");
		}
	#ifdef _DEBUG
		cout << "constructing VAO " << _id << endl;
	#endif
		existing_VAOs.insert(_id);
	}
	else if(existing_VAOs.count(_id) == 0)
	{
		throw glass::RuntimeError("VAO " + str::str(_id) + " has been destructed.");
	}
}

void VAO::del()
{
	uint count = existing_VAOs.count(_id);
	if(count > 0)
	{
		multiset_pop(existing_VAOs, _id);
		if(count == 1)
		{
		#ifdef _DEBUG
			cout << "destructing VAO " << _id << endl;
		#endif
			unbind();
			glDeleteVertexArrays(1, &_id);
		}
	}
}

VAO::VAO() {}

VAO::VAO(const VAO& vao) :
_id(vao._id), _ebo(vao._ebo), map_VAP(vao.map_VAP)
{
	if(existing_VAOs.count(_id) > 0)
	{
		existing_VAOs.insert(_id);
	}
}

VAO::VAO(VAO&& vao) :
_id(move(vao._id)), _ebo(move(vao._ebo)), map_VAP(move(vao.map_VAP))
{	
	vao._id = 0;
}

VAO& VAO::operator =(const VAO& vao)
{
	if(this != &vao && _id != vao._id)
	{
		del();

		_id = vao._id;
		_ebo = vao._ebo;
		map_VAP = vao.map_VAP;

		if(existing_VAOs.count(_id) > 0)
		{
			existing_VAOs.insert(_id);
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
		_ebo = move(vao._ebo);
		map_VAP = move(vao.map_VAP);
		
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

void VAO::addEBO(const EBO& __ebo)
{
	_ebo = __ebo;

	bind();
	_ebo.bind();
}

bool VAO::empty()const
{
	return (existing_VAOs.count(_id) == 0 || map_VAP.empty());
}

void VAO::drawArrays(uint n, GLenum type)
{
	if(empty() || n == 0)
	{
		return;
	}

	bind();
	glDrawArrays(type, 0, n);
}

void VAO::drawElements(GLenum type)
{
	if(empty() || _ebo.empty())
	{
		return;
	}

	bind();
	glDrawElements(type, _ebo.size(), GL_UNSIGNED_INT, 0);
}

EBO VAO::ebo()const
{
	return _ebo;
}