#pragma once
#include "globalFunctions.h"


//if write: right = left
//else left = right
template<typename t>
inline void serialize(t& left, t& right, cbool& write)
{
	if (write)
	{
		right = left;
	}
	else
	{
		left = right;
	}
}

#define castout reinterpret_cast<const char*>//for writing to file streams
#define castin reinterpret_cast<char*>//for reading from file streams