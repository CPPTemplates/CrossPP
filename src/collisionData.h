#pragma once
#include "math/rectangle/rectangletn.h"
#include "collisionTypeID.h"
struct collisionData
{
	collisionData(crectangle2 hitboxCollidingWith = crectangle2(), const collisionTypeID &type = collisionTypeID::willNotCollide, cvec2 &speed = cvec2(), cfp &collisionTime = INFINITY) : hitboxCollidingWith(hitboxCollidingWith), type(type), speed(speed), collisionTime(collisionTime) {}
	rectangle2 hitboxCollidingWith = crectangle2();
	collisionTypeID type = collisionTypeID::willNotCollide;
	vec2 speed = vec2();
	fp collisionTime = INFINITY;
	vect2<bool> axisCollided = vect2<bool>();
	void evaluate(crectangle2 &hitboxToTest, cvec2 &hitboxSpeed, cfp &maxDuration);
};
constexpr bool operator<(const collisionData &a, const collisionData &b)
{
	return (a.axisCollided.x || a.axisCollided.y) &&
		   (!(b.axisCollided.x || b.axisCollided.y) ||
			a.collisionTime < b.collisionTime);
}