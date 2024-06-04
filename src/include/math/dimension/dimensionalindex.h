#include "globalFunctions.h"
#pragma once
template<int Dimensioncount>
struct dimensionalindex
{
	inline dimensionalindex(int locations[Dimensioncount])
	{
		this->locations = locations;
	}
	template<int Dimensioncount>
	inline static dimensionalindex fromPointer(int* locations)
	{
		dimensionalindex<Dimensioncount> i = dimensionalindex<Dimensioncount>();
		i.locations = locations;
		return i;
	}
	int* locations;//a pointer
};
template<typename t, int DimensionCount>
struct Idimensional
{
	int index;
	inline Idimensional()
	{
		index = 0;
	}
	inline virtual t GetElementAtIndex(dimensionalindex<DimensionCount> index) const
	{
		return t();
	}
};
