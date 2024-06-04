#include "intersectableCuboid.h"

//p0: from
	//p1: to
	//source:
	//https://tavianator.com/fast-branchless-raybounding-box-intersections/
	//explanation:
	//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
	//collision point = p0 + (p1 - p0).normalized() * t
	//collision of a ray with a box (NO TRANSFORMATION)
bool intersectableCuboid::intersect(ray& r, intersection*& result)
{
	const vec3 inversenormal = 1 / r.directionNormal;

	vec3 boxp1 = box.pos1();
	fp tx0 = (box.pos0.x - r.position.x) * inversenormal.x;//t when intersecting with the boxp0.x plane
	fp tx1 = (boxp1.x - r.position.x) * inversenormal.x;//t when intersecting with the boxp1.x plane

	fp tmin = math::minimum(tx0, tx1);
	fp tmax = math::maximum(tx0, tx1);

	fp ty0 = (box.pos0.y - r.position.y) * inversenormal.y;//t when intersecting with the boxp0.y plane
	fp ty1 = (boxp1.y - r.position.y) * inversenormal.y;//t when intersecting with the boxp1.y plane

	tmin = math::maximum(tmin, math::minimum(ty0, ty1));
	tmax = math::minimum(tmax, math::maximum(ty0, ty1));

	fp tz0 = (box.pos0.z - r.position.z) * inversenormal.z;//t when intersecting with the boxp0.z plane
	fp tz1 = (boxp1.z - r.position.z) * inversenormal.z;//t when intersecting with the boxp1.z plane

	tmin = math::maximum(tmin, math::minimum(tz0, tz1));
	tmax = math::minimum(tmax, math::maximum(tz0, tz1));

	//nearest point?

	if (tmax >= tmin && tmax >= 0)//tmax >= 0 because else you could hit behind you
	{
		//intersects
		//todo: compute normal
		vec3 normal = vec3();
		result = new intersection(this, normal, 1, vec3(0, 0, 1), math::maximum(tmin, (fp)0.0));
		return true;
	}
	else
	{
		//does not intersect
		return false;
	}
}
