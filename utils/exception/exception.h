#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <iostream>
#include <path.h>
#include <str.h>

using namespace std;

class BaseException
{
	string __type = "BaseException";
	string __message = "";
	string __file_name;
	string __func_name;
	int __line_no;

public:
	BaseException(const string& _type, const string& _message, const string& _file_name, const string& _func_name, int _line_no) :
	__type(_type), __file_name(path::relpath(_file_name)), __func_name(_func_name), __line_no(_line_no)
	{
		cerr << endl << __file_name << ":" << str::str(__line_no) << ":" << __func_name << ": " << __type + ":" << endl;
		__message = str::replace(_message, "\n", "\n    ");
		cerr << "    " << __message << endl << endl;
	}
	friend ostream& operator <<(ostream& out, const BaseException& e)
	{
		out << e.__type << ":\n    " << e.__message;
		return out;
	}
	string message() {return __message;}
	string type() {return __type;}
};

#define DEFINE_EXCEPTION(name) \
class name : public BaseException\
{\
public:\
	name(const string& _message, const string& _file_name, const string& _func_name, int _line_no) :\
	BaseException(#name, _message, _file_name, _func_name, _line_no) {}\
};

DEFINE_EXCEPTION(Exception)
DEFINE_EXCEPTION(TypeError)
DEFINE_EXCEPTION(KeyError)
DEFINE_EXCEPTION(ValueError)
DEFINE_EXCEPTION(AssertionError)
DEFINE_EXCEPTION(AttributeError)
DEFINE_EXCEPTION(ArithmeticError)
DEFINE_EXCEPTION(ZeroDivisionError)
DEFINE_EXCEPTION(IOError)
DEFINE_EXCEPTION(MemoryError)
DEFINE_EXCEPTION(RuntimeError)
DEFINE_EXCEPTION(SyntaxError)

class IndexError : public BaseException
{
public:
	IndexError(int index, int lower_bound, int upper_bound, const string& _file_name, const string& _func_name, int _line_no) :
	BaseException("IndexError", string("Index ") + str::str(index) + string(" out of bound [") + str::str(lower_bound) + ", " + str::str(upper_bound) + "].", _file_name, _func_name, _line_no) {}
};

#define Exception(_message) Exception(_message, __FILE__, __func__, __LINE__)
#define TypeError(_message) TypeError(_message, __FILE__, __func__, __LINE__)
#define KeyError(_message) KeyError(_message, __FILE__, __func__, __LINE__)
#define ValueError(_message) ValueError(_message, __FILE__, __func__, __LINE__)
#define AssertionError(_message) AssertionError(_message, __FILE__, __func__, __LINE__)
#define AttributeError(_message) AttributeError(_message, __FILE__, __func__, __LINE__)
#define ArithmeticError(_message) ArithmeticError(_message, __FILE__, __func__, __LINE__)
#define ZeroDivisionError(_message) ZeroDivisionError(_message, __FILE__, __func__, __LINE__)
#define IOError(_message) IOError(_message, __FILE__, __func__, __LINE__)
#define IndexError(i, lower_bound, upper_bound) IndexError(i, lower_bound, upper_bound, __FILE__, __func__, __LINE__)
#define MemoryError(_message) MemoryError(_message, __FILE__, __func__, __LINE__)
#define RuntimeError(_message) RuntimeError(_message, __FILE__, __func__, __LINE__)
#define SyntaxError(_message) SyntaxError(_message, __FILE__, __func__, __LINE__)

#endif