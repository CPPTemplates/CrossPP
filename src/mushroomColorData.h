#pragma once
#include "interface/inamable.h"
struct mushroomColorData : INamable
{
	mushroomColorData(const std::wstring& name) : INamable(name) {}
};
extern idList<mushroomColorData*, mushroomColorID> mushroomColorDataList;