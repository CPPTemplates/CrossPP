#pragma once
#include "physics/gravity/gravity.h"
#include "collisions.h"

//where v1 = the first velocity you want to add,
//v2 = the second  velocity you want to add,
//c: maximum speed objects may reach (the "speed of light")
inline fp AddRelativisticVelocities(fp v1, fp v2, cfp& c)
{
	//Calculations
	fp Vf = abs(v1 + v2);
	v2 = -1 * v2;
	Vf = Vf / (1 + (v1 * v2) / (c * c));
	return Vf;
}
//where v1 = the first velocity you want to add,
//v2 = the second  velocity you want to add,
//c: maximum speed objects may reach (the "speed of light")
//reference frame: v1
inline vec2 AddRelativisticVelocities(vec2 v1, vec2 v2, cfp& c)
{
	// acceleration: change in velocity due to force f on object with mass m

	// new velocity if acceleration dv applied
	vec2 new_v = v1 + v2;

	fp b = 1 - v1.lengthSquared() / (c * c);
	if (b <= 0) b = DBL_MIN;

	fp lorentz_factor = sqrt(b);//1 / sqrt(b);
	v2 *= lorentz_factor;//v2 /= lorentz_factor;

	// apply acceleration to object's velocity
	return v1 + v2;
}

//fp because you never know how much iterations you want to speed up...
template<fsize_t dimensionCount>
inline void predictBehavior(cvecn<dimensionCount>& constantForce, vecn<dimensionCount>& speed, vecn<dimensionCount>& position, cfp& iterationCount)
{
	//now to simulate this behavior:
	//speed += force;
	//position += speed;
	//for a given amount of iterations

	//300000 km/sec
	//cfp lightSpeed = 300000000;
	position += speed * iterationCount + constantForce * math::calculateIterativeAddition(iterationCount);
	//speed = AddRelativisticVelocities(speed, constantForce * iterationCount, lightSpeed);
	speed += constantForce * iterationCount;
}