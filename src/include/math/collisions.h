#include "math/rectangle/rectangletn.h"
#include "math/graphics/texture.h"
#include "mattnxn.h"
#pragma once
bool collides1d(cfp& x0, cfp& w0, cfp& x1, cfp& w1);
bool collides2d(crectangle2& r1, crectangle2& r2);
bool collides2d(texture* tex1, mat3x3 transform1, texture* tex2, mat3x3 transform2);
bool collides2d(const std::vector<vec2>& v1, const std::vector<vec2>& v2, cbool& checkinside);
bool collides3d(const vec3& p0, const vec3& p1, const vec3& boxp0, const vec3& boxp1);
bool PointInPolygon(cvec2& point, const std::vector<vec2>&  points);
bool collidedistance3d(const vec3& p0, const vec3& p1, const vec3& boxp0, const vec3& boxp1, fp& tmin, fp& tmax);
bool collides2d(cvec2& p0, cvec2& p1, crectangle2& box);
bool collidedistance2d(cvec2& p0, cvec2& p1, crectangle2& box, fp& tmin, fp& tmax);
bool collidedistance2d(cvec2& p0, cvec2& p1, cvec2& boxp0, cvec2& boxp1, fp& tmin, fp& tmax);
bool collidedistance2dDirection(cvec2& p0, cvec2& direction, cvec2& boxp0, cvec2& boxp1, fp& tmin, fp& tmax);
bool collidedistance2dDirection(cvec2& p0, cvec2& direction, crectangle2& box, fp& tmin, fp& tmax);
bool pointleftofline(cvec2& point, cvec2& p0, cvec2& p1);

/// <summary>
/// 
/// </summary>
/// <param name="box0">the moving hitbox</param>
/// <param name="box1">the hitbox which stays still. substract distances to add the distance this hitbox moves</param>
/// <param name="box0MoveDistance">the distance which hitbox 0 will move as a vector</param>
/// <param name="axisCollided">a vector containing a boolean for each axis</param>
/// <returns>a number in range 0 to 1. 0 means collided instantly, 1 means not collided at all.</returns>
fp collideTime2d(crectangle2& box0, crectangle2& box1, cvec2& box0MoveDistance, vect2<bool>& axisCollided);
crectangle2 getBoxContaining(crectangle2& box, cvec2& boxSpeed);

bool collides2d(cvec2& a0, cvec2& a1, cvec2& b0, cvec2& b1);
bool collides2d(cvec2& a0, cvec2& a1, cvec2& b0, cvec2& b1, vec2& intersection);

bool collideraysphere(const vec3& rayorigin, const vec3& raydirection, cfp radius, fp& t0, fp& t1);
bool collideraysphere(const vec3& rayorigin, const vec3& raydirection, const vec3& spherecenter, cfp radius, fp& t0, fp& t1);
bool collidespheresSpeed(cvec2& spherePos1, cfp& sphereRadius1, cvec2& sphereVelocity1, cvec2& spherePos2, cfp& sphereRadius2, cvec2& sphereVelocity2, fp& time);


//https://gamedev.stackexchange.com/questions/48587/resolving-a-collision-with-forces
template<fsize_t dimensionCount>
constexpr vecn<dimensionCount> getSpeedAfterCollision(cvect2<vecn<dimensionCount>>& speedsBeforeCollision, cvec2& masses)
{
	if (masses[0] == masses[1])
	{
		return speedsBeforeCollision[1];
	}

	//because an object with infinite mass has an endless amount of energy, sharing is free.
	else if (masses[0] == INFINITY)
	{
		return speedsBeforeCollision[0];
	}
	else if (masses[1] == INFINITY)
	{
		//simplified
		return -speedsBeforeCollision[0] + (2 * speedsBeforeCollision[1]);
	}
	else
	{
		return (speedsBeforeCollision[0] * (masses[0] - masses[1]) + (2 * (masses[1] * speedsBeforeCollision[1]))) / (masses[0] + masses[1]);
	}
}

template<size_t dimensionCount>
constexpr vecn<dimensionCount> getSpeedAfterFriction(cvect2<vecn<dimensionCount>>& speedsBeforeFriction, cvec2& masses)
{
	if (masses[0] == masses[1])
	{
		return (speedsBeforeFriction[0] + speedsBeforeFriction[1]) / 2;
	}

	//because an object with infinite mass has an endless amount of energy, sharing is free.
	else if (masses[0] == INFINITY)
	{
		return speedsBeforeFriction[0];
	}
	else if (masses[1] == INFINITY)
	{
		//simplified
		return speedsBeforeFriction[1];
	}
	else
	{
		//redistribute the energy by mass
		//return the average speed which both particles will get
		return (((speedsBeforeFriction[0] * masses[0]) + (speedsBeforeFriction[1] * masses[1])) / (masses[0] + masses[1]));
	}
}

//the value you can multiply the speed by to get the speed after friction
constexpr fp getFrictionMultiplier(cfp& weight, cfp& frictionWeight) noexcept
{
	//make it so speed * mult * friction = speed

	//newspeed = (speed * weight) / (weight + frictionweight)

	//mult = weight / (weight + frictionweight)

	//is not true
	return weight / (weight + frictionWeight);
}

constexpr fp getTerminalVelocityMultiplier(cfp& frictionMultiplier)
{
	return 1 - frictionMultiplier;
}

//watch out! velocities being modified
inline void sphereCollisionResponse(cvec2& spherePos1, cfp& sphereMass1, cvec2& spherePos2, cfp& sphereMass2, vec2& sphereVelocity1, vec2& sphereVelocity2)
{
	vec2 U1x, U1y, U2x, U2y, V1x, V1y, V2x, V2y;

	fp m1, m2, x1, x2;
	vec2 v1temp, v1, v2, v1x, v2x, v1y, v2y, x(spherePos1 - spherePos2);

	x.normalize();
	v1 = sphereVelocity1;
	x1 = vec2::dot(x, v1);
	v1x = x * x1;
	v1y = v1 - v1x;
	m1 = sphereMass1;

	x = x * -1;
	v2 = sphereVelocity2;
	x2 = vec2::dot(x, v2);
	v2x = x * x2;
	v2y = v2 - v2x;
	m2 = sphereMass2;

	cfp frictionMult = 1;// 0.9;
	vec2 withoutFriction1 = vec2(v1x * (m1 - m2) / (m1 + m2) + v2x * (2 * m2) / (m1 + m2) + v1y);
	sphereVelocity2 = vec2(v1x * (2 * m1) / (m1 + m2) + v2x * (m2 - m1) / (m1 + m2) + v2y);
	sphereVelocity1 = math::lerp(sphereVelocity2, withoutFriction1, frictionMult);
	sphereVelocity2 = math::lerp(withoutFriction1, sphereVelocity2, frictionMult);
}