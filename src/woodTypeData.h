#pragma once
#include "woodtypeID.h"
#include "interface/inamable.h"
#include "idList.h"
struct woodTypeData : INamable
{
	woodTypeData(const std::wstring& name) : INamable(name) {}
};
extern idList<woodTypeData*, woodTypeID> woodTypeDataList;