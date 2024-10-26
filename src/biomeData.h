#pragma once
#include "math/graphics/color/color.h"
#include <string>
#include "idList.h"
#include "biomeID.h"
#include "interface/inamable.h"

struct biomeData : INamable
{
	color biomeColor;
	color grassColor = color();
	biomeData(const std::wstring& name, const color& biomeColor, const color& grassColor) : INamable(name), biomeColor(biomeColor), grassColor(grassColor) {}
};
extern idList<biomeData*, biomeID> biomeDataList;