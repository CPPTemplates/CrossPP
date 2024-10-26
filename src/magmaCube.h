#pragma once
#include "slime.h"
//magma cube
constexpr rectangle2 bottomEyeSliceTextureRect = crectangle2(24, 4, 8, 1);
constexpr rectangle2 topEyeSliceTextureRect = crectangle2(24, 13, 8, 1);
constexpr int bottomEyeSliceIndex = 5;
constexpr int topEyeSliceIndex = 6;


constexpr int magmaCubeLayerCount = 0x8;
constexpr int magmaCubeLayerThickness = (int)slimeOuterBodyTextureRect.size.getY() / magmaCubeLayerCount;
struct magmaCube : public slime
{
	bodyPart2D* layers[magmaCubeLayerCount]{};

	magmaCube(dimension* dimensionIn, cvec2& position);
	virtual void createBodyParts() override;
};