#include "glass/VBO"

#include <iostream>

using namespace std;

VBO::VBO(): BO(BO::VERTEX) {}

VBO::VBO(const VBO& vbo) : BO(vbo) {}

VBO::VBO(VBO&& vbo) : BO(move(vbo)) {}

VBO& VBO::operator =(const VBO& vbo)
{
	if(this != &vbo && self != vbo.self)
	{
		return static_cast<VBO&>(BO::operator=(vbo));
	}
	else
	{
		return *this;
	}
}

VBO& VBO::operator =(VBO&& vbo)
{
	if(this != &vbo)
	{
		return static_cast<VBO&>(BO::operator=(move(vbo)));
	}
	else
	{
		return *this;
	}
}
