#include "math/vector/vectn.h"
#pragma once
struct sphere
{
	vec3 center;
	fp radius;
	sphere(cvec3 &center, cfp &radius) : center(center), radius(radius)
	{
	}
	sphere() = default;
};