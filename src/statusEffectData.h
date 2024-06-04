#pragma once
#include "interface/inamable.h"
#include "statusEffectID.h"
#include "math/graphics/color/color.h"
struct statusEffectData :public INamable
{
	statusEffectID identifier = statusEffectID();
	color particleColor = color();
	statusEffectData(const std::wstring& name, const statusEffectID& identifier, const color& particleColor) :
		INamable(name), identifier(identifier), particleColor(particleColor) {}
};
extern idList<statusEffectData*, statusEffectID> statusEffectDataList;