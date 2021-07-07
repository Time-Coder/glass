#include "glass/RBO"
#include "glass/utils/exceptions.h"

using namespace std;

void RBO::del()
{
	delWithUserData<RBO_Instance>();
}

RBO::~RBO()
{
	this->del();
}

RBO::RBO(uint _width, uint _height, uint _dtype) : 
BO(RENDER)
{
	this->malloc(_width, _height, _dtype);
}

RBO::RBO() : BO(RENDER) {}

RBO::RBO(const RBO& rbo) : BO(rbo) {}

RBO::RBO(RBO&& rbo) : BO(move(rbo)) {}

RBO& RBO::operator =(const RBO& rbo)
{
	if(&rbo != this && self != rbo.self)
	{
		return static_cast<RBO&>(BO::operator=(rbo));
	}
	return *this;
}

RBO& RBO::operator =(RBO&& rbo)
{
	if(&rbo != this)
	{
		return static_cast<RBO&>(BO::operator=(move(rbo)));
	}
	return *this;
}

uint RBO::width()
{
	return userData<RBO_Instance>()->width;
}

uint RBO::height()
{
	return userData<RBO_Instance>()->height;
}

uint RBO::dtype()
{
	return userData<RBO_Instance>()->height;
}

bool RBO::empty()
{
	return (BO::empty() || userData<RBO_Instance>()->width == 0 || userData<RBO_Instance>()->height == 0);
}

void RBO::malloc(uint _width, uint _height, uint _dtype)
{
	if(_width == width() && _height == height() && _dtype == dtype())
	{
		return;
	}

	bind();
	glRenderbufferStorage(GL_RENDERBUFFER, _dtype, _width, _height);
	userData<RBO_Instance>()->width = _width;
	userData<RBO_Instance>()->height = _height;
	userData<RBO_Instance>()->dtype = _dtype;
}

void RBO::realloc(uint _width, uint _height)
{
	if(_width == width() && _height == height())
	{
		return;
	}

	bind();
	glRenderbufferStorage(GL_RENDERBUFFER, dtype(), _width, _height);
	userData<RBO_Instance>()->width = _width;
	userData<RBO_Instance>()->height = _height;
}