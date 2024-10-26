#include "repeaterData.h"
#include "tickableBlockContainer.h"
#include "nbt/nbtSerializer.h"
bool repeaterData::tick(tickableBlockContainer* containerIn, cveci2& position)
{
	inputData::tick(containerIn, position);
	const lightLevel& oldOutputLevel = containerIn->getEmittanceLevel(position, levelID::powerLevel);
	std::copy(delayedPowerLevels, delayedPowerLevels + delayArray[delayArraySize - 1] - 1, delayedPowerLevels + 1);

	delayedPowerLevels[0] = lastPowerLevelBehind;

	if (lastPowerLevelBelow == 0)
	{
		lockedPowerLevel = delayedPowerLevels[0];
		cint constantPowerLevel = delayedPowerLevels[0];
		powerLevel* endPtr = delayedPowerLevels + delayArray[delayArrayIndex];

		const lightLevel& newOutputLevel = containerIn->getEmittanceLevel(position, levelID::powerLevel);

		if (newOutputLevel != oldOutputLevel)
		{
			containerIn->addUpdatePosition(position);
		}

		if (std::find_if(&delayedPowerLevels[0], endPtr, [&constantPowerLevel](const auto& a) { return a != constantPowerLevel; }) != endPtr)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void repeaterData::serializeValue(nbtSerializer& s)
{
	inputData::serializeValue(s);
	s.serializeValue(std::wstring(L"delay array index"), delayArrayIndex);
	s.serializeArray(std::wstring(L"delayed powerlevels"), delayedPowerLevels, delayedPowerLevelArraySize);
}