#include "inputData.h"
#include "tickableBlockContainer.h"
#include "nbt/nbtSerializer.h"
bool inputData::tick(tickableBlockContainer* containerIn, cveci2& position)
{
	if (attached(containerIn, position))
	{
		cveci2& behindPosition = position + directionVectors2D[(int)flipDirection(directionFacing)];
		cveci2& belowPosition = position + cveci2(0, -1);
		const powerLevel& newPowerLevelBehind = math::maximum(containerIn->getPowerLevel(behindPosition) - containerIn->getDecayStrengthExitingFrom(behindPosition, position, levelID::powerLevel), 0);
		const powerLevel& newPowerLevelBelow = math::maximum(containerIn->getPowerLevel(belowPosition) - containerIn->getDecayStrengthExitingFrom(belowPosition, position, levelID::powerLevel), 0);
		if ((newPowerLevelBehind != lastPowerLevelBehind) || (newPowerLevelBelow != lastPowerLevelBelow))
		{
			lastPowerLevelBehind = newPowerLevelBehind;
			lastPowerLevelBelow = newPowerLevelBelow;
			containerIn->addUpdatePosition(position);
		}
		return false;
	}
	else
	{
		return attachedBlockData::tick(containerIn, position);
	}
}

void inputData::serializeValue(nbtSerializer& s)
{
	blockData::serializeValue(s);
	facingData::serializeValue(s);
	s.serializeValue(std::wstring(L"last power level behind"), (int&)lastPowerLevelBehind);
	s.serializeValue(std::wstring(L"last power level below"), (int&)lastPowerLevelBelow);
}