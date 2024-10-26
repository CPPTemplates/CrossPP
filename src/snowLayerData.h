#pragma once
#include "fallingBlockData.h"
constexpr fp minimalSnowLayerThickness = 1.0 / 8;
struct snowLayerData :fallingBlockData
{
	snowLayerData(cfp& layerThickness = minimalSnowLayerThickness);
	fp layerThickness = 0;
	virtual void serializeValue(nbtSerializer& s) override;
};