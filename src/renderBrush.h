#pragma once
#include "math/graphics/texture.h"
#include "math/mattnxn.h"
//template<typename brush0Type>
//void renderBrush(rectangle2 brushRect, mat3x3 transform, cvec2& rotationCentre, cbool& hasTransparency, const directionID& directionFacing, cbool& renderAnimation, const brush0Type& b, const texture& renderTarget);
//template<typename brush0Type>
//void renderBrush(crectangle2& brushRect, crectangle2& drawRect, cbool& hasTransparency, const directionID& directionFacing, cbool& renderAnimation, const brush0Type& b, const texture& renderTarget);

#include "world.h"
#include "settings.h"
template<typename brush0Type>
inline void renderBrush(rectangle2 brushRect, mat3x3 transform, cvec2& rotationCentreOnScreen, cbool& hasTransparency, const directionID& directionFacing, const brush0Type& b, const texture& renderTarget, cbool& renderAnimation = false)
{

	//transform will be: from 0, 0, texture width, texture width to [x]
	//brush rect will be 0,0, texture frame width, total texture height
	if (renderAnimation && (brushRect.h > brushRect.w))
	{
		//draw animation
		cint frameCount = (int)(brushRect.h / brushRect.w);
		cint animationIndex = currentWorld->ticksSinceStart % frameCount;
		if (animationIndex)
		{
			transform = mat3x3::cross(transform, mat3x3::translate(cvec2(0, -animationIndex * brushRect.w)));
		}
		brushRect = crectangle2(0, animationIndex * brushRect.w, brushRect.w, brushRect.w);
	}

	//standard direction: positiveY

	//first the transform, then mirroring, then rotating
	if (directionFacing == directionID::negativeX || directionFacing == directionID::negativeY)
	{
		transform = mat3x3::cross(mat3x3::mirror(axisID::y, rotationCentreOnScreen.y), transform);
	}

	if (directionFacing == directionID::negativeX || directionFacing == directionID::positiveX)
	{
		transform = mat3x3::cross(mat3x3::rotateDegrees(rotationCentreOnScreen, 90), transform);
	}

	if (settings::videoSettings::renderTransparency && hasTransparency)
	{
		fillTransparentRectangle(brushRect, transform, b, renderTarget);
	}
	else
	{
		fillTransformedBrushRectangle(brushRect, transform, b, renderTarget);
	}
}

template<typename brush0Type>
inline void renderBrush(crectangle2& brushRect, crectangle2& drawRect, cbool& hasTransparency, const directionID& directionFacing, const brush0Type& b, const texture& renderTarget, cbool& renderAnimation = false)
{
	renderBrush(brushRect, mat3x3::fromRectToRect(crectangle2(brushRect.x, brushRect.y, brushRect.w, renderAnimation ? brushRect.w : brushRect.h), drawRect), drawRect.getCenter(), hasTransparency, directionFacing, b, renderTarget, renderAnimation);
}