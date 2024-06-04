#pragma once
#include "math/rectangle/rectangletn.h"
//returns true if there's something left to draw
inline bool cropLine(vec2& p0, vec2& p1, crectangle2& cropRect)
{
	bool shouldSwap = p1.x < p0.x;

	typedef vect2<vec2> vec2x2;

	//swap so p1.x >= p0.x
	vec2x2 croppedPositions = vec2x2(shouldSwap ? p1 : p0, shouldSwap ? p0 : p1);

	cvec2 difference = croppedPositions[1] - croppedPositions[0];

	cfp& dydx = difference.y / difference.x;

	if (croppedPositions[0].x < cropRect.x)
	{
		if (croppedPositions[1].x < cropRect.x)
		{
			return false;
		}
		else
		{
			croppedPositions[0].y -= dydx * (croppedPositions[0].x - cropRect.pos0.x);
			croppedPositions[0].x = cropRect.pos0.x;
		}
	}

	cvec2& pos1 = cropRect.pos1();

	if (croppedPositions[1].x > pos1.x)
	{
		if (croppedPositions[0].x > pos1.x)
		{
			return false;
		}
		else
		{
			croppedPositions[1].y -= dydx * (croppedPositions[1].x - pos1.x);
			croppedPositions[1].x = pos1.x;
		}
	}

	if (croppedPositions[0].y < cropRect.pos0.y)
	{
		if (croppedPositions[1].y < cropRect.pos0.y)
		{
			return false;
		}
		else
		{
			croppedPositions[0].x -= (croppedPositions[0].y - cropRect.pos0.y) / dydx;
			croppedPositions[0].y = cropRect.pos0.y;
		}
	}
	else if (croppedPositions[1].y < cropRect.pos0.y)
	{
		croppedPositions[1].x -= (croppedPositions[1].y - cropRect.pos0.y) / dydx;
		croppedPositions[1].y = cropRect.pos0.y;
	}

	if (croppedPositions[0].y > pos1.y)
	{
		if (croppedPositions[1].y > pos1.y)
		{
			return false;
		}
		else
		{
			croppedPositions[0].x -= (croppedPositions[0].y - pos1.y) / dydx;
			croppedPositions[0].y = pos1.y;
		}
	}
	else if (croppedPositions[1].y > pos1.y)
	{
		croppedPositions[1].x -= (croppedPositions[1].y - pos1.y) / dydx;
		croppedPositions[1].y = pos1.y;
	}
	p0 = croppedPositions[shouldSwap ? 1 : 0];
	p1 = croppedPositions[shouldSwap ? 0 : 1];
	return true;
}