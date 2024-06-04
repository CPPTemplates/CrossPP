#pragma once
#include <array>
#include "math/mattnxn.h"

struct AABB;

struct Plane
{
	fp distanceToPoint(const vec3& point) const;

	fp distanceToOrigin;
	vec3 normal;
};

class ViewFrustum
{
public:
	void update(const mat4x4& projViewMatrix) noexcept;

	bool isBoxInFrustum(const vec3 position, const vec3 size) const noexcept;

private:
	std::array<Plane, 6> m_planes;
};