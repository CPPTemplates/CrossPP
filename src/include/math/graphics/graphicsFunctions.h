#pragma once
#include "array/arraynd/arraynd.h"
#include "math/rectangle/rectanglefunctions.h"
#include "math/graphics/resolutiontexture.h"

// the rect should be relative to the default size.
template <typename brush0Type>
inline void fillRectangle(const resolutionTexture &tex, rectangle2 rect, const brush0Type &b)
{
	cfp &scaleModifier = tex.getScaleModifier();
	if (scaleModifier != 1)
	{
		rect = rect.multiplied(scaleModifier);
		// cheaper than inverse transform
		cmat3x3 &inverseScaleTransform = mat3x3::scale(cvec2(1.0 / scaleModifier));
		const transformBrush<brush0Type> &brush = transformBrush<brush0Type>(inverseScaleTransform, b);
		fillRectangle(*tex.scaledTextures[0], rect, brush);
	}
	else
	{
		fillRectangle(*tex.scaledTextures[0], rect, b);
	}
	tex.recalculateScaledTextures();
}

// the rect should be relative to the default size.
template <typename brush0Type>
inline void fillTransformedRectangle(const resolutionTexture &tex, crectangle2 &brushRect, mat3x3 transform, const brush0Type &b)
{
	cfp &scaleModifier = tex.getScaleModifier();
	if (scaleModifier != 1)
	{
		cmat3x3 &scaleTransform = mat3x3::scale(cvec2(scaleModifier));
		const transformBrush<brush0Type> &brush = transformBrush<brush0Type>(scaleTransform.inverse(), b);
		transform = mat3x3::cross(scaleTransform, transform);
		fillTransformedRectangle(*tex.scaledTextures[0], brushRect, transform, brush);
	}
	else
	{
		fillTransformedRectangle(*tex.scaledTextures[0], brushRect, transform, b);
	}
	tex.recalculateScaledTextures();
}

// fill a row of pixels with a brush
template <typename t, typename brush0Type>
inline void fillRowUnsafe(const array2d<t> &array, cfsize_t &rowY, cfsize_t &minX, cfsize_t &maxX, const brush0Type &b)
{
	t *const rowPtr = array.baseArray + rowY * array.size.x;
	t *const endPtr = rowPtr + maxX;

	typedef typename brush0Type::inputType vectorType;
	typedef typename vectorType::axisType axisType;

	vectorType pos = vectorType((axisType)minX, (axisType)rowY);

	for (t *ptr = rowPtr + minX; ptr < endPtr; ptr++, pos.x++)
	{
		*ptr = b.getValue((typename brush0Type::inputType)pos);
	}
}
template <typename t, typename brush0Type>
inline void fillRowUnsafe(const array2d<t> &array, cfsize_t &rowY, cfsize_t &minX, cfsize_t &maxX, const transformBrush<brush0Type> &b)
{
	t *const rowPtr = array.baseArray + rowY * array.size.x;
	t *const endPtr = rowPtr + maxX;

	vec2 pos = b.modifiedTransform.multPointMatrix(vec2((fp)minX, (fp)rowY));
	vec2 step = b.modifiedTransform.getStep(axisID::x);

	for (t *ptr = rowPtr + minX; ptr < endPtr; ptr++, pos += step)
	{
		*ptr = b.baseBrush.getValue((typename brush0Type::inputType)pos);
	}
}

template <typename t, typename brush0Type, typename brush1Type, typename = std::enable_if_t<std::is_base_of_v<array2d<t>, brush1Type>>>
inline void fillRowUnsafe(const array2d<t> &array, cfsize_t &rowY, cfsize_t &minX, cfsize_t &maxX, const colorMixer<brush0Type, brush1Type> &b)
{
	if (&b.bottomBrush == &array)
	{
		t *const rowPtr = array.baseArray + rowY * array.size.x;
		t *const endPtr = rowPtr + maxX;

		typedef typename brush0Type::inputType vectorType;
		typedef typename vectorType::axisType axisType;

		vectorType pos = vectorType((axisType)minX, (axisType)rowY);

		for (t *ptr = rowPtr + minX; ptr < endPtr; ptr++, pos.x++)
		{
			ccolor &topColor = b.topBrush.getValue(pos);
			*ptr = topColor.a() == color::maxValue ? topColor : topColor.a() ? transitionColor(topColor, *ptr)
																			 : *ptr;
		}
	}
	else
	{
		fillRowUnsafe(array, rowY, minX, maxX, b);
	}
}

template <typename t, typename brush0Type>
inline void fillRow(const array2d<t> &array, cint &rowY, cint &minX, cint &maxX, const brush0Type &b)
{
	if ((rowY >= 0) && (rowY < (int)array.size.y) && (maxX > 0))
	{
		fillRowUnsafe(array, rowY, (fsize_t)math::maximum(minX, 0), math::minimum((fsize_t)maxX, array.size.x), b);
	}
}

template <typename t, typename brush0Type>
inline void fillRow(const array2d<t> &array, cint &rowY, cfp &minX, cfp &maxX, const brush0Type &b)
{
	// ceil rule
	fillRow(array, rowY, math::maximum((int)math::ceil(minX), 0), math::minimum((int)math::ceil(maxX), (int)array.size.x), b);
}
template <typename t, typename brush0Type>
inline void fillRectangleUnsafe(const array2d<t> &array, crectanglet2<fsize_t> &rect, const brush0Type &b)
{
	cvect2<fsize_t> &pos11 = rect.pos1();
	for (fsize_t currentY = rect.y; currentY < pos11.y; currentY++)
	{
		fillRowUnsafe(array, currentY, rect.pos0.x, pos11.x, b);
	}
}

template <typename t, typename brush0Type>
inline void fillRectangle(const array2d<t> &array, rectanglei2 rect, const brush0Type &b)
{
	if (array.getClientRect().cropClientRect(rect))
	{
		fillRectangleUnsafe(array, crectanglet2<fsize_t>(rect), b);
	}
}

template <typename t, typename brush0Type>
inline void fillRectangle(const array2d<t> &array, crectangle2 &rect, const brush0Type &b)
{
	fillRectangle(array, ceilRectangle(rect), b);
}

template <typename t, typename brush0Type>
inline void fillRectangleBorders(const array2d<t> &array, crectanglei2 &rect, cint &borderThickness, const brush0Type &b)
{
	fillRectangle(array, crectanglei2(rect.x, rect.y, rect.size.x, borderThickness), b);
	fillRectangle(array, crectanglei2(rect.x, rect.y + rect.size.y - borderThickness, rect.size.x, borderThickness), b);
	fillRectangle(array, crectanglei2(rect.x, rect.y, borderThickness, rect.size.y), b);
	fillRectangle(array, crectanglei2(rect.x + rect.size.x - borderThickness, rect.y, borderThickness, rect.size.y), b);
}
// expands inward
template <typename t, fsize_t n, typename brush0Type>
inline void fillRectangleBorders(const array2d<t> &array, cveci2 &pos00, cveci2 &pos11, cint &borderThickness, const brush0Type &b)
{
	fillRectangleBorders(array, crectanglei2(pos00, pos11 - pos00 + 1), borderThickness, b);
}
// x, y: pos00 position
// w, h: width, height
template <typename t, typename brush0Type>
inline void fillEllipse(const array2d<t> &array, crectangle2 &rect, const brush0Type &b)
{
	rectanglei2 croppedRect = ceilRectangle(rect);

	//+1 for also filling the last pixel
	croppedRect.h++;

	// crectanglei2& croppedRect = ceiledRect.cropped(crectanglei2(cveci2(),array.size));
	if (array.getClientRect().cropClientRect(croppedRect))
	{
		cveci2 &croppedPos1 = croppedRect.pos1();
		// crop

		cfp &midx = rect.x + rect.w * .5;
		cfp &midy = rect.y + rect.h * .5;
		cfp &multx = 2.0 / rect.w;
		cfp &multy = 2.0 / rect.h;

		for (int j = croppedRect.y; j < croppedPos1.y; j++)
		{
			// circle equation:
			// x * x + y * y = r * r
			// ellipse equation:
			//(dx * multx)^2 + (dy*multy)^2 = 1
			//(dx * multx)^2 = 1 - (dy * multy) ^2
			// dx * multx = sqrt(1 - (dy * multy) ^ 2)
			// dx = sqrt(1 - (dy * multy) ^2) / multx
			cfp val = 1 - math::squared((midy - j) * multy);
			if (val > 0)
			{
				cfp &dx = math::sqrt(val) / multx;

				// cfp &fromX = midx - dx;
				// cfp &toX = midx + dx;

				// 0.5 to 1.5:
				// only fill two pixels
				cint &minX = math::maximum((int)ceil(midx - dx), croppedRect.x);
				cint &maxX = math::minimum((int)floor(midx + dx) + 1, croppedPos1.x); //+1 for also filling the last pixel

				// t *ptr = array.baseArray + j * array.size.x + minX;

				fillRow(array, j, minX, maxX, b);
			}
		}
	}
}

template <typename t, typename brush0Type>
inline void fillEllipseCentered(const array2d<t> &array, crectangle2 &rect, const brush0Type &b)
{
	fillEllipse(array, crectangle2(rect.pos0 - rect.size * 0.5, rect.size), b);
}

// draw a line from p0 to p1
// source:
// http://blog.ruofeidu.com/bresenhams-line-algorithm-in-c/
// also worth checking out:
// https://www.redblobgames.com/grids/line-drawing.html
// https://www.geeksforgeeks.org/bresenhams-line-generation-algorithm/
template <typename t, typename brush0Type>
inline void fillLine(const array2d<t> &array, vec2 p0, vec2 p1, const brush0Type &b)
{
	if (cropLine(p0, p1, crectangle2(vec2(), vec2(array.size - 1))))
	{
		fillLineUnsafe(array, p0, p1, b);
	}
}

template <typename t, typename brush0Type>
inline void fillLineUnsafe(const array2d<t> &array, cvec2 &p0, cvec2 &p1, const brush0Type &b)
{
	veci2 currentPosition = veci2((int)p0.x, (int)p0.y);
	veci2 endPosition = veci2((int)p1.x, (int)p1.y);
	veci2 difference = veci2(abs(endPosition.x - currentPosition.x), abs(endPosition.y - currentPosition.y));
	veci2 sign = veci2(currentPosition.x < endPosition.x ? 1 : -1, currentPosition.y < endPosition.y ? 1 : -1);
	int err = difference.x - difference.y;

	while (true)
	{
		array.setValue(currentPosition, b.getValue(currentPosition));
		if (currentPosition == endPosition)
			return;
		cint &e2 = err * 2; // will result in a bit shift operation because of the optimizer.
		if (e2 > -difference.y)
		{
			err -= difference.y;
			currentPosition.x += sign.x;
		}
		if (e2 < difference.x)
		{
			err += difference.x;
			currentPosition.y += sign.y;
		}
	}
}

template <typename t>
inline constexpr int getStartY(cfp &minY, const array2d<t> &array)
{
	return math::maximum(math::ceil(minY), 0);
}
template <typename t>
inline constexpr int getEndY(cfp &maxY, const array2d<t> &array)
{
	return math::minimum(math::ceil(maxY), (int)array.size.y);
}

// this function fills a line expanded by a certain radius.
template <typename t, typename brush0Type>
inline void fillLine(const array2d<t> &array, cvec2 &p0, cvec2 &p1, cfp &radius, const brush0Type &b)
{
	cfp &radiusSquared = radius * radius;
	auto fillRadiusRow = [&radiusSquared, &array, &b](cvec2 &leftCenter, cvec2 &rightCenter, cint &y)
	{
		// now calculate the x at which the distance to the nearest point is exactly radius
		// r = sqrt(x * x + y * y)
		// we know r and y
		// r * r = x * x + y * y
		// x * x = r * r - y * y
		// x = sqrt(r * r - y * y)
		cfp &leftX = leftCenter.x - std::sqrt(radiusSquared - math::squared(y - leftCenter.y));
		cfp &rightX = rightCenter.x + std::sqrt(radiusSquared - math::squared(y - rightCenter.y));
		fillRow(array, y, leftX, rightX, b);
	};
	if (p0.y == p1.y)
	{
		// would be infinite slope
		cvec2 &leftPoint = p1.x > p0.x ? p0 : p1, rightPoint = p1.x > p0.x ? p1 : p0;

		cint &maxY = getEndY(p1.y + radius, array);
		for (int y = getStartY(p0.y - radius, array); y < maxY; y++)
		{
			fillRadiusRow(leftPoint, rightPoint, y);
		}
	}
	else
	{
		cvec2 &lowestPoint = p1.y > p0.y ? p0 : p1, highestPoint = p1.y > p0.y ? p1 : p0;
		// x = ay + b
		// calculate a
		// x / y
		cfp &lineSlope = (highestPoint.x - lowestPoint.x) / (highestPoint.y - lowestPoint.y);
		// calculate b
		// b = x - ay
		cfp &lineStart = lowestPoint.x - lineSlope * lowestPoint.y;

		// using the inverse of the slope, calculate the y offset of orthogonal points (we basically project 2 points onto the line)
		//  given that
		//  r = sqrt(x * x + y * y)
		//  x = y / slope (inverse slope!)
		//  solve for y
		//  substitute
		//  r = sqrt((slope * y) ^ 2 + y ^ 2)
		//  https://www.symbolab.com/solver/equation-calculator/solve%20for%20y%2C%20r%3D%5Csqrt%7B%5Cleft(sy%5Cright)%5E%7B2%7D%2By%5E%7B2%7D%7D?or=input
		//  y = r / sqrt(slope ^ 2 + 1), y = -r / sqrt(slope ^ 2 + 1)
		cfp &yOffset = (radius * lineSlope) / std::sqrt(lineSlope * lineSlope + 1);
		cfp &leftYOffset = -yOffset, rightYOffset = yOffset; // lineSlope > 0 ? -yOffset : yOffset, rightYOffset = -leftYOffset;

		cint &maxY = getEndY(highestPoint.y + radius, array);
		for (int y = getStartY(lowestPoint.y - radius, array); y < maxY; y++)
		{
			// evaluate infinite line formula at the nearest point
			cfp &nearestLineLeftY = math::clamp(y + leftYOffset, lowestPoint.y, highestPoint.y);
			cfp &nearestLineLeftX = lineStart + lineSlope * nearestLineLeftY;
			cfp &nearestLineRightY = math::clamp(y + rightYOffset, lowestPoint.y, highestPoint.y);
			cfp &nearestLineRightX = lineStart + lineSlope * nearestLineRightY;
			fillRadiusRow(cvec2(nearestLineLeftX, nearestLineLeftY), vec2(nearestLineRightX, nearestLineRightY), y);
		}
	}
}

// advanced images
// ONLY TRANSFORMS THE RECTANGLE, NOT THE COLORS

// the transform should be from the rectangle to the screen

// shifted so the first point is the lowest point
// clockwise!
template <typename t, typename brush0Type>
inline void fillPolygonShiftedUnsafe(const array2d<t> &array, const fastArray<vec2> &shiftedPositions, cfp &minY, cfp &maxY, const brush0Type &b)
{
	// 0 = left, 1 = right
	fsize_t nextPointIndex[2]{0, 0};
	cint windDirection[2]{1, -1}; // left: 1 (clockwise), right: -1 (counter clockwise)
	fp slope[2]{0, 0};
	cfsize_t sideCount = 0x2;
	fp currentX[2]{0, 0};

	cfsize_t &maxYCropped = math::minimum(math::ceil<fsize_t>(maxY), array.size.y);
	// initialize the first two lines by default
	for (fsize_t currentY = (fsize_t)math::maximum(math::ceil(minY), 0); currentY < maxYCropped; currentY++)
	{
		for (fsize_t side = 0; side < sideCount; side++)
		{
			bool changed = false;
			while (currentY >= shiftedPositions[nextPointIndex[side]].y)
			{
				// maybe there is a straight line or something
				if ((nextPointIndex[side] == 0) && (windDirection[side] == -1))
				{
					nextPointIndex[side] = (fsize_t)shiftedPositions.size - 1;
				}
				else
				{
					nextPointIndex[side] += windDirection[side];
				}
				changed = true;
			}
			// passed point, get next point
			if (changed)
			{
				cvec2 &selectedPos0 = shiftedPositions[(nextPointIndex[side] - windDirection[side]) % shiftedPositions.size];
				cvec2 &selectedPos1 = shiftedPositions[nextPointIndex[side]];
				// update
				slope[side] = (selectedPos1.x - selectedPos0.x) / (selectedPos1.y - selectedPos0.y);
				currentX[side] = selectedPos0.x + slope[side] * (currentY - selectedPos0.y);
			}
			else
			{
				currentX[side] += slope[side];
			}
		}
		// CAUTION WITH SLOPE
		cint &minX = math::maximum((int)ceil(currentX[0]), 0);
		cint &maxX = math::maximum((int)ceil(currentX[1]), 0);
		// draw line
		fillRow(array, currentY, minX, maxX, b);
	}
}

// positions can't overlap each other in the x axis
template <typename t, typename brush0Type>
inline void fillPolygon(const array2d<t> &array, const fastArray<vec2> &positions, const brush0Type &b)
{
	// find highest y pos
	fp highestY = 0;
	fp lowestY = (fp)array.size.y;
	fsize_t lowestIndex = 0;
	// get boundaries and highest y index
	for (fsize_t index = 0; index < positions.size; index++)
	{
		if (positions[index].y > highestY)
		{
			highestY = positions[index].y;
		}
		if (positions[index].y < lowestY)
		{
			lowestY = positions[index].y;
			lowestIndex = index;
		}
	}

	if (highestY > 0 && lowestY < array.size.y)
	{
		cbool &clockWise = woundClockwise(positions[0], positions[1], positions[2]);
		// shift
		fastArray<vec2> shiftedArray((fsize_t)positions.size, false);
		for (fsize_t index = 0; index < positions.size; index++)
		{
			fsize_t shiftedIndex = index;

			// do not swap these
			if (lowestIndex)
			{
				shiftedIndex = (shiftedIndex < lowestIndex ? shiftedIndex + (fsize_t)positions.size : shiftedIndex) - lowestIndex;
			}
			// 0 has to stay at 0
			// flip all others
			if (shiftedIndex && !clockWise)
			{
				shiftedIndex = (fsize_t)positions.size - shiftedIndex;
			}
			shiftedArray[shiftedIndex] = positions[index];
		}
		fillPolygonShiftedUnsafe(array, shiftedArray, lowestY, highestY, b);
	}
}

template <typename t, typename brush0Type>
inline void fillTransformedRectangle(const array2d<t> &array, rectangle2 brushRect, cmat3x3 &transform, const brush0Type &b)
{
	if (transform.isStraight()) // simple check which can save a lot of time
	{
		fillRectangle(array, transform.multRotatedRectMatrix(brushRect), b);
	}
	else
	{
		// expand the brushrect, so it contains the last row of pixels too
		// brushRect.size += 1;
		fastArray<vec2> positions({
			transform.multPointMatrix(brushRect.pos0),
			transform.multPointMatrix(brushRect.pos01()),
			transform.multPointMatrix(brushRect.pos1()),
			transform.multPointMatrix(brushRect.pos10()),
		});
		// check for clockwise
		fillPolygon(array, positions, b);
	}
}