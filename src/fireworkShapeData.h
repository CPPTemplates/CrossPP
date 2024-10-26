#pragma once
#include "interface/inamable.h"
#include "fireworkShapeID.h"
struct fireworkShapeData : INamable
{
	fireworkShapeData(const std::wstring& name) : INamable(name) {}
};
extern idList<fireworkShapeData*, fireworkShapeID> fireworkShapeDataList;