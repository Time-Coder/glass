#include "glass/RBO"
#include "glass/utils/exceptions.h"

using namespace std;

unordered_map<uint, RBO::RBO_Instance> RBO::existing_RBOs;

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
	if(&rbo != this && _id != rbo._id)
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

uint RBO::width()const
{
	return existing_RBOs.count(_id) ? existing_RBOs[_id].width : 0;
}

uint RBO::height()const
{
	return existing_RBOs.count(_id) ? existing_RBOs[_id].height : 0;
}

uint RBO::dtype()const
{
	return existing_RBOs.count(_id) ? existing_RBOs[_id].height : GL_DEPTH24_STENCIL8;
}

bool RBO::empty()const
{
	return (BO::empty() || existing_RBOs.count(_id) == 0 || existing_RBOs[_id].width == 0 || existing_RBOs[_id].height == 0);
}

void RBO::malloc(uint _width, uint _height, uint _dtype)
{
	if(_width == width() && _height == height() && _dtype == dtype())
	{
		return;
	}

	bind();
	glRenderbufferStorage(GL_RENDERBUFFER, _dtype, _width, _height);
	existing_RBOs[_id].width = _width;
	existing_RBOs[_id].height = _height;
	existing_RBOs[_id].dtype = _dtype;
}

void RBO::realloc(uint _width, uint _height)
{
	if(_width == width() && _height == height())
	{
		return;
	}

	bind();
	glRenderbufferStorage(GL_RENDERBUFFER, dtype(), _width, _height);
	existing_RBOs[_id].width = _width;
	existing_RBOs[_id].height = _height;
}