#pragma once
#include "globalFunctions.h"
//not an interface, because those don't have a compiler generated copy operator
//https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
struct IDestructable
{
	//this makes it so derived classes get deleted too when you delete the base class.
	//example: base* b = new derived();
	//delete b; <-- only base class destructor is called
	virtual ~IDestructable();
};