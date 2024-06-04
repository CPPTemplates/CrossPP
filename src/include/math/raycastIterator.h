#include "vector\vectn.h"
#pragma once
//http://www.cse.yorku.ca/~amana/research/grid.pdf

template<fsize_t axisCount>
struct rayCastIterator
{
	//values which change per step
	vecin<axisCount> currentPosition = vecin<axisCount>();

	//values initialized by the constructor
	//sign: the sign of the direction of the ray in this axis
	vecin<axisCount> sign = vecin<axisCount>();
	
	//how much do you have to increase t to increment this axis?
	vecn<axisCount> delta = vecn<axisCount>();

	//the lowest value of t where the ray crosses the border of this axis
	vecn<axisCount> nextAxisCrossingT;

	size_t lastChangedAxis = axisCount;

	iteratorInline rayCastIterator(const rayCastIterator& other) = default;

	iteratorInline rayCastIterator(cvecn<axisCount>& p0, cvecn<axisCount>& directionNormal)
	{
		currentPosition = floorVector(p0);

		//https://www.redblobgames.com/grids/line-drawing.html
			//http://www.cse.yorku.ca/~amana/research/grid.pdf

		for (size_t i = 0; i < axisCount; i++)
		{
			sign[i] = directionNormal[i] > 0 ? 1 : -1;

			//else if direction normal is -0, it will transform into -infinity, making it the only path traced
			delta[i] = (directionNormal[i] == 0) ? INFINITY : (1.0 / math::absolute(directionNormal[i]));

			cfp& relativePos = p0[i] - currentPosition[i];

			cfp& distanceToCellWand = sign[i] > 0 ? (1 - relativePos) : relativePos;

			nextAxisCrossingT[i] = (directionNormal[i] == 0) ? INFINITY : (distanceToCellWand * delta[i]);
		}
	}

	// ++prefix operator
	iteratorInline rayCastIterator& operator++()
	{
		lastChangedAxis = nextAxisCrossingT.minimumIndex([](cfp& lhs, cfp& rhs) {
			return lhs < rhs;
			});


		nextAxisCrossingT[lastChangedAxis] += delta[lastChangedAxis];
		currentPosition[lastChangedAxis] += sign[lastChangedAxis];

		return *this;
	}

	// --prefix operator
	iteratorInline rayCastIterator& operator--()
	{
		//just reverse everything
		currentPosition[lastChangedAxis] -= sign[lastChangedAxis];
		nextAxisCrossingT[lastChangedAxis] -= delta[lastChangedAxis];

		lastChangedAxis = nextAxisCrossingT.minimumIndex([](cfp& lhs, cfp& rhs) {
			return lhs < rhs;
			});

		return *this;
	}
	iteratorInline fp getT() const
	{
		return (lastChangedAxis == axisCount) ? 0 : (nextAxisCrossingT[lastChangedAxis] - delta[lastChangedAxis]);
	}
};