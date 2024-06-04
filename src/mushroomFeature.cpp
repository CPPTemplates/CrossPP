#include "mushroomFeature.h"
#include "idAnalysis.h"
#include "tickableBlockContainer.h"
bool mushroomFeature::isBottomBlock(const blockID& b)
{
	return (b == blockID::mushroom_stem);
}

blockID mushroomFeature::getBlockToPlace(blockContainer* const& containerIn, cveci2& worldPos, const blockID& structureBlock, blockData* const& data)
{
	if (isMushroomBlock(structureBlock))
	{
		if (!canReplaceByTree(containerIn->getBlockID(worldPos)))
		{
			return blockID::structure_void;
		}
	}
	return structureBlock;
}

bool mushroomFeature::canPlace(const structurePlacementBlueprint& bluePrint, tickableBlockContainer* containerIn)
{
	//check if there are blocks in the way of the logs

	cmat3x3i& structureToWorldTransform = bluePrint.placedStructure->getBlocksToWorldTransform(bluePrint.structurePos00, bluePrint.flipX);

	crectanglei2& structureRect = crectanglei2(cveci2(), cveci2(bluePrint.placedStructure->blockIDArray.size));

	for (cveci2& structurePos : structureRect)
	{
		const blockID& structureBlock = bluePrint.placedStructure->getBlockID(structurePos);
		if (structureBlock == blockID::mushroom_stem)
		{
			const blockID& worldBlock = containerIn->getBlockID(structureToWorldTransform.multPointMatrix(structurePos));
			if (!canReplaceByTree(worldBlock))
			{
				return false;
			}
		}
	}
	return true;
}