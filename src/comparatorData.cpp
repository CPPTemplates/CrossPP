#include "comparatorData.h"
#include "tickableBlockContainer.h"
#include "jukeBoxData.h"
#include "nbt/nbtSerializer.h"
void comparatorData::serializeValue(nbtSerializer& s)
{
	inputData::serializeValue(s);
	s.serializeValue(std::wstring(L"substraction mode"), substractionMode);
}

bool comparatorData::tick(tickableBlockContainer* containerIn, cveci2& position)
{
	inputData::tick(containerIn, position);
	cveci2& positionBehind = position - directionVectors2D[(int)directionFacing];
	const blockID& blockBehind = containerIn->getBlockID(positionBehind);
	const blockData& blockDataBehind = *containerIn->getBlockData(positionBehind);
	switch (blockBehind)
	{
	case blockID::jukebox:
		lastPowerLevelBehind = (int)((const jukeBoxData&)blockDataBehind).recordSlot->slots[0].stackItemID - (int)itemID::music_disc;
		break;
	default:
		break;
	}
	return false;
}