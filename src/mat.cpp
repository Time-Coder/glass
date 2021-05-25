#include <cmath>
#include <utility>

#include "glass/mat"
#include "glass/utils/exceptions.h"

using namespace std;

mat2::mat2() : col0(1.0f, 0.0f), col1(0.0f, 1.0f) {}
mat2::mat2(float v00, float v10, float v01, float v11) : col0(v00, v10), col1(v01, v11) {}
mat2::mat2(float value) : col0(value, 0), col1(0, value) {}
mat2::mat2(const vec2& _col0, const vec2& _col1) : col0(_col0), col1(_col1) {}
mat2::mat2(const mat2& A) : col0(A.col0), col1(A.col1) {}
mat2::mat2(mat2&& A)      : col0(move(A.col0)), col1(move(A.col1)), ptr_value(move(A.ptr_value)) {}
mat2::mat2(const mat3& A) : col0(A.col0), col1(A.col1) {}
mat2::mat2(const mat4& A) : col0(A.col0), col1(A.col1) {}
mat2::~mat2()
{
	if(ptr_value)
	{
		delete[] ptr_value;
	}
}

mat2& mat2::operator =(const mat2& A)
{
	if(&A != this)
	{
		col0 = A.col0;
		col1 = A.col1;
	}

	return *this;
}

mat2& mat2::operator =(mat2&& A)
{
	if(&A != this)
	{
		col0 = move(A.col0);
		col1 = move(A.col1);
		if(ptr_value)
		{
			delete[] ptr_value;
		}
		ptr_value = move(A.ptr_value);
	}

	return *this;
}

vec2& mat2::operator [](uint i)
{
	switch(i)
	{
		case 0: return col0;
		case 1: return col1;
		default: throw glass::IndexError(i, 0, 1);
	}
}

vec2 mat2::operator [](uint i)const
{
	switch(i)
	{
		case 0: return col0;
		case 1: return col1;
		default: throw glass::IndexError(i, 0, 1);
	}
}

mat2& mat2::operator +()
{
	return *this;
}

mat2& mat2::operator -()
{
	col0 = -col0;
	col1 = -col1;

	return *this;
}

mat2& mat2::operator +=(float value)
{
	col0 += value;
	col1 += value;

	return *this;
}

mat2& mat2::operator +=(const vec2& v)
{
	col0 += v;
	col1 += v;

	return *this;
}

mat2& mat2::operator +=(const mat2& A)
{
	col0 += A.col0;
	col1 += A.col1;

	return *this;
}

mat2& mat2::operator -=(float value)
{
	col0 -= value;
	col1 -= value;

	return *this;
}

mat2& mat2::operator -=(const vec2& v)
{
	col0 -= v;
	col1 -= v;

	return *this;
}

mat2& mat2::operator -=(const mat2& A)
{
	col0 -= A.col0;
	col1 -= A.col1;

	return *this;
}

mat2& mat2::operator *=(float value)
{
	col0 *= value;
	col1 *= value;

	return *this;
}

mat2& mat2::operator *=(const mat2& A)
{
	return *this = *this * A;
}

mat2& mat2::operator %=(float value)
{
	col0 *= value;
	col1 *= value;

	return *this;
}

mat2& mat2::operator %=(const vec2& v)
{
	col0 *= v;
	col1 *= v;

	return *this;
}

mat2& mat2::operator %=(const mat2& A)
{
	col0 *= A.col0;
	col1 *= A.col1;

	return *this;
}

mat2& mat2::operator /=(float value)
{
	col0 /= value;
	col1 /= value;

	return *this;
}

mat2& mat2::operator /=(const vec2& v)
{
	col0 /= v;
	col1 /= v;

	return *this;
}

mat2& mat2::operator /=(const mat2& A)
{
	col0 /= A.col0;
	col1 /= A.col1;

	return *this;
}

float* mat2::data()
{
	if(!ptr_value)
	{
		ptr_value = new float[4];
		if(!ptr_value)
		{
			throw glass::MemoryError("Failed to allocate memory.");
		}
	}
	
	int k = 0;
	for(int j = 0; j < 2; j++)
	{
		for(int i = 0; i < 2; i++)
		{
			ptr_value[k] = (*this)[j][i];
			k++;
		}
	}

	return ptr_value;
}

ostream& operator <<(ostream& out, const mat2& A)
{
	int *longest_size = new int[2];
	bool *has_minus = new bool[2];
    for(int j = 0; j < 2; j++)
	{
        longest_size[j] = 0;
        has_minus[j] = false;
		for(int i = 0; i < 2; i++)
		{
            int current_size = (int)(str::str(A[j][i]).size());
			if(current_size > longest_size[j])
			{
				longest_size[j] = current_size;
			}
			if(A[j][i] < 0)
			{
				has_minus[j] = true;
			}
		}
	}
	
	out << endl;
	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 2; j++)
		{
            int space_length = longest_size[j] - (int)(str::str(A[j][i]).size()) + 1;
			if(has_minus[j] && A[j][i] >= 0)
			{
				out << " " <<A[j][i] << str::multi_space(space_length);
			}
			else
			{
				out << A[j][i] << str::multi_space(space_length+1);
			}
		}
		if(i != 1)
		{
			out << endl;
		}
	}
	delete[] longest_size;
	delete[] has_minus;

	return out;
}

mat2 operator +(const mat2& A, float value)
{
	return mat2(A.col0 + value, A.col1 + value);
}

mat2 operator -(const mat2& A, float value)
{
	return mat2(A.col0 - value, A.col1 - value);
}

mat2 operator *(const mat2& A, float value)
{
	return mat2(A.col0 * value, A.col1 * value);
}

mat2 operator %(const mat2& A, float value)
{
	return mat2(A.col0 * value, A.col1 * value);
}

mat2 operator /(const mat2& A, float value)
{
	return mat2(A.col0 / value, A.col1 / value);
}

mat2 operator +(float value, const mat2& A)
{
	return mat2(value + A.col0, value + A.col1);
}

mat2 operator -(float value, const mat2& A)
{
	return mat2(value - A.col0, value - A.col1);
}

mat2 operator *(float value, const mat2& A)
{
	return mat2(value * A.col0, value * A.col1);
}

mat2 operator %(float value, const mat2& A)
{
	return mat2(value * A.col0, value * A.col1);
}

mat2 operator /(float value, const mat2& A)
{
	return mat2(value / A.col0, value / A.col1);
}

mat2 operator +(const mat2& A, const vec2& v)
{
	return mat2(A.col0 + v, A.col1 + v);
}

mat2 operator -(const mat2& A, const vec2& v)
{
	return mat2(A.col0 - v, A.col1 - v);
}

vec2 operator *(const mat2& A, const vec2& v)
{
	return vec2(A.col0.x * v.x + A.col1.x * v.y,
                A.col0.y * v.x + A.col1.y * v.y);
}

mat2 operator %(const mat2& A, const vec2& v)
{
	return mat2(A.col0 * v, A.col1 * v);
}

mat2 operator /(const mat2& A, const vec2& v)
{
	return mat2(A.col0 / v, A.col1 / v);
}

mat2 operator +(const vec2& v, const mat2& A)
{
	return mat2(v + A.col0, v + A.col1);
}

mat2 operator -(const vec2& v, const mat2& A)
{
	return mat2(v - A.col0, v - A.col1);
}

mat2 operator *(const vec2& v, const mat2& A)
{
	return mat2(v * A.col0, v * A.col1);
}

mat2 operator %(const vec2& v, const mat2& A)
{
	return mat2(v * A.col0, v * A.col1);
}

mat2 operator /(const vec2& v, const mat2& A)
{
	return mat2(v / A.col0, v / A.col1);
}

mat2 operator +(const mat2& A, const mat2& B)
{
	return mat2(A.col0 + B.col0, A.col1 + B.col1);
}

mat2 operator -(const mat2& A, const mat2& B)
{
	return mat2(A.col0 - B.col0, A.col1 - B.col1);
}

mat2 operator *(const mat2& A, const mat2& B)
{
	mat2 C;
	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 2; j++)
		{
			C[j][i] = 0.0f;
			for(int k = 0; k < 2; k++)
			{
				C[j][i] += A[k][i] * B[j][k];
			}
		}
	}

	return C;
}

mat2 operator %(const mat2& A, const mat2& B)
{
	return mat2(A.col0 * B.col0, A.col1 * B.col1);
}

mat2 operator /(const mat2& A, const mat2& B)
{
	return mat2(A.col0 / B.col0, A.col1 / B.col1);
}

mat2 pow(const mat2& A, float value)
{
	return mat2(pow(A.col0, value), pow(A.col1, value));
}

mat2 pow(const mat2& A, const mat2& B)
{
	return mat2(pow(A.col0, B.col0), pow(A.col1, B.col1));
}

mat2 pow(const mat2& A, const vec2& v)
{
	return mat2(pow(A.col0, v), pow(A.col1, v));
}

mat2 pow(float value, const mat2& A)
{
	return mat2(pow(value, A.col0), pow(value, A.col1));
}

mat2 pow(const vec2& v, const mat2& A)
{
	return mat2(pow(v, A.col0), pow(v, A.col1));
}

float det(const mat2& A)
{
	return A.col0.x * A.col1.y - A.col1.x * A.col0.y;
}

float trace(const mat2& A)
{
	return A.col0.x + A.col1.y;
}

float norm(const mat2& A)
{
	return trace(A * transpose(A));
}

mat2 inverse(const mat2& A)
{
	return mat2( A.col1.y, -A.col0.y,
		        -A.col1.x,  A.col0.x) / det(A);
}

mat2 transpose(const mat2& A)
{
	return mat2(A.col0.x, A.col1.x,
		        A.col0.y, A.col1.y);
}

mat2 sin(const mat2& A)
{
	return mat2(sin(A.col0), sin(A.col1));
}

mat2 cos(const mat2& A)
{
	return mat2(cos(A.col0), cos(A.col1));
}

mat2 tan(const mat2& A)
{
	return mat2(tan(A.col0), tan(A.col1));
}

mat2 sinh(const mat2& A)
{
	return mat2(sinh(A.col0), sinh(A.col1));
}

mat2 cosh(const mat2& A)
{
	return mat2(cosh(A.col0), cosh(A.col1));
}

mat2 tanh(const mat2& A)
{
	return mat2(tanh(A.col0), tanh(A.col1));
}

mat2 asin(const mat2& A)
{
	return mat2(asin(A.col0), asin(A.col1));
}

mat2 acos(const mat2& A)
{
	return mat2(acos(A.col0), acos(A.col1));
}

mat2 atan(const mat2& A)
{
	return mat2(atan(A.col0), atan(A.col1));
}

mat2 asinh(const mat2& A)
{
	return mat2(asinh(A.col0), asinh(A.col1));
}

mat2 acosh(const mat2& A)
{
	return mat2(acosh(A.col0), acosh(A.col1));
}

mat2 atanh(const mat2& A)
{
	return mat2(atanh(A.col0), atanh(A.col1));
}

mat2 atan2(const mat2& Y, const mat2& X)
{
	return mat2(atan2(Y.col0, X.col0), atan2(Y.col1, X.col1));
}

mat2 exp(const mat2& A)
{
	return mat2(exp(A.col0), exp(A.col1));
}

mat2 log(const mat2& A)
{
	return mat2(log(A.col0), log(A.col1));
}

mat2 log2(const mat2& A)
{
	return mat2(log2(A.col0), log2(A.col1));
}

mat2 log10(const mat2& A)
{
	return mat2(log10(A.col0), log10(A.col1));
}

mat2 abs(const mat2& A)
{
	return mat2(abs(A.col0), abs(A.col1));
}

mat2 floor(const mat2& A)
{
	return mat2(floor(A.col0), floor(A.col1));
}

mat2 ceil(const mat2& A)
{
	return mat2(ceil(A.col0), ceil(A.col1));
}

mat2 round(const mat2& A, uint n)
{
	return mat2(round(A.col0, n), round(A.col1, n));
}

mat2 trunc(const mat2& A)
{
	return mat2(trunc(A.col0), trunc(A.col1));
}

mat2 fix(const mat2& A)
{
	return mat2(fix(A.col0), fix(A.col1));
}

mat3::mat3() :
col0(1.0f, 0.0f, 0.0f),
col1(0.0f, 1.0f, 0.0f),
col2(0.0f, 0.0f, 1.0f) {}
mat3::mat3(float v00, float v10, float v20,
	       float v01, float v11, float v21,
	       float v02, float v12, float v22) :
col0(v00, v10, v20),
col1(v01, v11, v21),
col2(v02, v12, v22) {}
mat3::mat3(float value) :
col0(value, 0.0f, 0.0f),
col1(0.0f, value, 0.0f),
col2(0.0f, 0.0f, value) {}
mat3::mat3(const vec3& _col0, const vec3& _col1, const vec3& _col2) : col0(_col0), col1(_col1), col2(_col2) {}
mat3::mat3(const mat3& A) : col0(A.col0), col1(A.col1), col2(A.col2) {}
mat3::mat3(mat3&& A)      : col0(move(A.col0)), col1(move(A.col1)), col2(move(A.col2)), ptr_value(move(A.ptr_value)) {}
mat3::mat3(const mat4& A) : col0(A.col0), col1(A.col1), col2(A.col2) {}
mat3::~mat3()
{
	if(ptr_value)
	{
		delete[] ptr_value;
	}
}

mat3& mat3::operator =(const mat3& A)
{
	if(&A != this)
	{
		col0 = A.col0;
		col1 = A.col1;
		col2 = A.col2;
	}

	return *this;
}

mat3& mat3::operator =(mat3&& A)
{
	if(&A != this)
	{
		col0 = move(A.col0);
		col1 = move(A.col1);
		col2 = move(A.col2);
		if(ptr_value)
		{
			delete[] ptr_value;
		}
		ptr_value = move(A.ptr_value);
	}

	return *this;
}

vec3& mat3::operator [](uint i)
{
	switch(i)
	{
		case 0: return col0;
		case 1: return col1;
		case 2: return col2;
		default: throw glass::IndexError(i, 0, 2);
	}
}

vec3 mat3::operator [](uint i)const
{
	switch(i)
	{
		case 0: return col0;
		case 1: return col1;
		case 2: return col2;
		default: throw glass::IndexError(i, 0, 2);
	}
}

mat3& mat3::operator +()
{
	return *this;
}

mat3& mat3::operator -()
{
	col0 = -col0;
	col1 = -col1;
	col2 = -col2;

	return *this;
}

mat3& mat3::operator +=(float value)
{
	col0 += value;
	col1 += value;
	col2 += value;

	return *this;
}

mat3& mat3::operator +=(const vec3& v)
{
	col0 += v;
	col1 += v;
	col2 += v;

	return *this;
}

mat3& mat3::operator +=(const mat3& A)
{
	col0 += A.col0;
	col1 += A.col1;
	col2 += A.col2;

	return *this;
}

mat3& mat3::operator -=(float value)
{
	col0 -= value;
	col1 -= value;
	col2 -= value;

	return *this;
}

mat3& mat3::operator -=(const vec3& v)
{
	col0 -= v;
	col1 -= v;
	col2 -= v;

	return *this;
}

mat3& mat3::operator -=(const mat3& A)
{
	col0 -= A.col0;
	col1 -= A.col1;
	col2 -= A.col2;

	return *this;
}

mat3& mat3::operator *=(float value)
{
	col0 *= value;
	col1 *= value;
	col2 *= value;

	return *this;
}

mat3& mat3::operator *=(const mat3& A)
{
	return *this = *this * A;
}

mat3& mat3::operator %=(float value)
{
	col0 *= value;
	col1 *= value;
	col2 *= value;

	return *this;
}

mat3& mat3::operator %=(const vec3& v)
{
	col0 *= v;
	col1 *= v;
	col2 *= v;

	return *this;
}

mat3& mat3::operator %=(const mat3& A)
{
	col0 *= A.col0;
	col1 *= A.col1;
	col2 *= A.col2;

	return *this;
}

mat3& mat3::operator /=(float value)
{
	col0 /= value;
	col1 /= value;
	col2 /= value;

	return *this;
}

mat3& mat3::operator /=(const vec3& v)
{
	col0 /= v;
	col1 /= v;
	col2 /= v;

	return *this;
}

mat3& mat3::operator /=(const mat3& A)
{
	col0 /= A.col0;
	col1 /= A.col1;
	col2 /= A.col2;

	return *this;
}

float* mat3::data()
{
	if(!ptr_value)
	{
		ptr_value = new float[9];
		if(!ptr_value)
		{
			throw glass::MemoryError("Failed to allocate memory.");
		}
	}
	
	int k = 0;
	for(int j = 0; j < 3; j++)
	{
		for(int i = 0; i < 3; i++)
		{
			ptr_value[k] = (*this)[j][i];
			k++;
		}
	}

	return ptr_value;
}

ostream& operator <<(ostream& out, const mat3& A)
{
	int *longest_size = new int[3];
	bool *has_minus = new bool[3];
    for(int j = 0; j < 3; j++)
	{
        longest_size[j] = 0;
        has_minus[j] = false;
		for(int i = 0; i < 3; i++)
		{
            int current_size = (int)(str::str(A[j][i]).size());
			if(current_size > longest_size[j])
			{
				longest_size[j] = current_size;
			}
			if(A[j][i] < 0)
			{
				has_minus[j] = true;
			}
		}
	}
	
	out << endl;
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
            int space_length = longest_size[j] - (int)(str::str(A[j][i]).size()) + 1;
			if(has_minus[j] && A[j][i] >= 0)
			{
				out << " " <<A[j][i] << str::multi_space(space_length);
			}
			else
			{
				out << A[j][i] << str::multi_space(space_length+1);
			}
		}
		if(i != 2)
		{
			out << endl;
		}
	}
	delete[] longest_size;
	delete[] has_minus;

	return out;
}

mat3 operator +(const mat3& A, float value)
{
	return mat3(A.col0 + value, A.col1 + value, A.col2 + value);
}

mat3 operator -(const mat3& A, float value)
{
	return mat3(A.col0 - value, A.col1 - value, A.col2 - value);
}

mat3 operator *(const mat3& A, float value)
{
	return mat3(A.col0 * value, A.col1 * value, A.col2 * value);
}

mat3 operator %(const mat3& A, float value)
{
	return mat3(A.col0 * value, A.col1 * value, A.col2 * value);
}

mat3 operator /(const mat3& A, float value)
{
	return mat3(A.col0 / value, A.col1 / value, A.col2 / value);
}

mat3 operator +(float value, const mat3& A)
{
	return mat3(value + A.col0, value + A.col1, value + A.col2);
}

mat3 operator -(float value, const mat3& A)
{
	return mat3(value - A.col0, value - A.col1, value - A.col2);
}

mat3 operator *(float value, const mat3& A)
{
	return mat3(value * A.col0, value * A.col1, value * A.col2);
}

mat3 operator %(float value, const mat3& A)
{
	return mat3(value * A.col0, value * A.col1, value * A.col2);
}

mat3 operator /(float value, const mat3& A)
{
	return mat3(value / A.col0, value / A.col1, value / A.col2);
}

mat3 operator +(const mat3& A, const vec3& v)
{
	return mat3(A.col0 + v, A.col1 + v, A.col2 + v);
}

mat3 operator -(const mat3& A, const vec3& v)
{
	return mat3(A.col0 - v, A.col1 - v, A.col2 - v);
}

vec3 operator *(const mat3& A, const vec3& v)
{
	return vec3(A.col0.x * v.x + A.col1.x * v.y + A.col2.x * v.z,
                A.col0.y * v.x + A.col1.y * v.y + A.col2.y * v.z,
                A.col0.z * v.x + A.col1.z * v.y + A.col2.z * v.z);
}

mat3 operator %(const mat3& A, const vec3& v)
{
	return mat3(A.col0 * v, A.col1 * v, A.col2 * v);
}

mat3 operator /(const mat3& A, const vec3& v)
{
	return mat3(A.col0 / v, A.col1 / v, A.col2 / v);
}

mat3 operator +(const vec3& v, const mat3& A)
{
	return mat3(v + A.col0, v + A.col1, v + A.col2);
}

mat3 operator -(const vec3& v, const mat3& A)
{
	return mat3(v - A.col0, v - A.col1, v - A.col2);
}

mat3 operator *(const vec3& v, const mat3& A)
{
	return mat3(v * A.col0, v * A.col1, v * A.col2);
}

mat3 operator %(const vec3& v, const mat3& A)
{
	return mat3(v * A.col0, v * A.col1, v * A.col2);
}

mat3 operator /(const vec3& v, const mat3& A)
{
	return mat3(v / A.col0, v / A.col1, v / A.col2);
}

mat3 operator +(const mat3& A, const mat3& B)
{
	return mat3(A.col0 + B.col0, A.col1 + B.col1, A.col2 + B.col2);
}

mat3 operator -(const mat3& A, const mat3& B)
{
	return mat3(A.col0 - B.col0, A.col1 - B.col1, A.col2 - B.col2);
}

mat3 operator *(const mat3& A, const mat3& B)
{
	mat3 C;
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			C[j][i] = 0.0f;
			for(int k = 0; k < 3; k++)
			{
				C[j][i] += A[k][i] * B[j][k];
			}
		}
	}

	return C;
}

mat3 operator %(const mat3& A, const mat3& B)
{
	return mat3(A.col0 * B.col0, A.col1 * B.col1, A.col2 * B.col2);
}

mat3 operator /(const mat3& A, const mat3& B)
{
	return mat3(A.col0 / B.col0, A.col1 / B.col1, A.col2 / B.col2);
}

mat3 pow(const mat3& A, float value)
{
	return mat3(pow(A.col0, value), pow(A.col1, value), pow(A.col2, value));
}

mat3 pow(const mat3& A, const mat3& B)
{
	return mat3(pow(A.col0, B.col0), pow(A.col1, B.col1), pow(A.col2, B.col2));
}

mat3 pow(const mat3& A, const vec3& v)
{
	return mat3(pow(A.col0, v), pow(A.col1, v), pow(A.col2, v));
}

mat3 pow(float value, const mat3& A)
{
	return mat3(pow(value, A.col0), pow(value, A.col1), pow(value, A.col2));
}

mat3 pow(const vec3& v, const mat3& A)
{
	return mat3(pow(v, A.col0), pow(v, A.col1), pow(v, A.col2));
}

float det(mat3 A)
{
	int sig = 1;
	for(int j = 0; j < 3; j++)
	{
		float main_value = A[j][j];
		int main_col = j;
		for(int subj = j+1; subj < 3; subj++)
		{
			if(fabs(A[subj][j]) > fabs(main_value))
			{
				main_value = A[subj][j];
				main_col = subj;
			}
			if(is_zero(main_value))
			{
				return 0.0f;
			}

			if(main_col != j)
			{
				swap(A[j], A[main_col]);
				sig *= -1;
			}

			for(int subj = j+1; subj < 3; subj++)
			{
				A[subj] -= (A[subj][j]/main_value)*A[j];
			}
		}
	}
	float result = 1.0f;
	for(int j = 0; j < 3; j++)
	{
		result *= A[j][j];
	}

	return sig * result;
}

float trace(const mat3& A)
{
	return A.col0.x + A.col1.y + A.col2.z;
}

float norm(const mat3& A)
{
	return trace(A * transpose(A));
}

mat3 inverse(mat3 A)
{
	mat3 B;
	for(int j = 0; j < 3; j++)
	{
		float main_value = A[j][j];
		int main_col = j;
		for(int subj = j+1; subj < 3; subj++)
		{
			if(fabs(A[subj][j]) > fabs(main_value))
			{
				main_value = A[subj][j];
				main_col = subj;
			}
		}
		if(is_zero(main_value))
		{
			throw glass::ZeroDivisionError("Error using inverse(mat3 A):\ndet(A) is 0 now.");
		}

		if(main_col != j)
		{
			swap(A[j], A[main_col]);
			swap(B[j], B[main_col]);
		}
		
		A[j] /= main_value;
		B[j] /= main_value;

		A[j][j] = 1.0f; // float correction

		for(int subj = 0; subj < 3; subj++)
		{
			if(subj != j)
			{
				B[subj] -= A[subj][j]*B[j];
				A[subj] -= A[subj][j]*A[j];
				A[subj][j] = 0.0f; // float correction
			}
		}
	}
	return B;
}

mat3 transpose(const mat3& A)
{
	return mat3(A.col0.x, A.col1.x, A.col2.x,
		        A.col0.y, A.col1.y, A.col2.y,
		        A.col0.z, A.col1.z, A.col2.z);
}

mat3 sin(const mat3& A)
{
	return mat3(sin(A.col0), sin(A.col1), sin(A.col2));
}

mat3 cos(const mat3& A)
{
	return mat3(cos(A.col0), cos(A.col1), cos(A.col2));
}

mat3 tan(const mat3& A)
{
	return mat3(tan(A.col0), tan(A.col1), tan(A.col2));
}

mat3 sinh(const mat3& A)
{
	return mat3(sinh(A.col0), sinh(A.col1), sinh(A.col2));
}

mat3 cosh(const mat3& A)
{
	return mat3(cosh(A.col0), cosh(A.col1), cosh(A.col2));
}

mat3 tanh(const mat3& A)
{
	return mat3(tanh(A.col0), tanh(A.col1), tanh(A.col2));
}

mat3 asin(const mat3& A)
{
	return mat3(asin(A.col0), asin(A.col1), asin(A.col2));
}

mat3 acos(const mat3& A)
{
	return mat3(acos(A.col0), acos(A.col1), acos(A.col2));
}

mat3 atan(const mat3& A)
{
	return mat3(atan(A.col0), atan(A.col1), atan(A.col2));
}

mat3 asinh(const mat3& A)
{
	return mat3(asinh(A.col0), asinh(A.col1), asinh(A.col2));
}

mat3 acosh(const mat3& A)
{
	return mat3(acosh(A.col0), acosh(A.col1), acosh(A.col2));
}

mat3 atanh(const mat3& A)
{
	return mat3(atanh(A.col0), atanh(A.col1), atanh(A.col2));
}

mat3 atan2(const mat3& Y, const mat3& X)
{
	return mat3(atan2(Y.col0, X.col0), atan2(Y.col1, X.col1), atan2(Y.col2, X.col2));
}

mat3 exp(const mat3& A)
{
	return mat3(exp(A.col0), exp(A.col1), exp(A.col2));
}

mat3 log(const mat3& A)
{
	return mat3(log(A.col0), log(A.col1), log(A.col2));
}

mat3 log2(const mat3& A)
{
	return mat3(log2(A.col0), log2(A.col1), log2(A.col2));
}

mat3 log10(const mat3& A)
{
	return mat3(log10(A.col0), log10(A.col1), log10(A.col2));
}

mat3 abs(const mat3& A)
{
	return mat3(abs(A.col0), abs(A.col1), abs(A.col2));
}

mat3 floor(const mat3& A)
{
	return mat3(floor(A.col0), floor(A.col1), floor(A.col2));
}

mat3 ceil(const mat3& A)
{
	return mat3(ceil(A.col0), ceil(A.col1), ceil(A.col2));
}

mat3 round(const mat3& A, uint n)
{
	return mat3(round(A.col0, n), round(A.col1, n), round(A.col2, n));
}

mat3 trunc(const mat3& A)
{
	return mat3(trunc(A.col0), trunc(A.col1), trunc(A.col2));
}

mat3 fix(const mat3& A)
{
	return mat3(fix(A.col0), fix(A.col1), fix(A.col2));
}

mat3 cross(const vec3& v)
{
	return mat3( 0.0f, v.z,  -v.y,
				-v.z,  0.0f, v.x,
				 v.y, -v.x,  0.0f);
}

mat4::mat4() :
col0(1.0f, 0.0f, 0.0f, 0.0f),
col1(0.0f, 1.0f, 0.0f, 0.0f),
col2(0.0f, 0.0f, 1.0f, 0.0f),
col3(0.0f, 0.0f, 0.0f, 1.0f) {}
mat4::mat4(float v00, float v10, float v20, float v30,
	       float v01, float v11, float v21, float v31,
	       float v02, float v12, float v22, float v32,
	       float v03, float v13, float v23, float v33) :
col0(v00, v10, v20, v30),
col1(v01, v11, v21, v31),
col2(v02, v12, v22, v32),
col3(v03, v13, v23, v33) {}
mat4::mat4(float value) :
col0(value, 0.0f, 0.0f, 0.0f),
col1(0.0f, value, 0.0f, 0.0f),
col2(0.0f, 0.0f, value, 0.0f),
col3(0.0f, 0.0f, 0.0f, value) {}
mat4::mat4(const vec4& _col0, const vec4& _col1, const vec4& _col2, const vec4& _col3) :
col0(_col0), col1(_col1), col2(_col2), col3(_col3) {}
mat4::mat4(const mat4& A) : col0(A.col0), col1(A.col1), col2(A.col2), col3(A.col3) {}
mat4::mat4(mat4&& A) : col0(move(A.col0)), col1(move(A.col1)), col2(move(A.col2)), col3(move(A.col3)), ptr_value(move(A.ptr_value)) {}
mat4::~mat4()
{
	if(ptr_value)
	{
		delete[] ptr_value;
	}
}

mat4& mat4::operator =(const mat4& A)
{
	if(&A != this)
	{
		col0 = A.col0;
		col1 = A.col1;
		col2 = A.col2;
		col3 = A.col3;
	}

	return *this;
}

mat4& mat4::operator =(mat4&& A)
{
	if(&A != this)
	{
		col0 = move(A.col0);
		col1 = move(A.col1);
		col2 = move(A.col2);
		col3 = move(A.col3);
		if(ptr_value)
		{
			delete[] ptr_value;
		}
		ptr_value = move(A.ptr_value);
	}

	return *this;
}

vec4& mat4::operator [](uint i)
{
	switch(i)
	{
		case 0: return col0;
		case 1: return col1;
		case 2: return col2;
		case 3: return col3;
		default: throw glass::IndexError(i, 0, 3);
	}
}

vec4 mat4::operator [](uint i)const
{
	switch(i)
	{
		case 0: return col0;
		case 1: return col1;
		case 2: return col2;
		case 3: return col3;
		default: throw glass::IndexError(i, 0, 3);
	}
}

mat4& mat4::operator +()
{
	return *this;
}

mat4& mat4::operator -()
{
	col0 = -col0;
	col1 = -col1;
	col2 = -col2;
	col3 = -col3;

	return *this;
}

mat4& mat4::operator +=(float value)
{
	col0 += value;
	col1 += value;
	col2 += value;
	col3 += value;

	return *this;
}

mat4& mat4::operator +=(const vec4& v)
{
	col0 += v;
	col1 += v;
	col2 += v;
	col3 += v;

	return *this;
}

mat4& mat4::operator +=(const mat4& A)
{
	col0 += A.col0;
	col1 += A.col1;
	col2 += A.col2;
	col3 += A.col3;

	return *this;
}

mat4& mat4::operator -=(float value)
{
	col0 -= value;
	col1 -= value;
	col2 -= value;
	col3 -= value;

	return *this;
}

mat4& mat4::operator -=(const vec4& v)
{
	col0 -= v;
	col1 -= v;
	col2 -= v;
	col3 -= v;

	return *this;
}

mat4& mat4::operator -=(const mat4& A)
{
	col0 -= A.col0;
	col1 -= A.col1;
	col2 -= A.col2;
	col3 -= A.col3;

	return *this;
}

mat4& mat4::operator *=(float value)
{
	col0 *= value;
	col1 *= value;
	col2 *= value;
	col3 *= value;

	return *this;
}

mat4& mat4::operator *=(const mat4& A)
{
	return *this = *this * A;
}

mat4& mat4::operator %=(float value)
{
	col0 *= value;
	col1 *= value;
	col2 *= value;
	col3 *= value;

	return *this;
}

mat4& mat4::operator %=(const vec4& v)
{
	col0 *= v;
	col1 *= v;
	col2 *= v;
	col3 *= v;

	return *this;
}

mat4& mat4::operator %=(const mat4& A)
{
	col0 *= A.col0;
	col1 *= A.col1;
	col2 *= A.col2;
	col3 *= A.col3;

	return *this;
}

mat4& mat4::operator /=(float value)
{
	col0 /= value;
	col1 /= value;
	col2 /= value;
	col3 /= value;

	return *this;
}

mat4& mat4::operator /=(const vec4& v)
{
	col0 /= v;
	col1 /= v;
	col2 /= v;
	col3 /= v;

	return *this;
}

mat4& mat4::operator /=(const mat4& A)
{
	col0 /= A.col0;
	col1 /= A.col1;
	col2 /= A.col2;
	col3 /= A.col3;

	return *this;
}

float* mat4::data()
{
	if(!ptr_value)
	{
		ptr_value = new float[16];
		if(!ptr_value)
		{
			throw glass::MemoryError("Failed to allocate memory.");
		}
	}
	
	int k = 0;
	for(int j = 0; j < 4; j++)
	{
		for(int i = 0; i < 4; i++)
		{
			ptr_value[k] = (*this)[j][i];
			k++;
		}
	}

	return ptr_value;
}

ostream& operator <<(ostream& out, const mat4& A)
{
	int *longest_size = new int[4];
	bool *has_minus = new bool[4];
    for(int j = 0; j < 4; j++)
	{
        longest_size[j] = 0;
        has_minus[j] = false;
		for(int i = 0; i < 4; i++)
		{
            int current_size = (int)(str::str(A[j][i]).size());
			if(current_size > longest_size[j])
			{
				longest_size[j] = current_size;
			}
			if(A[j][i] < 0)
			{
				has_minus[j] = true;
			}
		}
	}
	
	out << endl;
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
            int space_length = longest_size[j] - (int)(str::str(A[j][i]).size()) + 1;
			if(has_minus[j] && A[j][i] >= 0)
			{
				out << " " <<A[j][i] << str::multi_space(space_length);
			}
			else
			{
				out << A[j][i] << str::multi_space(space_length+1);
			}
		}
		if(i != 3)
		{
			out << endl;
		}
	}
	delete[] longest_size;
	delete[] has_minus;

	return out;
}

mat4 operator +(const mat4& A, float value)
{
	return mat4(A.col0 + value,
		        A.col1 + value,
		        A.col2 + value,
		        A.col3 + value);
}

mat4 operator -(const mat4& A, float value)
{
	return mat4(A.col0 - value,
		        A.col1 - value,
		        A.col2 - value,
		        A.col3 - value);
}

mat4 operator *(const mat4& A, float value)
{
	return mat4(A.col0 * value,
		        A.col1 * value,
		        A.col2 * value,
		        A.col3 * value);
}

mat4 operator %(const mat4& A, float value)
{
	return mat4(A.col0 * value,
		        A.col1 * value,
		        A.col2 * value,
		        A.col3 * value);
}

mat4 operator /(const mat4& A, float value)
{
	return mat4(A.col0 / value,
		        A.col1 / value,
		        A.col2 / value,
		        A.col3 / value);
}

mat4 operator +(float value, const mat4& A)
{
	return mat4(value + A.col0,
		        value + A.col1,
		        value + A.col2,
		        value + A.col3);
}

mat4 operator -(float value, const mat4& A)
{
	return mat4(value - A.col0,
		        value - A.col1,
		        value - A.col2,
		        value - A.col3);
}

mat4 operator *(float value, const mat4& A)
{
	return mat4(value * A.col0,
		        value * A.col1,
		        value * A.col2,
		        value * A.col3);
}

mat4 operator %(float value, const mat4& A)
{
	return mat4(value * A.col0,
		        value * A.col1,
		        value * A.col2,
		        value * A.col3);
}

mat4 operator /(float value, const mat4& A)
{
	return mat4(value / A.col0,
		        value / A.col1,
		        value / A.col2,
		        value / A.col3);
}

mat4 operator +(const mat4& A, const vec4& v)
{
	return mat4(A.col0 + v,
		        A.col1 + v,
		        A.col2 + v,
		        A.col3 + v);
}

mat4 operator -(const mat4& A, const vec4& v)
{
	return mat4(A.col0 - v,
		        A.col1 - v,
		        A.col2 - v,
		        A.col3 - v);
}

vec4 operator *(const mat4& A, const vec4& v)
{
	return vec4(A.col0.x * v.x + A.col1.x * v.y + A.col2.x * v.z + A.col3.x * v.w,
                A.col0.y * v.x + A.col1.y * v.y + A.col2.y * v.z + A.col3.y * v.w,
                A.col0.z * v.x + A.col1.z * v.y + A.col2.z * v.z + A.col3.z * v.w,
                A.col0.w * v.x + A.col1.w * v.y + A.col2.w * v.z + A.col3.w * v.w);
}

mat4 operator %(const mat4& A, const vec4& v)
{
	return mat4(A.col0 * v,
		        A.col1 * v,
	            A.col2 * v,
	            A.col3 * v);
}

mat4 operator /(const mat4& A, const vec4& v)
{
	return mat4(A.col0 / v,
		        A.col1 / v,
		        A.col2 / v,
		        A.col3 / v);
}

mat4 operator +(const vec4& v, const mat4& A)
{
	return mat4(v + A.col0,
		        v + A.col1,
		        v + A.col2,
		        v + A.col3);
}

mat4 operator -(const vec4& v, const mat4& A)
{
	return mat4(v - A.col0,
		        v - A.col1,
		        v - A.col2,
		        v - A.col3);
}

mat4 operator *(const vec4& v, const mat4& A)
{
	return mat4(v * A.col0,
		        v * A.col1,
		        v * A.col2,
		        v * A.col3);
}

mat4 operator %(const vec4& v, const mat4& A)
{
	return mat4(v * A.col0,
		        v * A.col1,
		        v * A.col2,
		        v * A.col3);
}

mat4 operator /(const vec4& v, const mat4& A)
{
	return mat4(v / A.col0,
		        v / A.col1,
		        v / A.col2,
		        v / A.col3);
}

mat4 operator +(const mat4& A, const mat4& B)
{
	return mat4(A.col0 + B.col0,
		        A.col1 + B.col1,
		        A.col2 + B.col2,
		        A.col3 + B.col3);
}

mat4 operator -(const mat4& A, const mat4& B)
{
	return mat4(A.col0 - B.col0,
		        A.col1 - B.col1,
		        A.col2 - B.col2,
		        A.col3 - B.col3);
}

mat4 operator *(const mat4& A, const mat4& B)
{
	mat4 C;
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			C[j][i] = 0.0f;
			for(int k = 0; k < 4; k++)
			{
				C[j][i] += A[k][i] * B[j][k];
			}
		}
	}

	return C;
}

mat4 operator %(const mat4& A, const mat4& B)
{
	return mat4(A.col0 * B.col0,
		        A.col1 * B.col1,
		        A.col2 * B.col2,
		        A.col3 * B.col3);
}

mat4 operator /(const mat4& A, const mat4& B)
{
	return mat4(A.col0 / B.col0,
		        A.col1 / B.col1,
		        A.col2 / B.col2,
		        A.col3 / B.col3);
}

mat4 pow(const mat4& A, float value)
{
	return mat4(pow(A.col0, value),
		        pow(A.col1, value),
		        pow(A.col2, value),
		        pow(A.col3, value));
}

mat4 pow(const mat4& A, const mat4& B)
{
	return mat4(pow(A.col0, B.col0),
		        pow(A.col1, B.col1),
		        pow(A.col2, B.col2),
		        pow(A.col3, B.col3));
}

mat4 pow(const mat4& A, const vec4& v)
{
	return mat4(pow(A.col0, v),
		        pow(A.col1, v),
		        pow(A.col2, v),
		        pow(A.col3, v));
}

mat4 pow(float value, const mat4& A)
{
	return mat4(pow(value, A.col0),
		        pow(value, A.col1),
		        pow(value, A.col2),
		        pow(value, A.col3));
}

mat4 pow(const vec4& v, const mat4& A)
{
	return mat4(pow(v, A.col0),
		        pow(v, A.col1),
		        pow(v, A.col2),
		        pow(v, A.col3));
}

float det(mat4 A)
{
	int sig = 1;
	for(int j = 0; j < 4; j++)
	{
		float main_value = A[j][j];
		int main_col = j;
		for(int subj = j+1; subj < 4; subj++)
		{
			if(fabs(A[subj][j]) > fabs(main_value))
			{
				main_value = A[subj][j];
				main_col = subj;
			}
			if(is_zero(main_value))
			{
				return 0.0f;
			}

			if(main_col != j)
			{
				std::swap(A[j], A[main_col]);
				sig *= -1;
			}

			for(int subj = j+1; subj < 4; subj++)
			{
				A[subj] -= (A[subj][j]/main_value)*A[j];
			}
		}
	}
	float result = 1.0f;
	for(int j = 0; j < 4; j++)
	{
		result *= A[j][j];
	}

	return sig * result;
}

float trace(const mat4& A)
{
	return A.col0.x + A.col1.y + A.col2.z + A.col3.w;
}

float norm(const mat4& A)
{
	return trace(A * transpose(A));
}

mat4 inverse(mat4 A)
{
	mat4 B;
	for(int j = 0; j < 4; j++)
	{
		float main_value = A[j][j];
		int main_col = j;
		for(int subj = j+1; subj < 4; subj++)
		{
			if(fabs(A[subj][j]) > fabs(main_value))
			{
				main_value = A[subj][j];
				main_col = subj;
			}
		}
		if(is_zero(main_value))
		{
			throw glass::ZeroDivisionError("Error using inverse(mat4 A):\ndet(A) is 0 now.");
		}

		if(j != main_col)
		{
			swap(A[j], A[main_col]);
			swap(B[j], B[main_col]);
		}

		A[j] /= main_value;
		B[j] /= main_value;

		A[j][j] = 1.0f; // float correction

		for(int subj = 0; subj < 4; subj++)
		{
			if(subj != j)
			{
				B[subj] -= A[subj][j]*B[j];
				A[subj] -= A[subj][j]*A[j];
				A[subj][j] = 0.0f; // float correction
			}
		}
	}
	return B;
}

mat4 transpose(const mat4& A)
{
	return mat4(A.col0.x, A.col1.x, A.col2.x, A.col3.x,
		        A.col0.y, A.col1.y, A.col2.y, A.col3.y,
		        A.col0.z, A.col1.z, A.col2.z, A.col3.z,
		        A.col0.w, A.col1.w, A.col2.w, A.col3.w);
}

mat4 sin(const mat4& A)
{
	return mat4(sin(A.col0), sin(A.col1), sin(A.col2), sin(A.col3));
}

mat4 cos(const mat4& A)
{
	return mat4(cos(A.col0), cos(A.col1), cos(A.col2), cos(A.col3));
}

mat4 tan(const mat4& A)
{
	return mat4(tan(A.col0), tan(A.col1), tan(A.col2), tan(A.col3));
}

mat4 sinh(const mat4& A)
{
	return mat4(sinh(A.col0), sinh(A.col1), sinh(A.col2), sinh(A.col3));
}

mat4 cosh(const mat4& A)
{
	return mat4(cosh(A.col0), cosh(A.col1), cosh(A.col2), cosh(A.col3));
}

mat4 tanh(const mat4& A)
{
	return mat4(tanh(A.col0), tanh(A.col1), tanh(A.col2), tanh(A.col3));
}

mat4 asin(const mat4& A)
{
	return mat4(asin(A.col0), asin(A.col1), asin(A.col2), asin(A.col3));
}

mat4 acos(const mat4& A)
{
	return mat4(acos(A.col0), acos(A.col1), acos(A.col2), acos(A.col3));
}

mat4 atan(const mat4& A)
{
	return mat4(atan(A.col0), atan(A.col1), atan(A.col2), atan(A.col3));
}

mat4 asinh(const mat4& A)
{
	return mat4(asinh(A.col0), asinh(A.col1), asinh(A.col2), asinh(A.col3));
}

mat4 acosh(const mat4& A)
{
	return mat4(acosh(A.col0), acosh(A.col1), acosh(A.col2), acosh(A.col3));
}

mat4 atanh(const mat4& A)
{
	return mat4(atanh(A.col0), atanh(A.col1), atanh(A.col2), atanh(A.col3));
}

mat4 atan2(const mat4& Y, const mat4& X)
{
	return mat4(atan2(Y.col0, X.col0), atan2(Y.col1, X.col1), atan2(Y.col2, X.col2), atan2(Y.col3, X.col3));
}

mat4 exp(const mat4& A)
{
	return mat4(exp(A.col0), exp(A.col1), exp(A.col2), exp(A.col3));
}

mat4 log(const mat4& A)
{
	return mat4(log(A.col0), log(A.col1), log(A.col2), log(A.col3));
}

mat4 log2(const mat4& A)
{
	return mat4(log2(A.col0), log2(A.col1), log2(A.col2), log2(A.col3));
}

mat4 log10(const mat4& A)
{
	return mat4(log10(A.col0), log10(A.col1), log10(A.col2), log10(A.col3));
}

mat4 abs(const mat4& A)
{
	return mat4(abs(A.col0), abs(A.col1), abs(A.col2), abs(A.col3));
}

mat4 floor(const mat4& A)
{
	return mat4(floor(A.col0), floor(A.col1), floor(A.col2), floor(A.col3));
}

mat4 ceil(const mat4& A)
{
	return mat4(ceil(A.col0), ceil(A.col1), ceil(A.col2), ceil(A.col3));
}

mat4 round(const mat4& A, uint n)
{
	return mat4(round(A.col0, n), round(A.col1, n), round(A.col2, n), round(A.col3, n));
}

mat4 trunc(const mat4& A)
{
	return mat4(trunc(A.col0), trunc(A.col1), trunc(A.col2), trunc(A.col3));
}

mat4 fix(const mat4& A)
{
	return mat4(fix(A.col0), fix(A.col1), fix(A.col2), fix(A.col3));
}