#include "sapSpreadData.h"
#include "tickableBlockContainer.h"
#include "nbt/nbtSerializer.h"
void sapSpreadData::serializeValue(nbtSerializer &s)
{
	s.serializeValue(std::wstring(L"sap level"), sapLevel);
}

void sapSpreadData::randomTick(tickableBlockContainer *containerIn, cveci2 &position)
{
	int maxAdjacentSaplevel = 0;
	const woodTypeID &woodType = getWoodType(containerIn->getBlockID(position));
	// spread saps
	for (fsize_t i = 0; i < directionCount2D; i++)
	{
		cveci2 &adjacentCheckPosition = position + directionVectors2D[i];

		blockID adjacentBlockID = containerIn->getBlockID(adjacentCheckPosition);
		if (isTreeType(adjacentBlockID))
		{
			treeItemTypeID t = getTreeItemType(adjacentBlockID);
			if (getWoodType(adjacentBlockID) == woodType)
			{
				if (t == treeItemTypeID::log)
				{
					// a lot of fluid flows into this leave
					sapLevel = maxSapLevel;
					return;
				}
				else if (t == treeItemTypeID::leaves)
				{
					sapSpreadData *adjacentBlockData = dynamic_cast<sapSpreadData *>(containerIn->getBlockData(adjacentCheckPosition));
					maxAdjacentSaplevel = math::maximum(maxAdjacentSaplevel, adjacentBlockData->sapLevel);
				}
			}
		}
	}
	sapLevel = maxAdjacentSaplevel - 1;

	if (sapLevel <= 0)
	{
		containerIn->setBlockID(position, blockID::air);
	}
}
