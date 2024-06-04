#include "vector\vectn.h"
#pragma once

//up direction = 0 0 1
struct coordinateSystem
{
	vec3 cameraPosition = vec3();
	vec2 cameraAngles = vec2();

	constexpr coordinateSystem(vec3 cameraPosition = vec3(), vec2 cameraAngles = vec2()):
		cameraPosition(cameraPosition),cameraAngles(cameraAngles) {}

	inline vec3 getForwardDirection() const
	{
		return vec2::getrotatedvector(cameraAngles.x);
	}

	inline vec3 getRightDirection() const
	{
		return vec2::getrotatedvector(cameraAngles.x + math::PI2 * 0.25);
	}

	constexpr vec3 getUpDirection() const
	{
		return vec3(0, 0, 1);
	}

	inline vec3 getRotatedForwardDirection() const
	{
		return vec3::getrotatedvector(cameraAngles.x, cameraAngles.y);
	}

	inline vec3 getRotatedRightDirection() const
	{
		//does not take y into account, because the y rotates over the right direction
		return getRightDirection();
	}

	inline vec3 getRotatedUpDirection() const
	{
		return vec3::rotate(getRotatedRightDirection(), getRotatedForwardDirection(), math::PI2 * -0.25);
	}
};