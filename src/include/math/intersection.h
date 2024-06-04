#pragma once
#include "ray.h"
struct iIntersectable;

struct intersection 
{
	iIntersectable* intersectedWith;
	fp intersectionDistance = 0;//the distance the light has to travel to reach the intersection point
	vec3 intersectionColor = vec3();//the color of the intersection point
	fp intersectionOpacity = 0;//the opacity of the intersection
	fp reflectivity = 0;//the strength of the reflection
	//color equation: lerp(lerp(basecolor , reflectioncolor, reflectivity), behindcolor, intersectionOpacity)
	vec3 normal = vec3();//the normal of the edge the ray hit
	//TODO: the amount of angle a reflection can change its direction
	
	intersection(iIntersectable* intersectedWith, vec3 normal, fp intersectionOpacity, vec3 intersectionColor, fp intersectionDistance):
		intersectedWith(intersectedWith),intersectionDistance(intersectionDistance),intersectionColor(intersectionColor),intersectionOpacity(intersectionOpacity),normal(normal){}
};

