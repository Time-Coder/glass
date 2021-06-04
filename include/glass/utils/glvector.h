#ifndef __GLVECTOR_H__
#define __GLVECTOR_H__

#include <vector>

#include "glass/VBO"
#include "glass/vec"
#include "glass/mat"
#include "glass/utils/type.h"
#include "glass/utils/exceptions.h"

class Layout;
class GLVector
{
	std::string _dtype;
	void* vec = NULL;

	bool using_VBO = false;
	size_t _size = 0;
	size_t _capacity = 0;
	VBO data;

private:
	static const size_t DEFAULT_CAPACITY;

private:
	void expand();
	void shrink();

	template<class DataType>
	void setDtype();

	void setDtype(const std::string& __dtype);

public:
	uint location = -1;
	Layout* parent = NULL;

public:
	void init(const std::string& __dtype);

	template<class DataType>
	void init();

	void divisor(uint n);

public:
	GLVector(bool use_VBO = false);
	GLVector(const std::string& __dtype, bool use_VBO = false);
	GLVector(const std::string& __dtype, size_t n, bool use_VBO = false);

	template<class DataType>
	GLVector(size_t n, const DataType& value, bool use_VBO = false);
	
	template<class DataType>
	GLVector(typename std::vector<DataType>::const_iterator& first, typename std::vector<DataType>::const_iterator& last, bool use_VBO = false);
	
	GLVector(const GLVector& new_vec);
	GLVector(GLVector&& new_vec);

	template<class DataType>
	GLVector(const std::vector<DataType>& new_vec, bool use_VBO = false);
	~GLVector();

	bool empty()const;
	size_t size()const;
	const std::string& dtype()const;

	GLVector& operator =(const GLVector& new_vec);
	GLVector& operator =(GLVector&& new_vec);

	template<class DataType>
	GLVector& operator =(const std::vector<DataType>& new_vec);

	template<class DataType>
	void push_back(const DataType& element);

	template<class DataType>
	size_t insert(long long int i, const DataType& element);

	template<class DataType>
	size_t insert(long long int i, size_t n, const DataType& element);

	template<class DataType>
	size_t insert(long long int i, const typename std::vector<DataType>::const_iterator& first, const typename std::vector<DataType>::const_iterator& last);

	void pop_back();

	template<class DataType>
	DataType pop_back();

	size_t erase(long long int i, size_t len = 1);

	template<class DataType>
	void extend(const std::vector<DataType>& new_vec);

	template<class DataType>
	DataType front();

	template<class DataType>
	DataType back();

	template<class DataType>
	DataType get(long long int i);

	template<class DataType>
	void set(long long int i, const DataType& value);

	void* ptr(long long int i);

	template<class DataType>
	operator std::vector<DataType>()const;

	void clear();
	void apply();
};

template<class DataType>
void GLVector::setDtype() // reviewed
{
	if(same_type(DataType, uint)) _dtype = "uint";
	else if(same_type(DataType, int)) _dtype = "int";
	else if(same_type(DataType, float)) _dtype = "float";
	else if(same_type(DataType, double)) _dtype = "double";
	else if(same_type(DataType, ivec2)) _dtype = "ivec2";
	else if(same_type(DataType, ivec3)) _dtype = "ivec3";
	else if(same_type(DataType, ivec4)) _dtype = "ivec4";
	else if(same_type(DataType, uvec2)) _dtype = "uvec2";
	else if(same_type(DataType, uvec3)) _dtype = "uvec3";
	else if(same_type(DataType, uvec4)) _dtype = "uvec4";
	else if(same_type(DataType, vec2)) _dtype = "vec2";
	else if(same_type(DataType, vec3)) _dtype = "vec3";
	else if(same_type(DataType, vec4)) _dtype = "vec4";
	else if(same_type(DataType, dvec2)) _dtype = "dvec2";
	else if(same_type(DataType, dvec3)) _dtype = "dvec3";
	else if(same_type(DataType, dvec4)) _dtype = "dvec4";
	else if(same_type(DataType, mat2)) _dtype = "mat2";
	else if(same_type(DataType, mat3)) _dtype = "mat3";
	else if(same_type(DataType, mat4)) _dtype = "mat4";
	else throw glass::TypeError("Not supported DataType " + type_name(DataType) + ".");
}

template<class DataType>
void GLVector::init() // reviewed
{
	if(using_VBO)
	{
		if(_capacity != 0) return;
		setDtype<DataType>();
		_capacity = DEFAULT_CAPACITY;
		data.malloc(_capacity * GLSL::built_in_types[_dtype].glsl_size);
	}
	else
	{
		if(vec) return;
		setDtype<DataType>();
		vec = (void*)(new std::vector<DataType>());
		if(!vec) throw glass::MemoryError("Failed to allocate memory!");
	}
}

template<class DataType>
GLVector::GLVector(size_t n, const DataType& value, bool use_VBO): // reviewed
using_VBO(use_VBO)
{
	setDtype<DataType>();
	if(using_VBO)
	{
		_size = n;
		_capacity = std::max(DEFAULT_CAPACITY, 2*n);
		data.malloc(_capacity * GLSL::built_in_types[_dtype].glsl_size);
		unsigned char* pointer = (unsigned char*)(data.ptr());
		if(_dtype == "mat2")
		{
			for(int i = 0; i < n; i++)
			{
				memcpy((void*)pointer, force_cast<mat2>(value).data(), GLSL::built_in_types[_dtype].glsl_size);
				pointer += GLSL::built_in_types[_dtype].glsl_size;
			}
		}
		else if(_dtype == "mat3")
		{
			for(int i = 0; i < n; i++)
			{
				memcpy((void*)pointer, force_cast<mat3>(value).data(), GLSL::built_in_types[_dtype].glsl_size);
				pointer += GLSL::built_in_types[_dtype].glsl_size;
			}
		}
		else if(_dtype == "mat4")
		{
			for(int i = 0; i < n; i++)
			{
				memcpy((void*)pointer, force_cast<mat4>(value).data(), GLSL::built_in_types[_dtype].glsl_size);
				pointer += GLSL::built_in_types[_dtype].glsl_size;
			}
		}
		else
		{
			for(int i = 0; i < n; i++)
			{
				memcpy((void*)pointer, (void*)(&value), GLSL::built_in_types[_dtype].glsl_size);
				pointer += GLSL::built_in_types[_dtype].glsl_size;
			}
		}
		data.apply();
	}
	else
	{
		vec = (void*)(new std::vector<DataType>(n, value));
		if(!vec) throw glass::MemoryError("Failed to allocate memory!");
	}
}

template<class DataType>
GLVector::GLVector(typename std::vector<DataType>::const_iterator& first, typename std::vector<DataType>::const_iterator& last, bool use_VBO): // reviewed
using_VBO(use_VBO)
{
	if(first >= last)
	{
		init<DataType>();
		return;
	}

	setDtype<DataType>();
	if(using_VBO)
	{
		_size = last - first;
		_capacity = std::max(DEFAULT_CAPACITY, 2*_size);
		data.malloc(_capacity * GLSL::built_in_types[_dtype].glsl_size);
		unsigned char* pointer = (unsigned char*)(data.ptr());
		if(_dtype == "mat2")
		{
			for(auto it = first; it < last; it++)
			{
				memcpy((void*)pointer, force_cast<mat2>(*it).data(), GLSL::built_in_types[_dtype].glsl_size);
				pointer += GLSL::built_in_types[_dtype].glsl_size;
			}
		}
		else if(_dtype == "mat3")
		{
			for(auto it = first; it < last; it++)
			{
				memcpy((void*)pointer, force_cast<mat3>(*it).data(), GLSL::built_in_types[_dtype].glsl_size);
				pointer += GLSL::built_in_types[_dtype].glsl_size;
			}
		}
		else if(_dtype == "mat4")
		{
			for(auto it = first; it < last; it++)
			{
				memcpy((void*)pointer, force_cast<mat4>(*it).data(), GLSL::built_in_types[_dtype].glsl_size);
				pointer += GLSL::built_in_types[_dtype].glsl_size;
			}
		}
		else
		{
			for(auto it = first; it < last; it++)
			{
				memcpy((void*)pointer, (void*)(&(*it)), GLSL::built_in_types[_dtype].glsl_size);
				pointer += GLSL::built_in_types[_dtype].glsl_size;
			}
		}
		data.apply();
	}
	else
	{
		vec = (void*)(new std::vector<DataType>(first, last));
		if(!vec) throw glass::MemoryError("Failed to allocate memory!");
	}
}

template<class DataType>
GLVector::GLVector(const std::vector<DataType>& new_vec, bool use_VBO): // reviewed
using_VBO(use_VBO)
{
	setDtype<DataType>();
	if(using_VBO)
	{
		_size = new_vec.size();
		_capacity = new_vec.capacity();
		data.malloc(_capacity * GLSL::built_in_types[_dtype].glsl_size);
		unsigned char* pointer = (unsigned char*)(data.ptr());
		if(_dtype == "mat2")
		{
			for(auto it = new_vec.begin(); it < new_vec.end(); it++)
			{
				memcpy((void*)pointer, force_cast<mat2>(*it).data(), GLSL::built_in_types[_dtype].glsl_size);
				pointer += GLSL::built_in_types[_dtype].glsl_size;
			}
		}
		else if(_dtype == "mat3")
		{
			for(auto it = new_vec.begin(); it < new_vec.end(); it++)
			{
				memcpy((void*)pointer, force_cast<mat3>(*it).data(), GLSL::built_in_types[_dtype].glsl_size);
				pointer += GLSL::built_in_types[_dtype].glsl_size;
			}
		}
		else if(_dtype == "mat4")
		{
			for(auto it = new_vec.begin(); it < new_vec.end(); it++)
			{
				memcpy((void*)pointer, force_cast<mat4>(*it).data(), GLSL::built_in_types[_dtype].glsl_size);
				pointer += GLSL::built_in_types[_dtype].glsl_size;
			}
		}
		else
		{
			for(auto it = new_vec.begin(); it < new_vec.end(); it++)
			{
				memcpy((void*)pointer, (void*)(&(*it)), GLSL::built_in_types[_dtype].glsl_size);
				pointer += GLSL::built_in_types[_dtype].glsl_size;
			}
		}
		data.apply();
	}
	else
	{
		vec = (void*)(new std::vector<DataType>(new_vec));
		if(!vec) throw glass::MemoryError("Failed to allocate memory!");
	}
}

#define __SAME_WITH_DTYPE(DataType, _dtype) \
((_dtype == "uint" && same_type(DataType, uint)) ||\
(_dtype == "int" && same_type(DataType, int)) ||\
(_dtype == "float" && same_type(DataType, float)) ||\
(_dtype == "double" && same_type(DataType, double)) ||\
(_dtype == "ivec2" && same_type(DataType, ivec2)) ||\
(_dtype == "ivec3" && same_type(DataType, ivec3)) ||\
(_dtype == "ivec4" && same_type(DataType, ivec4)) ||\
(_dtype == "uvec2" && same_type(DataType, uvec2)) ||\
(_dtype == "uvec3" && same_type(DataType, uvec3)) ||\
(_dtype == "uvec4" && same_type(DataType, uvec4)) ||\
(_dtype == "vec2" && same_type(DataType, vec2)) ||\
(_dtype == "vec3" && same_type(DataType, vec3)) ||\
(_dtype == "vec4" && same_type(DataType, vec4)) ||\
(_dtype == "dvec2" && same_type(DataType, dvec2)) ||\
(_dtype == "dvec3" && same_type(DataType, dvec3)) ||\
(_dtype == "dvec4" && same_type(DataType, dvec4)) ||\
(_dtype == "mat2" && same_type(DataType, mat2)) ||\
(_dtype == "mat3" && same_type(DataType, mat3)) ||\
(_dtype == "mat4" && same_type(DataType, mat4)))

template<class DataType>
GLVector& GLVector::operator =(const std::vector<DataType>& new_vec) // reviewed
{
	if(!__SAME_WITH_DTYPE(DataType, _dtype))
	{
		clear();
		setDtype<DataType>();
	}

	if(using_VBO)
	{
		_size = new_vec.size();
		if(_capacity < _size)
		{
			_capacity = new_vec.capacity();
			data.malloc(_capacity * GLSL::built_in_types[_dtype].glsl_size);
		}

		unsigned char* pointer = (unsigned char*)(data.ptr());
		if(_dtype == "mat2")
		{
			for(auto it = new_vec.begin(); it < new_vec.end(); it++)
			{
				memcpy((void*)pointer, force_cast<mat2>(*it).data(), GLSL::built_in_types[_dtype].glsl_size);
				pointer += GLSL::built_in_types[_dtype].glsl_size;
			}
		}
		else if(_dtype == "mat3")
		{
			for(auto it = new_vec.begin(); it < new_vec.end(); it++)
			{
				memcpy((void*)pointer, force_cast<mat3>(*it).data(), GLSL::built_in_types[_dtype].glsl_size);
				pointer += GLSL::built_in_types[_dtype].glsl_size;
			}
		}
		else if(_dtype == "mat4")
		{
			for(auto it = new_vec.begin(); it < new_vec.end(); it++)
			{
				memcpy((void*)pointer, force_cast<mat4>(*it).data(), GLSL::built_in_types[_dtype].glsl_size);
				pointer += GLSL::built_in_types[_dtype].glsl_size;
			}
		}
		else
		{
			for(auto it = new_vec.begin(); it < new_vec.end(); it++)
			{
				memcpy((void*)pointer, (void*)(&(*it)), GLSL::built_in_types[_dtype].glsl_size);
				pointer += GLSL::built_in_types[_dtype].glsl_size;
			}
		}
		data.apply();
	}
	else
	{
		if(vec != NULL)
		{
			*((std::vector<DataType>*)vec) = new_vec;
		}
		else
		{
			vec = (void*)(new std::vector<DataType>(new_vec));
			if(!vec) throw glass::MemoryError("Failed to allocate memory!");
		}
	}
	return *this;
}

#define __HANDLE_ELEMENT(PtrType, DataType, element, operation) \
if(same_type(DataType, unsigned short)) operation((PtrType)force_cast<unsigned short>(element));\
else if(same_type(DataType, uint)) operation((PtrType)force_cast<uint>(element));\
else if(same_type(DataType, long uint)) operation((PtrType)force_cast<long uint>(element));\
else if(same_type(DataType, long long uint)) operation((PtrType)force_cast<long long uint>(element));\
else if(same_type(DataType, short)) operation((PtrType)force_cast<short>(element));\
else if(same_type(DataType, int)) operation((PtrType)force_cast<int>(element));\
else if(same_type(DataType, long int)) operation((PtrType)force_cast<long int>(element));\
else if(same_type(DataType, long long int)) operation((PtrType)force_cast<long long int>(element));\
else if(same_type(DataType, float)) operation((PtrType)force_cast<float>(element));\
else if(same_type(DataType, double)) operation((PtrType)force_cast<double>(element));\
else throw glass::TypeError("Try to insert " + type_name(DataType) + " value to std::vector<" + _dtype + ">")

template<class DataType>
void GLVector::push_back(const DataType& element) // reviewed
{
	init<DataType>();
	if(using_VBO)
	{
		_size++;
		expand();
		if(_dtype == "uint")
		{
			uint* pointer = (uint*)((unsigned char*)(data.ptr()) + (_size - 1) * GLSL::built_in_types[_dtype].glsl_size);
			__HANDLE_ELEMENT(uint, DataType, element, *pointer=);
			data.apply();
		}
		else if(_dtype == "int")
		{
			int* pointer = (int*)((unsigned char*)(data.ptr()) + (_size - 1) * GLSL::built_in_types[_dtype].glsl_size);
			__HANDLE_ELEMENT(int, DataType, element, *pointer=);
			data.apply();
		}
		else if(_dtype == "float")
		{
			float* pointer = (float*)((unsigned char*)(data.ptr()) + (_size - 1) * GLSL::built_in_types[_dtype].glsl_size);
			__HANDLE_ELEMENT(float, DataType, element, *pointer=);
			data.apply();
		}
		else if(_dtype == "double")
		{
			double* pointer = (double*)((unsigned char*)(data.ptr()) + (_size - 1) * GLSL::built_in_types[_dtype].glsl_size);
			__HANDLE_ELEMENT(double, DataType, element, *pointer=);
			data.apply();
		}
		else if(__SAME_WITH_DTYPE(DataType, _dtype))
		{
			void* pointer = (void*)((unsigned char*)(data.ptr()) + (_size - 1) * GLSL::built_in_types[_dtype].glsl_size);
			if(_dtype == "mat2")
			{
				memcpy(pointer, force_cast<mat2>(element).data(), GLSL::built_in_types[_dtype].glsl_size);
			}
			else if(_dtype == "mat3")
			{
				memcpy(pointer, force_cast<mat3>(element).data(), GLSL::built_in_types[_dtype].glsl_size);
			}
			else if(_dtype == "mat4")
			{
				memcpy(pointer, force_cast<mat4>(element).data(), GLSL::built_in_types[_dtype].glsl_size);
			}
			else // vec
			{
				memcpy(pointer, (void*)(&element), GLSL::built_in_types[_dtype].glsl_size);
			}
		}
		else
		{
			throw glass::TypeError("Try to push " + type_name(DataType) + " value to std::vector<" + _dtype + ">");
		}
	}
	else
	{
		if(_dtype == "uint")
		{
			std::vector<uint>* ptr_vec = (std::vector<uint>*)vec;
			__HANDLE_ELEMENT(uint, DataType, element, ptr_vec->push_back);
		}
		else if(_dtype == "int")
		{
			std::vector<int>* ptr_vec = (std::vector<int>*)vec;
			__HANDLE_ELEMENT(int, DataType, element, ptr_vec->push_back);
		}
		else if(_dtype == "float")
		{
			std::vector<float>* ptr_vec = (std::vector<float>*)vec;
			__HANDLE_ELEMENT(float, DataType, element, ptr_vec->push_back);
		}
		else if(_dtype == "double")
		{
			std::vector<double>* ptr_vec = (std::vector<double>*)vec;
			__HANDLE_ELEMENT(double, DataType, element, ptr_vec->push_back);
		}
		else if(__SAME_WITH_DTYPE(DataType, _dtype))
		{
			((std::vector<DataType>*)vec)->push_back(element);
		}
		else
		{
			throw glass::TypeError("Try to push " + type_name(DataType) + " value to std::vector<" + _dtype + ">");
		}
	}
}

#define __INSERT_ELEMENT(PtrType, DataType, element) \
if(same_type(DataType, unsigned short)) ptr_vec->insert(ptr_vec->begin()+i, (PtrType)force_cast<unsigned short>(element));\
else if(same_type(DataType, uint)) ptr_vec->insert(ptr_vec->begin()+i, (PtrType)force_cast<uint>(element));\
else if(same_type(DataType, long uint)) ptr_vec->insert(ptr_vec->begin()+i, (PtrType)force_cast<long uint>(element));\
else if(same_type(DataType, long long uint)) ptr_vec->insert(ptr_vec->begin()+i, (PtrType)force_cast<long long uint>(element));\
else if(same_type(DataType, short)) ptr_vec->insert(ptr_vec->begin()+i, (PtrType)force_cast<short>(element));\
else if(same_type(DataType, int)) ptr_vec->insert(ptr_vec->begin()+i, (PtrType)force_cast<int>(element));\
else if(same_type(DataType, long int)) ptr_vec->insert(ptr_vec->begin()+i, (PtrType)force_cast<long int>(element));\
else if(same_type(DataType, long long int)) ptr_vec->insert(ptr_vec->begin()+i, (PtrType)force_cast<long long int>(element));\
else if(same_type(DataType, float)) ptr_vec->insert(ptr_vec->begin()+i, (PtrType)force_cast<float>(element));\
else if(same_type(DataType, double)) ptr_vec->insert(ptr_vec->begin()+i, (PtrType)force_cast<double>(element));\
else throw glass::RuntimeError("Try to insert " + type_name(DataType) + " value to std::vector<" + _dtype + ">")

template<class DataType>
size_t GLVector::insert(long long int i, const DataType& element) // reviewed
{
	size_t length = size();
	if(i < 0)
	{
		i += length;
	}
	if(i < 0 || i > length)
	{
		throw glass::IndexError(i, 0, length);
	}

	init<DataType>();
	if(using_VBO)
	{
		_size++;
		expand();

		if(_dtype == "uint")
		{
			uint* pointer = (uint*)((unsigned char*)data.ptr() + i*GLSL::built_in_types[_dtype].glsl_size);
			if(_size-1-i != 0)
			{
				memmove((void*)((unsigned char*)pointer + GLSL::built_in_types[_dtype].glsl_size),
						(void*)pointer,
					    (_size-1-i)*GLSL::built_in_types[_dtype].glsl_size);
			}
			__HANDLE_ELEMENT(uint, DataType, element, *pointer=);
			data.apply();
		}
		else if(_dtype == "int")
		{
			int* pointer = (int*)((unsigned char*)data.ptr() + i*GLSL::built_in_types[_dtype].glsl_size);
			if(_size-1-i != 0)
			{
				memmove((void*)((unsigned char*)pointer + GLSL::built_in_types[_dtype].glsl_size),
						(void*)pointer,
					    (_size-1-i)*GLSL::built_in_types[_dtype].glsl_size);
			}
			__HANDLE_ELEMENT(int, DataType, element, *pointer=);
			data.apply();
		}
		else if(_dtype == "float")
		{
			float* pointer = (float*)((unsigned char*)data.ptr() + i*GLSL::built_in_types[_dtype].glsl_size);
			if(_size-1-i != 0)
			{
				memmove((void*)((unsigned char*)pointer+GLSL::built_in_types[_dtype].glsl_size),
						(void*)pointer,
					    (_size-1-i)*GLSL::built_in_types[_dtype].glsl_size);
			}
			__HANDLE_ELEMENT(float, DataType, element, *pointer=);
			data.apply();
		}
		else if(_dtype == "double")
		{
			double* pointer = (double*)((unsigned char*)data.ptr() + i*GLSL::built_in_types[_dtype].glsl_size);
			if(_size-1-i != 0)
			{
				memmove((void*)((unsigned char*)pointer+GLSL::built_in_types[_dtype].glsl_size),
						(void*)pointer,
					    (_size-1-i)*GLSL::built_in_types[_dtype].glsl_size);
			}
			__HANDLE_ELEMENT(double, DataType, element, *pointer=);
			data.apply();
		}
		else if(__SAME_WITH_DTYPE(DataType, _dtype))
		{
			void* pointer = (void*)((unsigned char*)data.ptr() + i*GLSL::built_in_types[_dtype].glsl_size);
			if(_size-1-i != 0)
			{
				memmove((void*)((unsigned char*)pointer+GLSL::built_in_types[_dtype].glsl_size),
						(void*)pointer,
					    (_size-1-i)*GLSL::built_in_types[_dtype].glsl_size);
			}
			if(_dtype == "mat2")
			{
				memcpy(pointer, force_cast<mat2>(element).data(), GLSL::built_in_types[_dtype].glsl_size);
			}
			else if(_dtype == "mat3")
			{
				memcpy(pointer, force_cast<mat3>(element).data(), GLSL::built_in_types[_dtype].glsl_size);
			}
			else if(_dtype == "mat4")
			{
				memcpy(pointer, force_cast<mat4>(element).data(), GLSL::built_in_types[_dtype].glsl_size);
			}
			else
			{
				memcpy(pointer, (void*)(&element), GLSL::built_in_types[_dtype].glsl_size);
			}
			data.apply();
		}
		else
		{
			throw glass::TypeError("Try to insert " + type_name(DataType) + " value to std::vector<" + _dtype + ">");
		}
	}
	else
	{
		if(_dtype == "uint")
		{
			std::vector<uint>* ptr_vec = (std::vector<uint>*)vec;
			__INSERT_ELEMENT(uint, DataType, element);
		}
		else if(_dtype == "int")
		{
			std::vector<int>* ptr_vec = (std::vector<int>*)vec;
			__INSERT_ELEMENT(int, DataType, element);
		}
		else if(_dtype == "float")
		{
			std::vector<float>* ptr_vec = (std::vector<float>*)vec;
			__INSERT_ELEMENT(float, DataType, element);
		}
		else if(_dtype == "double")
		{
			std::vector<double>* ptr_vec = (std::vector<double>*)vec;
			__INSERT_ELEMENT(double, DataType, element);
		}
		else if(__SAME_WITH_DTYPE(DataType, _dtype))
		{
			std::vector<DataType>* ptr_vec = (std::vector<DataType>*)vec;
			ptr_vec->insert(ptr_vec->begin()+i, element);
		}
		else
		{
			throw glass::TypeError("Try to insert " + type_name(DataType) + " value to std::vector<" + _dtype + ">");
		}
	}

	return i;
}

#define __INSERT_N_ELEMENTS(PtrType, DataType, n, element) \
if(same_type(DataType, unsigned short)) ptr_vec->insert(ptr_vec->begin()+index, n, (PtrType)force_cast<unsigned short>(element));\
else if(same_type(DataType, uint)) ptr_vec->insert(ptr_vec->begin()+index, n, (PtrType)force_cast<uint>(element));\
else if(same_type(DataType, long uint)) ptr_vec->insert(ptr_vec->begin()+index, n, (PtrType)force_cast<long uint>(element));\
else if(same_type(DataType, long long uint)) ptr_vec->insert(ptr_vec->begin()+index, n, (PtrType)force_cast<long long uint>(element));\
else if(same_type(DataType, short)) ptr_vec->insert(ptr_vec->begin()+index, n, (PtrType)force_cast<short>(element));\
else if(same_type(DataType, int)) ptr_vec->insert(ptr_vec->begin()+index, n, (PtrType)force_cast<int>(element));\
else if(same_type(DataType, long int)) ptr_vec->insert(ptr_vec->begin()+index, n, (PtrType)force_cast<long int>(element));\
else if(same_type(DataType, long long int)) ptr_vec->insert(ptr_vec->begin()+index, n, (PtrType)force_cast<long long int>(element));\
else if(same_type(DataType, float)) ptr_vec->insert(ptr_vec->begin()+index, n, (PtrType)force_cast<float>(element));\
else if(same_type(DataType, double)) ptr_vec->insert(ptr_vec->begin()+index, n, (PtrType)force_cast<double>(element));\
else throw glass::RuntimeError("Try to insert " + type_name(DataType) + " value to std::vector<" + _dtype + ">")

template<class DataType>
size_t GLVector::insert(long long int index, size_t n, const DataType& element) // reviewed
{
	size_t length = size();
	if(index < 0)
	{
		index += length;
	}
	if(index < 0 || index > length)
	{
		throw glass::IndexError(index, 0, length);
	}

	init<DataType>();
	if(using_VBO)
	{
		_size += n;
		expand();
		if(_dtype == "uint")
		{
			uint* pointer = (uint*)((unsigned char*)data.ptr() + index*GLSL::built_in_types[_dtype].glsl_size);
			if(_size-n-index != 0)
			{
				memmove((void*)((unsigned char*)pointer+n*GLSL::built_in_types[_dtype].glsl_size),
						(void*)pointer,
					    (_size-n-index)*GLSL::built_in_types[_dtype].glsl_size);
			}

			for(size_t i = 0; i < n; i++)
			{
				__HANDLE_ELEMENT(uint, DataType, element, *pointer=);
				pointer++;
			}

			data.apply();
		}
		else if(_dtype == "int")
		{
			int* pointer = (int*)((unsigned char*)data.ptr() + index*GLSL::built_in_types[_dtype].glsl_size);
			if(_size-n-index != 0)
			{
				memmove((void*)((unsigned char*)pointer+n*GLSL::built_in_types[_dtype].glsl_size),
						(void*)pointer,
					    (_size-n-index)*GLSL::built_in_types[_dtype].glsl_size);
			}

			for(size_t i = 0; i < n; i++)
			{
				__HANDLE_ELEMENT(int, DataType, element, *pointer=);
				pointer++;
			}

			data.apply();
		}
		else if(_dtype == "float")
		{
			float* pointer = (float*)((unsigned char*)data.ptr() + index*GLSL::built_in_types[_dtype].glsl_size);
			if(_size-n-index != 0)
			{
				memmove((void*)((unsigned char*)pointer+n*GLSL::built_in_types[_dtype].glsl_size),
						(void*)pointer,
					    (_size-n-index)*GLSL::built_in_types[_dtype].glsl_size);
			}

			for(size_t i = 0; i < n; i++)
			{
				__HANDLE_ELEMENT(float, DataType, element, *pointer=);
				pointer++;
			}

			data.apply();
		}
		else if(_dtype == "double")
		{
			double* pointer = (double*)((unsigned char*)data.ptr() + index*sizeof(double));
			if(_size-n-index != 0)
			{
				memmove((void*)((unsigned char*)pointer+n*sizeof(double)),
						(void*)pointer,
					    (_size-n-index)*sizeof(double));
			}

			for(size_t i = 0; i < n; i++)
			{
				__HANDLE_ELEMENT(double, DataType, element, *pointer=);
				pointer++;
			}
			data.apply();
		}
		else if(__SAME_WITH_DTYPE(DataType, _dtype))
		{
			unsigned char* pointer = (unsigned char*)data.ptr() + index*GLSL::built_in_types[_dtype].glsl_size;
			if(_size-n-index != 0)
			{
				memmove((void*)(pointer+n*GLSL::built_in_types[_dtype].glsl_size),
						(void*)pointer,
					    (_size-n-index)*GLSL::built_in_types[_dtype].glsl_size);
			}

			if(_dtype == "mat2")
			{
				for(size_t i = 0; i < n; i++)
				{
					memcpy((void*)pointer, force_cast<mat2>(element).data(), GLSL::built_in_types[_dtype].glsl_size);
					pointer += GLSL::built_in_types[_dtype].glsl_size;
				}
			}
			else if(_dtype == "mat3")
			{
				for(size_t i = 0; i < n; i++)
				{
					memcpy((void*)pointer, force_cast<mat3>(element).data(), GLSL::built_in_types[_dtype].glsl_size);
					pointer += GLSL::built_in_types[_dtype].glsl_size;
				}
			}
			else if(_dtype == "mat4")
			{
				for(size_t i = 0; i < n; i++)
				{
					memcpy((void*)pointer, force_cast<mat4>(element).data(), GLSL::built_in_types[_dtype].glsl_size);
					pointer += GLSL::built_in_types[_dtype].glsl_size;
				}
			}
			else // vec
			{
				for(size_t i = 0; i < n; i++)
				{
					memcpy((void*)pointer, (void*)(&element), GLSL::built_in_types[_dtype].glsl_size);
					pointer += GLSL::built_in_types[_dtype].glsl_size;
				}
			}
			
			data.apply();
		}
		else
		{
			throw glass::TypeError("Try to insert " + type_name(DataType) + " value to std::vector<" + _dtype + ">");
		}
	}
	else
	{
		if(_dtype == "uint")
		{
			std::vector<uint>* ptr_vec = (std::vector<uint>*)vec;
			__INSERT_N_ELEMENTS(uint, DataType, n, element);
		}
		else if(_dtype == "int")
		{
			std::vector<int>* ptr_vec = (std::vector<int>*)vec;
			__INSERT_N_ELEMENTS(int, DataType, n, element);
		}
		else if(_dtype == "float")
		{
			std::vector<float>* ptr_vec = (std::vector<float>*)vec;
			__INSERT_N_ELEMENTS(float, DataType, n, element);
		}
		else if(_dtype == "double")
		{
			std::vector<double>* ptr_vec = (std::vector<double>*)vec;
			__INSERT_N_ELEMENTS(double, DataType, n, element);
		}
		else if(__SAME_WITH_DTYPE(DataType, _dtype))
		{
			std::vector<DataType>* ptr_vec = (std::vector<DataType>*)vec;
			ptr_vec->insert(ptr_vec->begin()+index, n, element);
		}
		else
		{
			throw glass::TypeError("Try to insert " + type_name(DataType) + " value to std::vector<" + _dtype + ">");
		}
	}

	return index;
}

template<class DataType>
size_t GLVector::insert(long long int index, const typename std::vector<DataType>::const_iterator& first, const typename std::vector<DataType>::const_iterator& last) // reviewed
{
	size_t length = size();
	if(index < 0)
	{
		index += length;
	}
	if(index < 0 || index > length)
	{
		throw glass::IndexError(index, 0, length);
	}

	if(first >= last)
	{
		return index;
	}

	size_t n = last - first;

	init<DataType>();
	if(using_VBO)
	{
		_size += n;
		expand();
		if(_dtype == "uint")
		{
			uint* pointer = (uint*)((unsigned char*)data.ptr() + index*GLSL::built_in_types[_dtype].glsl_size);
			if(_size-n-index != 0)
			{
				memmove((void*)((unsigned char*)pointer+n*GLSL::built_in_types[_dtype].glsl_size),
						(void*)pointer,
					    (_size-n-index)*GLSL::built_in_types[_dtype].glsl_size);
			}

			for(auto it = first; it != last; it++)
			{
				__HANDLE_ELEMENT(uint, DataType, *it, *pointer=);
				pointer++;
			}

			data.apply();
		}
		else if(_dtype == "int")
		{
			int* pointer = (int*)((unsigned char*)data.ptr() + index*GLSL::built_in_types[_dtype].glsl_size);
			if(_size-n-index != 0)
			{
				memmove((void*)((unsigned char*)pointer+n*GLSL::built_in_types[_dtype].glsl_size),
						(void*)pointer,
					    (_size-n-index)*GLSL::built_in_types[_dtype].glsl_size);
			}

			for(auto it = first; it != last; it++)
			{
				__HANDLE_ELEMENT(int, DataType, *it, *pointer=);
				pointer++;
			}

			data.apply();
		}
		else if(_dtype == "float")
		{
			float* pointer = (float*)((unsigned char*)data.ptr() + index*GLSL::built_in_types[_dtype].glsl_size);
			if(_size-n-index != 0)
			{
				memmove((void*)((unsigned char*)pointer+n*GLSL::built_in_types[_dtype].glsl_size),
						(void*)pointer,
					    (_size-n-index)*GLSL::built_in_types[_dtype].glsl_size);
			}

			for(auto it = first; it != last; it++)
			{
				__HANDLE_ELEMENT(float, DataType, *it, *pointer=);
				pointer++;
			}

			data.apply();
		}
		else if(_dtype == "double")
		{
			double* pointer = (double*)((unsigned char*)data.ptr() + index*sizeof(double));
			if(_size-n-index != 0)
			{
				memmove((void*)((unsigned char*)pointer+n*sizeof(double)),
						(void*)pointer,
					    (_size-n-index)*sizeof(double));
			}

			for(auto it = first; it != last; it++)
			{
				__HANDLE_ELEMENT(double, DataType, *it, *pointer=);
				pointer++;
			}
			data.apply();
		}
		else if(__SAME_WITH_DTYPE(DataType, _dtype))
		{
			unsigned char* pointer = (unsigned char*)data.ptr() + index*GLSL::built_in_types[_dtype].glsl_size;
			if(_size-n-index != 0)
			{
				memmove((void*)(pointer+n*GLSL::built_in_types[_dtype].glsl_size),
						(void*)pointer,
					    (_size-n-index)*GLSL::built_in_types[_dtype].glsl_size);
			}

			if(_dtype == "mat2")
			{
				for(auto it = first; it != last; it++)
				{
					memcpy((void*)pointer, force_cast<mat2>(*it).data(), GLSL::built_in_types[_dtype].glsl_size);
					pointer += GLSL::built_in_types[_dtype].glsl_size;
				}
			}
			else if(_dtype == "mat3")
			{
				for(auto it = first; it != last; it++)
				{
					memcpy((void*)pointer, force_cast<mat3>(*it).data(), GLSL::built_in_types[_dtype].glsl_size);
					pointer += GLSL::built_in_types[_dtype].glsl_size;
				}
			}
			else if(_dtype == "mat4")
			{
				for(auto it = first; it != last; it++)
				{
					memcpy((void*)pointer, force_cast<mat4>(*it).data(), GLSL::built_in_types[_dtype].glsl_size);
					pointer += GLSL::built_in_types[_dtype].glsl_size;
				}
			}
			else // vec
			{
				for(auto it = first; it != last; it++)
				{
					memcpy((void*)pointer, (void*)(&(*it)), GLSL::built_in_types[_dtype].glsl_size);
					pointer += GLSL::built_in_types[_dtype].glsl_size;
				}
			}
			
			data.apply();
		}
		else
		{
			throw glass::TypeError("Try to insert " + type_name(DataType) + " value to std::vector<" + _dtype + ">");
		}
	}
	else
	{
		if(__SAME_WITH_DTYPE(DataType, _dtype))
		{
			std::vector<DataType>* ptr_vec = (std::vector<DataType>*)vec;
			ptr_vec->insert(ptr_vec->begin()+index, first, last);
		}
		else
		{
			throw glass::TypeError("Try to insert " + type_name(DataType) + " value to std::vector<" + _dtype + ">");
		}
	}

	return index;
}

#define __POP_BACK_SET_RETURN \
if(same_type(DataType, unsigned short)) return_value = force_cast<DataType>((unsigned short)back_value);\
else if(same_type(DataType, uint)) return_value = force_cast<DataType>((uint)back_value);\
else if(same_type(DataType, long uint)) return_value = force_cast<DataType>((long uint)back_value);\
else if(same_type(DataType, long long uint)) return_value = force_cast<DataType>((long long uint)back_value);\
else if(same_type(DataType, short)) return_value = force_cast<DataType>((short)back_value);\
else if(same_type(DataType, int)) return_value = force_cast<DataType>((int)back_value);\
else if(same_type(DataType, long int)) return_value = force_cast<DataType>((long int)back_value);\
else if(same_type(DataType, long long int)) return_value = force_cast<DataType>((long long int)back_value);\
else if(same_type(DataType, float)) return_value = force_cast<DataType>((float)back_value);\
else if(same_type(DataType, double)) return_value = force_cast<DataType>((double)back_value);\
else throw glass::TypeError("Try to pop " + type_name(DataType) + " value from std::vector<" + _dtype + ">");

template<class DataType>
DataType GLVector::pop_back()
{
	if(empty())
	{
		throw glass::RuntimeError("Try to pop from empty std::vector");
	}

	DataType return_value = this->back<DataType>();
	if(using_VBO)
	{
		_size--;
		shrink();
	}
	else
	{
		((std::vector<DataType>*)vec)->pop_back();
	}
	
	return return_value;
}

template<class DataType>
void GLVector::set(long long int i, const DataType& value)
{
	if(empty())
	{
		throw glass::RuntimeError("Try to set value in empty std::vector");
	}

	size_t length = size();
	if(i < 0)
	{
		i += length;
	}
	if(i < 0 || i >= length)
	{
		throw glass::IndexError(i, 0, length-1);
	}
	
	if(using_VBO)
	{
		if(_dtype == "uint")
		{
			uint* pointer = (uint*)((unsigned char*)data.ptr() + i*GLSL::built_in_types[_dtype].glsl_size);
			__HANDLE_ELEMENT(uint, DataType, value, *pointer=);
			data.apply();
		}
		else if(_dtype == "int")
		{
			int* pointer = (int*)((unsigned char*)data.ptr() + i*GLSL::built_in_types[_dtype].glsl_size);
			__HANDLE_ELEMENT(int, DataType, value, *pointer=);
			data.apply();
		}
		else if(_dtype == "float")
		{
			float* pointer = (float*)((unsigned char*)data.ptr() + i*GLSL::built_in_types[_dtype].glsl_size);
			__HANDLE_ELEMENT(float, DataType, value, *pointer=);
			data.apply();
		}
		else if(_dtype == "double")
		{
			double* pointer = (double*)((unsigned char*)data.ptr() + i*GLSL::built_in_types[_dtype].glsl_size);
			__HANDLE_ELEMENT(double, DataType, value, *pointer=);
			data.apply();
		}
		else if(__SAME_WITH_DTYPE(DataType, _dtype))
		{
			void* pointer = (void*)((unsigned char*)data.ptr() + i*GLSL::built_in_types[_dtype].glsl_size);
			if(_dtype == "mat2")
			{
				memcpy(pointer, force_cast<mat2>(value).data(), GLSL::built_in_types[_dtype].glsl_size);
			}
			else if(_dtype == "mat3")
			{
				memcpy(pointer, force_cast<mat3>(value).data(), GLSL::built_in_types[_dtype].glsl_size);
			}
			else if(_dtype == "mat4")
			{
				memcpy(pointer, force_cast<mat4>(value).data(), GLSL::built_in_types[_dtype].glsl_size);
			}
			else
			{
				memcpy(pointer, (void*)(&value), GLSL::built_in_types[_dtype].glsl_size);
			}
			data.apply();
		}
		else
		{
			throw glass::TypeError("Try to insert " + type_name(DataType) + " value to std::vector<" + _dtype + ">");
		}
	}
	else
	{
		if(_dtype == "uint")
		{
			std::vector<uint>* ptr_vec = (std::vector<uint>*)vec;
			__INSERT_ELEMENT(uint, DataType, value);
		}
		else if(_dtype == "int")
		{
			std::vector<int>* ptr_vec = (std::vector<int>*)vec;
			__INSERT_ELEMENT(int, DataType, value);
		}
		else if(_dtype == "float")
		{
			std::vector<float>* ptr_vec = (std::vector<float>*)vec;
			__INSERT_ELEMENT(float, DataType, value);
		}
		else if(_dtype == "double")
		{
			std::vector<double>* ptr_vec = (std::vector<double>*)vec;
			__INSERT_ELEMENT(double, DataType, value);
		}
		else if(__SAME_WITH_DTYPE(DataType, _dtype))
		{
			std::vector<DataType>* ptr_vec = (std::vector<DataType>*)vec;
			ptr_vec->insert(ptr_vec->begin()+i, value);
		}
		else
		{
			throw glass::TypeError("Try to insert " + type_name(DataType) + " value to std::vector<" + _dtype + ">");
		}
	}
}

template<class DataType>
DataType GLVector::get(long long int i)
{
	if(empty())
	{
		throw glass::RuntimeError("Try to access value in empty std::vector");
	}

	size_t length = size();
	if(i < 0)
	{
		i += length;
	}
	if(i < 0 || i >= length)
	{
		throw glass::IndexError(i, 0, length-1);
	}

	if(!__SAME_WITH_DTYPE(DataType, _dtype))
	{
		throw glass::TypeError("Try to access " + type_name(DataType) + " value from std::vector<" + _dtype + ">");
	}
	
	if(using_VBO)
	{
		DataType return_value;
		unsigned char* dest_ptr = (unsigned char*)(&return_value);
		unsigned char* src_ptr = (unsigned char*)data.ptr() + i * GLSL::built_in_types[_dtype].glsl_size;

		if(_dtype == "int" || _dtype == "uint" || _dtype == "float" || _dtype == "double")
		{
			return_value = *((DataType*)src_ptr);
		}
		else if(_dtype == "mat2")
		{
			memcpy((void*)dest_ptr, (void*)src_ptr, GLSL::built_in_types["vec2"].glsl_size);
			src_ptr += GLSL::built_in_types["vec2"].glsl_size;
			dest_ptr += GLSL::built_in_types["vec2"].size;
			memcpy((void*)dest_ptr, (void*)src_ptr, GLSL::built_in_types["vec2"].glsl_size);
		}
		else if(_dtype == "mat3")
		{
			memcpy((void*)dest_ptr, (void*)src_ptr, GLSL::built_in_types["vec3"].glsl_size);
			src_ptr += GLSL::built_in_types["vec3"].glsl_size;
			dest_ptr += GLSL::built_in_types["vec3"].size;
			memcpy((void*)dest_ptr, (void*)src_ptr, GLSL::built_in_types["vec3"].glsl_size);
			src_ptr += GLSL::built_in_types["vec3"].glsl_size;
			dest_ptr += GLSL::built_in_types["vec3"].size;
			memcpy((void*)dest_ptr, (void*)src_ptr, GLSL::built_in_types["vec3"].glsl_size);
		}
		else if(_dtype == "mat4")
		{
			memcpy((void*)dest_ptr, (void*)src_ptr, GLSL::built_in_types["vec4"].glsl_size);
			src_ptr += GLSL::built_in_types["vec4"].glsl_size;
			dest_ptr += GLSL::built_in_types["vec4"].size;
			memcpy((void*)dest_ptr, (void*)src_ptr, GLSL::built_in_types["vec4"].glsl_size);
			src_ptr += GLSL::built_in_types["vec4"].glsl_size;
			dest_ptr += GLSL::built_in_types["vec4"].size;
			memcpy((void*)dest_ptr, (void*)src_ptr, GLSL::built_in_types["vec4"].glsl_size);
			src_ptr += GLSL::built_in_types["vec4"].glsl_size;
			dest_ptr += GLSL::built_in_types["vec4"].size;
			memcpy((void*)dest_ptr, (void*)src_ptr, GLSL::built_in_types["vec4"].glsl_size);
		}
		else // vec
		{
			memcpy((void*)dest_ptr, (void*)src_ptr, GLSL::built_in_types[_dtype].glsl_size);
		}
		data.apply();

		return return_value;
	}
	else
	{
		return ((std::vector<DataType>*)vec)->at(i);
	}
}

template<class DataType>
DataType GLVector::back()
{
	return get<DataType>(-1);
}

template<class DataType>
DataType GLVector::front()
{
	return get<DataType>(0);
}

template<class DataType>
GLVector::operator std::vector<DataType>()const
{
	if(empty())
	{
		if(same_type(DataType, uint) ||
		   same_type(DataType, int) ||
		   same_type(DataType, float) ||
		   same_type(DataType, double) ||
		   same_type(DataType, ivec2) ||
		   same_type(DataType, ivec3) ||
		   same_type(DataType, ivec4) ||
		   same_type(DataType, uvec2) ||
		   same_type(DataType, uvec3) ||
		   same_type(DataType, uvec4) ||
		   same_type(DataType, vec2) ||
		   same_type(DataType, vec3) ||
		   same_type(DataType, vec4) ||
		   same_type(DataType, dvec2) ||
		   same_type(DataType, dvec3) ||
		   same_type(DataType, dvec4) ||
		   same_type(DataType, mat2) ||
		   same_type(DataType, mat3) ||
		   same_type(DataType, mat4))
		{
			return std::vector<DataType>();
		}
		else
		{
			throw glass::TypeError("Not supported DataType " + type_name(DataType));
		}
	}

	if(!__SAME_WITH_DTYPE(DataType, _dtype))
	{
		throw glass::TypeError("Can not convert std::vector<" + _dtype + "> to std::vector<" + type_name(DataType) + ">");
	}
	
	if(using_VBO)
	{
		int length = size();
		std::vector<DataType> result(length);
		for(int i = 0; i < length; i++)
		{
			result[i] = get<DataType>(i);
		}

		return result;
	}
	else
	{
		return *((std::vector<DataType>*)vec);
	}
}

#endif