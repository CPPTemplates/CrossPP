#include "treeFeature.h"
#include "idAnalysis.h"
#include "tickableBlockContainer.h"
bool treeFeature::isBottomBlock(const blockID& b)
{
	return isTreeType(b) && (getTreeItemType(b) == treeItemTypeID::log);
}

blockID treeFeature::getBlockToPlace(blockContainer* const& containerIn, cveci2& worldPos, const blockID& structureBlock, blockData* const& data)
{
	if ((isTreeType(structureBlock) && (getTreeItemType(structureBlock) == treeItemTypeID::leaves)) || isMushroom(structureBlock) || isMushroomBlock(structureBlock) || (structureBlock == blockID::vine))
	{
		if (!canReplaceByTree(containerIn->getBlockID(worldPos)))
		{
			return blockID::structure_void;
		}
	}

	return structureBlock;
}

bool treeFeature::canPlace(const structurePlacementBlueprint& bluePrint, tickableBlockContainer* containerIn)
{
	//check if there are blocks in the way of the logs

	cmat3x3i& structureToWorldTransform = bluePrint.placedStructure->getBlocksToWorldTransform(bluePrint.structurePos00, bluePrint.flipX);

	crectanglei2& structureRect = crectanglei2((veci2)bluePrint.placedStructure->blockIDArray.size);

	for (cveci2& structurePos : structureRect)
	{
		const blockID& structureBlock = bluePrint.placedStructure->getBlockID(structurePos);
		if (isTreeType(structureBlock) && (getTreeItemType(structureBlock) == treeItemTypeID::log))
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