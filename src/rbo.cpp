#include "glass/RBO"
#include "glass/utils/exceptions.h"
#include "glass/utils/helper.h"

using namespace std;

multiset<uint> RBO::existing_RBOs;
uint RBO::active_RBO = 0;

void RBO::init()
{
	if(_id == 0)
	{
		glGenRenderbuffers(1, &_id);
		if(_id == 0)
		{
			throw glass::RuntimeError("Failed to create RBO!");
		}
	#ifdef _DEBUG
		cout << "constructing RBO " << _id << endl;
	#endif
		existing_RBOs.insert(_id);
	}
	else if(existing_RBOs.count(_id) == 0)
	{
		throw glass::RuntimeError("RBO " + str::str(_id) + " has already been destructed!");
	}
}

void RBO::del()
{
	uint count = existing_RBOs.count(_id);
	if(count > 0)
	{
		multiset_pop(existing_RBOs, _id);
		if(count == 1)
		{
		#ifdef _DEBUG
			cout << "destructing RBO " << _id << endl;
		#endif
			unbind();
			glDeleteRenderbuffers(1, &_id);
		}
	}
}

RBO::RBO(uint __width, uint __height, uint __dtype)
{
	this->malloc(__width, __height, __dtype);
}

RBO::RBO() {}

RBO::RBO(const RBO& rbo) :
_id(rbo._id), _width(rbo._width), _height(rbo._height), _dtype(rbo._dtype)
{
	if(existing_RBOs.count(_id) > 0)
	{
		existing_RBOs.insert(_id);
	}
}

RBO::RBO(RBO&& rbo) :
_id(move(rbo._id)), _width(move(rbo._width)), _height(move(rbo._height)), _dtype(move(rbo._dtype))
{
	rbo._id = 0;
}

RBO::~RBO()
{
	del();
}

RBO& RBO::operator =(const RBO& rbo)
{
	if(&rbo != this && _id != rbo._id)
	{
		del();

		_id = rbo._id;
		_width = rbo._width;
		_height = rbo._height;
		_dtype = rbo._dtype;

		if(existing_RBOs.count(_id) > 0)
		{
			existing_RBOs.insert(_id);
		}
	}
	return *this;
}

RBO& RBO::operator =(RBO&& rbo)
{
	if(&rbo != this)
	{
		del();

		_id = move(rbo._id);
		_width = move(rbo._width);
		_height = move(rbo._height);
		_dtype = move(rbo._dtype);

		rbo._id = 0;
	}
	return *this;
}

uint RBO::id()const
{
	return _id;
}

uint RBO::width()const
{
	return _width;
}

uint RBO::height()const
{
	return _height;
}

uint RBO::dtype()const
{
	return _dtype;
}

void RBO::bind()
{
	init();
	glBindRenderbuffer(GL_RENDERBUFFER, _id);
	active_RBO = _id;
}

void RBO::unbind()const
{
	if(active_RBO == _id)
	{
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		active_RBO = 0;
	}
}

bool RBO::empty()const
{
	return (existing_RBOs.count(_id) == 0 || _width == 0 || _height == 0);
}

void RBO::clear()
{
	if(empty())
	{
		return;
	}

	unbind();

	existing_RBOs.erase(_id);
#ifdef _DEBUG
	cout << "destructing RBO " << _id << endl;
#endif
	glDeleteRenderbuffers(1, &_id);

	_id = 0;
	_width = 0;
	_height = 0;
	_dtype = GL_DEPTH24_STENCIL8;
}

void RBO::malloc(uint __width, uint __height, uint __dtype)
{
	if(__width == _width && __height == _height && __dtype == _dtype)
	{
		return;
	}

	bind();
	glRenderbufferStorage(GL_RENDERBUFFER, __dtype, __width, __height);
	_width = __width;
	_height = __height;
	_dtype = __dtype;
}

void RBO::realloc(uint __width, uint __height)
{
	if(__width == _width && __height == _height)
	{
		return;
	}

	bind();
	glRenderbufferStorage(GL_RENDERBUFFER, _dtype, __width, __height);
	_width = __width;
	_height = __height;
}