#pragma once
#include <cmath>
#include "globalFunctions.h"
#include "..\..\vector\vectn.h"

//calculates speed per second for object 1 to object 2 with mass 1
// distance is squared
constexpr fp calculateGravity1Outside(cfp distancesquared, cfp gravityConstant)
{
	return gravityConstant / distancesquared;
}
constexpr fp calculateGravityMassOutside(cfp distanceSquared, cfp m2, cfp gravityConstant)
{
	return calculateGravity1Outside(distanceSquared, gravityConstant) * m2;
}
//calculates speed per second for object 1 inside object 2 with mass 1
//distance is not squared
constexpr fp calculateGravity1Inside(cfp& distance, cfp& gravityConstant, cfp& radius3)
{
	return (gravityConstant * distance) / radius3;
}
constexpr fp calculateGravityMassInside(cfp& distance, cfp& gravityConstant, cfp& radius3, cfp& m2)
{
	return calculateGravity1Inside(distance, gravityConstant, radius3) * m2;
}

//calculates speed per second for object 1 to object 2 with mass 1
//distance is squared
constexpr fp calculateGravity1(cfp distanceSquared, cfp gravityConstant, cfp radius)
{
	cfp radius2 = radius * radius;
	return distanceSquared < radius ?
		//inside
		calculateGravity1Inside(sqrt(distanceSquared), gravityConstant, radius * radius2) :
		//outside
		calculateGravity1Outside(distanceSquared, gravityConstant);
}

constexpr fp calculateGravityMass(cfp distanceSquared, cfp m2, cfp gravityConstant, cfp radius)
{
	return calculateGravity1(distanceSquared, gravityConstant, radius) * m2;
}
//force between two objects
constexpr fp CalculateGravityForce(cfp distancesquared, cfp m1, cfp m2, cfp gravityConstant, cfp radius)
{
	return m1 * calculateGravityMass(distancesquared, m2, gravityConstant, radius);
}
//calculates acceleration for object 0
template<fsize_t dimensionCount>
constexpr vecn<dimensionCount> calculateAcceleration(cvecn<dimensionCount>& selectedPos0, cvecn<dimensionCount>& selectedPos1, cfp& m1, cfp& gravityConstant, cfp& radius1)
{
	cvec2& difference = selectedPos1 - selectedPos0;
	cfp& distanceSquared = difference.lengthSquared();
	if (distanceSquared == 0)
	{
		return vecn<dimensionCount>();
	}
	//add some distance
	cvecn<dimensionCount>& normal = difference / sqrt(distanceSquared);
	return calculateGravityMass(distanceSquared, m1, gravityConstant,radius1) * normal;
}
//distance is not squared
inline fp calculateOrbitalVelocity(cfp& distance, cfp& m2, cfp& gravityConstant)
{
	//https://www.softschools.com/formulas/physics/orbital_velocity_formula/76/
	return sqrt((m2 * gravityConstant) / distance);
}


template<fsize_t dimensionCount>
inline vecn<dimensionCount> getOrbitalVelocity(cvecn<dimensionCount>& p1, cvecn<dimensionCount>& p2, cfp& m2, cfp& gravityConstant)
{
	cvecn<dimensionCount>& difference = p2 - p1;
	cfp& distance = difference.length();
	return ((difference / distance) * calculateOrbitalVelocity(distance, m2, gravityConstant)).rotateXY(90);
}