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

constexpr axisID getAxis(const directionID &checkDirection)
{
	return (axisID)((int)checkDirection / 2);
}

constexpr bool isPositive(const directionID &checkDirection)
{
	return (bool)((int)checkDirection % 2);
}

/// returns -1 for negative directions, +1 for positive directions
constexpr int getSign(const directionID &direction)
{
	return ((int)direction % 2) * 2 - 1;
}

#pragma warning(push)
#pragma warning(disable : 4061)
constexpr int getAngle2DDegrees(const directionID &direction)
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
#pragma warning(pop)

// rotationindegrees can be 0, 90, 180 or 270
constexpr directionID rotate2DDegrees(const directionID &direction, cint &rotationInDegrees)
{
	// this integer stores the table of rotations, ordered by direction (row) and rotation (column).
	constexpr uint32_t rotationTable = // 0b00011011111000010100111010110100
									   // the table is reversed here, because lower numbers are further to the right.
		0b00011110101100010100101111100100;
	cint &tableIndex = ((int)direction +										// column offset (rotationindegrees / 90 * 4)
						rotationInDegrees / 22)									// row offset
					   * 2;														// multiply by element size
	return (directionID)((rotationTable & (0b11 << tableIndex)) >> tableIndex); // each item takes 2 bits
}

constexpr fp getAngle2D(const directionID &direction)
{
	return getAngle2DDegrees(direction) * math::degreesToRadians;
}

constexpr directionID getDirection(caxisID &axis, cbool &positive)
{
	return (directionID)(((int)axis * 2) + (positive ? 1 : 0));
}
constexpr directionID flipDirection(const directionID &directionToFlip, cint &shouldFlip = 1)
{
	return (directionID)((int)directionToFlip ^ shouldFlip);
}