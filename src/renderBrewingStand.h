#pragma once
#include "math/vector/vectn.h"
#include "textureList.h"
/// <summary>
/// 
/// </summary>
/// <typeparam name="targetType"></typeparam>
/// <param name="drawRect"></param>
/// <param name="hasBottle"></param>
/// <param name="renderTarget"></param>
template<typename targetType>
inline void renderBrewingStand(cmat3x3& normalRectToScreenTransform, cbool hasBottle[brewingStandPotionCapacity], const targetType& renderTarget)
{
	//render base
	crectangle2 baseRelativeRect = crectangle2(0, 0, 1, brewingStandBaseHeight);
	fillTransformedBrushRectangle(getAbsoluteRect(crectangle2(brewingStandBaseTexture->getClientRect()), baseRelativeRect),mat3x3::cross(normalRectToScreenTransform, mat3x3::fromRectToRect(crectangle2(brewingStandBaseTexture->getClientRect()), crectangle2(0,0,1,1))), *brewingStandBaseTexture, renderTarget);
	//only render 2 bottles, left and right

	constexpr int bottleIndexesToDraw[directionCount1D]
	{
		0,
		2
	};

	for (fsize_t i = 0; i < directionCount1D; i++)
	{
		crectangle2& brewingStandTopTextureClientRect = brewingStandTopTexture->getClientRect();
		cint bottleIndexToDraw = bottleIndexesToDraw[i];
		rectangle2 brushRect = rectangle2(hasBottle[bottleIndexToDraw] ? 0 : (brewingStandTopTextureClientRect.w * 0.5), 0, brewingStandTopTextureClientRect.w * 0.5, brewingStandTopTextureClientRect.h);

		mat3x3 transform = mat3x3::cross(normalRectToScreenTransform, mat3x3::fromRectToRect(brushRect, crectangle2(((i == 1) ? 0.5 : 0), 0, 0.5, 1)));

		//mirror -> drawrect

		if (hasBottle[bottleIndexToDraw] == (i == 1))
		{
			cfp mirrorX = brewingStandTopTextureClientRect.w * (hasBottle[bottleIndexToDraw] ? 0.25 : 0.75);
			transform = mat3x3::cross(transform, mat3x3::mirror(axisID::x, mirrorX));
		}
		fillTransparentRectangle(brushRect, transform, *brewingStandTopTexture, renderTarget);
	}
}