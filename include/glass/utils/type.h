#ifndef __TYPE_H__
#define __TYPE_H__

#include <typeinfo>
#include <iostream>
#include <cstdlib>
#include <type_traits>

#ifdef __GNUC__
#include <cxxabi.h>
#endif

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
std::string __type_name()
{
    int status;
    std::string tname = typeid(DataType).name();

#ifdef __GNUC__
    char *demangled_name = abi::__cxa_demangle(tname.c_str(), NULL, NULL, &status);
    if(status == 0)
    {
        tname = demangled_name;
        delete demangled_name;
    }
#endif

    int pos_brace = tname.find("[");
    if(pos_brace != std::string::npos && pos_brace != 0 && tname[pos_brace-1] == ' ')
    {
        tname = tname.substr(0,pos_brace-1) + tname.substr(pos_brace);
    }
    return tname;
}

#define type_name(DataType) __type_name<DataType>()
#define array_size(a) (sizeof(a)/sizeof(a[0]))

#endif