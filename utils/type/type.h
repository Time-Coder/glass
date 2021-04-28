#ifndef __TYPE_H__
#define __TYPE_H__

#include <typeinfo>
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;
#ifdef __GNUC__
#include <cxxabi.h>
#endif

namespace glass
{
    extern double pi;
};

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned char byte;

template<class DataType, class ElemType>
inline DataType& force_cast(const ElemType& value)
{
	return *((DataType*)(&value));
}

#define same_type(DataType1, DataType2) (typeid(DataType1).hash_code() == typeid(DataType2).hash_code())

template<typename DataType>
string __type_name()
{
    int status;
    string tname = typeid(DataType).name();

#ifdef __GNUC__
    char *demangled_name = abi::__cxa_demangle(tname.c_str(), NULL, NULL, &status);
    if(status == 0)
    {
        tname = demangled_name;
        delete demangled_name;
    }
#endif

    return tname;
}

#define type_name(DataType) __type_name<DataType>()

#endif