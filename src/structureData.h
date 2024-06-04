#pragma once
#include "interface/inamable.h"
#include "structureID.h"
#include "idList.h"
struct structureData : INamable
{
	structureData(const std::wstring& name) : INamable(name) {}
};
extern idList<structureData*, structureID> structureDataList;