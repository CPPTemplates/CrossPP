#include "lastPowerLevelData.h"
#include "tickableBlockContainer.h"
#include "nbt/nbtSerializer.h"
bool lastPowerLevelData::tick(tickableBlockContainer* containerIn, cveci2& position)
{
	lastPowerLevel = containerIn->getPowerLevel(position);
	return false;
}

void lastPowerLevelData::serializeValue(nbtSerializer& s)
{
	s.serializeValue(std::wstring(L"last power level"), (int&)lastPowerLevel);
}