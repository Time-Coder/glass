#include "glass/utils/glvector.h"
#include "glass/Layout"

using namespace std;

 const size_t GLVector::DEFAULT_CAPACITY = 4;

void GLVector::expand()
{
	if(_size < _capacity) return;
	_capacity = 2 * _size;

	data.realloc(_capacity);
}

void GLVector::shrink()
{
	if(_capacity <= DEFAULT_CAPACITY || 4*_size >= _capacity) return;
	_capacity /= 2;

	data.realloc(_capacity);
}

void GLVector::divisor(uint n)
{
	if(parent == NULL)
	{
		throw glass::RuntimeError("current vector is not in a Layout!");
	}
	parent->vao[location].divisor(n);
}

void GLVector::setDtype(const string& __dtype)
{
	if(__dtype == "uint" || __dtype == "int"  || __dtype == "float" || __dtype == "double" ||
	   __dtype == "ivec2" || __dtype == "ivec3" || __dtype == "ivec4" ||
	   __dtype == "uvec2" || __dtype == "uvec3" || __dtype == "uvec4" ||
	   __dtype == "vec2"  || __dtype == "vec3"  || __dtype == "vec4"  ||
	   __dtype == "dvec2" || __dtype == "dvec3" || __dtype == "dvec4" ||
	   __dtype == "mat2"  || __dtype == "mat3"  || __dtype == "mat4")
	{
		_dtype = __dtype;
	}
	else
	{
		throw glass::TypeError("Not supported DataType " + __dtype + ".");
	}
}

void GLVector::init(const string& __dtype)
{
	if(using_VBO)
	{
		if(_capacity == 0)
		{
			setDtype(__dtype);
			_capacity = DEFAULT_CAPACITY;
			data.malloc(_capacity * GLSL::built_in_types[_dtype].glsl_size);
		}
		return;
	}

	if(vec) return;

	if(__dtype == "uint") vec = (void*)(new vector<uint>());
	else if(__dtype == "int") vec = (void*)(new vector<int>());
	else if(__dtype == "float") vec = (void*)(new vector<float>());
	else if(__dtype == "double") vec = (void*)(new vector<double>());
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
	else throw glass::TypeError("Not supported DataType " + __dtype + ".");

	if(!vec) throw glass::MemoryError("Failed to allocate memory!");

	_dtype = __dtype;
}

void GLVector::clear()
{
	if(using_VBO)
	{
		_size = 0;
		_dtype.clear();
		return;
	}

	if(!vec)
	{
		_dtype.clear();
		return;
	}
	
	if(_dtype == "uint") delete (vector<uint>*)(vec);
	else if(_dtype == "int") delete (vector<int>*)(vec);
	else if(_dtype == "float") delete (vector<float>*)(vec);
	else if(_dtype == "double") delete (vector<double>*)(vec);
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

GLVector::GLVector(bool use_VBO):
using_VBO(use_VBO) {}

GLVector::~GLVector()
{
	clear();
}

GLVector::GLVector(const string& __dtype, bool use_VBO):
using_VBO(use_VBO)
{
	init(__dtype);
}

GLVector::GLVector(const string& __dtype, size_t n, bool use_VBO):
using_VBO(use_VBO)
{
	if(using_VBO)
	{
		_size = n;
		_capacity = std::max(2*n, DEFAULT_CAPACITY);
		setDtype(__dtype);
		data.malloc(_capacity * GLSL::built_in_types[_dtype].glsl_size);
		return;
	}

	if(__dtype == "uint") vec = (void*)(new vector<uint>(n));
	else if(__dtype == "int") vec = (void*)(new vector<int>(n));
	else if(__dtype == "float") vec = (void*)(new vector<float>(n));
	else if(__dtype == "double") vec = (void*)(new vector<double>(n));
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
	else throw glass::TypeError("Not supported DataType " + __dtype + ".");

	if(!vec) throw glass::MemoryError("Failed to allocate memory!");

	_dtype = __dtype;
}

GLVector::GLVector(const GLVector& new_vec) :
_dtype(new_vec._dtype),
using_VBO(new_vec.using_VBO),
_capacity(new_vec._capacity),
_size(new_vec._size)
{
	if(using_VBO)
	{
		VBO vbo = new_vec.data;
		data = vbo.clone<VBO>();
		return;
	}

	if(new_vec.vec == NULL) return;

	if(_dtype == "uint") vec = (void*)(new vector<uint>(*((vector<uint>*)(new_vec.vec))));
	else if(_dtype == "int") vec = (void*)(new vector<int>(*((vector<int>*)(new_vec.vec))));
	else if(_dtype == "float") vec = (void*)(new vector<float>(*((vector<float>*)(new_vec.vec))));
	else if(_dtype == "double") vec = (void*)(new vector<double>(*((vector<double>*)(new_vec.vec))));
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

	if(!(_dtype.empty()) && !vec) throw glass::MemoryError("Failed to allocate memory!");
}

GLVector::GLVector(GLVector&& new_vec) :
_dtype(move(new_vec._dtype)),
vec(move(new_vec.vec)),
using_VBO(move(new_vec.using_VBO)),
_capacity(move(new_vec._capacity)),
_size(move(new_vec._size))
{
	if(using_VBO)
	{
		data = move(new_vec.data);
		return;
	}

	new_vec._dtype = string();
	new_vec.vec = NULL;
}

GLVector& GLVector::operator =(const GLVector& new_vec)
{
	if(this == &new_vec)
	{
		return *this;
	}

	if(!using_VBO && new_vec.using_VBO)
	{
		clear();
	}
	using_VBO = new_vec.using_VBO;
	if(using_VBO)
	{
		_size = new_vec._size;
		_capacity = new_vec._capacity;
		VBO vbo = new_vec.data;
		data = vbo.clone<VBO>();
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
		if(_dtype == "uint") *((vector<uint>*)vec) = *((vector<uint>*)(new_vec.vec));
		else if(_dtype == "int") *((vector<int>*)vec) = *((vector<int>*)(new_vec.vec));
		else if(_dtype == "float") *((vector<float>*)vec) = *((vector<float>*)(new_vec.vec));
		else if(_dtype == "double") *((vector<double>*)vec) = *((vector<double>*)(new_vec.vec));
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

		if(_dtype == "uint") vec = (void*)(new vector<uint>(*((vector<uint>*)(new_vec.vec))));
		else if(_dtype == "int") vec = (void*)(new vector<int>(*((vector<int>*)(new_vec.vec))));
		else if(_dtype == "float") vec = (void*)(new vector<float>(*((vector<float>*)(new_vec.vec))));
		else if(_dtype == "double") vec = (void*)(new vector<double>(*((vector<double>*)(new_vec.vec))));
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
		
		if(!(_dtype.empty()) && !vec) throw glass::MemoryError("Failed to allocate memory!");
	}

	return *this;
}

GLVector& GLVector::operator =(GLVector&& new_vec) // reviewed
{
	if(this == &new_vec)
	{
		return *this;
	}

	clear();
	using_VBO = new_vec.using_VBO;
	if(using_VBO)
	{
		_capacity = move(new_vec._capacity);
		_size = move(new_vec._size);
		data = move(new_vec.data);
		return *this;
	}

	_dtype = move(new_vec._dtype);
	vec = move(new_vec.vec);
	
	return *this;
}

void GLVector::pop_back() // reviewed
{
	if(empty())
	{
		throw glass::RuntimeError("Try to pop from empty vector.");
	}

	if(using_VBO)
	{
		_size -= 1;
		shrink();
	}

	if(_dtype == "uint") ((vector<uint>*)vec)->pop_back();
	else if(_dtype == "int") ((vector<int>*)vec)->pop_back();
	else if(_dtype == "float") ((vector<float>*)vec)->pop_back();
	else if(_dtype == "double") ((vector<double>*)vec)->pop_back();
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

size_t GLVector::erase(long long int i, size_t len) // reviewed
{
	if(empty())
	{
		return 0;
	}
	size_t length = size();
	long long int first = i;
	if(first < 0)
	{
		first += length;
	}

	long long int last = i + len;
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

	if(using_VBO)
	{
		void* pointer = data.ptr();
		void* start = (void*)((unsigned char*)pointer + first*GLSL::built_in_types[_dtype].glsl_size);
		void* end = (void*)((unsigned char*)pointer + last*GLSL::built_in_types[_dtype].glsl_size);
		size_t block_size = std::min(last-first, (long long int)length-last)*GLSL::built_in_types[_dtype].glsl_size;
		memmove(start, end, block_size);
		data.apply();
		
		_size -= (last-first);
		shrink();
		return last;
	}

	if(_dtype == "uint") ((vector<uint>*)vec)->erase(((vector<uint>*)vec)->begin()+first, ((vector<uint>*)vec)->begin()+last);
	else if(_dtype == "int") ((vector<int>*)vec)->erase(((vector<int>*)vec)->begin()+first, ((vector<int>*)vec)->begin()+last);
	else if(_dtype == "float") ((vector<float>*)vec)->erase(((vector<float>*)vec)->begin()+first, ((vector<float>*)vec)->begin()+last);
	else if(_dtype == "double") ((vector<double>*)vec)->erase(((vector<double>*)vec)->begin()+first, ((vector<double>*)vec)->begin()+last);
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

size_t GLVector::size()const
{
	if(using_VBO)
	{
		return _size;
	}

	if(!vec)
	{
		return 0;
	}

	if(_dtype == "uint") return ((vector<uint>*)vec)->size();
	else if(_dtype == "int") return ((vector<int>*)vec)->size();
	else if(_dtype == "float") return ((vector<float>*)vec)->size();
	else if(_dtype == "double") return ((vector<double>*)vec)->size();
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
	if(using_VBO)
	{
		return (_size == 0) || (_capacity == 0);
	}

	if(!vec)
	{
		return true;
	}

	if(_dtype == "uint") return ((vector<uint>*)vec)->empty();
	else if(_dtype == "int") return ((vector<int>*)vec)->empty();
	else if(_dtype == "float") return ((vector<float>*)vec)->empty();
	else if(_dtype == "double") return ((vector<double>*)vec)->empty();
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

void* GLVector::ptr(long long int i)
{
	if(using_VBO)
	{
		throw glass::RuntimeError("Cannot access ptr when GLVector is using VBO.");
	}

	if(empty())
	{
		throw glass::RuntimeError("Try to access value pointer in empty vector");
	}

	int length = size();
	if(i < 0)
	{
		i += length;
	}
	if(i < 0 || i >= length)
	{
		throw glass::IndexError(i, 0, length-1);
	}

	if(_dtype == "uint") return (void*)(&(((vector<uint>*)vec)->at(i)));
	else if(_dtype == "int") return (void*)(&(((vector<int>*)vec)->at(i)));
	else if(_dtype == "float") return (void*)(&(((vector<float>*)vec)->at(i)));
	else if(_dtype == "double") return (void*)(&(((vector<double>*)vec)->at(i)));
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

void GLVector::apply()
{
	if(using_VBO)
	{
		data.apply();
	}
}