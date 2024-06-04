#pragma once
#include "vector\vectn.h"
#include "intersectable.h"
struct ray
{
	vec3 position;
	vec3 directionNormal;
	ray(vec3 position, vec3 directionNormal) :position(position), directionNormal(directionNormal) {}
};