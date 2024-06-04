#pragma once
#include "vector\vectn.h"

//this function moves through a grid in 2d space
template<class compareFunction>
inline bool findRayCast(cvec2& p0, cvec2& p1, veci2& resultingBlockPos, veci2& adjacentBlockPosition, vec2& exactIntersection, const compareFunction&& function)
{
	//https://www.redblobgames.com/grids/line-drawing.html
		//http://www.cse.yorku.ca/~amana/research/grid.pdf
	cvec2& d0to1 = p1 - p0;//dx
	cveci2& sign = veci2(d0to1.x > 0 ? 1 : -1, d0to1.y > 0 ? 1 : -1);

	//not dx * sign_x, because you can get -0 and -inf
	cvec2& absd0to1 = d0to1.absolute();//nx
	veci2 currentPosition = veci2((int)floor(p0.x), (int)floor(p0.y));
	int dimension = -1;
	cvec2& delta = { 1 / absd0to1.x,1 / absd0to1.y };//deltax
	//vx
	vec2 progress = {
	d0to1.x > 0 ? currentPosition.axis[0] + 1 - p0.x : p0.x - currentPosition.axis[0],
	d0to1.y > 0 ? currentPosition.axis[1] + 1 - p0.y : p0.y - currentPosition.axis[1]
	};
	progress *= delta;//'divide' by total length
	bool looping;
	bool collided = false;
	do {
		//check point
		collided = function(currentPosition);
		if (collided)
		{
			break;
		}
		dimension = progress.x < progress.y ? 0 : 1;
		currentPosition.axis[dimension] += sign.axis[dimension];//step in the active dimenstion
		looping = progress.axis[dimension] <= 1;
		progress.axis[dimension] += delta.axis[dimension];//update progress
	} while (looping);
	//hit = getValue(currentPosition);
	resultingBlockPos = currentPosition;
	adjacentBlockPosition = currentPosition;
	if (dimension != -1)
	{
		adjacentBlockPosition.axis[dimension] -= sign.axis[dimension];
		if (!collided)
		{
			resultingBlockPos = adjacentBlockPosition;
		}
	}
	if (collided)
	{
		if (dimension == -1)
		{
			exactIntersection = p0;
		}
		else
		{
			//calculate the exact intersection point.
			exactIntersection.axis[dimension] = currentPosition.axis[dimension];
			if (sign.axis[dimension] == -1)
			{
				exactIntersection.axis[dimension]++;//this axis has to be increased because the ray hit on the plus side of the block.
			}
			cfp distance = abs(exactIntersection.axis[dimension] - p0.axis[dimension]);
			int otherAxis = dimension == 0 ? 1 : 0;
			fp part = distance / absd0to1.axis[dimension];//the part of the ray that has been marched
			fp exactCoordinate = p0.axis[otherAxis] + part * d0to1.axis[otherAxis];
			exactIntersection.axis[otherAxis] = exactCoordinate;
		}
	}
	else
	{
		exactIntersection = p1;
	}
	return collided;
}
