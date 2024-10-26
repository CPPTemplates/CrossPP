#include "stemPlantData.h"
#include "tickableBlockContainer.h"
#include "nbt/nbtSerializer.h"
#include "nbt/serializeColor.h"
#include "constants/vectorConstants.h"
#include "include/math/random/random.h"
void stemPlantData::randomTick(tickableBlockContainer* containerIn, cveci2& position)
{
	if (relativeFruitPosition == cveci2())
	{
		cropData::randomTick(containerIn, position);
		cint stemPlantType = (int)containerIn->getBlockID(position) - (int)blockID::melon_stem;
		if (age == (stemPlantGrowthStageCount[stemPlantType] - 1))
		{
			if (randFp() < stemPlantChanceToGrow[stemPlantType])
			{
				attemptToGrowFruit(containerIn, position);
			}
		}
	}
}

bool stemPlantData::tick(tickableBlockContainer* containerIn, cveci2& position)
{
	if (relativeFruitPosition != cveci2())
	{
		cint stemPlantType = (int)containerIn->getBlockID(position) - (int)blockID::melon_stem;
		if (containerIn->getBlockID(position + relativeFruitPosition) != (blockID)((int)blockID::melon + stemPlantType))
		{
			relativeFruitPosition = cveci2();
		}
	}
	return false;
}

void stemPlantData::serializeValue(nbtSerializer& s)
{
	cropData::serializeValue(s);
	serializeNBTValue(s, std::wstring(L"relative fruit position"), relativeFruitPosition);
}

void stemPlantData::attemptToGrowFruit(tickableBlockContainer* containerIn, cveci2& position)
{
	cint stemPlantType = (int)containerIn->getBlockID(position) - (int)blockID::melon_stem;
	//grow a block on the stem
	std::vector<veci2> possibilities = std::vector<veci2>();
	for (fsize_t i = 0; i < directionCount1D; i++)
	{
		cveci2& absoluteCheckPosition = position + directionVectors2D[i];
		if (containerIn->getBlockID(absoluteCheckPosition) == blockID::air)
		{
			possibilities.push_back(directionVectors2D[i]);
		}
	}
	if (possibilities.size())
	{
		//grow a block on one of the positions
		csize_t& index = randIndex(currentRandom, possibilities.size());
		relativeFruitPosition = possibilities[index];
		containerIn->setBlockID(position + relativeFruitPosition, (blockID)((int)blockID::melon + stemPlantType), chunkLoadLevel::updateLoaded);
	}
}