#pragma once
//not an interface, because those don't have a compiler generated copy operator
struct testStruct
{
	//this makes it so base classes get deleted too.
	virtual ~testStruct();
};