#pragma once
#include "interface/inamable.h"
#include "enchantmentID.h"
#include "array/fastlist.h"
#include "math/rectangle/rectangletn.h"
#include "idList.h"
struct enchantmentData :public INamable
{
	enchantmentID identifier = (enchantmentID)0;
	int maxLevel = 0;
	int weight = 0;
	fastList<rectanglei1> powerRanges;
	enchantmentData(const std::wstring& name, const enchantmentID& identifier, cint& maxLevel, const fastList<veci2>& powerRangesList, cint& weight) :
		INamable(name), identifier(identifier), maxLevel(maxLevel), weight(weight)
	{
		powerRanges = fastList<rectanglei1>(powerRangesList.size, false);

		for (size_t i = 0; i < powerRanges.size; i++)
		{
			powerRanges[i] = rectanglei1(veci1(powerRangesList[i].x), veci1(powerRangesList[i].y));
		}
	}
};
extern idList<enchantmentData*, enchantmentID> enchantmentDataList;