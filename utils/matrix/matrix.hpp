#ifndef __MATRIX_H__
#define __MATRIX_H__

#define AUTO -1

#include <iostream>
#include <ctime>
#include <cmath>
#include <sstream>
#include <vector>

using namespace std;

template<class DataType = float>
class Matrix
{
private:
	static string cout2string(const DataType& element);
	static string multi_space(int n);
	static bool isZero(double a, double epsilon = 1E-6);
	void swap_row(int row1, int row2);
	void divide_row(int row1, const DataType& k);
	void add_row(int row1, const DataType& k, int row2);

public:
	static int sign(float a);

protected:
	int n_rows = 0;
	int n_cols = 0;
	int _size = 0;
	DataType *_data = NULL;

public:
	Matrix(){}
	Matrix(int rows, int cols);

	template<class ElemType>
	Matrix(int rows, int cols, const ElemType* values);

	template<class ElemType>
	Matrix(int rows, int cols, const ElemType& value);

	Matrix(const Matrix<DataType>& A);
	Matrix(Matrix<DataType>&& A);

	template<class ElemType>
	Matrix(const Matrix<ElemType>& A);

	template<class ElemType>
	Matrix(const vector< vector<ElemType> >& X);

	template<class ElemType>
	Matrix(const vector<ElemType>& x);

	template<class ElemType>
	Matrix(const initializer_list< initializer_list<ElemType> >& X);

	template<class ElemType>
    Matrix(const initializer_list< Matrix<ElemType> >& X);

	template<class ElemType>
	Matrix(const initializer_list<ElemType>& x);

    template<class ElemType>
	Matrix(const ElemType& x);
	~Matrix();

	void clear();
	bool empty()const;
	int rows()const;
	int cols()const;
	int size()const;
	DataType* data()const;

	int rank()const;
	DataType trace()const;
	DataType det()const;
	DataType max()const;
	DataType min()const;
    void print_size()const;

	Matrix<DataType> t()const;
	Matrix<DataType> inv()const;
	Matrix<DataType>& reshape(int m, int n);
	Matrix<DataType> repmat(int m, int n)const;
	Matrix<DataType> repelem(int m, int n)const;
	Matrix<DataType> submat(int row_start, int row_end, int col_start, int col_end)const;
	Matrix<DataType> row(int i)const;
	Matrix<DataType> col(int j)const;

	Matrix<DataType>& operator =(const Matrix<DataType>& A);
	Matrix<DataType>& operator =(Matrix<DataType>&& A);

	template<class ElemType>
    Matrix<DataType>& operator =(const Matrix<ElemType>& A);

	template<class ElemType>
	operator Matrix<ElemType>()const;

    DataType& operator ()(int i);
    DataType& operator [](int i);
    DataType operator ()(int i)const;
    DataType operator [](int i)const;
    DataType& operator ()(int i, int j);
    DataType operator ()(int i, int j)const;

	Matrix<DataType>  operator -()const;

	Matrix<DataType>& operator +=(const Matrix<DataType>& A);
	Matrix<DataType>& operator -=(const Matrix<DataType>& A);
	Matrix<DataType>& operator *=(const Matrix<DataType>& A); // 右乘
	Matrix<DataType>& operator %=(const Matrix<DataType>& A);
	Matrix<DataType>& operator /=(const Matrix<DataType>& A);
	Matrix<DataType>& operator ^=(const Matrix<DataType>& A);

	Matrix<DataType>& operator +=(const DataType& a);
	Matrix<DataType>& operator -=(const DataType& a);
	Matrix<DataType>& operator *=(const DataType& a);
	Matrix<DataType>& operator /=(const DataType& a);
	Matrix<DataType>& operator ^=(const DataType& a);

	template<class ElemType>
	friend ostream& operator <<(ostream& o, Matrix<ElemType> A);

	template<class ElemType>
	Matrix<ElemType> friend operator +(const Matrix<ElemType>& A, const Matrix<ElemType>& B);

	template<class ElemType>
	Matrix<ElemType> friend operator -(const Matrix<ElemType>& A, const Matrix<ElemType>& B);

	template<class ElemType>
	Matrix<ElemType> friend operator *(const Matrix<ElemType>& A, const Matrix<ElemType>& B);

	template<class ElemType>
	Matrix<ElemType> friend operator %(const Matrix<ElemType>& A, const Matrix<ElemType>& B);

	template<class ElemType>
	Matrix<ElemType> friend operator /(const Matrix<ElemType>& A, const Matrix<ElemType>& B);

	template<class ElemType>
	Matrix<ElemType> friend operator ^(const Matrix<ElemType>& A, const Matrix<ElemType>& B);

	template<class ElemType>
	Matrix<ElemType> friend operator +(const Matrix<ElemType>& A, const ElemType& b);

	template<class ElemType>
	Matrix<ElemType> friend operator -(const Matrix<ElemType>& A, const ElemType& b);

	template<class ElemType>
	Matrix<ElemType> friend operator *(const Matrix<ElemType>& A, const ElemType& b);

	template<class ElemType>
	Matrix<ElemType> friend operator %(const Matrix<ElemType>& A, const ElemType& b);

	template<class ElemType>
	Matrix<ElemType> friend operator /(const Matrix<ElemType>& A, const ElemType& b);

	template<class ElemType>
	Matrix<ElemType> friend operator ^(const Matrix<ElemType>& A, const ElemType& b);

	template<class ElemType>
	Matrix<ElemType> friend operator +(const ElemType& a, const Matrix<ElemType>& B);

	template<class ElemType>
	Matrix<ElemType> friend operator -(const ElemType& a, const Matrix<ElemType>& B);

	template<class ElemType>
	Matrix<ElemType> friend operator *(const ElemType& a, const Matrix<ElemType>& B);

	template<class ElemType>
	Matrix<ElemType> friend operator %(const ElemType& a, const Matrix<ElemType>& B);

	template<class ElemType>
	Matrix<ElemType> friend operator /(const ElemType& a, const Matrix<ElemType>& B);

	template<class ElemType>
	Matrix<ElemType> friend operator ^(const ElemType& a, const Matrix<ElemType>& B);

	template<class ElemType>
	Matrix<bool> friend operator >(const Matrix<ElemType>& A, const Matrix<ElemType>& B);

	template<class ElemType>
	Matrix<bool> friend operator <(const Matrix<ElemType>& A, const Matrix<ElemType>& B);

	template<class ElemType>
	Matrix<bool> friend operator >=(const Matrix<ElemType>& A, const Matrix<ElemType>& B);

	template<class ElemType>
	Matrix<bool> friend operator <=(const Matrix<ElemType>& A, const Matrix<ElemType>& B);

	template<class ElemType>
	Matrix<bool> friend operator ==(const Matrix<ElemType>& A, const Matrix<ElemType>& B);

	template<class ElemType>
	Matrix<bool> friend operator !=(const Matrix<ElemType>& A, const Matrix<ElemType>& B);

	template<class ElemType>

	Matrix<bool> friend operator >(const Matrix<ElemType>& A, const ElemType& b);

	template<class ElemType>
	Matrix<bool> friend operator <(const Matrix<ElemType>& A, const ElemType& b);

	template<class ElemType>
	Matrix<bool> friend operator >=(const Matrix<ElemType>& A, const ElemType& b);

	template<class ElemType>
	Matrix<bool> friend operator <=(const Matrix<ElemType>& A, const ElemType& b);

	template<class ElemType>
	Matrix<bool> friend operator ==(const Matrix<ElemType>& A, const ElemType& b);

	template<class ElemType>
	Matrix<bool> friend operator !=(const Matrix<ElemType>& A, const ElemType& b);

	template<class ElemType>
	Matrix<bool> friend operator >(const ElemType& b, const Matrix<ElemType>& A);

	template<class ElemType>
	Matrix<bool> friend operator <(const ElemType& b, const Matrix<ElemType>& A);

	template<class ElemType>
	Matrix<bool> friend operator >=(const ElemType& b, const Matrix<ElemType>& A);

	template<class ElemType>
	Matrix<bool> friend operator <=(const ElemType& b, const Matrix<ElemType>& A);

	template<class ElemType>
	Matrix<bool> friend operator ==(const ElemType& b, const Matrix<ElemType>& A);

	template<class ElemType>
	Matrix<bool> friend operator !=(const ElemType& b, const Matrix<ElemType>& A);

	friend float norm(const Matrix<float>& v);
	friend float dot(const Matrix<float>& v1, const Matrix<float>& v2);
	friend Matrix<float> cross(const Matrix<float>& v1, const Matrix<float>& v2);
	friend Matrix<float> sin(const Matrix<float>& A);
	friend Matrix<float> cos(const Matrix<float>& A);
	friend Matrix<float> tan(const Matrix<float>& A);
	friend Matrix<float> asin(const Matrix<float>& A);
	friend Matrix<float> acos(const Matrix<float>& A);
	friend Matrix<float> atan(const Matrix<float>& A);
	friend Matrix<float> atan2(const Matrix<float>& Y, const Matrix<float>& X);
	friend Matrix<float> sinh(const Matrix<float>& A);
	friend Matrix<float> cosh(const Matrix<float>& A);
	friend Matrix<float> tanh(const Matrix<float>& A);
	friend Matrix<float> asinh(const Matrix<float>& A);
	friend Matrix<float> acosh(const Matrix<float>& A);
	friend Matrix<float> atanh(const Matrix<float>& A);
	friend Matrix<float> exp(const Matrix<float>& A);
	friend Matrix<float> log(const Matrix<float>& A);
	friend Matrix<float> log2(const Matrix<float>& A);
	friend Matrix<float> log10(const Matrix<float>& A);
	friend Matrix<float> abs(const Matrix<float>& A);
	friend Matrix<int> floor(const Matrix<float>& A);
	friend Matrix<int> ceil(const Matrix<float>& A);
	friend Matrix<int> round(const Matrix<float>& A);
	friend Matrix<float> inv(Matrix<float> A);
	friend Matrix<float> pow(const Matrix<float>& A, double n);

    template<class ElemData>
    friend void meshgrid(Matrix<ElemData>& x, Matrix<ElemData>& y);
};

Matrix<float> linspace(float start, float end, int n);

template<class ElemType>
Matrix<float> zeros(const Matrix<ElemType>& A);
Matrix<float> zeros(int rows, int cols);
Matrix<float> zeros(int n);

template<class ElemType>
Matrix<float> ones(const Matrix<ElemType>& A);
Matrix<float> ones(int rows, int cols);
Matrix<float> ones(int n);

template<class ElemType>
Matrix<float> eye(const Matrix<ElemType>& A);
Matrix<float> eye(int rows, int cols);
Matrix<float> eye(int n);

template<class ElemType>
Matrix<float> rand(const Matrix<ElemType>& A);
Matrix<float> rand(int rows, int cols);
Matrix<float> rand(int n);
Matrix<float> cross(const Matrix<float>& vec);

Matrix<float> normalize(const Matrix<float>& A);

template<class DataType>
Matrix<DataType> reshape(const Matrix<DataType>& A, int m, int n);

template<class DataType>
Matrix<DataType> repmat(const Matrix<DataType>& A, int m, int n);

template<class DataType>
Matrix<DataType> repelem(const Matrix<DataType>& A, int m, int n);

template<class DataType>
Matrix<DataType> hstack(const Matrix<DataType>& A);

template<class DataType>
Matrix<DataType> hstack(const Matrix<DataType>& A, const Matrix<DataType>& B);

template<class DataType, class ...Matrices>
Matrix<DataType> hstack(const Matrix<DataType>& first, Matrices... rear);

template<class DataType>
Matrix<DataType> hstack(initializer_list< Matrix<DataType> >& mats);

template<class DataType>
Matrix<DataType> vstack(const Matrix<DataType>& A);

template<class DataType>
Matrix<DataType> vstack(const Matrix<DataType>& A, const Matrix<DataType>& B);

template<class DataType, class ...Matrices>
Matrix<DataType> vstack(const Matrix<DataType>& first, Matrices... rear);

template<class DataType>
Matrix<DataType> vstack(initializer_list< Matrix<DataType> >& mats);

template<class DataType>
bool same_size(const Matrix<DataType>& A, const Matrix<DataType>& B);

Matrix<int> range(int start, int end = INT_MAX, int step = INT_MAX);
Matrix<float> sphere2xyz(const Matrix<float>& sphere_coord);

template<class DataType>
Matrix<DataType> interp1(const Matrix<float>& x, const Matrix<float>& v, const Matrix<float>& xi);

template<class DataType>
Matrix<DataType> interp2(const Matrix<float>& x, const Matrix<float>& y, const Matrix<DataType>& v,
						  const Matrix<float>& xi, const Matrix<float>& yi);

// ****************************************************************
// Implementation of all templates functions
// ****************************************************************

template<class DataType>
Matrix<DataType>::Matrix(int rows, int cols)
{
	n_rows = rows;
	n_cols = cols;
	_size = rows * cols;
	_data = new DataType[rows * cols];
}

template<class DataType>
void Matrix<DataType>::print_size()const
{
    cout << n_rows << "x" << n_cols << " ";
}

template<class DataType>
template<class ElemType>
Matrix<DataType>::Matrix(int rows, int cols, const ElemType& value)
{
	n_rows = rows;
	n_cols = cols;
	_size = rows * cols;
	_data = new DataType[_size];
	for(int i = 0; i < _size; i++)
	{
		_data[i] = (DataType)value;
	}
}

template<class DataType>
template<class ElemType>
Matrix<DataType>::Matrix(int rows, int cols, const ElemType* values)
{
	n_rows = rows;
	n_cols = cols;
	_size = rows * cols;
	_data = new DataType[_size];
	for(int i = 0; i < _size; i++)
	{
		_data[i] = (DataType)(values[i]);
	}
}

template<class DataType>
Matrix<DataType>::Matrix(const Matrix<DataType>& A)
{
	n_rows = A.n_rows;
	n_cols = A.n_cols;
	_size = A._size;
	_data = new DataType[_size];
	for(int i = 0; i < _size; i++)
	{
		_data[i] = A._data[i];
	}
}

template<class DataType>
template<class ElemType>
Matrix<DataType>::Matrix(const Matrix<ElemType>& A)
{
	n_rows = A.rows();
	n_cols = A.cols();
	_size = n_rows * n_cols;
	_data = new DataType[_size];
	for(int i = 0; i < _size; i++)
	{
		_data[i] = (DataType)(A(i));
		// _data[i] = (DataType)(A._data[i]);
	}
}

template<class DataType>
Matrix<DataType>::Matrix(Matrix<DataType>&& A)
{
	n_rows = A.n_rows;
	n_cols = A.n_cols;
	_size = A._size;
	_data = A._data;

	A.n_rows = 0;
	A.n_cols = 0;
	A._size = 0;
	A._data = NULL;
}

template<class DataType>
template<class ElemType>
Matrix<DataType>::Matrix(const vector<ElemType>& x)
{
	n_rows = x.size();
	if(!n_rows)
	{
		return;
	}
	n_cols = 1;
	_size = n_rows;
	_data = new DataType[_size];
	for(int i = 0; i < _size; i++)
	{
		_data[i] = (DataType)(x[i]);
	}
}

template<class DataType>
template<class ElemType>
Matrix<DataType>::Matrix(const vector< vector<ElemType> >& X)
{
    n_rows = (int)(X.size());
	if(n_rows == 0)
	{
		return;
	}

    n_cols = (int)(X[0].size());
    _size = n_rows * n_cols;
	_data = new DataType[_size];
	for(int i = 0; i < n_rows; i++)
	{
        if(X[i].size() != (unsigned int)n_cols)
		{
			cout << "Warning in \'Matrix<DataType>::Matrix<DataType>(const vector< vector<float> >& X)\'" << endl
				 << "Each vector<DataType> doesn't has the same size." << endl;
			clear();
			return;
		}
		for(int j = 0; j < n_cols; j++)
		{
			_data[i + j * n_rows] = (DataType)(X[i][j]);
		}
	}
}

template<class DataType>
template<class ElemType>
Matrix<DataType>::Matrix(const initializer_list<ElemType>& x)
{
    n_rows = (int)(x.size());
	if(!n_rows)
	{
		return;
	}
	n_cols = 1;
	_size = n_rows;
	_data = new DataType[_size];
	typename initializer_list<ElemType>::const_iterator it = x.begin();
	for(int i = 0; i < _size; i++, it++)
	{
		_data[i] = (DataType)(*it);
	}
}

template<class DataType>
template<class ElemType>
Matrix<DataType>::Matrix(const initializer_list< initializer_list<ElemType> >& X)
{
    n_rows = (int)(X.size());
	if(n_rows == 0)
	{
		return;
	}

	typename initializer_list< initializer_list<ElemType> >::const_iterator it = X.begin();
    n_cols = (int)(it->size());
    _size = n_rows * n_cols;
	_data = new DataType[n_rows * n_cols];
	for(int i = 0; i < n_rows; i++, it++)
	{
        if(it->size() != (unsigned int)n_cols)
		{
			cout << "Warning in \'Matrix<DataType>::Matrix<DataType>(const initializer_list< initializer_list<ElemType> >& X)\'" << endl
				 << "Each initializer_list<ElemType> doesn't has the same size." << endl;
			clear();
			return;
		}
		typename initializer_list<ElemType>::iterator subit = it->begin();
		for(int j = 0; j < n_cols; j++, subit++)
		{
			_data[i + j * n_rows] = (DataType)(*subit);
		}
	}
}

template<class DataType>
template<class ElemType>
Matrix<DataType>::Matrix(const initializer_list< Matrix<ElemType> >& X)
{
    int sum_cols = 0;
    int common_rows = X.begin()->n_rows;
    for(auto it = X.begin(); it != X.end(); it++)
    {
        sum_cols += it->n_cols;
        if(it->n_rows != common_rows)
        {
            cout << "Error when using initializer_list< Matrix<DataType> > to initialize a Matrix!" << endl
                 << "Each Matrix must have the same size!" << endl;
            exit(-1);
        }
    }

    n_rows = common_rows;
    n_cols = sum_cols;
    _size = n_rows * n_cols;
    _data = new DataType[_size];

    DataType* ptr_data = _data;
    for(auto it = X.begin(); it != X.end(); it++)
    {
        int it_size = it->size();
        for(int i = 0; i < it_size; i++)
        {
            *ptr_data = (DataType)(it->_data[i]);
            ptr_data++;
        }
    }
}

template<class DataType>
template<class ElemType>
Matrix<DataType>::Matrix(const ElemType& x)
{
	n_rows = 1;
	n_cols = 1;
	_size = 1;
	_data = new DataType[1];
	_data[0] = (DataType)x;
}

template<class DataType>
Matrix<DataType>::~Matrix()
{
	n_rows = 0;
	n_cols = 0;
	_size = 0;
	if(_data)
	{
		delete [] _data;
		_data = NULL;
	}
}

template<class DataType>
void Matrix<DataType>::clear()
{
	n_rows = 0;
	n_cols = 0;
	_size = 0;
	if(_data)
	{
		delete [] _data;
		_data = NULL;
	}
}

template<class DataType>
Matrix<DataType>& Matrix<DataType>::operator =(const Matrix<DataType>& A)
{
	if(&A == this)
	{
		return *this;
	}

	if(n_rows != A.n_rows || n_cols != A.n_cols)
	{
		if(_size != A._size)
		{
			_size = A._size;
			delete [] _data;
			_data = new DataType[_size];
		}
		n_rows = A.n_rows;
		n_cols = A.n_cols;
	}

	for(int i = 0; i < _size; i++)
	{
		_data[i] = A._data[i];
	}

	return *this;
}

template<class DataType>
Matrix<DataType>& Matrix<DataType>::operator =(Matrix<DataType>&& A)
{
	if(&A == this)
	{
		return *this;
	}

	n_rows = A.n_rows;
	n_cols = A.n_cols;
	_size = A._size;
	_data = A._data;

	A.n_rows = 0;
	A.n_cols = 0;
	A._size = 0;
	A._data = NULL;

	return *this;
}

template<class DataType>
template<class ElemType>
Matrix<DataType>& Matrix<DataType>::operator =(const Matrix<ElemType>& A)
{
    if(n_rows != A.rows() || n_cols != A.cols())
	{
        if(_size != A.size())
		{
            _size = A.size();
			delete [] _data;
			_data = new DataType[_size];
		}
        n_rows = A.rows();
        n_cols = A.cols();
	}

	for(int i = 0; i < _size; i++)
	{
        _data[i] = (DataType)(A.data()[i]);
	}

	return *this;
}

template<class DataType>
template<class ElemType>
Matrix<DataType>::operator Matrix<ElemType>()const
{
	Matrix<ElemType> A(n_rows, n_cols);
	for(int i = 0; i < _size; i++)
	{
		A._data[i] = (ElemType)(_data[i]);
	}

	return A;
}

template<class DataType>
DataType& Matrix<DataType>::operator ()(int i)
{
    if(i < 0 || i >= _size)
    {
        cout << "Error using Matrix<DataType>(i):" << endl
             << "index is out of bande!" << endl;
        exit(-1);
    }

    return _data[i];
}

template<class DataType>
DataType Matrix<DataType>::operator ()(int i)const
{
    if(i < 0 || i >= _size)
    {
        cout << "Error using Matrix<DataType>(i):" << endl
             << "index is out of bande!" << endl;
        exit(-1);
    }

    return _data[i];
}

template<class DataType>
DataType& Matrix<DataType>::operator [](int i)
{
    if(i < 0 || i >= _size)
    {
        cout << "Error using Matrix<DataType>(i):" << endl
             << "index is out of bande!" << endl;
        exit(-1);
    }

    return _data[i];
}

template<class DataType>
DataType Matrix<DataType>::operator [](int i)const
{
    if(i < 0 || i >= _size)
    {
        cout << "Error using Matrix<DataType>(i):" << endl
             << "index is out of bande!" << endl;
        exit(-1);
    }

    return _data[i];
}

template<class DataType>
DataType& Matrix<DataType>::operator ()(int i, int j)
{
	if(i < 0 || i >= n_rows || j < 0 || j >= n_cols)
	{
		cout << "Error using Matrix<DataType>(i, j):" << endl
			 << "index is out of bande!" << endl;
		exit(-1);
	}

	return _data[i + j * n_rows];
}

template<class DataType>
DataType Matrix<DataType>::operator ()(int i, int j)const
{
	if(i < 0 || i >= n_rows || j < 0 || j >= n_cols)
	{
		cout << "Error using Matrix<DataType>(i, j):" << endl
			 << "index is out of bande!" << endl;
		exit(-1);
	}

	return _data[i + j * n_rows];
}

template<class DataType>
int Matrix<DataType>::rows()const
{
	return n_rows;
}

template<class DataType>
int Matrix<DataType>::cols()const
{
	return n_cols;
}

template<class DataType>
int Matrix<DataType>::size()const
{
	return _size;
}

template<class DataType>
DataType* Matrix<DataType>::data()const
{
	return _data;
}

template<class DataType>
bool Matrix<DataType>::empty()const
{
	return _size == 0;
}

template<class DataType>
DataType Matrix<DataType>::trace()const
{
	if(empty())
	{
		cout << "Error using \'float trace()const\':" << endl
			 << "Matrix<DataType> is empty!" << endl;
		exit(-1);
	}
	if(n_rows != n_cols)
	{
		cout << "Error using \'float trace()const\':" << endl
			 << "Matrix<DataType> must be square!" << endl;
		exit(-1);
	}

	DataType tr = _data[0];
	int k = n_rows + 1;
	for(int i = 1; i < n_rows; i++)
	{
		tr += _data[i * k];
	}

	return tr;
}

template<class DataType>
Matrix<DataType> Matrix<DataType>::t()const
{
	Matrix<DataType> A(n_cols, n_rows);
	for(int i = 0; i < A.n_rows; i++)
	{
		for(int j = 0; j < A.n_cols; j++)
		{
			A(i, j) = (*this)(j, i);
		}
	}

	return A;
}

template<class DataType>
Matrix<DataType>& Matrix<DataType>::reshape(int m, int n)
{
	if(m == AUTO)
	{
		m = _size / n;
	}
	if(n == AUTO)
	{
		n = _size / m;
	}

	if(m * n != _size)
	{
		cout << "Error using \'A.reshape(m, n)\'" << endl
			 << "m * n != A.n_rows * A.n_cols" << endl;
		exit(-1);
	}

	n_rows = m;
	n_cols = n;

	return *this;
}

template<class DataType>
Matrix<DataType> Matrix<DataType>::repmat(int m, int n)const
{
	if(m == 1 && n == 1)
	{
		return *this;
	}

	Matrix<DataType> B(m * n_rows, n * n_cols);
	for(int i = 0; i < B.n_rows; i++)
	{
		for(int j = 0; j < B.n_cols; j++)
		{
			B(i, j) = (*this)(i % n_rows, j % n_cols);
		}
	}

	return B;
}

template<class DataType>
Matrix<DataType> Matrix<DataType>::repelem(int m, int n)const
{
	if(m == 1 && n == 1)
	{
		return (*this);
	}

	Matrix<DataType> B(m * n_rows, n * n_cols);
	for(int i = 0; i < B.n_rows; i++)
	{
		for(int j = 0; j < B.n_cols; j++)
		{
			B(i, j) = (*this)(i / m, j / n);
		}
	}

	return B;
}

template<class DataType>
Matrix<DataType> Matrix<DataType>::submat(int row_start, int row_end, int col_start, int col_end)const
{
	if(row_start < 0) row_start = 0;
	if(row_start > n_rows) row_start = n_rows;
	if(row_end < 0) row_end = 0;
	if(row_end > n_rows) row_end = n_rows;
	if(row_start > row_end) std::swap(row_start, row_end);

	if(col_start < 0) col_start = 0;
	if(col_start > n_cols) col_start = n_cols;
	if(col_end < 0) col_end = 0;
	if(col_end > n_cols) col_end = n_cols;
	if(col_start > col_end) std::swap(col_start, col_end);

	Matrix<DataType> result(row_end-row_start, col_end-col_start);
	for(int i = 0; i < result.n_rows; i++)
	{
		for(int j = 0; j < result.n_cols; j++)
		{
			result(i, j) = (*this)(row_start+i, col_start+j);
		}
	}
	return result;
}

template<class DataType>
Matrix<DataType> Matrix<DataType>::row(int i)const
{
	if(i < 0 || i >= n_rows)
	{
		return Matrix<DataType>(0, n_cols);
	}
	Matrix<DataType> result(1, n_cols);
	for(int j = 0; j < n_cols; j++)
	{
		result._data[j] = _data[i+j*n_rows];
	}
	return result;
}

template<class DataType>
Matrix<DataType> Matrix<DataType>::col(int j)const
{
	if(j < 0 || j >= n_cols)
	{
		return Matrix<DataType>(n_rows, 0);
	}
	Matrix<DataType> result(n_rows, 1);
	for(int i = 0; i < n_rows; i++)
	{
		result._data[i] = _data[i+j*n_rows];
	}
	return result;
}

template<class DataType>
Matrix<DataType> vstack(const Matrix<DataType>& A)
{
	return A;
}

template<class DataType>
Matrix<DataType> vstack(const Matrix<DataType>& A, const Matrix<DataType>& B)
{
	if(A.cols() != B.cols())
	{
		cout << "Error using vstack(Matrix A, Matrix B):" << endl
		     << "Matrix A and B must have the same columns!" << endl;
		exit(-1);
	}
	Matrix<DataType> C(A.rows()+B.rows(), A.cols());
	for(int i = 0; i < A.rows(); i++)
	{
		for(int j = 0; j < A.cols(); j++)
		{
			C(i, j) = A(i, j);
		}
	}
	for(int i = 0; i < B.rows(); i++)
	{
		for(int j = 0; j < B.cols(); j++)
		{
			C(i+A.rows(), j) = B(i, j);
		}
	}
	return C;
}

template<class DataType, class ...Matrices>
Matrix<DataType> vstack(const Matrix<DataType>& first, Matrices... rear)
{
	return vstack(first, vstack(rear...));
}

template<class DataType, class ...Matrices>
Matrix<DataType> vstack(const initializer_list< Matrix<DataType> >& mats)
{
	int row_start = 0;
	int common_cols = mats.begin()->cols();
	Matrix<DataType> result;
	for(auto it = mats.begin(); it != mats.end(); it++)
	{
		if(it->cols() != common_cols)
		{
			cout << "Error using vstack(Matrices... mats):" << endl
			     << "All mats must have the same columns!" << endl;
			exit(-1);
		}
		for(int i = 0; i < it->rows(); i++)
		{
			for(int j = 0; j < it->cols(); j++)
			{
				result(i+row_start, j) = (*it)(i, j);
			}
		}
		row_start += it->rows();
	}

	return result;
}

template<class DataType>
Matrix<DataType> hstack(const Matrix<DataType>& A)
{
	return A;
}

template<class DataType>
Matrix<DataType> hstack(const Matrix<DataType>& A, const Matrix<DataType>& B)
{
	if(A.rows() != B.rows())
	{
		cout << "Error using hstack(Matrix A, Matrix B):" << endl
		     << "Matrix A and B must have the same rows!" << endl;
		exit(-1);
	}
	Matrix<DataType> C(A.rows(), A.cols()+B.cols());
	for(int i = 0; i < A.rows(); i++)
	{
		for(int j = 0; j < A.cols(); j++)
		{
			C(i, j) = A(i, j);
		}
	}
	for(int i = 0; i < B.rows(); i++)
	{
		for(int j = 0; j < B.cols(); j++)
		{
			C(i, j+A.cols()) = B(i, j);
		}
	}
	return C;
}

template<class DataType, class ...Matrices>
Matrix<DataType> hstack(const Matrix<DataType>& first, Matrices... rear)
{
	return hstack(first, hstack(rear...));
}

template<class DataType, class ...Matrices>
Matrix<DataType> hstack(const initializer_list< Matrix<DataType> >& mats)
{
	int col_start = 0;
	int common_rows = mats.start()->rows();
	Matrix<DataType> result;
	for(auto it = mats.start(); it != mats.end(); it++)
	{
		if(it->rows() != common_rows)
		{
			cout << "Error using hstack(Matrices... mats):" << endl
			     << "All mats must have the same rows!" << endl;
			exit(-1);
		}
		for(int i = 0; i < it->rows(); i++)
		{
			for(int j = 0; j < it->cols(); j++)
			{
				result(i, j+col_start) = (*it)(i, j);
			}
		}
		col_start += it->cols();
	}

	return result;
}

template<class DataType>
DataType Matrix<DataType>::max()const
{
	DataType Max = _data[0];
	for(int i = 0; i < _size; i++)
	{
		if( _data[i] > Max )
		{
			Max = _data[i];
		}
	}

	return Max;
}

template<class DataType>
DataType Matrix<DataType>::min()const
{
	DataType Min = _data[0];
	for(int i = 0; i < _size; i++)
	{
		if( _data[i] < Min )
		{
			Min = _data[i];
		}
	}

	return Min;
}

template<class DataType>
Matrix<DataType>& Matrix<DataType>::operator +=(const Matrix<DataType>& A)
{
	if(n_rows != A.n_rows || n_cols != A.n_cols)
	{
		cout << "Error using +=, two matrix must have the same size!" << endl;
		exit(-1);
	}

	for(int i = 0; i < _size; i++)
	{
		_data[i] += A._data[i];
	}

	return *this;
}

template<class DataType>
Matrix<DataType>& Matrix<DataType>::operator -=(const Matrix<DataType>& A)
{
	if(n_rows != A.n_rows || n_cols != A.n_cols)
	{
		cout << "Error using -=, two matrix must have the same size!" << endl;
		exit(-1);
	}

	for(int i = 0; i < _size; i++)
	{
		_data[i] -= A._data[i];
	}

	return *this;
}

template<class DataType>
Matrix<DataType>& Matrix<DataType>::operator *=(const Matrix<DataType>& A)
{
	return *this = ((*this) * A);
}

template<class DataType>
Matrix<DataType>& Matrix<DataType>::operator %=(const Matrix<DataType>& A)
{
	if(n_rows != A.n_rows || n_cols != A.n_cols)
	{
		cout << "Error using %=, two matrix must have the same size!" << endl;
		exit(-1);
	}
	
	for(int i = 0; i < _size; i++)
	{
		_data[i] *= A._data[i];
	}

	return *this;
}

template<class DataType>
Matrix<DataType>& Matrix<DataType>::operator /=(const Matrix<DataType>& A)
{
	if(n_rows != A.n_rows || n_cols != A.n_cols)
	{
		cout << "Error using /=, two matrix must have the same size!" << endl;
		exit(-1);
	}

	for(int i = 0; i < _size; i++)
	{
		_data[i] /= A._data[i];
	}

	return *this;
}

template<class DataType>
Matrix<DataType>& Matrix<DataType>::operator ^=(const Matrix<DataType>& A)
{
	return *this = (*this) ^ A;
}

template<class DataType>
Matrix<DataType>& Matrix<DataType>::operator +=(const DataType& a)
{
	for(int i = 0; i < _size; i++)
	{
		_data[i] += a;
	}

	return *this;
}

template<class DataType>
Matrix<DataType>& Matrix<DataType>::operator -=(const DataType& a)
{
	for(int i = 0; i < _size; i++)
	{
		_data[i] -= a;
	}

	return *this;
}

template<class DataType>
Matrix<DataType>& Matrix<DataType>::operator *=(const DataType& a)
{
	for(int i = 0; i < _size; i++)
	{
		_data[i] *= a;
	}

	return *this;
}

template<class DataType>
Matrix<DataType>& Matrix<DataType>::operator /=(const DataType& a)
{
	for(int i = 0; i < _size; i++)
	{
		_data[i] /= a;
	}

	return *this;
}

template<class DataType>
Matrix<DataType>& Matrix<DataType>::operator ^=(const DataType& a)
{
	for(int i = 0; i < _size; i++)
	{
		_data[i] = pow(_data[i], a);
	}

	return *this;
}

template<class DataType>
Matrix<DataType> Matrix<DataType>::operator -()const
{
	Matrix<DataType> A(n_rows, n_cols);
	for(int i = 0; i < _size; i++)
	{
		A._data[i] = -_data[i];
	}

	return A;
}

template<class DataType>
Matrix<DataType> operator +(const Matrix<DataType>& A, const Matrix<DataType>& B)
{
	if(!same_size(A, B))
	{
		cout << "Error using \'A + B\':" << endl
			 << "Matrix<DataType> dimension is not match!" << endl;
		exit(-1);
	}

	Matrix<DataType> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = A._data[i] + B._data[i];
	}

	return C;
}

template<class DataType>
Matrix<DataType> operator -(const Matrix<DataType>& A, const Matrix<DataType>& B)
{
	if(!same_size(A, B))
	{
		cout << "Error using \'A - B\':" << endl
			 << "Matrix<DataType> dimension is not match!" << endl;
		exit(-1);
	}

	Matrix<DataType> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = A._data[i] - B._data[i];
	}

	return C;
}

template<class DataType>
Matrix<DataType> operator *(const Matrix<DataType>& A, const Matrix<DataType>& B)
{
	if(A.n_cols != B.n_rows)
	{
		cout << "Error using \'A * B\':" << endl
			 << "Matrix<DataType> dimension is not match!" << endl;
		exit(-1);
	}

	Matrix<DataType> C(A.n_rows, B.n_cols);
	for(int i = 0; i < C.n_rows; i++)
	{
		for(int j = 0; j < C.n_cols; j++)
		{
			float S = 0.0f;
			for(int k = 0; k < A.n_cols; k++)
			{
				S += A(i, k) * B(k, j);
			}
			C(i, j) = S;
		}
	}

	return C;
}

template<class DataType>
Matrix<DataType> operator %(const Matrix<DataType>& A, const Matrix<DataType>& B)
{
	if(!same_size(A, B))
	{
		cout << "Error using \'A % B\':" << endl
			 << "Matrix<DataType> dimension is not match!" << endl;
		exit(-1);
	}

	Matrix<DataType> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = A._data[i] * B._data[i];
	}

	return C;
}

template<class DataType>
Matrix<DataType> operator ^(const Matrix<DataType>& A, const Matrix<DataType>& B)
{
	if(!same_size(A, B))
	{
		cout << "Error using \'A ^ B\':" << endl
			 << "Matrix<DataType> dimension is not match!" << endl;
		exit(-1);
	}

	Matrix<DataType> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = pow(A._data[i], B._data[i]);
	}

	return C;
}

template<class DataType>
Matrix<DataType> operator +(const Matrix<DataType>& A, const DataType& b)
{
	Matrix<DataType> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = A._data[i] + b;
	}

	return C;
}

template<class DataType>
Matrix<DataType> operator -(const Matrix<DataType>& A, const DataType& b)
{
	Matrix<DataType> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = A._data[i] - b;
	}

	return C;
}

template<class DataType>
Matrix<DataType> operator *(const Matrix<DataType>& A, const DataType& b)
{
	Matrix<DataType> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = A._data[i] * b;
	}

	return C;
}

template<class DataType>
Matrix<DataType> operator %(const Matrix<DataType>& A, const DataType& b)
{
	Matrix<DataType> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = A._data[i] * b;
	}

	return C;
}

template<class DataType>
Matrix<DataType> operator /(const Matrix<DataType>& A, const DataType& b)
{
	Matrix<DataType> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = A._data[i] / b;
	}

	return C;
}

template<class DataType>
Matrix<DataType> operator ^(const Matrix<DataType>& A, const DataType& b)
{
	Matrix<DataType> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = pow(A._data[i], b);
	}

	return C;
}

template<class DataType>
Matrix<DataType> operator +(const DataType& a, const Matrix<DataType>& B)
{
	Matrix<DataType> C(B.n_rows, B.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = a + B._data[i];
	}

	return C;
}

template<class DataType>
Matrix<DataType> operator -(const DataType& a, const Matrix<DataType>& B)
{
	Matrix<DataType> C(B.n_rows, B.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = a - B._data[i];
	}

	return C;
}

template<class DataType>
Matrix<DataType> operator *(const DataType& a, const Matrix<DataType>& B)
{
	Matrix<DataType> C(B.n_rows, B.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = a * B._data[i];
	}

	return C;
}

template<class DataType>
Matrix<DataType> operator %(const DataType& a, const Matrix<DataType>& B)
{
	Matrix<DataType> C(B.n_rows, B.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = a * B._data[i];
	}

	return C;
}

template<class DataType>
Matrix<DataType> operator /(const DataType& a, const Matrix<DataType>& B)
{
	Matrix<DataType> C(B.n_rows, B.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = a / B._data[i];
	}

	return C;
}

template<class DataType>
Matrix<DataType> operator ^(const DataType& a, const Matrix<DataType>& B)
{
	Matrix<DataType> C(B.n_rows, B.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = pow(a, B._data[i]);
	}

	return C;
}

template<class DataType>
Matrix<bool> operator >(const Matrix<DataType>& A, const Matrix<DataType>& B)
{
	if(!same_size(A, B))
	{
		cout << "Error using \'A > B\'" << endl
			 << "Matrix<DataType> dimension is not match!" << endl;
		exit(-1);
	}

	Matrix<bool> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = (A._data[i] > B._data[i]);
	}

	return C;
}

template<class DataType>
Matrix<bool> operator <(const Matrix<DataType>& A, const Matrix<DataType>& B)
{
	if(!same_size(A, B))
	{
		cout << "Error using \'A < B\'" << endl
			 << "Matrix<DataType> dimension is not match!" << endl;
		exit(-1);
	}

	Matrix<bool> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = (A._data[i] < B._data[i]);
	}

	return C;
}

template<class DataType>
Matrix<bool> operator >=(const Matrix<DataType>& A, const Matrix<DataType>& B)
{
	if(!same_size(A, B))
	{
		cout << "Error using \'A >= B\'" << endl
			 << "Matrix<DataType> dimension is not match!" << endl;
		exit(-1);
	}

	Matrix<bool> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = (A._data[i] >= B._data[i]);
	}

	return C;
}

template<class DataType>
Matrix<bool> operator <=(const Matrix<DataType>& A, const Matrix<DataType>& B)
{
	if(!same_size(A, B))
	{
		cout << "Error using \'A <= B\'" << endl
			 << "Matrix<DataType> dimension is not match!" << endl;
		exit(-1);
	}

	Matrix<bool> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = (A._data[i] <= B._data[i]);
	}

	return C;
}

template<class DataType>
Matrix<bool> operator ==(const Matrix<DataType>& A, const Matrix<DataType>& B)
{
	if(!same_size(A, B))
	{
		cout << "Error using \'A == B\'" << endl
			 << "Matrix<DataType> dimension is not match!" << endl;
		exit(-1);
	}

	Matrix<bool> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = (A._data[i] == B._data[i]);
	}

	return C;
}

template<class DataType>
Matrix<bool> operator !=(const Matrix<DataType>& A, const Matrix<DataType>& B)
{
	if(!same_size(A, B))
	{
		cout << "Error using \'A != B\'" << endl
			 << "Matrix<DataType> dimension is not match!" << endl;
		exit(-1);
	}

	Matrix<bool> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = (A._data[i] != B._data[i]);
	}

	return C;
}

template<class DataType>
Matrix<bool> operator >(const Matrix<DataType>& A, const DataType& b)
{
	Matrix<bool> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = (A._data[i] > b);
	}

	return C;
}

template<class DataType>
Matrix<bool> operator <(const Matrix<DataType>& A, const DataType& b)
{
	Matrix<bool> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = (A._data[i] < b);
	}

	return C;
}

template<class DataType>
Matrix<bool> operator >=(const Matrix<DataType>& A, const DataType b)
{
	Matrix<bool> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = (A._data[i] >= b);
	}

	return C;
}

template<class DataType>
Matrix<bool> operator <=(const Matrix<DataType>& A, const DataType& b)
{
	Matrix<bool> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = (A._data[i] <= b);
	}

	return C;
}

template<class DataType>
Matrix<bool> operator ==(const Matrix<DataType>& A, const DataType& b)
{
	Matrix<bool> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = (A._data[i] == b);
	}

	return C;
}

template<class DataType>
Matrix<bool> operator !=(const Matrix<DataType>& A, const DataType& b)
{
	Matrix<bool> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = (A._data[i] != b);
	}

	return C;
}

template<class DataType>
Matrix<bool> operator >(const DataType& b, const Matrix<DataType>& A)
{
	Matrix<bool> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = (b > A._data[i]);
	}

	return C;
}

template<class DataType>
Matrix<bool> operator <(const DataType& b, const Matrix<DataType>& A)
{
	Matrix<bool> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = (b < A._data[i]);
	}

	return C;
}

template<class DataType>
Matrix<bool> operator >=(const DataType& b, const Matrix<DataType>& A)
{
	Matrix<bool> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = (b >= A._data[i]);
	}

	return C;
}

template<class DataType>
Matrix<bool> operator <=(const DataType& b, const Matrix<DataType>& A)
{
	Matrix<bool> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = (b <= A._data[i]);
	}

	return C;
}

template<class DataType>
Matrix<bool> operator ==(const DataType& b, const Matrix<DataType>& A)
{
	Matrix<bool> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = (b == A._data[i]);
	}

	return C;
}

template<class DataType>
Matrix<bool> operator !=(const DataType& b, const Matrix<DataType>& A)
{
	Matrix<bool> C(A.n_rows, A.n_cols);
	for(int i = 0; i < C._size; i++)
	{
		C._data[i] = (b != A._data[i]);
	}

	return C;
}

template<class DataType>
ostream & operator <<(ostream& o, Matrix<DataType> A)
{
	o << endl;
	int *longest_size = new int[A.n_cols];
    for(int j = 0; j < A.n_cols; j++)
	{
        longest_size[j] = (int)(Matrix<DataType>::cout2string(A(0, j)).size());
		for(int i = 1; i < A.n_rows; i++)
		{
            int current_size = (int)(Matrix<DataType>::cout2string(A(i, j)).size());
			if(current_size > longest_size[j])
			{
				longest_size[j] = current_size;
			}
		}
	}
	
	for(int i = 0; i < A.n_rows; i++)
	{
		for(int j = 0; j < A.n_cols; j++)
		{
            int space_length = longest_size[j] - (int)(Matrix<DataType>::cout2string(A(i, j)).size()) + 2;
			if(A(i, j) >= 0)
			{
				o << " " << A(i, j) << Matrix<DataType>::multi_space(space_length);
			}
			else
			{
				o << A(i, j) << Matrix<DataType>::multi_space(space_length+1);
			}
		}
		if(i != A.n_rows-1)
		{
			o << endl;
		}
	}
	delete[] longest_size;

	return o;
}

template<class DataType>
void meshgrid(Matrix<DataType>& x, Matrix<DataType>& y)
{
	x.reshape(1, x._size);
	y.reshape(y._size, 1);

	Matrix<DataType> temp = ones(x);
	x = ones(y) * x;
	y = y * temp;
}

template<class DataType>
Matrix<float> zeros(const Matrix<DataType>& A)
{
	return Matrix<float>(A.rows(), A.cols(), 0.0f);
}

template<class DataType>
Matrix<float> ones(const Matrix<DataType>& A)
{
	return Matrix<float>(A.rows(), A.cols(), 1.0f);
}

template<class DataType>
Matrix<float> eye(const Matrix<DataType>& A)
{
	return eye(A.rows(), A.cols());
}

template<class DataType>
Matrix<float> rand(const Matrix<DataType>& A)
{
	return rand(A.rows(), A.cols());
}

template<class DataType>
Matrix<DataType> reshape(const Matrix<DataType>& A, int m, int n)
{
	Matrix<DataType> C(A);
	return C.reshape(m, n);
}

template<class DataType>
Matrix<DataType> repmat(const Matrix<DataType>& A, int m, int n)
{
	int A_rows = A.rows();
	int A_cols = A.cols();
	int B_rows = m * A_rows;
	int B_cols = n * A_cols;
	Matrix<DataType> B(B_rows, B_cols);
	
	for(int i = 0; i < B_rows; i++)
	{
		for(int j = 0; j < B_cols; j++)
		{
			B(i, j) = A(i % A_rows, j % A_cols);
		}
	}

	return B;
}

template<class DataType>
Matrix<DataType> repelem(const Matrix<DataType>& A, int m, int n)
{
	int A_rows = A.rows();
	int A_cols = A.cols();
	int B_rows = m * A_rows;
	int B_cols = n * A_cols;
	Matrix<DataType> B(B_rows, B_cols);

	for(int i = 0; i < B_rows; i++)
	{
		for(int j = 0; j < B_cols; j++)
		{
			B(i, j) = A(i / m, j / n);
		}
	}

	return B;
}

template<class DataType>
bool same_size(const Matrix<DataType>& A, const Matrix<DataType>& B)
{
    return (A.rows() == B.rows()) && (A.cols() == B.cols());
}

template<class DataType>
string Matrix<DataType>::cout2string(const DataType& element)
{
    ostringstream oss;
    oss << element;
    return oss.str();
}

template<class DataType>
int Matrix<DataType>::sign(float a)
{
    return a >= 0 ? 1 : -1;
}

template<class DataType>
string Matrix<DataType>::multi_space(int n)
{
    string space;
    for(int i = 0; i < n; i++)
    {
        space += " ";
    }

    return space;
}

template<class DataType>
bool Matrix<DataType>::isZero(double a, double epsilon)
{
	return a < epsilon && a > -epsilon;
}

template<class DataType>
void Matrix<DataType>::swap_row(int row1, int row2)
{
	for(int j = 0; j < n_cols; j++)
	{
		std::swap(_data[row1 + j * n_rows], _data[row2 + j * n_rows]);
	}
}

template<class DataType>
void Matrix<DataType>::divide_row(int row1, const DataType& k)
{
	for(int j = 0; j < n_cols; j++)
	{
		_data[row1 + j * n_rows] /= k;
	}
}

template<class DataType>
void Matrix<DataType>::add_row(int row1, const DataType& k, int row2)
{
	for(int j = 0; j < n_cols; j++)
	{
		_data[row2 + j * n_rows] += k * _data[row1 + j * n_rows];
	}
}

// 没有使用二分查找
template<class DataType>
Matrix<DataType> interp1(const Matrix<float>& x, const Matrix<DataType>& v, const Matrix<float>& xi)
{
	int n  = x.size();
	if(v.size() != n)
	{
		cout << "Error in \"Matrix<DataType> interp1(const Matrix<float>& x, const Matrix<DataType>& v, const Matrix<float>& xi):\"" << endl
			 << "x and v must have the same size!" << endl;
		exit(-1);
	}
	if(n == 0)
	{
		cout << "Error in \"Matrix<DataType> interp1(const Matrix<float>& x, const Matrix<DataType>& v, const Matrix<float>& xi):\"" << endl
			 << "x and v cannot be empty!" << endl;
		exit(-1);
	}
	if(n == 1)
	{
		return Matrix<DataType>(xi.rows(), xi.cols(), v(0));
	}
	for(int i = 0; i < n-1; i++)
	{
		if(x(i) >= x(i+1))
		{
			cout << "Error in \"Matrix<DataType> interp1(const Matrix<float>& x, const Matrix<DataType>& v, const Matrix<float>& xi):\"" << endl
				 << "x must be strictly increasing!" << endl;
			exit(-1);
		}
	}

	Matrix<DataType> vi(xi.rows(), xi.cols());
	int ni = xi.size();
	for(int i = 0; i < ni; i++)
	{
		if(xi(i) < x(0))
		{
			vi(i) = (xi(i) - x(0))*(v(1)-v(0))/(x(1)-x(0));
			continue;
		}
		if(xi(i) > x(n-1))
		{
			vi(i) = (xi(i) - x(n-2))*(v(n-1)-v(n-2))/(x(n-1)-x(n-2));
			continue;
		}
		for(int subi = 0; subi < n-1; subi++)
		{
			if(x(subi) <= xi(i) && x(subi+1) >= xi(i))
			{
				vi(i) = (xi(i) - x(subi))*(v(subi+1)-v(subi))/(x(subi+1)-x(subi));
				break;
			}
		}
	}
}

// 没有写超出边界的情况
// 没有使用二分查找
// 没有写 x y v 元素过少的情况
template<class DataType>
Matrix<DataType> interp2(const Matrix<float>& x, const Matrix<float>& y, const Matrix<DataType>& v,
						  const Matrix<float>& xi, const Matrix<float>& yi)
{
	int n_rows = x.rows();
	int n_cols = x.cols();
	if(y.rows() != n_rows || y.cols() != n_cols || v.rows() != n_rows || v.cols() != n_cols)
	{
		cout << "Error in \"Matrix<DataType> interp2(const Matrix<float>& x,\n"
				"                                    const Matrix<float>& y,\n"
				"                                    const Matrix<DataType>& v,\n"
				"                                    const Matrix<float>& xi,\n"
				"                                    const Matrix<float>& yi)\":" << endl
			<< "x, y and v must have the same size!" << endl;
		exit(-1);
	}
	if(n_rows == 0 || n_cols == 0)
	{
		cout << "Error in \"Matrix<DataType> interp2(const Matrix<float>& x,\n"
				"                                    const Matrix<float>& y,\n"
				"                                    const Matrix<DataType>& v,\n"
				"                                    const Matrix<float>& xi,\n"
				"                                    const Matrix<float>& yi)\":" << endl
			<< "x, y and v cannot be empty!" << endl;
		exit(-1);
	}

	for(int j = 0; j < n_cols-1; j++)
	{
		if(x(0, j) >= x(0, j+1))
		{
			cout << "Error in \"Matrix<DataType> interp2(const Matrix<float>& x,\n"
					"                                    const Matrix<float>& y,\n"
					"                                    const Matrix<DataType>& v,\n"
					"                                    const Matrix<float>& xi,\n"
					"                                    const Matrix<float>& yi)\":" << endl
				 << "x and y must be normal grid!" << endl;
			exit(-1);
		}
		for(int i = 0; i < n_rows; i++)
		{
			if(x(i, j) != x(0, j))
			{
				cout << "Error in \"Matrix<DataType> interp2(const Matrix<float>& x,\n"
						"                                    const Matrix<float>& y,\n"
						"                                    const Matrix<DataType>& v,\n"
						"                                    const Matrix<float>& xi,\n"
						"                                    const Matrix<float>& yi)\":" << endl
					 << "x and y must be normal grid!" << endl;
				exit(-1);
			}
		}
	}
	
	for(int i = 0; i < n_rows-1; i++)
	{
		if(y(i, 0) >= y(i+1, 0))
		{
			cout << "Error in \"Matrix<DataType> interp2(const Matrix<float>& x,\n"
					"                                    const Matrix<float>& y,\n"
					"                                    const Matrix<DataType>& v,\n"
					"                                    const Matrix<float>& xi,\n"
					"                                    const Matrix<float>& yi)\":" << endl
				 << "x and y must be normal grid!" << endl;
			exit(-1);
		}
		for(int j = 0; j < n_cols; j++)
		{
			if(y(i, j) != y(i, 0))
			{
				cout << "Error in \"Matrix<DataType> interp2(const Matrix<float>& x,\n"
						"                                    const Matrix<float>& y,\n"
						"                                    const Matrix<DataType>& v,\n"
						"                                    const Matrix<float>& xi,\n"
						"                                    const Matrix<float>& yi)\":" << endl
					 << "x and y must be normal grid!" << endl;
				exit(-1);
			}
		}
	}

	int ni_rows = xi.rows();
	int ni_cols = xi.cols();
	if(yi.rows() != ni_rows || yi.cols() != ni_cols)
	{
		cout << "Error in \"Matrix<DataType> interp2(const Matrix<float>& x,\n"
				"                                    const Matrix<float>& y,\n"
				"                                    const Matrix<DataType>& v,\n"
				"                                    const Matrix<float>& xi,\n"
				"                                    const Matrix<float>& yi)\":" << endl
			 << "xi and yi must have the same size!" << endl;
		exit(-1);
	}

	int ni = xi.size();
	int row0 = 0, col0 = 0;
	Matrix<DataType> vi(ni_rows, ni_cols);

	for(int i = 0; i < ni; i++)
	{
		for(int it_col = 0; it_col < n_cols-1; it_col++)
		{
			if(xi(i) >= x(0, it_col) && xi(i) <= x(0, it_col+1))
			{
				col0 = it_col;
				break;
			}
		}

		for(int it_row = 0; it_row < n_rows-1; it_row++)
		{
			if(yi(i) >= y(it_row, 0) && yi(i) <= y(it_row+1, 0))
			{
				row0 = it_row;
				break;
			}
		}

		float x_rear = xi(i) - x(0, col0);
		float y_rear = yi(i) - y(row0, 0);
		DataType v_temp1 = v(row0, col0) + x_rear * (v(row0, col0+1) - v(row0, col0));
		DataType v_temp2 = v(row0+1, col0) + x_rear * (v(row0+1, col0+1) - v(row0+1, col0));
		vi(i) = v_temp1 + y_rear * (v_temp2 - v_temp1);
	}

	return vi;
}
#endif
