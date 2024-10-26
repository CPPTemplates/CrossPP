#pragma once
#include "structure.h"
#include "structurePlacementBlueprint.h"
//https://fabricmc.net/wiki/tutorial:jigsaw
struct structureFeature
{
	bool replaceBlocks = false;

	structureFeature(cbool& replaceBlocks = true) : replaceBlocks(replaceBlocks) {}

	bool placeStructure(structure* firstStructure, tickableBlockContainer* containerIn, cveci2& firstStructurePos00, std::mt19937& randomToUse, cint& expansionLevels = 0, cbool& includeEntities = true, cbool& replaceStructureVoid = true);
	bool placeStructure(structure* firstStructure, tickableBlockContainer* containerIn, cveci2& firstStructurePos00, std::mt19937& randomToUse, cbool& flipX, cint& expansionLevels = 0, cbool& includeEntities = true, cbool& replaceStructureVoid = true);

	virtual void placeStructurePart(structure* s, tickableBlockContainer* containerIn, cveci2& pos00, cbool& flipX, cint& levels = 0, cbool& includeEntities = true, cbool& replaceStructureVoid = true);
	//returns the relative position of a structure that always has to be connected with the ground
	virtual veci2 getStructureBottom(structure* s);
	virtual bool isBottomBlock(const blockID& b);
	virtual bool canPlace(const structurePlacementBlueprint& bluePrint, tickableBlockContainer* containerIn);

	static blockData* cloneBlockData(const blockID& id, blockData* data, cbool& flipX);

	virtual blockID getBlockToPlace(blockContainer* const& containerIn, cveci2& worldPos, const blockID& structureBlock, blockData* const& data);
};