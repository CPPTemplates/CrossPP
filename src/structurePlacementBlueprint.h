#pragma once
#include "structure.h"
struct structurePlacementBlueprint
{
	structurePlacementBlueprint() {};
	structurePlacementBlueprint(structure* placedStructure, veci2 structurePos00, std::vector<veci2> positionsToConnect, int structureLevel, bool flipX) :
		placedStructure(placedStructure), structurePos00(structurePos00), positionsToConnect(positionsToConnect), structureLevel(structureLevel), flipX(flipX) {}
	structurePlacementBlueprint(structure* placedStructure, veci2 structurePos00, int structureLevel, bool flipX) :
		placedStructure(placedStructure), structurePos00(structurePos00), positionsToConnect(placedStructure->findBlocks(blockID::jigsaw)), structureLevel(structureLevel), flipX(flipX) {}

	structure* placedStructure = nullptr;
	veci2 structurePos00 = veci2();
	std::vector<veci2> positionsToConnect = std::vector<veci2>();
	int structureLevel = 0;
	bool flipX = false;
};