#include "glass/utils/helper.h"

template<class DataType>
void multiset_pop(std::multiset<DataType>& mset, DataType value)
{
	auto it = mset.find(value);
	if(it != mset.end())
	{
		mset.erase(it);
	}
}

template void multiset_pop(std::multiset<uint>& mset, uint value);