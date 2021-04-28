#ifndef __GLVECTOR_H__
#define __GLVECTOR_H__

#include "vec.h"
#include "mat.h"
#include "type.h"
#include "exception.h"
#include <vector>

class GLVector
{
	string _dtype;
	void* vec = NULL;

public:
	void init(const string& __dtype);

	template<class DataType>
	void init();

public:
	GLVector();
	GLVector(const string& __dtype);
	GLVector(const string& __dtype, uint n);

	template<class DataType>
	GLVector(uint n, const DataType& value);
	// GLVector(const_iterator& first, const_iterator& last);
	
	template<class DataType>
	GLVector(typename vector<DataType>::const_iterator& first, typename vector<DataType>::const_iterator& last);
	
	GLVector(const GLVector& new_vec);
	GLVector(GLVector&& new_vec);

	template<class DataType>
	GLVector(const vector<DataType>& new_vec);
	~GLVector();

	bool empty()const;
	uint size()const;
	const string& dtype()const;

	GLVector& operator =(const GLVector& new_vec);
	GLVector& operator =(GLVector&& new_vec);

	template<class DataType>
	GLVector& operator =(const vector<DataType>& new_vec);

	template<class DataType>
	void push_back(const DataType& element);

	template<class DataType>
	void push_front(const DataType& element);

	// template<class DataType>
	// iterator insert(const_iterator& it, const DataType& element);

	// template<class DataType>
	// iterator insert(const_iterator& it, int n, const DataType& element);

	// template<class DataType>
	// iterator insert(const_iterator& it, vector<DataType>::const_iterator& first, vector<DataType>::const_iterator& last);

	// iterator insert(const_iterator& it, const_iterator& first, const_iterator& last);

	template<class DataType>
	int insert(int i, const DataType& element);

	template<class DataType>
	int insert(int i, int n, const DataType& element);

	template<class DataType>
	int insert(int i, typename vector<DataType>::const_iterator& first, typename vector<DataType>::const_iterator& last);

	// int insert(int i, const_iterator& first, const_iterator& last);

	void pop_back();

	template<class DataType>
	DataType pop_back();

	void pop_front();

	template<class DataType>
	DataType pop_front();

	void pop(int i = -1);

	template<class DataType>
	DataType pop(int i = -1);

	// void pop(const_iterator& it);

	// template<class DataType>
	// DataType pop(const_iterator& it);

	int erase(int i, int len = 1);
	// iterator erase(const_iterator& it, int len = 1);
	// iterator erase(const_iterator& first, const_iterator& last);

	GLVector slice(int pos_start, int pos_end);
	// GLVector slice(const_iterator& first, const_iterator& last);

	// template<class DataType>
	// vector<DataType> slice(const_iterator& first, const_iterator& last);

	template<class DataType>
	vector<DataType> slice(int pos_start, int pos_end);

	void extend(const GLVector& new_vec);

	template<class DataType>
	void extend(const vector<DataType>& new_vec);

	template<class DataType>
	const DataType& front()const;

	template<class DataType>
	DataType& front();

	template<class DataType>
	const DataType& back()const;

	template<class DataType>
	DataType& back();

	template<class DataType>
	const DataType& at(int i)const;

	template<class DataType>
	DataType& at(int i);

	template<class DataType>
	DataType* ptr(int i);

	void* ptr(int i);

	template<class DataType>
	operator vector<DataType>()const;

	void clear();
};

template<class DataType>
void GLVector::init()
{
	if(vec) return;

	if(same_type(DataType, bool) || same_type(DataType, _Bit_reference)) _dtype = "bool";
	else if(same_type(DataType, uint)) _dtype = "uint";
	else if(same_type(DataType, int)) _dtype = "int";
	else if(same_type(DataType, float)) _dtype = "float";
	else if(same_type(DataType, double)) _dtype = "double";
	else if(same_type(DataType, bvec2)) _dtype = "bvec2";
	else if(same_type(DataType, bvec3)) _dtype = "bvec3";
	else if(same_type(DataType, bvec4)) _dtype = "bvec4";
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
	else throw TypeError("Not supported DataType " + type_name(DataType) + ".");

	if(same_type(DataType, bool) || same_type(DataType, _Bit_reference))
		vec = (void*)(new vector<byte>());
	else
		vec = (void*)(new vector<DataType>());

	if(!vec) throw MemoryError("Failed to allocate memory!");
}

template<class DataType>
GLVector::GLVector(uint n, const DataType& value)
{
	if(same_type(DataType, bool) || same_type(DataType, _Bit_reference)) _dtype = "bool";
	else if(same_type(DataType, uint)) _dtype = "uint";
	else if(same_type(DataType, int)) _dtype = "int";
	else if(same_type(DataType, float)) _dtype = "float";
	else if(same_type(DataType, double)) _dtype = "double";
	else if(same_type(DataType, bvec2)) _dtype = "bvec2";
	else if(same_type(DataType, bvec3)) _dtype = "bvec3";
	else if(same_type(DataType, bvec4)) _dtype = "bvec4";
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
	else throw TypeError("Not supported DataType " + type_name(DataType) + ".");

	if(same_type(DataType, bool))
		vec = (void*)(new vector<byte>(n, (byte)(force_cast<bool>(value))));
	else if(same_type(DataType, _Bit_reference))
		vec = (void*)(new vector<byte>(n, (byte)((bool)(force_cast<_Bit_reference>(value)))));
	else
		vec = (void*)(new vector<DataType>(n, value));

	if(!vec) throw MemoryError("Failed to allocate memory!");
}

template<class DataType>
GLVector::GLVector(typename vector<DataType>::const_iterator& first, typename vector<DataType>::const_iterator& last)
{
	if(first >= last)
	{
		init<DataType>();
		return;
	}

	if(same_type(DataType, bool)) _dtype = "bool";
	else if(same_type(DataType, uint)) _dtype = "uint";
	else if(same_type(DataType, int)) _dtype = "int";
	else if(same_type(DataType, float)) _dtype = "float";
	else if(same_type(DataType, double)) _dtype = "double";
	else if(same_type(DataType, bvec2)) _dtype = "bvec2";
	else if(same_type(DataType, bvec3)) _dtype = "bvec3";
	else if(same_type(DataType, bvec4)) _dtype = "bvec4";
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
	else throw TypeError("Not supported DataType " + type_name(DataType));

	if(same_type(DataType, bool))
	{
		int length = last-begin;
		if(length <= 0)
		{
			vec = (void*)(new vector<byte>());
			if(!vec) throw MemoryError("Failed to allocate memory!");
		}
		else
		{
			vec = (void*)(new vector<byte>(length));
			if(!vec) throw MemoryError("Failed to allocate memory!");

			for(int i = 0; i < length; i++)
			{
				(*((vector<byte>*)vec))[i] = (byte)((bool)force_cast<_Bit_reference>(*(first + i)));
			}
		}
	}
	else
	{
		vec = (void*)(new vector<DataType>(first, last));
		if(!vec) throw MemoryError("Failed to allocate memory!");
	}
}

template<class DataType>
GLVector::GLVector(const vector<DataType>& new_vec)
{
	if(same_type(DataType, bool)) _dtype = "bool";
	else if(same_type(DataType, uint)) _dtype = "uint";
	else if(same_type(DataType, int)) _dtype = "int";
	else if(same_type(DataType, float)) _dtype = "float";
	else if(same_type(DataType, double)) _dtype = "double";
	else if(same_type(DataType, bvec2)) _dtype = "bvec2";
	else if(same_type(DataType, bvec3)) _dtype = "bvec3";
	else if(same_type(DataType, bvec4)) _dtype = "bvec4";
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
	else throw TypeError("Not supported DataType " + type_name(DataType) + ".");

	if(same_type(DataType, bool))
	{
		int length = new_vec.size();

		vec = (void*)(new vector<byte>(length));
		if(!vec) throw MemoryError("Failed to allocate memory!");
		
		for(int i = 0; i < length; i++)
		{
			(*((vector<byte>*)vec))[i] = (byte)((bool)force_cast<_Bit_reference>(new_vec[i]));
		}
	}
	else
	{
		vec = (void*)(new vector<DataType>(new_vec));
		if(!vec) throw MemoryError("Failed to allocate memory!");
	}
}

template<class DataType>
GLVector& GLVector::operator =(const vector<DataType>& new_vec)
{
	if(((_dtype == "uint" && same_type(DataType, uint)) ||
	    (_dtype == "int" && same_type(DataType, int)) ||
	    (_dtype == "float" && same_type(DataType, float)) ||
	    (_dtype == "double" && same_type(DataType, double)) ||
	    (_dtype == "bvec2" && same_type(DataType, bvec2)) ||
	    (_dtype == "bvec3" && same_type(DataType, bvec3)) ||
	    (_dtype == "bvec4" && same_type(DataType, bvec4)) ||
	    (_dtype == "ivec2" && same_type(DataType, ivec2)) ||
	    (_dtype == "ivec3" && same_type(DataType, ivec3)) ||
	    (_dtype == "ivec4" && same_type(DataType, ivec4)) ||
	    (_dtype == "uvec2" && same_type(DataType, uvec2)) ||
	    (_dtype == "uvec3" && same_type(DataType, uvec3)) ||
	    (_dtype == "uvec4" && same_type(DataType, uvec4)) ||
	    (_dtype == "vec2" && same_type(DataType, vec2)) ||
	    (_dtype == "vec3" && same_type(DataType, vec3)) ||
	    (_dtype == "vec4" && same_type(DataType, vec4)) ||
	    (_dtype == "dvec2" && same_type(DataType, dvec2)) ||
	    (_dtype == "dvec3" && same_type(DataType, dvec3)) ||
	    (_dtype == "dvec4" && same_type(DataType, dvec4)) ||
	    (_dtype == "mat2" && same_type(DataType, mat2)) ||
	    (_dtype == "mat3" && same_type(DataType, mat3)) ||
	    (_dtype == "mat4" && same_type(DataType, mat4))) && vec != NULL)
	{
		*((vector<DataType>*)vec) = new_vec;

		return *this;
	}
	else if(_dtype == "bool" && same_type(DataType, bool) && vec != NULL)
	{
		vector<byte>* ptr_vec = (vector<byte>*)vec;
		for(int i = 0; i < ptr_vec->size(); i++)
		{
			(*ptr_vec)[i] = (byte)((bool)force_cast<_Bit_reference>(new_vec[i]));
		}
		for(int i = ptr_vec->size(); i < new_vec.size(); i++)
		{
			ptr_vec->push_back((byte)((bool)force_cast<_Bit_reference>(new_vec[i])));
		}
	}

	string temp_dtype;
	if(same_type(DataType, bool)) temp_dtype = "bool";
	else if(same_type(DataType, uint)) temp_dtype = "uint";
	else if(same_type(DataType, int)) temp_dtype = "int";
	else if(same_type(DataType, float)) temp_dtype = "float";
	else if(same_type(DataType, double)) temp_dtype = "double";
	else if(same_type(DataType, bvec2)) temp_dtype = "bvec2";
	else if(same_type(DataType, bvec3)) temp_dtype = "bvec3";
	else if(same_type(DataType, bvec4)) temp_dtype = "bvec4";
	else if(same_type(DataType, ivec2)) temp_dtype = "ivec2";
	else if(same_type(DataType, ivec3)) temp_dtype = "ivec3";
	else if(same_type(DataType, ivec4)) temp_dtype = "ivec4";
	else if(same_type(DataType, uvec2)) temp_dtype = "uvec2";
	else if(same_type(DataType, uvec3)) temp_dtype = "uvec3";
	else if(same_type(DataType, uvec4)) temp_dtype = "uvec4";
	else if(same_type(DataType, vec2)) temp_dtype = "vec2";
	else if(same_type(DataType, vec3)) temp_dtype = "vec3";
	else if(same_type(DataType, vec4)) temp_dtype = "vec4";
	else if(same_type(DataType, dvec2)) temp_dtype = "dvec2";
	else if(same_type(DataType, dvec3)) temp_dtype = "dvec3";
	else if(same_type(DataType, dvec4)) temp_dtype = "dvec4";
	else if(same_type(DataType, mat2)) temp_dtype = "mat2";
	else if(same_type(DataType, mat3)) temp_dtype = "mat3";
	else if(same_type(DataType, mat4)) temp_dtype = "mat4";
	else throw TypeError("Not supported DataType " + type_name(DataType));
	
	clear();

	_dtype = temp_dtype;

	if(same_type(DataType, bool))
	{
		int length = new_vec.size();
		vec = (void*)(new vector<byte>(length));
		if(!vec) throw MemoryError("Failed to allocate memory!");

		vector<byte>* ptr_vec = (vector<byte>*)vec;
		for(int i = 0; i < length; i++)
		{
			(*ptr_vec)[i] = (byte)((bool)force_cast<_Bit_reference>(new_vec[i]));
		}
	}
	else
	{
		vec = (void*)(new vector<DataType>(new_vec));
		if(!vec) throw MemoryError("Failed to allocate memory!");
	}

	return *this;
}

template<class DataType>
void GLVector::push_back(const DataType& element)
{
	init<DataType>();
	if(_dtype == "bool")
	{
		vector<byte>* ptr_vec = (vector<byte>*)vec;
		if(same_type(DataType, bool)) ptr_vec->push_back((byte)force_cast<bool>(element));
		else if(same_type(DataType, _Bit_reference)) ptr_vec->push_back((byte)((bool)force_cast<_Bit_reference>(element)));
		else if(same_type(DataType, uint)) ptr_vec->push_back((byte)((bool)force_cast<uint>(element)));
		else if(same_type(DataType, int)) ptr_vec->push_back((byte)((bool)force_cast<int>(element)));
		else if(same_type(DataType, float)) ptr_vec->push_back((byte)((bool)force_cast<float>(element)));
		else if(same_type(DataType, double)) ptr_vec->push_back((byte)((bool)force_cast<double>(element)));
		else throw TypeError("Try to push " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}
	else if(_dtype == "uint")
	{
		vector<uint>* ptr_vec = (vector<uint>*)vec;
		if(same_type(DataType, bool)) ptr_vec->push_back((uint)(force_cast<bool>(element)));
		else if(same_type(DataType, _Bit_reference)) ptr_vec->push_back((uint)((bool)force_cast<_Bit_reference>(element)));
		else if(same_type(DataType, uint)) ptr_vec->push_back(force_cast<uint>(element));
		else if(same_type(DataType, int)) ptr_vec->push_back((uint)(force_cast<int>(element)));
		else if(same_type(DataType, float)) ptr_vec->push_back((uint)(force_cast<float>(element)));
		else if(same_type(DataType, double)) ptr_vec->push_back((uint)(force_cast<double>(element)));
		else throw TypeError("Try to push " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}
	else if(_dtype == "int")
	{
		vector<int>* ptr_vec = (vector<int>*)vec;
		if(same_type(DataType, bool)) ptr_vec->push_back((int)(force_cast<bool>(element)));
		else if(same_type(DataType, _Bit_reference)) ptr_vec->push_back((int)((bool)force_cast<_Bit_reference>(element)));
		else if(same_type(DataType, uint)) ptr_vec->push_back((int)(force_cast<uint>(element)));
		else if(same_type(DataType, int)) ptr_vec->push_back(force_cast<int>(element));
		else if(same_type(DataType, float)) ptr_vec->push_back((int)(force_cast<float>(element)));
		else if(same_type(DataType, double)) ptr_vec->push_back((int)(force_cast<double>(element)));
		else throw TypeError("Try to push " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}
	else if(_dtype == "float")
	{
		vector<float>* ptr_vec = (vector<float>*)vec;
		if(same_type(DataType, bool)) ptr_vec->push_back((float)(force_cast<bool>(element)));
		else if(same_type(DataType, _Bit_reference)) ptr_vec->push_back((float)((bool)force_cast<_Bit_reference>(element)));
		else if(same_type(DataType, uint)) ptr_vec->push_back((float)(force_cast<uint>(element)));
		else if(same_type(DataType, int)) ptr_vec->push_back((float)(force_cast<int>(element)));
		else if(same_type(DataType, float)) ptr_vec->push_back(force_cast<float>(element));
		else if(same_type(DataType, double)) ptr_vec->push_back((float)(force_cast<double>(element)));
		else throw TypeError("Try to push " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}
	else if(_dtype == "double")
	{
		vector<double>* ptr_vec = (vector<double>*)vec;
		if(same_type(DataType, bool)) ptr_vec->push_back((double)(force_cast<bool>(element)));
		else if(same_type(DataType, _Bit_reference)) ptr_vec->push_back((double)((bool)force_cast<_Bit_reference>(element)));
		else if(same_type(DataType, uint)) ptr_vec->push_back((double)(force_cast<uint>(element)));
		else if(same_type(DataType, int)) ptr_vec->push_back((double)(force_cast<int>(element)));
		else if(same_type(DataType, float)) ptr_vec->push_back((double)(force_cast<float>(element)));
		else if(same_type(DataType, double)) ptr_vec->push_back(force_cast<double>(element));
		else throw TypeError("Try to push " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}
	else if((_dtype == "bvec2" && same_type(DataType, bvec2)) ||
		    (_dtype == "bvec3" && same_type(DataType, bvec3)) ||
		    (_dtype == "bvec4" && same_type(DataType, bvec4)) ||
		    (_dtype == "ivec2" && same_type(DataType, ivec2)) ||
		    (_dtype == "ivec3" && same_type(DataType, ivec3)) ||
		    (_dtype == "ivec4" && same_type(DataType, ivec4)) ||
		    (_dtype == "uvec2" && same_type(DataType, uvec2)) ||
		    (_dtype == "uvec3" && same_type(DataType, uvec3)) ||
		    (_dtype == "uvec4" && same_type(DataType, uvec4)) ||
		    (_dtype == "vec2" && same_type(DataType, vec2)) ||
		    (_dtype == "vec3" && same_type(DataType, vec3)) ||
		    (_dtype == "vec4" && same_type(DataType, vec4)) ||
		    (_dtype == "dvec2" && same_type(DataType, dvec2)) ||
		    (_dtype == "dvec3" && same_type(DataType, dvec3)) ||
		    (_dtype == "dvec4" && same_type(DataType, dvec4)) ||
		    (_dtype == "mat2" && same_type(DataType, mat2)) ||
		    (_dtype == "mat3" && same_type(DataType, mat3)) ||
		    (_dtype == "mat4" && same_type(DataType, mat4)))
	{
		((vector<DataType>*)vec)->push_back(element);
	}
	else
	{
		throw TypeError("Try to push " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}
}

template<class DataType>
void GLVector::push_front(const DataType& element)
{
	init<DataType>();
	if(_dtype == "bool")
	{
		vector<byte>* ptr_vec = (vector<byte>*)vec;
		if(same_type(DataType, bool)) ptr_vec->insert(ptr_vec->begin(), (byte)force_cast<bool>(element));
		else if(same_type(DataType, _Bit_reference)) ptr_vec->insert(ptr_vec->begin(), (byte)((bool)force_cast<_Bit_reference>(element)));
		else if(same_type(DataType, uint)) ptr_vec->insert(ptr_vec->begin(), (byte)((bool)force_cast<uint>(element)));
		else if(same_type(DataType, int)) ptr_vec->insert(ptr_vec->begin(), (byte)((bool)force_cast<int>(element)));
		else if(same_type(DataType, float)) ptr_vec->insert(ptr_vec->begin(), (byte)((bool)force_cast<float>(element)));
		else if(same_type(DataType, double)) ptr_vec->insert(ptr_vec->begin(), (byte)((bool)force_cast<double>(element)));
		else throw TypeError("Try to push " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}
	else if(_dtype == "uint")
	{
		vector<uint>* ptr_vec = (vector<uint>*)vec;
		if(same_type(DataType, bool)) ptr_vec->insert(ptr_vec->begin(), (uint)(force_cast<bool>(element)));
		else if(same_type(DataType, _Bit_reference)) ptr_vec->insert(ptr_vec->begin(), (uint)((bool)force_cast<_Bit_reference>(element)));
		else if(same_type(DataType, uint)) ptr_vec->insert(ptr_vec->begin(), force_cast<uint>(element));
		else if(same_type(DataType, int)) ptr_vec->insert(ptr_vec->begin(), (uint)(force_cast<int>(element)));
		else if(same_type(DataType, float)) ptr_vec->insert(ptr_vec->begin(), (uint)(force_cast<float>(element)));
		else if(same_type(DataType, double)) ptr_vec->insert(ptr_vec->begin(), (uint)(force_cast<double>(element)));
		else throw TypeError("Try to push " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}
	else if(_dtype == "int")
	{
		vector<int>* ptr_vec = (vector<int>*)vec;
		if(same_type(DataType, bool)) ptr_vec->insert(ptr_vec->begin(), (int)(force_cast<bool>(element)));
		else if(same_type(DataType, _Bit_reference)) ptr_vec->insert(ptr_vec->begin(), (int)((bool)force_cast<_Bit_reference>(element)));
		else if(same_type(DataType, uint)) ptr_vec->insert(ptr_vec->begin(), (int)(force_cast<uint>(element)));
		else if(same_type(DataType, int)) ptr_vec->insert(ptr_vec->begin(), force_cast<int>(element));
		else if(same_type(DataType, float)) ptr_vec->insert(ptr_vec->begin(), (int)(force_cast<float>(element)));
		else if(same_type(DataType, double)) ptr_vec->insert(ptr_vec->begin(), (int)(force_cast<double>(element)));
		else throw TypeError("Try to push " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}
	else if(_dtype == "float")
	{
		vector<float>* ptr_vec = (vector<float>*)vec;
		if(same_type(DataType, bool)) ptr_vec->insert(ptr_vec->begin(), (float)(force_cast<bool>(element)));
		else if(same_type(DataType, _Bit_reference)) ptr_vec->insert(ptr_vec->begin(), (float)((bool)force_cast<_Bit_reference>(element)));
		else if(same_type(DataType, uint)) ptr_vec->insert(ptr_vec->begin(), (float)(force_cast<uint>(element)));
		else if(same_type(DataType, int)) ptr_vec->insert(ptr_vec->begin(), (float)(force_cast<int>(element)));
		else if(same_type(DataType, float)) ptr_vec->insert(ptr_vec->begin(), force_cast<float>(element));
		else if(same_type(DataType, double)) ptr_vec->insert(ptr_vec->begin(), (float)(force_cast<double>(element)));
		else throw TypeError("Try to push " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}
	else if(_dtype == "double")
	{
		vector<double>* ptr_vec = (vector<double>*)vec;
		if(same_type(DataType, bool)) ptr_vec->insert(ptr_vec->begin(), (double)(force_cast<bool>(element)));
		else if(same_type(DataType, _Bit_reference)) ptr_vec->insert(ptr_vec->begin(), (double)((bool)force_cast<_Bit_reference>(element)));
		else if(same_type(DataType, uint)) ptr_vec->insert(ptr_vec->begin(), (double)(force_cast<uint>(element)));
		else if(same_type(DataType, int)) ptr_vec->insert(ptr_vec->begin(), (double)(force_cast<int>(element)));
		else if(same_type(DataType, float)) ptr_vec->insert(ptr_vec->begin(), (double)(force_cast<float>(element)));
		else if(same_type(DataType, double)) ptr_vec->insert(ptr_vec->begin(), force_cast<double>(element));
		else throw TypeError("Try to push " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}
	else if((_dtype == "bvec2" && same_type(DataType, bvec2)) ||
		    (_dtype == "bvec3" && same_type(DataType, bvec3)) ||
		    (_dtype == "bvec4" && same_type(DataType, bvec4)) ||
		    (_dtype == "ivec2" && same_type(DataType, ivec2)) ||
		    (_dtype == "ivec3" && same_type(DataType, ivec3)) ||
		    (_dtype == "ivec4" && same_type(DataType, ivec4)) ||
		    (_dtype == "uvec2" && same_type(DataType, uvec2)) ||
		    (_dtype == "uvec3" && same_type(DataType, uvec3)) ||
		    (_dtype == "uvec4" && same_type(DataType, uvec4)) ||
		    (_dtype == "vec2" && same_type(DataType, vec2)) ||
		    (_dtype == "vec3" && same_type(DataType, vec3)) ||
		    (_dtype == "vec4" && same_type(DataType, vec4)) ||
		    (_dtype == "dvec2" && same_type(DataType, dvec2)) ||
		    (_dtype == "dvec3" && same_type(DataType, dvec3)) ||
		    (_dtype == "dvec4" && same_type(DataType, dvec4)) ||
		    (_dtype == "mat2" && same_type(DataType, mat2)) ||
		    (_dtype == "mat3" && same_type(DataType, mat3)) ||
		    (_dtype == "mat4" && same_type(DataType, mat4)))
	{
		vector<DataType>* ptr_vec = (vector<DataType>*)vec;
		ptr_vec->insert(ptr_vec->begin(), element);
	}
	else
	{
		throw TypeError("Try to push " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}
}

// template<class DataType>
// GLVector::iterator GLVector::insert(GLVector::const_iterator& it, vector<DataType>::const_iterator& first, vector<DataType>::const_iterator& last)
// {
// 	int i = it - begin();
// 	insert(i, first, last);
// 	return begin() + i;
// }

template<class DataType>
int GLVector::insert(int i, const DataType& element)
{
	int length = size();
	if(i < 0)
	{
		i += length;
	}
	if(i < 0 || i > length)
	{
		throw IndexError(i, 0, length);
	}

	init<DataType>();
	if(_dtype == "bool")
	{
		vector<byte>* ptr_vec = (vector<byte>*)vec;
		if(same_type(DataType, bool)) ptr_vec->insert(ptr_vec->begin()+i, (byte)force_cast<bool>(element));
		else if(same_type(DataType, _Bit_reference)) ptr_vec->insert(ptr_vec->begin()+i, (byte)((bool)force_cast<_Bit_reference>(element)));
		else if(same_type(DataType, uint)) ptr_vec->insert(ptr_vec->begin()+i, (byte)((bool)force_cast<uint>(element)));
		else if(same_type(DataType, int)) ptr_vec->insert(ptr_vec->begin()+i, (byte)((bool)force_cast<int>(element)));
		else if(same_type(DataType, float)) ptr_vec->insert(ptr_vec->begin()+i, (byte)((bool)force_cast<float>(element)));
		else if(same_type(DataType, double)) ptr_vec->insert(ptr_vec->begin()+i, (byte)((bool)force_cast<double>(element)));
		else throw RuntimeError("Try to insert " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}
	else if(_dtype == "uint")
	{
		vector<uint>* ptr_vec = (vector<uint>*)vec;
		if(same_type(DataType, bool)) ptr_vec->insert(ptr_vec->begin()+i, (uint)(force_cast<bool>(element)));
		else if(same_type(DataType, _Bit_reference)) ptr_vec->insert(ptr_vec->begin()+i, (uint)((bool)force_cast<_Bit_reference>(element)));
		else if(same_type(DataType, uint)) ptr_vec->insert(ptr_vec->begin()+i, force_cast<uint>(element));
		else if(same_type(DataType, int)) ptr_vec->insert(ptr_vec->begin()+i, (uint)(force_cast<int>(element)));
		else if(same_type(DataType, float)) ptr_vec->insert(ptr_vec->begin()+i, (uint)(force_cast<float>(element)));
		else if(same_type(DataType, double)) ptr_vec->insert(ptr_vec->begin()+i, (uint)(force_cast<double>(element)));
		else throw RuntimeError("Try to insert " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}
	else if(_dtype == "int")
	{
		vector<int>* ptr_vec = (vector<int>*)vec;
		if(same_type(DataType, bool)) ptr_vec->insert(ptr_vec->begin()+i, (int)(force_cast<bool>(element)));
		else if(same_type(DataType, _Bit_reference)) ptr_vec->insert(ptr_vec->begin()+i, (int)((bool)force_cast<_Bit_reference>(element)));
		else if(same_type(DataType, uint)) ptr_vec->insert(ptr_vec->begin()+i, (int)(force_cast<uint>(element)));
		else if(same_type(DataType, int)) ptr_vec->insert(ptr_vec->begin()+i, force_cast<int>(element));
		else if(same_type(DataType, float)) ptr_vec->insert(ptr_vec->begin()+i, (int)(force_cast<float>(element)));
		else if(same_type(DataType, double)) ptr_vec->insert(ptr_vec->begin()+i, (int)(force_cast<double>(element)));
		else throw RuntimeError("Try to insert " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}
	else if(_dtype == "float")
	{
		vector<float>* ptr_vec = (vector<float>*)vec;
		if(same_type(DataType, bool)) ptr_vec->insert(ptr_vec->begin()+i, (float)(force_cast<bool>(element)));
		else if(same_type(DataType, _Bit_reference)) ptr_vec->insert(ptr_vec->begin()+i, (float)((bool)force_cast<_Bit_reference>(element)));
		else if(same_type(DataType, uint)) ptr_vec->insert(ptr_vec->begin()+i, (float)(force_cast<uint>(element)));
		else if(same_type(DataType, int)) ptr_vec->insert(ptr_vec->begin()+i, (float)(force_cast<int>(element)));
		else if(same_type(DataType, float)) ptr_vec->insert(ptr_vec->begin()+i, force_cast<float>(element));
		else if(same_type(DataType, double)) ptr_vec->insert(ptr_vec->begin()+i, (float)(force_cast<double>(element)));
		else throw RuntimeError("Try to insert " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}
	else if(_dtype == "double")
	{
		vector<double>* ptr_vec = (vector<double>*)vec;
		if(same_type(DataType, bool)) ptr_vec->insert(ptr_vec->begin()+i, (double)(force_cast<bool>(element)));
		else if(same_type(DataType, _Bit_reference)) ptr_vec->insert(ptr_vec->begin()+i, (double)((bool)force_cast<_Bit_reference>(element)));
		else if(same_type(DataType, uint)) ptr_vec->insert(ptr_vec->begin()+i, (double)(force_cast<uint>(element)));
		else if(same_type(DataType, int)) ptr_vec->insert(ptr_vec->begin()+i, (double)(force_cast<int>(element)));
		else if(same_type(DataType, float)) ptr_vec->insert(ptr_vec->begin()+i, (double)(force_cast<float>(element)));
		else if(same_type(DataType, double)) ptr_vec->insert(ptr_vec->begin()+i, force_cast<double>(element));
		else throw RuntimeError("Try to insert " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}
	else if((_dtype == "bvec2" && same_type(DataType, bvec2)) ||
		    (_dtype == "bvec3" && same_type(DataType, bvec3)) ||
		    (_dtype == "bvec4" && same_type(DataType, bvec4)) ||
		    (_dtype == "ivec2" && same_type(DataType, ivec2)) ||
		    (_dtype == "ivec3" && same_type(DataType, ivec3)) ||
		    (_dtype == "ivec4" && same_type(DataType, ivec4)) ||
		    (_dtype == "uvec2" && same_type(DataType, uvec2)) ||
		    (_dtype == "uvec3" && same_type(DataType, uvec3)) ||
		    (_dtype == "uvec4" && same_type(DataType, uvec4)) ||
		    (_dtype == "vec2" && same_type(DataType, vec2)) ||
		    (_dtype == "vec3" && same_type(DataType, vec3)) ||
		    (_dtype == "vec4" && same_type(DataType, vec4)) ||
		    (_dtype == "dvec2" && same_type(DataType, dvec2)) ||
		    (_dtype == "dvec3" && same_type(DataType, dvec3)) ||
		    (_dtype == "dvec4" && same_type(DataType, dvec4)) ||
		    (_dtype == "mat2" && same_type(DataType, mat2)) ||
		    (_dtype == "mat3" && same_type(DataType, mat3)) ||
		    (_dtype == "mat4" && same_type(DataType, mat4)))
	{
		vector<DataType>* ptr_vec = (vector<DataType>*)vec;
		ptr_vec->insert(ptr_vec->begin()+i, element);
	}
	else
	{
		throw TypeError("Try to insert " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}

	return i;
}

template<class DataType>
int GLVector::insert(int i, int n, const DataType& element)
{
	int length = size();
	if(i < 0)
	{
		i += length;
	}
	if(i < 0 || i > length)
	{
		throw IndexError(i, 0, length);
	}

	init<DataType>();
	if(_dtype == "bool")
	{
		vector<byte>* ptr_vec = (vector<byte>*)vec;
		if(same_type(DataType, bool)) ptr_vec->insert(ptr_vec->begin()+i, n, (byte)force_cast<bool>(element));
		else if(same_type(DataType, _Bit_reference)) ptr_vec->insert(ptr_vec->begin()+i, n, (byte)((bool)force_cast<_Bit_reference>(element)));
		else if(same_type(DataType, uint)) ptr_vec->insert(ptr_vec->begin()+i, n, (byte)((bool)force_cast<uint>(element)));
		else if(same_type(DataType, int)) ptr_vec->insert(ptr_vec->begin()+i, n, (byte)((bool)force_cast<int>(element)));
		else if(same_type(DataType, float)) ptr_vec->insert(ptr_vec->begin()+i, n, (byte)((bool)force_cast<float>(element)));
		else if(same_type(DataType, double)) ptr_vec->insert(ptr_vec->begin()+i, n, (byte)((bool)force_cast<double>(element)));
		else throw TypeError("Try to insert " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}
	else if(_dtype == "uint")
	{
		vector<uint>* ptr_vec = (vector<uint>*)vec;
		if(same_type(DataType, bool)) ptr_vec->insert(ptr_vec->begin()+i, n, (uint)(force_cast<bool>(element)));
		else if(same_type(DataType, _Bit_reference)) ptr_vec->insert(ptr_vec->begin()+i, n, (uint)((bool)force_cast<_Bit_reference>(element)));
		else if(same_type(DataType, uint)) ptr_vec->insert(ptr_vec->begin()+i, n, force_cast<uint>(element));
		else if(same_type(DataType, int)) ptr_vec->insert(ptr_vec->begin()+i, n, (uint)(force_cast<int>(element)));
		else if(same_type(DataType, float)) ptr_vec->insert(ptr_vec->begin()+i, n, (uint)(force_cast<float>(element)));
		else if(same_type(DataType, double)) ptr_vec->insert(ptr_vec->begin()+i, n, (uint)(force_cast<double>(element)));
		else throw RuntimeError("Try to insert " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}
	else if(_dtype == "int")
	{
		vector<int>* ptr_vec = (vector<int>*)vec;
		if(same_type(DataType, bool)) ptr_vec->insert(ptr_vec->begin()+i, n, (int)(force_cast<bool>(element)));
		else if(same_type(DataType, _Bit_reference)) ptr_vec->insert(ptr_vec->begin()+i, n, (int)((bool)force_cast<_Bit_reference>(element)));
		else if(same_type(DataType, uint)) ptr_vec->insert(ptr_vec->begin()+i, n, (int)(force_cast<uint>(element)));
		else if(same_type(DataType, int)) ptr_vec->insert(ptr_vec->begin()+i, n, force_cast<int>(element));
		else if(same_type(DataType, float)) ptr_vec->insert(ptr_vec->begin()+i, n, (int)(force_cast<float>(element)));
		else if(same_type(DataType, double)) ptr_vec->insert(ptr_vec->begin()+i, n, (int)(force_cast<double>(element)));
		else throw RuntimeError("Try to insert " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}
	else if(_dtype == "float")
	{
		vector<float>* ptr_vec = (vector<float>*)vec;
		if(same_type(DataType, bool)) ptr_vec->insert(ptr_vec->begin()+i, n, (float)(force_cast<bool>(element)));
		else if(same_type(DataType, _Bit_reference)) ptr_vec->insert(ptr_vec->begin()+i, n, (float)((bool)force_cast<_Bit_reference>(element)));
		else if(same_type(DataType, uint)) ptr_vec->insert(ptr_vec->begin()+i, n, (float)(force_cast<uint>(element)));
		else if(same_type(DataType, int)) ptr_vec->insert(ptr_vec->begin()+i, n, (float)(force_cast<int>(element)));
		else if(same_type(DataType, float)) ptr_vec->insert(ptr_vec->begin()+i, n, force_cast<float>(element));
		else if(same_type(DataType, double)) ptr_vec->insert(ptr_vec->begin()+i, n, (float)(force_cast<double>(element)));
		else throw RuntimeError("Try to insert " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}
	else if(_dtype == "double")
	{
		vector<double>* ptr_vec = (vector<double>*)vec;
		if(same_type(DataType, bool)) ptr_vec->insert(ptr_vec->begin()+i, n, (double)(force_cast<bool>(element)));
		else if(same_type(DataType, _Bit_reference)) ptr_vec->insert(ptr_vec->begin()+i, n, (double)((bool)force_cast<_Bit_reference>(element)));
		else if(same_type(DataType, uint)) ptr_vec->insert(ptr_vec->begin()+i, n, (double)(force_cast<uint>(element)));
		else if(same_type(DataType, int)) ptr_vec->insert(ptr_vec->begin()+i, n, (double)(force_cast<int>(element)));
		else if(same_type(DataType, float)) ptr_vec->insert(ptr_vec->begin()+i, n, (double)(force_cast<float>(element)));
		else if(same_type(DataType, double)) ptr_vec->insert(ptr_vec->begin()+i, n, force_cast<double>(element));
		else throw RuntimeError("Try to insert " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}
	else if((_dtype == "bvec2" && same_type(DataType, bvec2)) ||
		    (_dtype == "bvec3" && same_type(DataType, bvec3)) ||
		    (_dtype == "bvec4" && same_type(DataType, bvec4)) ||
		    (_dtype == "ivec2" && same_type(DataType, ivec2)) ||
		    (_dtype == "ivec3" && same_type(DataType, ivec3)) ||
		    (_dtype == "ivec4" && same_type(DataType, ivec4)) ||
		    (_dtype == "uvec2" && same_type(DataType, uvec2)) ||
		    (_dtype == "uvec3" && same_type(DataType, uvec3)) ||
		    (_dtype == "uvec4" && same_type(DataType, uvec4)) ||
		    (_dtype == "vec2" && same_type(DataType, vec2)) ||
		    (_dtype == "vec3" && same_type(DataType, vec3)) ||
		    (_dtype == "vec4" && same_type(DataType, vec4)) ||
		    (_dtype == "dvec2" && same_type(DataType, dvec2)) ||
		    (_dtype == "dvec3" && same_type(DataType, dvec3)) ||
		    (_dtype == "dvec4" && same_type(DataType, dvec4)) ||
		    (_dtype == "mat2" && same_type(DataType, mat2)) ||
		    (_dtype == "mat3" && same_type(DataType, mat3)) ||
		    (_dtype == "mat4" && same_type(DataType, mat4)))
	{
		vector<DataType>* ptr_vec = (vector<DataType>*)vec;
		ptr_vec->insert(ptr_vec->begin()+i, n, element);
	}
	else
	{
		throw TypeError("Try to insert " + type_name(DataType) + " value to vector<" + _dtype + ">");
	}

	return i;
}

// template<class DataType>
// GLVector::iterator GLVector::insert(GLVector::const_iterator& it, int n, const DataType& element) // reviewed
// {
// 	int i = it - begin();
// 	insert(i, n, element);
// 	return begin() + i;
// }

// template<class DataType>
// GLVector::iterator GLVector::insert(GLVector::const_iterator& it, const DataType& element) // reviewed
// {
// 	int i = it - begin();
// 	insert(i, element);
// 	return begin() + i;
// }

template<class DataType>
int GLVector::insert(int i, typename vector<DataType>::const_iterator& first, typename vector<DataType>::const_iterator& last)
{
	int length = size();

	if(i < 0)
	{
		i += length;
	}
	if(i < 0 || i > length)
	{
		throw IndexError(i, 0, length);
	}

	if(first >= last)
	{
		return i;
	}

	init<DataType>();
	if((_dtype == "uint" && same_type(DataType, uint)) ||
	   (_dtype == "int" && same_type(DataType, int)) ||
	   (_dtype == "float" && same_type(DataType, float)) ||
	   (_dtype == "double" && same_type(DataType, double)) ||
	   (_dtype == "bvec2" && same_type(DataType, bvec2)) ||
	   (_dtype == "bvec3" && same_type(DataType, bvec3)) ||
	   (_dtype == "bvec4" && same_type(DataType, bvec4)) ||
	   (_dtype == "ivec2" && same_type(DataType, ivec2)) ||
	   (_dtype == "ivec3" && same_type(DataType, ivec3)) ||
	   (_dtype == "ivec4" && same_type(DataType, ivec4)) ||
	   (_dtype == "uvec2" && same_type(DataType, uvec2)) ||
	   (_dtype == "uvec3" && same_type(DataType, uvec3)) ||
	   (_dtype == "uvec4" && same_type(DataType, uvec4)) ||
	   (_dtype == "vec2" && same_type(DataType, vec2)) ||
	   (_dtype == "vec3" && same_type(DataType, vec3)) ||
	   (_dtype == "vec4" && same_type(DataType, vec4)) ||
	   (_dtype == "dvec2" && same_type(DataType, dvec2)) ||
	   (_dtype == "dvec3" && same_type(DataType, dvec3)) ||
	   (_dtype == "dvec4" && same_type(DataType, dvec4)) ||
	   (_dtype == "mat2" && same_type(DataType, mat2)) ||
	   (_dtype == "mat3" && same_type(DataType, mat3)) ||
	   (_dtype == "mat4" && same_type(DataType, mat4)))
	{
		vector<DataType>* dest_vec = (vector<DataType>*)vec;
		dest_vec->insert(dest_vec->begin()+i, first, last);
	}
	else if(_dtype == "bool" && same_type(DataType, bool))
	{
		int src_length = last - begin;
		vector<byte> temp_vec(src_length);
		for(int j = 0; j < src_length; j++)
		{
			temp_vec[j] = (byte)((bool)force_cast<_Bit_reference>(*(first+j)));
		}

		vector<byte>* dest_vec = (vector<byte>*)vec;
		dest_vec->insert(dest_vec->begin()+i, temp_vec.begin(), temp_vec.end());
	}
	else
	{
		throw TypeError("Try to insert vector<" + type_name(DataType) + "> to vector<" + _dtype + ">");
	}

	return i;
}

template<class DataType>
DataType GLVector::pop_back() // reviewed
{
	if(empty())
	{
		throw RuntimeError("Try to pop from empty vector");
	}

	DataType return_value;
	if(_dtype == "bool")
	{
		bool back_value = (bool)(((vector<byte>*)vec)->back());
		if(same_type(DataType, bool)) return_value = force_cast<DataType>(back_value);
		else if(same_type(DataType, uint)) return_value = force_cast<DataType>((uint)back_value);
		else if(same_type(DataType, int)) return_value = force_cast<DataType>((int)back_value);
		else if(same_type(DataType, float)) return_value = force_cast<DataType>((float)back_value);
		else if(same_type(DataType, double)) return_value = force_cast<DataType>((double)back_value);
		else throw TypeError("Try to pop " + type_name(DataType) + " value from vector<" + _dtype + ">");
		((vector<byte>*)vec)->pop_back();
	}
	else if(_dtype == "uint")
	{
		uint back_value = ((vector<uint>*)vec)->back();
		if(same_type(DataType, bool)) return_value = force_cast<DataType>((bool)back_value);
		else if(same_type(DataType, uint)) return_value = force_cast<DataType>(back_value);
		else if(same_type(DataType, int)) return_value = force_cast<DataType>((int)back_value);
		else if(same_type(DataType, float)) return_value = force_cast<DataType>((float)back_value);
		else if(same_type(DataType, double)) return_value = force_cast<DataType>((double)back_value);
		else throw TypeError("Try to pop " + type_name(DataType) + " value from vector<" + _dtype + ">");
		((vector<uint>*)vec)->pop_back();
	}
	else if(_dtype == "int")
	{
		int back_value = ((vector<int>*)vec)->back();
		if(same_type(DataType, bool)) return_value = force_cast<DataType>((bool)back_value);
		else if(same_type(DataType, uint)) return_value = force_cast<DataType>((uint)back_value);
		else if(same_type(DataType, int)) return_value = force_cast<DataType>(back_value);
		else if(same_type(DataType, float)) return_value = force_cast<DataType>((float)back_value);
		else if(same_type(DataType, double)) return_value = force_cast<DataType>((double)back_value);
		else throw TypeError("Try to pop " + type_name(DataType) + " value from vector<" + _dtype + ">");
		((vector<int>*)vec)->pop_back();
	}
	else if(_dtype == "float")
	{
		float back_value = ((vector<float>*)vec)->back();
		if(same_type(DataType, bool)) return_value = force_cast<DataType>((bool)back_value);
		else if(same_type(DataType, uint)) return_value = force_cast<DataType>((uint)back_value);
		else if(same_type(DataType, int)) return_value = force_cast<DataType>((int)back_value);
		else if(same_type(DataType, float)) return_value = force_cast<DataType>(back_value);
		else if(same_type(DataType, double)) return_value = force_cast<DataType>((double)back_value);
		else throw TypeError("Try to pop " + type_name(DataType) + " value from vector<" + _dtype + ">");
		((vector<float>*)vec)->pop_back();
	}
	else if(_dtype == "double")
	{
		double back_value = ((vector<double>*)vec)->back();
		if(same_type(DataType, bool)) return_value = force_cast<DataType>((bool)back_value);
		else if(same_type(DataType, uint)) return_value = force_cast<DataType>((uint)back_value);
		else if(same_type(DataType, int)) return_value = force_cast<DataType>((int)back_value);
		else if(same_type(DataType, float)) return_value = force_cast<DataType>((float)back_value);
		else if(same_type(DataType, double)) return_value = force_cast<DataType>(back_value);
		else throw TypeError("Try to pop " + type_name(DataType) + " value from vector<" + _dtype + ">");
		((vector<double>*)vec)->pop_back();
	}
	else if((_dtype == "bvec2" && same_type(DataType, bvec2)) ||
		    (_dtype == "bvec3" && same_type(DataType, bvec3)) ||
		    (_dtype == "bvec4" && same_type(DataType, bvec4)) ||
		    (_dtype == "ivec2" && same_type(DataType, ivec2)) ||
		    (_dtype == "ivec3" && same_type(DataType, ivec3)) ||
		    (_dtype == "ivec4" && same_type(DataType, ivec4)) ||
		    (_dtype == "uvec2" && same_type(DataType, uvec2)) ||
		    (_dtype == "uvec3" && same_type(DataType, uvec3)) ||
		    (_dtype == "uvec4" && same_type(DataType, uvec4)) ||
		    (_dtype == "vec2" && same_type(DataType, vec2)) ||
		    (_dtype == "vec3" && same_type(DataType, vec3)) ||
		    (_dtype == "vec4" && same_type(DataType, vec4)) ||
		    (_dtype == "dvec2" && same_type(DataType, dvec2)) ||
		    (_dtype == "dvec3" && same_type(DataType, dvec3)) ||
		    (_dtype == "dvec4" && same_type(DataType, dvec4)) ||
		    (_dtype == "mat2" && same_type(DataType, mat2)) ||
		    (_dtype == "mat3" && same_type(DataType, mat3)) ||
		    (_dtype == "mat4" && same_type(DataType, mat4)))
	{
		return_value = ((vector<DataType>*)vec)->back();
		((vector<DataType>*)vec)->pop_back();
	}
	else
	{
		throw TypeError("Try to pop " + type_name(DataType) + " value from vector<" + _dtype + ">");
	}
	
	return return_value;
}

template<class DataType>
DataType GLVector::pop_front() // reviewed
{
	if(empty())
	{
		throw RuntimeError("Try to pop from empty vector");
	}

	DataType return_value;
	if(_dtype == "bool")
	{
		bool front_value = (bool)(((vector<byte>*)vec)->front());
		if(same_type(DataType, bool)) return_value = force_cast<DataType>(front_value);
		else if(same_type(DataType, uint)) return_value = force_cast<DataType>((uint)front_value);
		else if(same_type(DataType, int)) return_value = force_cast<DataType>((int)front_value);
		else if(same_type(DataType, float)) return_value = force_cast<DataType>((float)front_value);
		else if(same_type(DataType, double)) return_value = force_cast<DataType>((double)front_value);
		else throw TypeError("Try to pop " + type_name(DataType) + " value from vector<" + _dtype + ">");
		vector<byte>* ptr_vec = (vector<byte>*)vec;
		ptr_vec->erase(ptr_vec->begin());
	}
	else if(_dtype == "uint")
	{
		uint front_value = ((vector<uint>*)vec)->front();
		if(same_type(DataType, bool)) return_value = force_cast<DataType>((bool)front_value);
		else if(same_type(DataType, uint)) return_value = force_cast<DataType>(front_value);
		else if(same_type(DataType, int)) return_value = force_cast<DataType>((int)front_value);
		else if(same_type(DataType, float)) return_value = force_cast<DataType>((float)front_value);
		else if(same_type(DataType, double)) return_value = force_cast<DataType>((double)front_value);
		else throw TypeError("Try to pop " + type_name(DataType) + " value from vector<" + _dtype + ">");
		vector<uint>* ptr_vec = (vector<uint>*)vec;
		ptr_vec->erase(ptr_vec->begin());
	}
	else if(_dtype == "int")
	{
		int front_value = ((vector<int>*)vec)->front();
		if(same_type(DataType, bool)) return_value = force_cast<DataType>((bool)front_value);
		else if(same_type(DataType, uint)) return_value = force_cast<DataType>((uint)front_value);
		else if(same_type(DataType, int)) return_value = force_cast<DataType>(front_value);
		else if(same_type(DataType, float)) return_value = force_cast<DataType>((float)front_value);
		else if(same_type(DataType, double)) return_value = force_cast<DataType>((double)front_value);
		else throw TypeError("Try to pop " + type_name(DataType) + " value from vector<" + _dtype + ">");
		vector<int>* ptr_vec = (vector<int>*)vec;
		ptr_vec->erase(ptr_vec->begin());
	}
	else if(_dtype == "float")
	{
		float front_value = ((vector<float>*)vec)->front();
		if(same_type(DataType, bool)) return_value = force_cast<DataType>((bool)front_value);
		else if(same_type(DataType, uint)) return_value = force_cast<DataType>((uint)front_value);
		else if(same_type(DataType, int)) return_value = force_cast<DataType>((int)front_value);
		else if(same_type(DataType, float)) return_value = force_cast<DataType>(front_value);
		else if(same_type(DataType, double)) return_value = force_cast<DataType>((double)front_value);
		else throw TypeError("Try to pop " + type_name(DataType) + " value from vector<" + _dtype + ">");
		vector<float>* ptr_vec = (vector<float>*)vec;
		ptr_vec->erase(ptr_vec->begin());
	}
	else if(_dtype == "double")
	{
		double front_value = ((vector<double>*)vec)->front();
		if(same_type(DataType, bool)) return_value = force_cast<DataType>((bool)front_value);
		else if(same_type(DataType, uint)) return_value = force_cast<DataType>((uint)front_value);
		else if(same_type(DataType, int)) return_value = force_cast<DataType>((int)front_value);
		else if(same_type(DataType, float)) return_value = force_cast<DataType>((float)front_value);
		else if(same_type(DataType, double)) return_value = force_cast<DataType>(front_value);
		else throw TypeError("Try to pop " + type_name(DataType) + " value from vector<" + _dtype + ">");
		vector<double>* ptr_vec = (vector<double>*)vec;
		ptr_vec->erase(ptr_vec->begin());
	}
	else if((_dtype == "bvec2" && same_type(DataType, bvec2)) ||
		    (_dtype == "bvec3" && same_type(DataType, bvec3)) ||
		    (_dtype == "bvec4" && same_type(DataType, bvec4)) ||
		    (_dtype == "ivec2" && same_type(DataType, ivec2)) ||
		    (_dtype == "ivec3" && same_type(DataType, ivec3)) ||
		    (_dtype == "ivec4" && same_type(DataType, ivec4)) ||
		    (_dtype == "uvec2" && same_type(DataType, uvec2)) ||
		    (_dtype == "uvec3" && same_type(DataType, uvec3)) ||
		    (_dtype == "uvec4" && same_type(DataType, uvec4)) ||
		    (_dtype == "vec2" && same_type(DataType, vec2)) ||
		    (_dtype == "vec3" && same_type(DataType, vec3)) ||
		    (_dtype == "vec4" && same_type(DataType, vec4)) ||
		    (_dtype == "dvec2" && same_type(DataType, dvec2)) ||
		    (_dtype == "dvec3" && same_type(DataType, dvec3)) ||
		    (_dtype == "dvec4" && same_type(DataType, dvec4)) ||
		    (_dtype == "mat2" && same_type(DataType, mat2)) ||
		    (_dtype == "mat3" && same_type(DataType, mat3)) ||
		    (_dtype == "mat4" && same_type(DataType, mat4)))
	{
		return_value = ((vector<DataType>*)vec)->front();
		vector<DataType>* ptr_vec = (vector<DataType>*)vec;
		ptr_vec->erase(ptr_vec->begin());
	}
	else
	{
		throw TypeError("Try to pop " + type_name(DataType) + " value from vector<" + _dtype + ">");
	}
	
	return return_value;
}

template<class DataType>
DataType GLVector::pop(int i) // reviewed
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

	DataType return_value;
	if(_dtype == "bool")
	{
		bool value = (bool)(((vector<byte>*)vec)->at(i));
		if(same_type(DataType, bool)) return_value = force_cast<DataType>(value);
		else if(same_type(DataType, uint)) return_value = force_cast<DataType>((uint)value);
		else if(same_type(DataType, int)) return_value = force_cast<DataType>((int)value);
		else if(same_type(DataType, float)) return_value = force_cast<DataType>((float)value);
		else if(same_type(DataType, double)) return_value = force_cast<DataType>((double)value);
		else throw TypeError("Try to pop " + type_name(DataType) + " value from vector<" + _dtype + ">");
		((vector<byte>*)vec)->erase(((vector<byte>*)vec)->begin()+i);
	}
	else if(_dtype == "uint")
	{
		uint value = ((vector<uint>*)vec)->at(i);
		if(same_type(DataType, bool)) return_value = force_cast<DataType>((bool)value);
		else if(same_type(DataType, uint)) return_value = force_cast<DataType>(value);
		else if(same_type(DataType, int)) return_value = force_cast<DataType>((int)value);
		else if(same_type(DataType, float)) return_value = force_cast<DataType>((float)value);
		else if(same_type(DataType, double)) return_value = force_cast<DataType>((double)value);
		else throw TypeError("Try to pop " + type_name(DataType) + " value from vector<" + _dtype + ">");
		((vector<uint>*)vec)->erase(((vector<uint>*)vec)->begin()+i);
	}
	else if(_dtype == "int")
	{
		int value = ((vector<int>*)vec)->at(i);
		if(same_type(DataType, bool)) return_value = force_cast<DataType>((bool)value);
		else if(same_type(DataType, uint)) return_value = force_cast<DataType>((uint)value);
		else if(same_type(DataType, int)) return_value = force_cast<DataType>(value);
		else if(same_type(DataType, float)) return_value = force_cast<DataType>((float)value);
		else if(same_type(DataType, double)) return_value = force_cast<DataType>((double)value);
		else throw TypeError("Try to pop " + type_name(DataType) + " value from vector<" + _dtype + ">");
		((vector<int>*)vec)->erase(((vector<int>*)vec)->begin()+i);
	}
	else if(_dtype == "float")
	{
		float value = ((vector<float>*)vec)->at(i);
		if(same_type(DataType, bool)) return_value = force_cast<DataType>((bool)value);
		else if(same_type(DataType, uint)) return_value = force_cast<DataType>((uint)value);
		else if(same_type(DataType, int)) return_value = force_cast<DataType>((int)value);
		else if(same_type(DataType, float)) return_value = force_cast<DataType>(value);
		else if(same_type(DataType, double)) return_value = force_cast<DataType>((double)value);
		else throw TypeError("Try to pop " + type_name(DataType) + " value from vector<" + _dtype + ">");
		((vector<float>*)vec)->erase(((vector<float>*)vec)->begin()+i);
	}
	else if(_dtype == "double")
	{
		double value = ((vector<double>*)vec)->at(i);
		if(same_type(DataType, bool)) return_value = force_cast<DataType>((bool)value);
		else if(same_type(DataType, uint)) return_value = force_cast<DataType>((uint)value);
		else if(same_type(DataType, int)) return_value = force_cast<DataType>((int)value);
		else if(same_type(DataType, float)) return_value = force_cast<DataType>((float)value);
		else if(same_type(DataType, double)) return_value = force_cast<DataType>(value);
		else throw TypeError("Try to pop " + type_name(DataType) + " value from vector<" + _dtype + ">");
		((vector<double>*)vec)->erase(((vector<double>*)vec)->begin()+i);
	}
	else if((_dtype == "bvec2" && same_type(DataType, bvec2)) ||
		    (_dtype == "bvec3" && same_type(DataType, bvec3)) ||
		    (_dtype == "bvec4" && same_type(DataType, bvec4)) ||
		    (_dtype == "ivec2" && same_type(DataType, ivec2)) ||
		    (_dtype == "ivec3" && same_type(DataType, ivec3)) ||
		    (_dtype == "ivec4" && same_type(DataType, ivec4)) ||
		    (_dtype == "uvec2" && same_type(DataType, uvec2)) ||
		    (_dtype == "uvec3" && same_type(DataType, uvec3)) ||
		    (_dtype == "uvec4" && same_type(DataType, uvec4)) ||
		    (_dtype == "vec2" && same_type(DataType, vec2)) ||
		    (_dtype == "vec3" && same_type(DataType, vec3)) ||
		    (_dtype == "vec4" && same_type(DataType, vec4)) ||
		    (_dtype == "dvec2" && same_type(DataType, dvec2)) ||
		    (_dtype == "dvec3" && same_type(DataType, dvec3)) ||
		    (_dtype == "dvec4" && same_type(DataType, dvec4)) ||
		    (_dtype == "mat2" && same_type(DataType, mat2)) ||
		    (_dtype == "mat3" && same_type(DataType, mat3)) ||
		    (_dtype == "mat4" && same_type(DataType, mat4)))
	{
		return_value = ((vector<DataType>*)vec)->at(i);
		((vector<DataType>*)vec)->erase(((vector<DataType>*)vec)->begin()+i);
	}
	else
	{
		throw TypeError("Try to pop " + type_name(DataType) + " value from vector<" + _dtype + ">");
	}

	return return_value;
}

// template<class DataType>
// DataType GLVector::pop(GLVector::const_iterator& it) // reviewed
// {
// 	int i = it - begin();
// 	return pop<DataType>(i);
// }

// template<class DataType>
// vector<DataType> GLVector::slice(GLVector::const_iterator& first, GLVector::const_iterator& last)
// {
// 	return slice<DataType>(first-begin(), last-begin());
// }

template<class DataType>
vector<DataType> GLVector::slice(int pos_start, int pos_end)
{
	if((_dtype == "bool" && !same_type(DataType, bool)) ||
	   (_dtype == "uint" && !same_type(DataType, uint)) ||
	   (_dtype == "int" && !same_type(DataType, int)) ||
	   (_dtype == "float" && !same_type(DataType, float)) ||
	   (_dtype == "double" && !same_type(DataType, double)) ||
	   (_dtype == "bvec2" && !same_type(DataType, bvec2)) ||
	   (_dtype == "bvec3" && !same_type(DataType, bvec3)) ||
	   (_dtype == "bvec4" && !same_type(DataType, bvec4)) ||
	   (_dtype == "ivec2" && !same_type(DataType, ivec2)) ||
	   (_dtype == "ivec3" && !same_type(DataType, ivec3)) ||
	   (_dtype == "ivec4" && !same_type(DataType, ivec4)) ||
	   (_dtype == "uvec2" && !same_type(DataType, uvec2)) ||
	   (_dtype == "uvec3" && !same_type(DataType, uvec3)) ||
	   (_dtype == "uvec4" && !same_type(DataType, uvec4)) ||
	   (_dtype == "vec2" && !same_type(DataType, vec2)) ||
	   (_dtype == "vec3" && !same_type(DataType, vec3)) ||
	   (_dtype == "vec4" && !same_type(DataType, vec4)) ||
	   (_dtype == "dvec2" && !same_type(DataType, dvec2)) ||
	   (_dtype == "dvec3" && !same_type(DataType, dvec3)) ||
	   (_dtype == "dvec4" && !same_type(DataType, dvec4)) ||
	   (_dtype == "mat2" && !same_type(DataType, mat2)) ||
	   (_dtype == "mat3" && !same_type(DataType, mat3)) ||
	   (_dtype == "mat4" && !same_type(DataType, mat4)))
	{
		throw TypeError("Try to get " + type_name(DataType) + " slice from vector<" + _dtype + ">");
	}

	if(empty())
	{
		return vector<DataType>();
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
		return vector<DataType>();
	}

	int n = pos_end-pos_start;
	vector<DataType> result(n);
	for(int i = 0; i < n; i++)
	{
		if(_dtype == "bool") result[i] = force_cast<DataType>(at<bool>(i-pos_start));
		else if(_dtype == "uint") result[i] = force_cast<DataType>(at<uint>(i-pos_start));
		else if(_dtype == "int") result[i] = force_cast<DataType>(at<int>(i-pos_start));
		else if(_dtype == "float") result[i] = force_cast<DataType>(at<float>(i-pos_start));
		else if(_dtype == "double") result[i] = force_cast<DataType>(at<double>(i-pos_start));
		else if(_dtype == "bvec2") result[i] = force_cast<DataType>(at<bvec2>(i-pos_start));
		else if(_dtype == "bvec3") result[i] = force_cast<DataType>(at<bvec3>(i-pos_start));
		else if(_dtype == "bvec4") result[i] = force_cast<DataType>(at<bvec4>(i-pos_start));
		else if(_dtype == "ivec2") result[i] = force_cast<DataType>(at<ivec2>(i-pos_start));
		else if(_dtype == "ivec3") result[i] = force_cast<DataType>(at<ivec3>(i-pos_start));
		else if(_dtype == "ivec4") result[i] = force_cast<DataType>(at<ivec4>(i-pos_start));
		else if(_dtype == "uvec2") result[i] = force_cast<DataType>(at<uvec2>(i-pos_start));
		else if(_dtype == "uvec3") result[i] = force_cast<DataType>(at<uvec3>(i-pos_start));
		else if(_dtype == "uvec4") result[i] = force_cast<DataType>(at<uvec4>(i-pos_start));
		else if(_dtype == "vec2") result[i] = force_cast<DataType>(at<vec2>(i-pos_start));
		else if(_dtype == "vec3") result[i] = force_cast<DataType>(at<vec3>(i-pos_start));
		else if(_dtype == "vec4") result[i] = force_cast<DataType>(at<vec4>(i-pos_start));
		else if(_dtype == "dvec2") result[i] = force_cast<DataType>(at<dvec2>(i-pos_start));
		else if(_dtype == "dvec3") result[i] = force_cast<DataType>(at<dvec3>(i-pos_start));
		else if(_dtype == "dvec4") result[i] = force_cast<DataType>(at<dvec4>(i-pos_start));
		else if(_dtype == "mat2") result[i] = force_cast<DataType>(at<mat2>(i-pos_start));
		else if(_dtype == "mat3") result[i] = force_cast<DataType>(at<mat3>(i-pos_start));
		else if(_dtype == "mat4") result[i] = force_cast<DataType>(at<mat4>(i-pos_start));
	}

	return result;
}

template<class DataType>
void GLVector::extend(const vector<DataType>& new_vec)
{
	insert(size(), new_vec.begin(), new_vec.end());
}

template<class DataType>
const DataType& GLVector::at(int i)const
{
	if(empty())
	{
		throw RuntimeError("Try to access value in empty vector");
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

	if((_dtype == "uint" && same_type(DataType, uint)) ||
	   (_dtype == "int" && same_type(DataType, int)) ||
	   (_dtype == "float" && same_type(DataType, float)) ||
	   (_dtype == "double" && same_type(DataType, double)) ||
	   (_dtype == "bvec2" && same_type(DataType, bvec2)) ||
	   (_dtype == "bvec3" && same_type(DataType, bvec3)) ||
	   (_dtype == "bvec4" && same_type(DataType, bvec4)) ||
	   (_dtype == "ivec2" && same_type(DataType, ivec2)) ||
	   (_dtype == "ivec3" && same_type(DataType, ivec3)) ||
	   (_dtype == "ivec4" && same_type(DataType, ivec4)) ||
	   (_dtype == "uvec2" && same_type(DataType, uvec2)) ||
	   (_dtype == "uvec3" && same_type(DataType, uvec3)) ||
	   (_dtype == "uvec4" && same_type(DataType, uvec4)) ||
	   (_dtype == "vec2" && same_type(DataType, vec2)) ||
	   (_dtype == "vec3" && same_type(DataType, vec3)) ||
	   (_dtype == "vec4" && same_type(DataType, vec4)) ||
	   (_dtype == "dvec2" && same_type(DataType, dvec2)) ||
	   (_dtype == "dvec3" && same_type(DataType, dvec3)) ||
	   (_dtype == "dvec4" && same_type(DataType, dvec4)) ||
	   (_dtype == "mat2" && same_type(DataType, mat2)) ||
	   (_dtype == "mat3" && same_type(DataType, mat3)) ||
	   (_dtype == "mat4" && same_type(DataType, mat4)))
	{
		return force_cast<DataType>(((vector<DataType>*)vec)->at(i));
	}
	else if(_dtype == "bool" && same_type(DataType, bool))
	{
		return force_cast<bool>(((vector<byte>*)vec)->at(i));
	}
	else
	{
		throw TypeError("Try to get " + type_name(DataType) + " value from vector<" + _dtype + ">");
	}
}

template<class DataType>
DataType& GLVector::at(int i)
{
	if(empty())
	{
		throw RuntimeError("Try to access value in empty vector");
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

	if((_dtype == "uint" && same_type(DataType, uint)) ||
	   (_dtype == "int" && same_type(DataType, int)) ||
	   (_dtype == "float" && same_type(DataType, float)) ||
	   (_dtype == "double" && same_type(DataType, double)) ||
	   (_dtype == "bvec2" && same_type(DataType, bvec2)) ||
	   (_dtype == "bvec3" && same_type(DataType, bvec3)) ||
	   (_dtype == "bvec4" && same_type(DataType, bvec4)) ||
	   (_dtype == "ivec2" && same_type(DataType, ivec2)) ||
	   (_dtype == "ivec3" && same_type(DataType, ivec3)) ||
	   (_dtype == "ivec4" && same_type(DataType, ivec4)) ||
	   (_dtype == "uvec2" && same_type(DataType, uvec2)) ||
	   (_dtype == "uvec3" && same_type(DataType, uvec3)) ||
	   (_dtype == "uvec4" && same_type(DataType, uvec4)) ||
	   (_dtype == "vec2" && same_type(DataType, vec2)) ||
	   (_dtype == "vec3" && same_type(DataType, vec3)) ||
	   (_dtype == "vec4" && same_type(DataType, vec4)) ||
	   (_dtype == "dvec2" && same_type(DataType, dvec2)) ||
	   (_dtype == "dvec3" && same_type(DataType, dvec3)) ||
	   (_dtype == "dvec4" && same_type(DataType, dvec4)) ||
	   (_dtype == "mat2" && same_type(DataType, mat2)) ||
	   (_dtype == "mat3" && same_type(DataType, mat3)) ||
	   (_dtype == "mat4" && same_type(DataType, mat4)))
	{
		return force_cast<DataType>(((vector<DataType>*)vec)->at(i));
	}
	else if(_dtype == "bool" && same_type(DataType, bool))
	{
		return force_cast<DataType>(((vector<byte>*)vec)->at(i));
	}
	else
	{
		throw TypeError("Try to get " + type_name(DataType) + " value from vector<" + _dtype + ">");
	}
}

template<class DataType>
const DataType& GLVector::back()const
{
	return at<DataType>(-1);
}

template<class DataType>
DataType& GLVector::back()
{
	return at<DataType>(-1);
}

template<class DataType>
const DataType& GLVector::front()const
{
	return at<DataType>(0);
}

template<class DataType>
DataType& GLVector::front()
{
	return at<DataType>(0);
}

template<class DataType>
GLVector::operator vector<DataType>()const
{
	if(empty())
	{
		if(same_type(DataType, bool) ||
		   same_type(DataType, uint) ||
		   same_type(DataType, int) ||
		   same_type(DataType, float) ||
		   same_type(DataType, double) ||
		   same_type(DataType, bvec2) ||
		   same_type(DataType, bvec3) ||
		   same_type(DataType, bvec4) ||
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
			return vector<DataType>();
		}
		else
		{
			throw TypeError("Not supported DataType " + type_name(DataType));
		}
	}

	if((same_type(DataType, uint) && _dtype == "uint") ||
	   (same_type(DataType, int) && _dtype == "int") ||
	   (same_type(DataType, float) && _dtype == "float") ||
	   (same_type(DataType, double) && _dtype == "double") ||
	   (same_type(DataType, bvec2) && _dtype == "bvec2") ||
	   (same_type(DataType, bvec3) && _dtype == "bvec3") ||
	   (same_type(DataType, bvec4) && _dtype == "bvec4") ||
	   (same_type(DataType, ivec2) && _dtype == "ivec2") ||
	   (same_type(DataType, ivec3) && _dtype == "ivec3") ||
	   (same_type(DataType, ivec4) && _dtype == "ivec4") ||
	   (same_type(DataType, uvec2) && _dtype == "uvec2") ||
	   (same_type(DataType, uvec3) && _dtype == "uvec3") ||
	   (same_type(DataType, uvec4) && _dtype == "uvec4") ||
	   (same_type(DataType, vec2) && _dtype == "vec2") ||
	   (same_type(DataType, vec3) && _dtype == "vec3") ||
	   (same_type(DataType, vec4) && _dtype == "vec4") ||
	   (same_type(DataType, dvec2) && _dtype == "dvec2") ||
	   (same_type(DataType, dvec3) && _dtype == "dvec3") ||
	   (same_type(DataType, dvec4) && _dtype == "dvec4") ||
	   (same_type(DataType, mat2) && _dtype == "mat2") ||
	   (same_type(DataType, mat3) && _dtype == "mat3") ||
	   (same_type(DataType, mat4) && _dtype == "mat4"))
	{
		return *((vector<DataType>*)vec);
	}
	else if(same_type(DataType, bool) && _dtype == "bool")
	{
		int length = size();
		vector<bool> result(length);
		vector<byte>* ptr_vec = (vector<byte>*)vec;
		for(int i = 0; i < length; i++)
		{
			result[i] = (bool)((*ptr_vec)[i]);
		}
		return force_cast< vector<DataType> >(result);
	}
	else
	{
		throw TypeError("Can not convert vector<" + _dtype + "> to vector<" + type_name(DataType) + ">");
	}
}

template<class DataType>
DataType* GLVector::ptr(int i)
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

	if(_dtype == "bool") return (DataType*)(&(((vector<byte>*)vec)->at(i)));
	else if(_dtype == "uint") return (DataType*)(&(((vector<uint>*)vec)->at(i)));
	else if(_dtype == "int") return (DataType*)(&(((vector<int>*)vec)->at(i)));
	else if(_dtype == "float") return (DataType*)(&(((vector<float>*)vec)->at(i)));
	else if(_dtype == "double") return (DataType*)(&(((vector<double>*)vec)->at(i)));
	else if(_dtype == "bvec2") return (DataType*)(&(((vector<bvec2>*)vec)->at(i)));
	else if(_dtype == "bvec3") return (DataType*)(&(((vector<bvec3>*)vec)->at(i)));
	else if(_dtype == "bvec4") return (DataType*)(&(((vector<bvec4>*)vec)->at(i)));
	else if(_dtype == "ivec2") return (DataType*)(&(((vector<ivec2>*)vec)->at(i)));
	else if(_dtype == "ivec3") return (DataType*)(&(((vector<ivec3>*)vec)->at(i)));
	else if(_dtype == "ivec4") return (DataType*)(&(((vector<ivec4>*)vec)->at(i)));
	else if(_dtype == "uvec2") return (DataType*)(&(((vector<uvec2>*)vec)->at(i)));
	else if(_dtype == "uvec3") return (DataType*)(&(((vector<uvec3>*)vec)->at(i)));
	else if(_dtype == "uvec4") return (DataType*)(&(((vector<uvec4>*)vec)->at(i)));
	else if(_dtype == "vec2") return (DataType*)(&(((vector<vec2>*)vec)->at(i)));
	else if(_dtype == "vec3") return (DataType*)(&(((vector<vec3>*)vec)->at(i)));
	else if(_dtype == "vec4") return (DataType*)(&(((vector<vec4>*)vec)->at(i)));
	else if(_dtype == "dvec2") return (DataType*)(&(((vector<dvec2>*)vec)->at(i)));
	else if(_dtype == "dvec3") return (DataType*)(&(((vector<dvec3>*)vec)->at(i)));
	else if(_dtype == "dvec4") return (DataType*)(&(((vector<dvec4>*)vec)->at(i)));
	else if(_dtype == "mat2") return (DataType*)(&(((vector<mat2>*)vec)->at(i)));
	else if(_dtype == "mat3") return (DataType*)(&(((vector<mat3>*)vec)->at(i)));
	else if(_dtype == "mat4") return (DataType*)(&(((vector<mat4>*)vec)->at(i)));
	else return (DataType*)NULL;
}

#endif