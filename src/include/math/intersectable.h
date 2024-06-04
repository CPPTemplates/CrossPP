#pragma once

#include "intersection.h"
struct iIntersectable 
{
	virtual bool intersect(ray& r, intersection*& result);
};