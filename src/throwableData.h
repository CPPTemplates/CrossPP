#pragma once
#include "math/graphics/resolutiontexture.h"
#include "entityData.h"
struct throwableData : public entityData
{
	resolutionTexture* tex = nullptr;
	fp throwPower = 0;//the initial velocity
	throwableData(const entityData& data, resolutionTexture* tex, cfp& throwPower);
};