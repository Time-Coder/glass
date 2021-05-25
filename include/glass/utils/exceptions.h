#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include <iostream>
#include <exception>
#include "glass/utils/str.h"

#define DEFINE_EXCEPTION(name) \
class name : public std::exception\
{\
	std::string __message;\
\
public:\
	name(const std::string& _message) :\
	__message(_message) {}\
\
	friend std::ostream& operator <<(std::ostream& out, const name& e)\
	{\
		out << e.__message;\
		return out;\
	}\
	std::string message() {return __message;}\
	std::string type() {return #name;}\
\
	virtual const char* what() const throw ()\
	{\
        return __message.c_str();\
    }\
};

namespace glass
{

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

class IndexError : public std::exception
{
	std::string __message;

public:
	IndexError(int index, int lower_bound, int upper_bound)
	{
		__message = std::string("Index ") + str::str(index) + std::string(" out of bound [") + str::str(lower_bound) + ", " + str::str(upper_bound) + "].";
	}

	friend std::ostream& operator <<(std::ostream& out, const IndexError& e)
	{
		out << e.__message;
		return out;
	}
	std::string message() {return __message;}
	std::string type() {return "IndexError";}

	virtual const char* what() const throw ()
	{
        return __message.c_str();
    }
};

}

#endif