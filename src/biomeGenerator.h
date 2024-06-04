#pragma once
#include "interface/idestructable.h"
#include "biomeID.h"
#include "itemID.h"
#include "blockContainer.h"
#include "mushroomColorID.h"

crectangle1 hillyNoiseRange = crectangle1::fromOppositeCorners(cvec1(0), cvec1(0x20));
crectangle1 flatNoiseRange = crectangle1::fromOppositeCorners(cvec1(0), cvec1(0x10));


struct biomeGenerator :IDestructable
{
	biomeGenerator(const biomeID& identifier);
	biomeID identifier = biomeID();

	static void setLeave(blockContainer* containerIn, cveci2& position, const blockID& leavesToUse);
	static void setLeaves(blockContainer* containerIn, cveci2& pos00, cveci2& pos11, const blockID& leavesToUse);

	static void placeRandomlyGrownCrop(blockContainer* containerIn, cveci2& position, const blockID& cropID, std::mt19937& randomToUse);
	static void placeRandomlyGrownStemPlant(blockContainer* containerIn, cveci2& position, const blockID& stemPlantID, std::mt19937& randomToUse);
	//CAUTION! topRADIUS, not topWIDTH
	static void setLeaveSphere(blockContainer* containerIn, cveci2& pos00, cveci2& pos11, cfp& topRadius, const blockID& leavesToUse);
	static bool placeTree(tickableBlockContainer* containerIn, cveci2& pos, const woodTypeID& type, cbool& generating, std::mt19937& randomToUse);
	static bool placeCactus(blockContainer* containerIn, cveci2& pos, std::mt19937& randomToUse);
	static bool placeSugarCane(blockContainer* containerIn, cveci2& pos, std::mt19937& randomToUse);
	static bool placeTreeStructure(tickableBlockContainer* containerIn, cveci2& pos, const woodTypeID& type, cbool& generating, std::mt19937& randomToUse);
	static bool placeTreeStructure(tickableBlockContainer* containerIn, cveci2& pos, const std::wstring& treeName, cbool& generating, std::mt19937& randomToUse);
	static bool placeIceSpike(tickableBlockContainer* containerIn, cveci2& pos, std::mt19937& randomToUse);
	static bool placeHugeMushroom(tickableBlockContainer* containerIn, cveci2& pos, std::mt19937& randomToUse);
	static bool placeHugeMushroom(tickableBlockContainer* containerIn, cveci2& pos, const mushroomColorID& mushroomColor, std::mt19937& randomToUse);
	static bool placeChorusTree(blockContainer* containerIn, cveci2& pos, std::mt19937& randomToUse);
	static void generateTopping(blockContainer* containerIn, cveci2& pos, const std::vector<blockID>& blocksToReplace, const blockID& block, std::mt19937& randomToUse);
	static void generateTopping(blockContainer* containerIn, cveci2& pos, const std::vector<blockID>& blocksToReplace, const blockID& block, cint& dripDepth = 4);
	static void generateCeilingTopping(blockContainer* containerIn, cveci2& pos, const std::vector<blockID>& blocksToReplace, const blockID& block, cint& dripDepth = 4);
	static void generateGrassTopping(blockContainer* containerIn, cveci2& pos, std::mt19937& randomToUse);
	static void generateGrassTopping(blockContainer* containerIn, cveci2& pos, std::mt19937& randomToUse, const blockID& grassType);
	static void placeDesertPyramid(tickableBlockContainer* containerIn, cveci2& pos, std::mt19937& randomToUse);
	static void placeShipWreck(tickableBlockContainer* containerIn, cveci2& pos, std::mt19937& randomToUse);
	static void placeJungleTemple(tickableBlockContainer* containerIn, cveci2& pos, std::mt19937& randomToUse);
	static void placeOceanRuins(tickableBlockContainer* containerIn, cveci2& pos, std::mt19937& randomToUse);
	static void placeMineShaft(tickableBlockContainer* containerIn, cveci2& pos, std::mt19937& randomToUse);
	static void placeNamedStructure(tickableBlockContainer* containerIn, cveci2& pos, const std::wstring& structureName, std::mt19937& randomToUse);
	static void placeNetherFortress(tickableBlockContainer* containerIn, cveci2& pos, std::mt19937& randomToUse);
	static void placeStronghold(tickableBlockContainer* containerIn, cveci2& pos, std::mt19937& randomToUse);
	static void placeDungeon(tickableBlockContainer* containerIn, cveci2& pos, std::mt19937& randomToUse);
	static void placeRuinedPortal(tickableBlockContainer* containerIn, cveci2& pos, std::mt19937& randomToUse);

	static void placeEndCity(tickableBlockContainer* containerIn, cveci2& pos, std::mt19937& randomToUse);

	static void placeFossil(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse);

	//pos.x: the x coordinate
	//pos.y: the exact height at which the ground is (30 means stone is at 0 to 29)
	virtual void attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const;
};