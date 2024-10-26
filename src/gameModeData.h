#pragma once
#include "interface/inamable.h"
#include "idList.h"
#include "gameModeID.h"
struct gameModeData : public INamable
{
	bool canFly = false;
	bool canTakeDamage = false;
	gameModeData(const std::wstring& name, cbool& canFly, cbool& canTakeDamage) :INamable(name), canFly(canFly), canTakeDamage(canTakeDamage) {}
};
extern idList<gameModeData*, gameModeID> gameModeDataList;