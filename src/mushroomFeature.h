#pragma once
#include "structureFeature.h"
struct mushroomFeature : structureFeature
{
	virtual bool isBottomBlock(const blockID& b) override;
	virtual blockID getBlockToPlace(blockContainer* const& containerIn, cveci2& worldPos, const blockID& structureBlock, blockData* const& data) override;
	virtual bool canPlace(const structurePlacementBlueprint& bluePrint, tickableBlockContainer* containerIn) override;
};