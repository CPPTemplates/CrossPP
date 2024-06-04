#pragma once
#include "globalFunctions.h"
#include "axis.h"
#include "optimization/optimization.h"
#include "math/mathFunctions.h"

template <fsize_t dimensionCount>
constexpr fsize_t directionCount()
{
	return dimensionCount * 2;
}

constexpr fsize_t directionCount1D = directionCount<1>();
constexpr fsize_t directionCount2D = directionCount<2>();
constexpr fsize_t directionCount3D = directionCount<3>();

enum class directionID : int
{
	negativeX,
	positiveX,
	negativeY,
	positiveY,
	negativeZ,
	positiveZ,
	negativeW,
	positiveW,
};

constexpr axisID getAxis(const directionID& checkDirection)
{
	return (axisID)((int)checkDirection / 2);
}

constexpr bool isPositive(const directionID& checkDirection)
{
	return (bool)((int)checkDirection % 2);
}

#pragma warning( push )
#pragma warning( disable : 4061)
constexpr int getAngle2DDegrees(const directionID& direction)
{
	switch (direction)
	{
	case directionID::negativeX:
		return 270;
	case directionID::positiveX:
		return 90;
	case directionID::negativeY:
		return 180;
	case directionID::positiveY:
		return 0;
	default:
		assumeInRelease2(false, L"angle not allowed");
		return 0;
	}
}
#pragma warning( pop )

constexpr fp getAngle2D(const directionID& direction)
{
	return getAngle2DDegrees(direction) * math::degreesToRadians;
}

constexpr directionID getDirection(caxisID& axis, cbool& positive)
{
	return (directionID)(((int)axis * 2) + (positive ? 1 : 0));
}
constexpr directionID flipDirection(const directionID& directionToFlip)
{
	return getDirection(getAxis(directionToFlip), !isPositive(directionToFlip));
}