#pragma once
#include "fastList.h"
template<typename t, typename vectn>
struct tile:IDestructable
{
	vectn position = vec2();
	//the elements in this tile
	fastList<t>* elements = nullptr;
	tile(vectn position, fastList<t>* elements) 
	{
		this->position = position;
		this->elements = elements;
	}
	virtual ~tile() override
	{
		if (elements) 
		{

		}
		delete elements;
	}
};