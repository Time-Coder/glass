#include "glvector.h"

void GLVector::init(const string& __dtype)
{
	if(vec) return;

	if(__dtype == "bool") vec = (void*)(new vector<byte>());
	else if(__dtype == "uint") vec = (void*)(new vector<uint>());
	else if(__dtype == "int") vec = (void*)(new vector<int>());
	else if(__dtype == "float") vec = (void*)(new vector<float>());
	else if(__dtype == "double") vec = (void*)(new vector<double>());
	else if(__dtype == "bvec2") vec = (void*)(new vector<bvec2>());
	else if(__dtype == "bvec3") vec = (void*)(new vector<bvec3>());
	else if(__dtype == "bvec4") vec = (void*)(new vector<bvec4>());
	else if(__dtype == "ivec2") vec = (void*)(new vector<ivec2>());
	else if(__dtype == "ivec3") vec = (void*)(new vector<ivec3>());
	else if(__dtype == "ivec4") vec = (void*)(new vector<ivec4>());
	else if(__dtype == "uvec2") vec = (void*)(new vector<uvec2>());
	else if(__dtype == "uvec3") vec = (void*)(new vector<uvec3>());
	else if(__dtype == "uvec4") vec = (void*)(new vector<uvec4>());
	else if(__dtype == "vec2") vec = (void*)(new vector<vec2>());
	else if(__dtype == "vec3") vec = (void*)(new vector<vec3>());
	else if(__dtype == "vec4") vec = (void*)(new vector<vec4>());
	else if(__dtype == "dvec2") vec = (void*)(new vector<dvec2>());
	else if(__dtype == "dvec3") vec = (void*)(new vector<dvec3>());
	else if(__dtype == "dvec4") vec = (void*)(new vector<dvec4>());
	else if(__dtype == "mat2") vec = (void*)(new vector<mat2>());
	else if(__dtype == "mat3") vec = (void*)(new vector<mat3>());
	else if(__dtype == "mat4") vec = (void*)(new vector<mat4>());
	else throw TypeError("Not supported DataType " + __dtype + ".");

	if(!vec) throw MemoryError("Failed to allocate memory!");

	_dtype = __dtype;
}

void GLVector::clear()
{
	if(!vec)
	{
		_dtype.clear();
		return;
	}

	if(_dtype == "bool") delete (vector<byte>*)(vec);
	else if(_dtype == "uint") delete (vector<uint>*)(vec);
	else if(_dtype == "int") delete (vector<int>*)(vec);
	else if(_dtype == "float") delete (vector<float>*)(vec);
	else if(_dtype == "double") delete (vector<double>*)(vec);
	else if(_dtype == "bvec2") delete (vector<bvec2>*)(vec);
	else if(_dtype == "bvec3") delete (vector<bvec3>*)(vec);
	else if(_dtype == "bvec4") delete (vector<bvec4>*)(vec);
	else if(_dtype == "ivec2") delete (vector<ivec2>*)(vec);
	else if(_dtype == "ivec3") delete (vector<ivec3>*)(vec);
	else if(_dtype == "ivec4") delete (vector<ivec4>*)(vec);
	else if(_dtype == "uvec2") delete (vector<uvec2>*)(vec);
	else if(_dtype == "uvec3") delete (vector<uvec3>*)(vec);
	else if(_dtype == "uvec4") delete (vector<uvec4>*)(vec);
	else if(_dtype == "vec2") delete (vector<vec2>*)(vec);
	else if(_dtype == "vec3") delete (vector<vec3>*)(vec);
	else if(_dtype == "vec4") delete (vector<vec4>*)(vec);
	else if(_dtype == "dvec2") delete (vector<dvec2>*)(vec);
	else if(_dtype == "dvec3") delete (vector<dvec3>*)(vec);
	else if(_dtype == "dvec4") delete (vector<dvec4>*)(vec);
	else if(_dtype == "mat2") delete (vector<mat2>*)(vec);
	else if(_dtype == "mat3") delete (vector<mat3>*)(vec);
	else if(_dtype == "mat4") delete (vector<mat4>*)(vec);
	vec = NULL;
	_dtype.clear();
}

GLVector::GLVector() {}

GLVector::~GLVector()
{
	clear();
}

GLVector::GLVector(const string& __dtype)
{
	init(__dtype);
}

GLVector::GLVector(const string& __dtype, uint n)
{
	if(__dtype == "bool") vec = (void*)(new vector<byte>(n));
	else if(__dtype == "uint") vec = (void*)(new vector<uint>(n));
	else if(__dtype == "int") vec = (void*)(new vector<int>(n));
	else if(__dtype == "float") vec = (void*)(new vector<float>(n));
	else if(__dtype == "double") vec = (void*)(new vector<double>(n));
	else if(__dtype == "bvec2") vec = (void*)(new vector<bvec2>(n));
	else if(__dtype == "bvec3") vec = (void*)(new vector<bvec3>(n));
	else if(__dtype == "bvec4") vec = (void*)(new vector<bvec4>(n));
	else if(__dtype == "ivec2") vec = (void*)(new vector<ivec2>(n));
	else if(__dtype == "ivec3") vec = (void*)(new vector<ivec3>(n));
	else if(__dtype == "ivec4") vec = (void*)(new vector<ivec4>(n));
	else if(__dtype == "uvec2") vec = (void*)(new vector<uvec2>(n));
	else if(__dtype == "uvec3") vec = (void*)(new vector<uvec3>(n));
	else if(__dtype == "uvec4") vec = (void*)(new vector<uvec4>(n));
	else if(__dtype == "vec2") vec = (void*)(new vector<vec2>(n));
	else if(__dtype == "vec3") vec = (void*)(new vector<vec3>(n));
	else if(__dtype == "vec4") vec = (void*)(new vector<vec4>(n));
	else if(__dtype == "dvec2") vec = (void*)(new vector<dvec2>(n));
	else if(__dtype == "dvec3") vec = (void*)(new vector<dvec3>(n));
	else if(__dtype == "dvec4") vec = (void*)(new vector<dvec4>(n));
	else if(__dtype == "mat2") vec = (void*)(new vector<mat2>(n));
	else if(__dtype == "mat3") vec = (void*)(new vector<mat3>(n));
	else if(__dtype == "mat4") vec = (void*)(new vector<mat4>(n));
	else throw TypeError("Not supported DataType " + __dtype + ".");

	if(!vec) throw MemoryError("Failed to allocate memory!");

	_dtype = __dtype;
}

GLVector::GLVector(const GLVector& new_vec) :
_dtype(new_vec._dtype)
{
	if(new_vec.vec == NULL) return;

	if(_dtype == "bool") vec = (void*)(new vector<byte>(*((vector<byte>*)(new_vec.vec))));
	else if(_dtype == "uint") vec = (void*)(new vector<uint>(*((vector<uint>*)(new_vec.vec))));
	else if(_dtype == "int") vec = (void*)(new vector<int>(*((vector<int>*)(new_vec.vec))));
	else if(_dtype == "float") vec = (void*)(new vector<float>(*((vector<float>*)(new_vec.vec))));
	else if(_dtype == "double") vec = (void*)(new vector<double>(*((vector<double>*)(new_vec.vec))));
	else if(_dtype == "bvec2") vec = (void*)(new vector<bvec2>(*((vector<bvec2>*)(new_vec.vec))));
	else if(_dtype == "bvec3") vec = (void*)(new vector<bvec3>(*((vector<bvec3>*)(new_vec.vec))));
	else if(_dtype == "bvec4") vec = (void*)(new vector<bvec4>(*((vector<bvec4>*)(new_vec.vec))));
	else if(_dtype == "ivec2") vec = (void*)(new vector<ivec2>(*((vector<ivec2>*)(new_vec.vec))));
	else if(_dtype == "ivec3") vec = (void*)(new vector<ivec3>(*((vector<ivec3>*)(new_vec.vec))));
	else if(_dtype == "ivec4") vec = (void*)(new vector<ivec4>(*((vector<ivec4>*)(new_vec.vec))));
	else if(_dtype == "uvec2") vec = (void*)(new vector<uvec2>(*((vector<uvec2>*)(new_vec.vec))));
	else if(_dtype == "uvec3") vec = (void*)(new vector<uvec3>(*((vector<uvec3>*)(new_vec.vec))));
	else if(_dtype == "uvec4") vec = (void*)(new vector<uvec4>(*((vector<uvec4>*)(new_vec.vec))));
	else if(_dtype == "vec2") vec = (void*)(new vector<vec2>(*((vector<vec2>*)(new_vec.vec))));
	else if(_dtype == "vec3") vec = (void*)(new vector<vec3>(*((vector<vec3>*)(new_vec.vec))));
	else if(_dtype == "vec4") vec = (void*)(new vector<vec4>(*((vector<vec4>*)(new_vec.vec))));
	else if(_dtype == "dvec2") vec = (void*)(new vector<dvec2>(*((vector<dvec2>*)(new_vec.vec))));
	else if(_dtype == "dvec3") vec = (void*)(new vector<dvec3>(*((vector<dvec3>*)(new_vec.vec))));
	else if(_dtype == "dvec4") vec = (void*)(new vector<dvec4>(*((vector<dvec4>*)(new_vec.vec))));
	else if(_dtype == "mat2") vec = (void*)(new vector<mat2>(*((vector<mat2>*)(new_vec.vec))));
	else if(_dtype == "mat3") vec = (void*)(new vector<mat3>(*((vector<mat3>*)(new_vec.vec))));
	else if(_dtype == "mat4") vec = (void*)(new vector<mat4>(*((vector<mat4>*)(new_vec.vec))));

	if(!(_dtype.empty()) && !vec) throw MemoryError("Failed to allocate memory!");
}

GLVector::GLVector(GLVector&& new_vec) :
_dtype(move(new_vec._dtype)), vec(move(new_vec.vec))
{
	new_vec._dtype = string();
	new_vec.vec = NULL;
}

// GLVector::GLVector(GLVector::const_iterator& first, GLVector::const_iterator& last)
// {
// 	if(first.parent->empty())
// 	{
// 		return;
// 	}

// 	_dtype = first.parent->_dtype;
// 	if(first >= last)
// 	{
// 		init(_dtype);
// 		return;
// 	}

// 	int i_first = first-first.parent.begin();
// 	int i_last = last-last.parent.begin();
// 	if(_dtype == "bool")
// 	{
// 		vector<byte>* src_vec = (vector<byte>*)(new_vec.vec);
// 		vec = (void*)(new vector<byte>(src_vec->begin()+i_first, src_vec->begin()+i_last));
// 	}
// 	else if(_dtype == "uint")
// 	{
// 		vector<uint>* src_vec = (vector<uint>*)(new_vec.vec);
// 		vec = (void*)(new vector<uint>(src_vec->begin()+i_first, src_vec->begin()+i_last));
// 	}
// 	else if(_dtype == "int")
// 	{
// 		vector<int>* src_vec = (vector<int>*)(new_vec.vec);
// 		vec = (void*)(new vector<int>(src_vec->begin()+i_first, src_vec->begin()+i_last));
// 	}
// 	else if(_dtype == "float")
// 	{
// 		vector<float>* src_vec = (vector<float>*)(new_vec.vec);
// 		vec = (void*)(new vector<float>(src_vec->begin()+i_first, src_vec->begin()+i_last));
// 	}
// 	else if(_dtype == "vec2")
// 	{
// 		vector<vec2>* src_vec = (vector<vec2>*)(new_vec.vec);
// 		vec = (void*)(new vector<vec2>(src_vec->begin()+i_first, src_vec->begin()+i_last));
// 	}
// 	else if(_dtype == "vec3")
// 	{
// 		vector<vec3>* src_vec = (vector<vec3>*)(new_vec.vec);
// 		vec = (void*)(new vector<vec3>(src_vec->begin()+i_first, src_vec->begin()+i_last));
// 	}
// 	else if(_dtype == "vec4")
// 	{
// 		vector<vec4>* src_vec = (vector<vec4>*)(new_vec.vec);
// 		vec = (void*)(new vector<vec4>(src_vec->begin()+i_first, src_vec->begin()+i_last));
// 	}
// 	else if(_dtype == "mat2")
// 	{
// 		vector<mat2>* src_vec = (vector<mat2>*)(new_vec.vec);
// 		vec = (void*)(new vector<mat2>(src_vec->begin()+i_first, src_vec->begin()+i_last));
// 	}
// 	else if(_dtype == "mat3")
// 	{
// 		vector<mat3>* src_vec = (vector<mat3>*)(new_vec.vec);
// 		vec = (void*)(new vector<mat3>(src_vec->begin()+i_first, src_vec->begin()+i_last));
// 	}
// 	else if(_dtype == "mat4")
// 	{
// 		vector<mat4>* src_vec = (vector<mat4>*)(new_vec.vec);
// 		vec = (void*)(new vector<mat4>(src_vec->begin()+i_first, src_vec->begin()+i_last));
// 	}

// 	if(!(_dtype.empty()) && !vec) throw MemoryError("Failed to allocate memory!");
// }

GLVector& GLVector::operator =(const GLVector& new_vec)
{
	if(this == &new_vec)
	{
		return *this;
	}

	if(new_vec.vec == NULL)
	{
		clear();
		_dtype = new_vec._dtype;

		return *this;
	}
	
	if(_dtype == new_vec._dtype && vec != NULL)
	{
		if(_dtype == "bool") *((vector<byte>*)vec) = *((vector<byte>*)(new_vec.vec));
		else if(_dtype == "uint") *((vector<uint>*)vec) = *((vector<uint>*)(new_vec.vec));
		else if(_dtype == "int") *((vector<int>*)vec) = *((vector<int>*)(new_vec.vec));
		else if(_dtype == "float") *((vector<float>*)vec) = *((vector<float>*)(new_vec.vec));
		else if(_dtype == "double") *((vector<double>*)vec) = *((vector<double>*)(new_vec.vec));
		else if(_dtype == "bvec2") *((vector<bvec2>*)vec) = *((vector<bvec2>*)(new_vec.vec));
		else if(_dtype == "bvec3") *((vector<bvec3>*)vec) = *((vector<bvec3>*)(new_vec.vec));
		else if(_dtype == "bvec4") *((vector<bvec4>*)vec) = *((vector<bvec4>*)(new_vec.vec));
		else if(_dtype == "ivec2") *((vector<ivec2>*)vec) = *((vector<ivec2>*)(new_vec.vec));
		else if(_dtype == "ivec3") *((vector<ivec3>*)vec) = *((vector<ivec3>*)(new_vec.vec));
		else if(_dtype == "ivec4") *((vector<ivec4>*)vec) = *((vector<ivec4>*)(new_vec.vec));
		else if(_dtype == "uvec2") *((vector<uvec2>*)vec) = *((vector<uvec2>*)(new_vec.vec));
		else if(_dtype == "uvec3") *((vector<uvec3>*)vec) = *((vector<uvec3>*)(new_vec.vec));
		else if(_dtype == "uvec4") *((vector<uvec4>*)vec) = *((vector<uvec4>*)(new_vec.vec));
		else if(_dtype == "vec2") *((vector<vec2>*)vec) = *((vector<vec2>*)(new_vec.vec));
		else if(_dtype == "vec3") *((vector<vec3>*)vec) = *((vector<vec3>*)(new_vec.vec));
		else if(_dtype == "vec4") *((vector<vec4>*)vec) = *((vector<vec4>*)(new_vec.vec));
		else if(_dtype == "dvec2") *((vector<dvec2>*)vec) = *((vector<dvec2>*)(new_vec.vec));
		else if(_dtype == "dvec3") *((vector<dvec3>*)vec) = *((vector<dvec3>*)(new_vec.vec));
		else if(_dtype == "dvec4") *((vector<dvec4>*)vec) = *((vector<dvec4>*)(new_vec.vec));
		else if(_dtype == "mat2") *((vector<mat2>*)vec) = *((vector<mat2>*)(new_vec.vec));
		else if(_dtype == "mat3") *((vector<mat3>*)vec) = *((vector<mat3>*)(new_vec.vec));
		else if(_dtype == "mat4") *((vector<mat4>*)vec) = *((vector<mat4>*)(new_vec.vec));
	}
	else
	{
		clear();
		_dtype = new_vec._dtype;

		if(_dtype == "bool") vec = (void*)(new vector<byte>(*((vector<byte>*)(new_vec.vec))));
		else if(_dtype == "uint") vec = (void*)(new vector<uint>(*((vector<uint>*)(new_vec.vec))));
		else if(_dtype == "int") vec = (void*)(new vector<int>(*((vector<int>*)(new_vec.vec))));
		else if(_dtype == "float") vec = (void*)(new vector<float>(*((vector<float>*)(new_vec.vec))));
		else if(_dtype == "double") vec = (void*)(new vector<double>(*((vector<double>*)(new_vec.vec))));
		else if(_dtype == "bvec2") vec = (void*)(new vector<bvec2>(*((vector<bvec2>*)(new_vec.vec))));
		else if(_dtype == "bvec3") vec = (void*)(new vector<bvec3>(*((vector<bvec3>*)(new_vec.vec))));
		else if(_dtype == "bvec4") vec = (void*)(new vector<bvec4>(*((vector<bvec4>*)(new_vec.vec))));
		else if(_dtype == "ivec2") vec = (void*)(new vector<ivec2>(*((vector<ivec2>*)(new_vec.vec))));
		else if(_dtype == "ivec3") vec = (void*)(new vector<ivec3>(*((vector<ivec3>*)(new_vec.vec))));
		else if(_dtype == "ivec4") vec = (void*)(new vector<ivec4>(*((vector<ivec4>*)(new_vec.vec))));
		else if(_dtype == "uvec2") vec = (void*)(new vector<uvec2>(*((vector<uvec2>*)(new_vec.vec))));
		else if(_dtype == "uvec3") vec = (void*)(new vector<uvec3>(*((vector<uvec3>*)(new_vec.vec))));
		else if(_dtype == "uvec4") vec = (void*)(new vector<uvec4>(*((vector<uvec4>*)(new_vec.vec))));
		else if(_dtype == "vec2") vec = (void*)(new vector<vec2>(*((vector<vec2>*)(new_vec.vec))));
		else if(_dtype == "vec3") vec = (void*)(new vector<vec3>(*((vector<vec3>*)(new_vec.vec))));
		else if(_dtype == "vec4") vec = (void*)(new vector<vec4>(*((vector<vec4>*)(new_vec.vec))));
		else if(_dtype == "dvec2") vec = (void*)(new vector<dvec2>(*((vector<dvec2>*)(new_vec.vec))));
		else if(_dtype == "dvec3") vec = (void*)(new vector<dvec3>(*((vector<dvec3>*)(new_vec.vec))));
		else if(_dtype == "dvec4") vec = (void*)(new vector<dvec4>(*((vector<dvec4>*)(new_vec.vec))));
		else if(_dtype == "mat2") vec = (void*)(new vector<mat2>(*((vector<mat2>*)(new_vec.vec))));
		else if(_dtype == "mat3") vec = (void*)(new vector<mat3>(*((vector<mat3>*)(new_vec.vec))));
		else if(_dtype == "mat4") vec = (void*)(new vector<mat4>(*((vector<mat4>*)(new_vec.vec))));
		
		if(!(_dtype.empty()) && !vec) throw MemoryError("Failed to allocate memory!");
	}

	return *this;
}

GLVector& GLVector::operator =(GLVector&& new_vec) // reviewed
{
	if(this != &new_vec)
	{
		clear();
		_dtype = move(new_vec._dtype);
		vec = move(new_vec.vec);
	}

	return *this;
}

void GLVector::pop_back() // reviewed
{
	if(empty())
	{
		throw RuntimeError("Try to pop from empty vector.");
	}

	if(_dtype == "bool") ((vector<byte>*)vec)->pop_back();
	else if(_dtype == "uint") ((vector<uint>*)vec)->pop_back();
	else if(_dtype == "int") ((vector<int>*)vec)->pop_back();
	else if(_dtype == "float") ((vector<float>*)vec)->pop_back();
	else if(_dtype == "double") ((vector<double>*)vec)->pop_back();
	else if(_dtype == "bvec2") ((vector<bvec2>*)vec)->pop_back();
	else if(_dtype == "bvec3") ((vector<bvec3>*)vec)->pop_back();
	else if(_dtype == "bvec4") ((vector<bvec4>*)vec)->pop_back();
	else if(_dtype == "ivec2") ((vector<ivec2>*)vec)->pop_back();
	else if(_dtype == "ivec3") ((vector<ivec3>*)vec)->pop_back();
	else if(_dtype == "ivec4") ((vector<ivec4>*)vec)->pop_back();
	else if(_dtype == "uvec2") ((vector<uvec2>*)vec)->pop_back();
	else if(_dtype == "uvec3") ((vector<uvec3>*)vec)->pop_back();
	else if(_dtype == "uvec4") ((vector<uvec4>*)vec)->pop_back();
	else if(_dtype == "vec2") ((vector<vec2>*)vec)->pop_back();
	else if(_dtype == "vec3") ((vector<vec3>*)vec)->pop_back();
	else if(_dtype == "vec4") ((vector<vec4>*)vec)->pop_back();
	else if(_dtype == "dvec2") ((vector<dvec2>*)vec)->pop_back();
	else if(_dtype == "dvec3") ((vector<dvec3>*)vec)->pop_back();
	else if(_dtype == "dvec4") ((vector<dvec4>*)vec)->pop_back();
	else if(_dtype == "mat2") ((vector<mat2>*)vec)->pop_back();
	else if(_dtype == "mat3") ((vector<mat3>*)vec)->pop_back();
	else if(_dtype == "mat4") ((vector<mat4>*)vec)->pop_back();
}

void GLVector::pop_front() // reviewed
{
	if(empty())
	{
		throw RuntimeError("Try to pop from empty vector.");
	}

	if(_dtype == "bool") ((vector<byte>*)vec)->erase(((vector<byte>*)vec)->begin());
	else if(_dtype == "uint") ((vector<uint>*)vec)->erase(((vector<uint>*)vec)->begin());
	else if(_dtype == "int") ((vector<int>*)vec)->erase(((vector<int>*)vec)->begin());
	else if(_dtype == "float") ((vector<float>*)vec)->erase(((vector<float>*)vec)->begin());
	else if(_dtype == "double") ((vector<double>*)vec)->erase(((vector<double>*)vec)->begin());
	else if(_dtype == "bvec2") ((vector<bvec2>*)vec)->erase(((vector<bvec2>*)vec)->begin());
	else if(_dtype == "bvec3") ((vector<bvec3>*)vec)->erase(((vector<bvec3>*)vec)->begin());
	else if(_dtype == "bvec4") ((vector<bvec4>*)vec)->erase(((vector<bvec4>*)vec)->begin());
	else if(_dtype == "ivec2") ((vector<ivec2>*)vec)->erase(((vector<ivec2>*)vec)->begin());
	else if(_dtype == "ivec3") ((vector<ivec3>*)vec)->erase(((vector<ivec3>*)vec)->begin());
	else if(_dtype == "ivec4") ((vector<ivec4>*)vec)->erase(((vector<ivec4>*)vec)->begin());
	else if(_dtype == "uvec2") ((vector<uvec2>*)vec)->erase(((vector<uvec2>*)vec)->begin());
	else if(_dtype == "uvec3") ((vector<uvec3>*)vec)->erase(((vector<uvec3>*)vec)->begin());
	else if(_dtype == "uvec4") ((vector<uvec4>*)vec)->erase(((vector<uvec4>*)vec)->begin());
	else if(_dtype == "vec2") ((vector<vec2>*)vec)->erase(((vector<vec2>*)vec)->begin());
	else if(_dtype == "vec3") ((vector<vec3>*)vec)->erase(((vector<vec3>*)vec)->begin());
	else if(_dtype == "vec4") ((vector<vec4>*)vec)->erase(((vector<vec4>*)vec)->begin());
	else if(_dtype == "dvec2") ((vector<dvec2>*)vec)->erase(((vector<dvec2>*)vec)->begin());
	else if(_dtype == "dvec3") ((vector<dvec3>*)vec)->erase(((vector<dvec3>*)vec)->begin());
	else if(_dtype == "dvec4") ((vector<dvec4>*)vec)->erase(((vector<dvec4>*)vec)->begin());
	else if(_dtype == "mat2") ((vector<mat2>*)vec)->erase(((vector<mat2>*)vec)->begin());
	else if(_dtype == "mat3") ((vector<mat3>*)vec)->erase(((vector<mat3>*)vec)->begin());
	else if(_dtype == "mat4") ((vector<mat4>*)vec)->erase(((vector<mat4>*)vec)->begin());
}

void GLVector::pop(int i)
{
	if(empty())
	{
		throw RuntimeError("Try to pop from empty vector.");
	}
	int length = size();
	if(i < 0)
	{
		i += length;
	}
	if(i < 0 || i >= length)
	{
		throw IndexError(i, 0, length-1);
	}

	if(_dtype == "bool") ((vector<byte>*)vec)->erase(((vector<byte>*)vec)->begin()+i);
	else if(_dtype == "uint") ((vector<uint>*)vec)->erase(((vector<uint>*)vec)->begin()+i);
	else if(_dtype == "int") ((vector<int>*)vec)->erase(((vector<int>*)vec)->begin()+i);
	else if(_dtype == "float") ((vector<float>*)vec)->erase(((vector<float>*)vec)->begin()+i);
	else if(_dtype == "double") ((vector<double>*)vec)->erase(((vector<double>*)vec)->begin()+i);
	else if(_dtype == "bvec2") ((vector<bvec2>*)vec)->erase(((vector<bvec2>*)vec)->begin()+i);
	else if(_dtype == "bvec3") ((vector<bvec3>*)vec)->erase(((vector<bvec3>*)vec)->begin()+i);
	else if(_dtype == "bvec4") ((vector<bvec4>*)vec)->erase(((vector<bvec4>*)vec)->begin()+i);
	else if(_dtype == "ivec2") ((vector<ivec2>*)vec)->erase(((vector<ivec2>*)vec)->begin()+i);
	else if(_dtype == "ivec3") ((vector<ivec3>*)vec)->erase(((vector<ivec3>*)vec)->begin()+i);
	else if(_dtype == "ivec4") ((vector<ivec4>*)vec)->erase(((vector<ivec4>*)vec)->begin()+i);
	else if(_dtype == "uvec2") ((vector<uvec2>*)vec)->erase(((vector<uvec2>*)vec)->begin()+i);
	else if(_dtype == "uvec3") ((vector<uvec3>*)vec)->erase(((vector<uvec3>*)vec)->begin()+i);
	else if(_dtype == "uvec4") ((vector<uvec4>*)vec)->erase(((vector<uvec4>*)vec)->begin()+i);
	else if(_dtype == "vec2") ((vector<vec2>*)vec)->erase(((vector<vec2>*)vec)->begin()+i);
	else if(_dtype == "vec3") ((vector<vec3>*)vec)->erase(((vector<vec3>*)vec)->begin()+i);
	else if(_dtype == "vec4") ((vector<vec4>*)vec)->erase(((vector<vec4>*)vec)->begin()+i);
	else if(_dtype == "dvec2") ((vector<dvec2>*)vec)->erase(((vector<dvec2>*)vec)->begin()+i);
	else if(_dtype == "dvec3") ((vector<dvec3>*)vec)->erase(((vector<dvec3>*)vec)->begin()+i);
	else if(_dtype == "dvec4") ((vector<dvec4>*)vec)->erase(((vector<dvec4>*)vec)->begin()+i);
	else if(_dtype == "mat2") ((vector<mat2>*)vec)->erase(((vector<mat2>*)vec)->begin()+i);
	else if(_dtype == "mat3") ((vector<mat3>*)vec)->erase(((vector<mat3>*)vec)->begin()+i);
	else if(_dtype == "mat4") ((vector<mat4>*)vec)->erase(((vector<mat4>*)vec)->begin()+i);
}

// void GLVector::pop(GLVector::const_iterator& it) // reviewed
// {
// 	int i = it - begin();
// 	pop(i);
// }

int GLVector::erase(int i, int len) // reviewed
{
	if(empty())
	{
		return 0;
	}
	int length = size();
	int first = i;
	if(first < 0)
	{
		first += length;
	}

	int last = i + len;
	if(first > last)
	{
		std::swap(first, last);
	}

	if(first < 0)
	{
		first = 0;
	}
	if(first >= length)
	{
		return length;
	}

	if(last < 0)
	{
		return 0;
	}
	if(last > length)
	{
		last = length;
	}

	if(_dtype == "bool") ((vector<byte>*)vec)->erase(((vector<byte>*)vec)->begin()+first, ((vector<byte>*)vec)->begin()+last);
	else if(_dtype == "uint") ((vector<uint>*)vec)->erase(((vector<uint>*)vec)->begin()+first, ((vector<uint>*)vec)->begin()+last);
	else if(_dtype == "int") ((vector<int>*)vec)->erase(((vector<int>*)vec)->begin()+first, ((vector<int>*)vec)->begin()+last);
	else if(_dtype == "float") ((vector<float>*)vec)->erase(((vector<float>*)vec)->begin()+first, ((vector<float>*)vec)->begin()+last);
	else if(_dtype == "double") ((vector<double>*)vec)->erase(((vector<double>*)vec)->begin()+first, ((vector<double>*)vec)->begin()+last);
	else if(_dtype == "bvec2") ((vector<bvec2>*)vec)->erase(((vector<bvec2>*)vec)->begin()+first, ((vector<bvec2>*)vec)->begin()+last);
	else if(_dtype == "bvec3") ((vector<bvec3>*)vec)->erase(((vector<bvec3>*)vec)->begin()+first, ((vector<bvec3>*)vec)->begin()+last);
	else if(_dtype == "bvec4") ((vector<bvec4>*)vec)->erase(((vector<bvec4>*)vec)->begin()+first, ((vector<bvec4>*)vec)->begin()+last);
	else if(_dtype == "ivec2") ((vector<ivec2>*)vec)->erase(((vector<ivec2>*)vec)->begin()+first, ((vector<ivec2>*)vec)->begin()+last);
	else if(_dtype == "ivec3") ((vector<ivec3>*)vec)->erase(((vector<ivec3>*)vec)->begin()+first, ((vector<ivec3>*)vec)->begin()+last);
	else if(_dtype == "ivec4") ((vector<ivec4>*)vec)->erase(((vector<ivec4>*)vec)->begin()+first, ((vector<ivec4>*)vec)->begin()+last);
	else if(_dtype == "uvec2") ((vector<uvec2>*)vec)->erase(((vector<uvec2>*)vec)->begin()+first, ((vector<uvec2>*)vec)->begin()+last);
	else if(_dtype == "uvec3") ((vector<uvec3>*)vec)->erase(((vector<uvec3>*)vec)->begin()+first, ((vector<uvec3>*)vec)->begin()+last);
	else if(_dtype == "uvec4") ((vector<uvec4>*)vec)->erase(((vector<uvec4>*)vec)->begin()+first, ((vector<uvec4>*)vec)->begin()+last);
	else if(_dtype == "vec2") ((vector<vec2>*)vec)->erase(((vector<vec2>*)vec)->begin()+first, ((vector<vec2>*)vec)->begin()+last);
	else if(_dtype == "vec3") ((vector<vec3>*)vec)->erase(((vector<vec3>*)vec)->begin()+first, ((vector<vec3>*)vec)->begin()+last);
	else if(_dtype == "vec4") ((vector<vec4>*)vec)->erase(((vector<vec4>*)vec)->begin()+first, ((vector<vec4>*)vec)->begin()+last);
	else if(_dtype == "dvec2") ((vector<dvec2>*)vec)->erase(((vector<dvec2>*)vec)->begin()+first, ((vector<dvec2>*)vec)->begin()+last);
	else if(_dtype == "dvec3") ((vector<dvec3>*)vec)->erase(((vector<dvec3>*)vec)->begin()+first, ((vector<dvec3>*)vec)->begin()+last);
	else if(_dtype == "dvec4") ((vector<dvec4>*)vec)->erase(((vector<dvec4>*)vec)->begin()+first, ((vector<dvec4>*)vec)->begin()+last);
	else if(_dtype == "mat2") ((vector<mat2>*)vec)->erase(((vector<mat2>*)vec)->begin()+first, ((vector<mat2>*)vec)->begin()+last);
	else if(_dtype == "mat3") ((vector<mat3>*)vec)->erase(((vector<mat3>*)vec)->begin()+first, ((vector<mat3>*)vec)->begin()+last);
	else if(_dtype == "mat4") ((vector<mat4>*)vec)->erase(((vector<mat4>*)vec)->begin()+first, ((vector<mat4>*)vec)->begin()+last);

	return last;
}

// GLVector::iterator GLVector::erase(GLVector::const_iterator& it, int len) // reviewed
// {
// 	int first = it-begin();
// 	int last = erase(first, len);
// 	return begin() + (last - first);
// }

// GLVector::iterator GLVector::erase(GLVector::const_iterator& first, GLVector::const_iterator& last) // reviewed
// {
// 	return begin() + erase(first-begin(), last-first);
// }

// GLVector::iterator GLVector::insert(GLVector::const_iterator& it, GLVector::const_iterator& first, GLVector::const_iterator& last)
// {
// 	int i = it - begin();
// 	insert(i, first, last);
// 	return begin() + i;
// }

// int GLVector::insert(int i, GLVector::const_iterator& first, GLVector::const_iterator& last)
// {
// 	int length = size();
// 	if(i < 0 || i > length)
// 	{
// 		throw IndexError(i, 0, length);
// 	}

// 	if(first >= last)
// 	{
// 		return i;
// 	}

// 	init(first.parent->_dtype);
// 	if(_dtype != first.parent->_dtype)
// 	{
// 		throw TypeError("Try to insert vector<" + first.parent->_dtype + "> to vector<" + _dtype + ">");
// 	}

// 	int i_first = first - first.parent->begin();
// 	int i_last = last - last.parent->begin();

// 	if(_dtype == "bool")
// 	{
// 		vector<byte>* dest_vec = (vector<byte>*)vec;
// 		vector<byte>* src_vec = (vector<byte>*)(first.parent->vec);
// 		dest_vec->insert(dest_vec->begin()+i, src_vec->begin()+i_first, src_vec->begin()+i_last);
// 	}
// 	else if(_dtype == "uint")
// 	{
// 		vector<uint>* dest_vec = (vector<uint>*)vec;
// 		vector<uint>* src_vec = (vector<uint>*)(first.parent->vec);
// 		dest_vec->insert(dest_vec->begin()+i, src_vec->begin()+i_first, src_vec->begin()+i_last);
// 	}
// 	else if(_dtype == "int")
// 	{
// 		vector<int>* dest_vec = (vector<int>*)vec;
// 		vector<int>* src_vec = (vector<int>*)(first.parent->vec);
// 		dest_vec->insert(dest_vec->begin()+i, src_vec->begin()+i_first, src_vec->begin()+i_last);
// 	}
// 	else if(_dtype == "float")
// 	{
// 		vector<float>* dest_vec = (vector<float>*)vec;
// 		vector<float>* src_vec = (vector<float>*)(first.parent->vec);
// 		dest_vec->insert(dest_vec->begin()+i, src_vec->begin()+i_first, src_vec->begin()+i_last);
// 	}
// 	else if(_dtype == "vec2")
// 	{
// 		vector<vec2>* dest_vec = (vector<vec2>*)vec;
// 		vector<vec2>* src_vec = (vector<vec2>*)(first.parent->vec);
// 		dest_vec->insert(dest_vec->begin()+i, src_vec->begin()+i_first, src_vec->begin()+i_last);
// 	}
// 	else if(_dtype == "vec3")
// 	{
// 		vector<vec3>* dest_vec = (vector<vec3>*)vec;
// 		vector<vec3>* src_vec = (vector<vec3>*)(first.parent->vec);
// 		dest_vec->insert(dest_vec->begin()+i, src_vec->begin()+i_first, src_vec->begin()+i_last);
// 	}
// 	else if(_dtype == "vec4")
// 	{
// 		vector<vec4>* dest_vec = (vector<vec4>*)vec;
// 		vector<vec4>* src_vec = (vector<vec4>*)(first.parent->vec);
// 		dest_vec->insert(dest_vec->begin()+i, src_vec->begin()+i_first, src_vec->begin()+i_last);
// 	}
// 	else if(_dtype == "mat2")
// 	{
// 		vector<mat2>* dest_vec = (vector<mat2>*)vec;
// 		vector<mat2>* src_vec = (vector<mat2>*)(first.parent->vec);
// 		dest_vec->insert(dest_vec->begin()+i, src_vec->begin()+i_first, src_vec->begin()+i_last);
// 	}
// 	else if(_dtype == "mat3")
// 	{
// 		vector<mat3>* dest_vec = (vector<mat3>*)vec;
// 		vector<mat3>* src_vec = (vector<mat3>*)(first.parent->vec);
// 		dest_vec->insert(dest_vec->begin()+i, src_vec->begin()+i_first, src_vec->begin()+i_last);
// 	}
// 	else if(_dtype == "mat4")
// 	{
// 		vector<mat4>* dest_vec = (vector<mat4>*)vec;
// 		vector<mat4>* src_vec = (vector<mat4>*)(first.parent->vec);
// 		dest_vec->insert(dest_vec->begin()+i, src_vec->begin()+i_first, src_vec->begin()+i_last);
// 	}

// 	return i;
// }

GLVector GLVector::slice(int pos_start, int pos_end)
{
	if(empty())
	{
		return *this;
	}

	int length = size();
	if(pos_start < 0)
	{
		pos_start += length;
	}
	if(pos_end < 0)
	{
		pos_end += length;
	}
	if(pos_end <= pos_start)
	{
		return GLVector(_dtype);
	}

	int n = pos_end-pos_start;
	GLVector result(_dtype, n);
	for(int i = 0; i < n; i++)
	{
		if(_dtype == "bool") result.at<bool>(i) = at<bool>(i-pos_start);
		else if(_dtype == "uint") result.at<uint>(i) = at<uint>(i-pos_start);
		else if(_dtype == "int") result.at<int>(i) = at<int>(i-pos_start);
		else if(_dtype == "float") result.at<float>(i) = at<float>(i-pos_start);
		else if(_dtype == "double") result.at<double>(i) = at<double>(i-pos_start);
		else if(_dtype == "bvec2") result.at<bvec2>(i) = at<bvec2>(i-pos_start);
		else if(_dtype == "bvec3") result.at<bvec3>(i) = at<bvec3>(i-pos_start);
		else if(_dtype == "bvec4") result.at<bvec4>(i) = at<bvec4>(i-pos_start);
		else if(_dtype == "ivec2") result.at<ivec2>(i) = at<ivec2>(i-pos_start);
		else if(_dtype == "ivec3") result.at<ivec3>(i) = at<ivec3>(i-pos_start);
		else if(_dtype == "ivec4") result.at<ivec4>(i) = at<ivec4>(i-pos_start);
		else if(_dtype == "uvec2") result.at<uvec2>(i) = at<uvec2>(i-pos_start);
		else if(_dtype == "uvec3") result.at<uvec3>(i) = at<uvec3>(i-pos_start);
		else if(_dtype == "uvec4") result.at<uvec4>(i) = at<uvec4>(i-pos_start);
		else if(_dtype == "vec2") result.at<vec2>(i) = at<vec2>(i-pos_start);
		else if(_dtype == "vec3") result.at<vec3>(i) = at<vec3>(i-pos_start);
		else if(_dtype == "vec4") result.at<vec4>(i) = at<vec4>(i-pos_start);
		else if(_dtype == "dvec2") result.at<dvec2>(i) = at<dvec2>(i-pos_start);
		else if(_dtype == "dvec3") result.at<dvec3>(i) = at<dvec3>(i-pos_start);
		else if(_dtype == "dvec4") result.at<dvec4>(i) = at<dvec4>(i-pos_start);
		else if(_dtype == "mat2") result.at<mat2>(i) = at<mat2>(i-pos_start);
		else if(_dtype == "mat3") result.at<mat3>(i) = at<mat3>(i-pos_start);
		else if(_dtype == "mat4") result.at<mat4>(i) = at<mat4>(i-pos_start);
	}

	return result;
}

// GLVector GLVector::slice(GLVector::const_iterator& first, GLVector::const_iterator& last)
// {
// 	return slice(first-begin(), last-begin());
// }

void GLVector::extend(const GLVector& new_vec)
{
	if(new_vec.empty())
	{
		return;
	}
	if(_dtype.empty())
	{
		*this = new_vec;
		return;
	}
	if(new_vec._dtype != _dtype)
	{
		throw TypeError("Cannot extend vector<" + new_vec._dtype + "> to vector<" + _dtype + ">");
	}

	if(_dtype == "bool")
	{
		vector<byte>* dest_vec = (vector<byte>*)vec;
		vector<byte>* src_vec = (vector<byte>*)(new_vec.vec);
		dest_vec->insert(dest_vec->end(), src_vec->begin(), src_vec->end());
	}
	else if(_dtype == "uint")
	{
		vector<uint>* dest_vec = (vector<uint>*)vec;
		vector<uint>* src_vec = (vector<uint>*)(new_vec.vec);
		dest_vec->insert(dest_vec->end(), src_vec->begin(), src_vec->end());
	}
	else if(_dtype == "int")
	{
		vector<int>* dest_vec = (vector<int>*)vec;
		vector<int>* src_vec = (vector<int>*)(new_vec.vec);
		dest_vec->insert(dest_vec->end(), src_vec->begin(), src_vec->end());
	}
	else if(_dtype == "float")
	{
		vector<float>* dest_vec = (vector<float>*)vec;
		vector<float>* src_vec = (vector<float>*)(new_vec.vec);
		dest_vec->insert(dest_vec->end(), src_vec->begin(), src_vec->end());
	}
	else if(_dtype == "double")
	{
		vector<double>* dest_vec = (vector<double>*)vec;
		vector<double>* src_vec = (vector<double>*)(new_vec.vec);
		dest_vec->insert(dest_vec->end(), src_vec->begin(), src_vec->end());
	}
	else if(_dtype == "bvec2")
	{
		vector<bvec2>* dest_vec = (vector<bvec2>*)vec;
		vector<bvec2>* src_vec = (vector<bvec2>*)(new_vec.vec);
		dest_vec->insert(dest_vec->end(), src_vec->begin(), src_vec->end());
	}
	else if(_dtype == "bvec3")
	{
		vector<bvec3>* dest_vec = (vector<bvec3>*)vec;
		vector<bvec3>* src_vec = (vector<bvec3>*)(new_vec.vec);
		dest_vec->insert(dest_vec->end(), src_vec->begin(), src_vec->end());
	}
	else if(_dtype == "bvec4")
	{
		vector<bvec4>* dest_vec = (vector<bvec4>*)vec;
		vector<bvec4>* src_vec = (vector<bvec4>*)(new_vec.vec);
		dest_vec->insert(dest_vec->end(), src_vec->begin(), src_vec->end());
	}
	else if(_dtype == "ivec2")
	{
		vector<ivec2>* dest_vec = (vector<ivec2>*)vec;
		vector<ivec2>* src_vec = (vector<ivec2>*)(new_vec.vec);
		dest_vec->insert(dest_vec->end(), src_vec->begin(), src_vec->end());
	}
	else if(_dtype == "ivec3")
	{
		vector<ivec3>* dest_vec = (vector<ivec3>*)vec;
		vector<ivec3>* src_vec = (vector<ivec3>*)(new_vec.vec);
		dest_vec->insert(dest_vec->end(), src_vec->begin(), src_vec->end());
	}
	else if(_dtype == "ivec4")
	{
		vector<ivec4>* dest_vec = (vector<ivec4>*)vec;
		vector<ivec4>* src_vec = (vector<ivec4>*)(new_vec.vec);
		dest_vec->insert(dest_vec->end(), src_vec->begin(), src_vec->end());
	}
	else if(_dtype == "uvec2")
	{
		vector<uvec2>* dest_vec = (vector<uvec2>*)vec;
		vector<uvec2>* src_vec = (vector<uvec2>*)(new_vec.vec);
		dest_vec->insert(dest_vec->end(), src_vec->begin(), src_vec->end());
	}
	else if(_dtype == "uvec3")
	{
		vector<uvec3>* dest_vec = (vector<uvec3>*)vec;
		vector<uvec3>* src_vec = (vector<uvec3>*)(new_vec.vec);
		dest_vec->insert(dest_vec->end(), src_vec->begin(), src_vec->end());
	}
	else if(_dtype == "uvec4")
	{
		vector<uvec4>* dest_vec = (vector<uvec4>*)vec;
		vector<uvec4>* src_vec = (vector<uvec4>*)(new_vec.vec);
		dest_vec->insert(dest_vec->end(), src_vec->begin(), src_vec->end());
	}
	else if(_dtype == "vec2")
	{
		vector<vec2>* dest_vec = (vector<vec2>*)vec;
		vector<vec2>* src_vec = (vector<vec2>*)(new_vec.vec);
		dest_vec->insert(dest_vec->end(), src_vec->begin(), src_vec->end());
	}
	else if(_dtype == "vec3")
	{
		vector<vec3>* dest_vec = (vector<vec3>*)vec;
		vector<vec3>* src_vec = (vector<vec3>*)(new_vec.vec);
		dest_vec->insert(dest_vec->end(), src_vec->begin(), src_vec->end());
	}
	else if(_dtype == "vec4")
	{
		vector<vec4>* dest_vec = (vector<vec4>*)vec;
		vector<vec4>* src_vec = (vector<vec4>*)(new_vec.vec);
		dest_vec->insert(dest_vec->end(), src_vec->begin(), src_vec->end());
	}
	else if(_dtype == "dvec2")
	{
		vector<dvec2>* dest_vec = (vector<dvec2>*)vec;
		vector<dvec2>* src_vec = (vector<dvec2>*)(new_vec.vec);
		dest_vec->insert(dest_vec->end(), src_vec->begin(), src_vec->end());
	}
	else if(_dtype == "dvec3")
	{
		vector<dvec3>* dest_vec = (vector<dvec3>*)vec;
		vector<dvec3>* src_vec = (vector<dvec3>*)(new_vec.vec);
		dest_vec->insert(dest_vec->end(), src_vec->begin(), src_vec->end());
	}
	else if(_dtype == "dvec4")
	{
		vector<dvec4>* dest_vec = (vector<dvec4>*)vec;
		vector<dvec4>* src_vec = (vector<dvec4>*)(new_vec.vec);
		dest_vec->insert(dest_vec->end(), src_vec->begin(), src_vec->end());
	}
	else if(_dtype == "mat2")
	{
		vector<mat2>* dest_vec = (vector<mat2>*)vec;
		vector<mat2>* src_vec = (vector<mat2>*)(new_vec.vec);
		dest_vec->insert(dest_vec->end(), src_vec->begin(), src_vec->end());
	}
	else if(_dtype == "mat3")
	{
		vector<mat3>* dest_vec = (vector<mat3>*)vec;
		vector<mat3>* src_vec = (vector<mat3>*)(new_vec.vec);
		dest_vec->insert(dest_vec->end(), src_vec->begin(), src_vec->end());
	}
	else if(_dtype == "mat4")
	{
		vector<mat4>* dest_vec = (vector<mat4>*)vec;
		vector<mat4>* src_vec = (vector<mat4>*)(new_vec.vec);
		dest_vec->insert(dest_vec->end(), src_vec->begin(), src_vec->end());
	}
}

uint GLVector::size()const
{
	if(!vec)
	{
		return 0;
	}

	if(_dtype == "bool") return ((vector<byte>*)vec)->size();
	else if(_dtype == "uint") return ((vector<uint>*)vec)->size();
	else if(_dtype == "int") return ((vector<int>*)vec)->size();
	else if(_dtype == "float") return ((vector<float>*)vec)->size();
	else if(_dtype == "double") return ((vector<double>*)vec)->size();
	else if(_dtype == "bvec2") return ((vector<bvec2>*)vec)->size();
	else if(_dtype == "bvec3") return ((vector<bvec3>*)vec)->size();
	else if(_dtype == "bvec4") return ((vector<bvec4>*)vec)->size();
	else if(_dtype == "ivec2") return ((vector<ivec2>*)vec)->size();
	else if(_dtype == "ivec3") return ((vector<ivec3>*)vec)->size();
	else if(_dtype == "ivec4") return ((vector<ivec4>*)vec)->size();
	else if(_dtype == "uvec2") return ((vector<uvec2>*)vec)->size();
	else if(_dtype == "uvec3") return ((vector<uvec3>*)vec)->size();
	else if(_dtype == "uvec4") return ((vector<uvec4>*)vec)->size();
	else if(_dtype == "vec2") return ((vector<vec2>*)vec)->size();
	else if(_dtype == "vec3") return ((vector<vec3>*)vec)->size();
	else if(_dtype == "vec4") return ((vector<vec4>*)vec)->size();
	else if(_dtype == "dvec2") return ((vector<dvec2>*)vec)->size();
	else if(_dtype == "dvec3") return ((vector<dvec3>*)vec)->size();
	else if(_dtype == "dvec4") return ((vector<dvec4>*)vec)->size();
	else if(_dtype == "mat2") return ((vector<mat2>*)vec)->size();
	else if(_dtype == "mat3") return ((vector<mat3>*)vec)->size();
	else if(_dtype == "mat4") return ((vector<mat4>*)vec)->size();
	else return 0;
}

bool GLVector::empty()const
{
	if(!vec)
	{
		return true;
	}

	if(_dtype == "bool") return ((vector<byte>*)vec)->empty();
	else if(_dtype == "uint") return ((vector<uint>*)vec)->empty();
	else if(_dtype == "int") return ((vector<int>*)vec)->empty();
	else if(_dtype == "float") return ((vector<float>*)vec)->empty();
	else if(_dtype == "double") return ((vector<double>*)vec)->empty();
	else if(_dtype == "bvec2") return ((vector<bvec2>*)vec)->empty();
	else if(_dtype == "bvec3") return ((vector<bvec3>*)vec)->empty();
	else if(_dtype == "bvec4") return ((vector<bvec4>*)vec)->empty();
	else if(_dtype == "ivec2") return ((vector<ivec2>*)vec)->empty();
	else if(_dtype == "ivec3") return ((vector<ivec3>*)vec)->empty();
	else if(_dtype == "ivec4") return ((vector<ivec4>*)vec)->empty();
	else if(_dtype == "uvec2") return ((vector<uvec2>*)vec)->empty();
	else if(_dtype == "uvec3") return ((vector<uvec3>*)vec)->empty();
	else if(_dtype == "uvec4") return ((vector<uvec4>*)vec)->empty();
	else if(_dtype == "vec2") return ((vector<vec2>*)vec)->empty();
	else if(_dtype == "vec3") return ((vector<vec3>*)vec)->empty();
	else if(_dtype == "vec4") return ((vector<vec4>*)vec)->empty();
	else if(_dtype == "dvec2") return ((vector<dvec2>*)vec)->empty();
	else if(_dtype == "dvec3") return ((vector<dvec3>*)vec)->empty();
	else if(_dtype == "dvec4") return ((vector<dvec4>*)vec)->empty();
	else if(_dtype == "mat2") return ((vector<mat2>*)vec)->empty();
	else if(_dtype == "mat3") return ((vector<mat3>*)vec)->empty();
	else if(_dtype == "mat4") return ((vector<mat4>*)vec)->empty();
	else return true;
}

const string& GLVector::dtype()const
{
	return _dtype;
}

void* GLVector::ptr(int i)
{
	if(empty())
	{
		throw RuntimeError("Try to access value pointer in empty vector");
	}

	int length = size();
	if(i < 0)
	{
		i += length;
	}
	if(i < 0 || i >= length)
	{
		throw IndexError(i, 0, length-1);
	}

	if(_dtype == "bool") return (void*)(&(((vector<byte>*)vec)->at(i)));
	else if(_dtype == "uint") return (void*)(&(((vector<uint>*)vec)->at(i)));
	else if(_dtype == "int") return (void*)(&(((vector<int>*)vec)->at(i)));
	else if(_dtype == "float") return (void*)(&(((vector<float>*)vec)->at(i)));
	else if(_dtype == "double") return (void*)(&(((vector<double>*)vec)->at(i)));
	else if(_dtype == "bvec2") return (void*)(&(((vector<bvec2>*)vec)->at(i)));
	else if(_dtype == "bvec3") return (void*)(&(((vector<bvec3>*)vec)->at(i)));
	else if(_dtype == "bvec4") return (void*)(&(((vector<bvec4>*)vec)->at(i)));
	else if(_dtype == "ivec2") return (void*)(&(((vector<ivec2>*)vec)->at(i)));
	else if(_dtype == "ivec3") return (void*)(&(((vector<ivec3>*)vec)->at(i)));
	else if(_dtype == "ivec4") return (void*)(&(((vector<ivec4>*)vec)->at(i)));
	else if(_dtype == "uvec2") return (void*)(&(((vector<uvec2>*)vec)->at(i)));
	else if(_dtype == "uvec3") return (void*)(&(((vector<uvec3>*)vec)->at(i)));
	else if(_dtype == "uvec4") return (void*)(&(((vector<uvec4>*)vec)->at(i)));
	else if(_dtype == "vec2") return (void*)(&(((vector<vec2>*)vec)->at(i)));
	else if(_dtype == "vec3") return (void*)(&(((vector<vec3>*)vec)->at(i)));
	else if(_dtype == "vec4") return (void*)(&(((vector<vec4>*)vec)->at(i)));
	else if(_dtype == "dvec2") return (void*)(&(((vector<dvec2>*)vec)->at(i)));
	else if(_dtype == "dvec3") return (void*)(&(((vector<dvec3>*)vec)->at(i)));
	else if(_dtype == "dvec4") return (void*)(&(((vector<dvec4>*)vec)->at(i)));
	else if(_dtype == "mat2") return (void*)(&(((vector<mat2>*)vec)->at(i)));
	else if(_dtype == "mat3") return (void*)(&(((vector<mat3>*)vec)->at(i)));
	else if(_dtype == "mat4") return (void*)(&(((vector<mat4>*)vec)->at(i)));
	else return (void*)NULL;
}