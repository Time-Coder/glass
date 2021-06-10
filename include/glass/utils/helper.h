#ifndef __HELPER_H__
#define __HELPER_H__

#include <set>

typedef unsigned int uint;
typedef unsigned char byte;
typedef unsigned char uchar;

#define PI 3.14159265358979323846

template<class DataType>
void multiset_pop(std::multiset<DataType>& mset, DataType value);

#endif