#include "biomeGenerator.h"
#include "block.h"
#include "cropData.h"
#include "cropBlock.h"
#include "stemPlantData.h"
#include "woodTypeData.h"
#include "jigsawPool.h"
#include "folderList.h"
#include "treeFeature.h"
#include "mushroomFeature.h"
#include "mushroomColorData.h"
#include "decayedStructureFeature.h"
#include "netherFortressFeature.h"
#include "tickableBlockContainer.h"
#include "dimension.h"
#include "math/random/random.h"
biomeGenerator::biomeGenerator(const biomeID &identifier)
{
	this->identifier = identifier;
}
void biomeGenerator::setLeave(blockContainer *containerIn, cveci2 &position, const blockID &leavesToUse)
{
	setLeaves(containerIn, position, position, leavesToUse);
}
void biomeGenerator::setLeaves(blockContainer *containerIn, cveci2 &pos00, cveci2 &pos11, const blockID &leavesToUse)
{
	containerIn->replaceBlockRange(pos00, pos11, leavesToUse, {blockID::air, blockID::grass, blockID::tall_grass, blockID::snow});
}
void biomeGenerator::placeRandomlyGrownCrop(blockContainer *containerIn, cveci2 &position, const blockID &cropID, std::mt19937 &randomToUse)
{
	// for fluid pools
	if (containerIn->getBlockID(position) == blockID::air)
	{
		cint &growthStageCount = ((cropBlock *)blockList[cropID])->growthStageCount;
		blockData *data = createBlockData(cropID);
		dynamic_cast<cropData *>(data)->age = randIndex(randomToUse, growthStageCount);
		containerIn->setBlockWithData(position, cropID, data);
	}
	containerIn->setBlockID(cveci2(position) + cveci2(0, -1), blockID::farmland);
}
void biomeGenerator::placeRandomlyGrownStemPlant(blockContainer *containerIn, cveci2 &position, const blockID &stemPlantID, std::mt19937 &randomToUse)
{
	cint stemplantType = (int)stemPlantID - (int)blockID::melon;

	const blockID &stemID = (blockID)((int)blockID::melon_stem + stemplantType);

	cbool &hasFruit = randChance(randomToUse, 2);
	stemPlantData *data = dynamic_cast<stemPlantData *>(createBlockData(stemID));
	data->age = hasFruit ? stemPlantGrowthStageCount[stemplantType] - 1 : randIndex(randomToUse, stemPlantGrowthStageCount[stemplantType]);

	containerIn->setBlockWithData(position, stemID, data);

	// grow a block on the stem
	std::vector<veci2> possibilities = std::vector<veci2>();
	for (fsize_t i = 0; i < directionCount1D; i++)
	{
		cveci2 &absoluteCheckPosition = position + directionVectors2D[i];
		if (containerIn->getBlockID(absoluteCheckPosition) == blockID::air)
		{
			possibilities.push_back(absoluteCheckPosition);
		}
	}
	if (possibilities.size())
	{
		// grow a block on one of the positions
		containerIn->setBlockID(possibilities[randIndex(currentRandom, (int)possibilities.size())], stemPlantID, chunkLoadLevel::updateLoaded);
	}
}
void biomeGenerator::setLeaveSphere(blockContainer *containerIn, cveci2 &pos00, cveci2 &pos11, cfp &topRadius, const blockID &leavesToUse)
{
	cfp middle = (pos11.x + pos00.x) * 0.5;
	// add + 0.5 to make the spheres more beautiful
	cvec2 maxDistance = cvec2((pos11.x - middle), pos11.y - pos00.y) + 0.5;
	cfp yInfluence = maxDistance.x - topRadius;
	cfp distanceMultiplier = (0.5 * math::PI) / maxDistance.y;
	for (int y = pos00.y; y <= pos11.y; y++)
	{
		cfp distance = y - pos00.y;
		cfp radius = topRadius + cos(distance * distanceMultiplier) * yInfluence;

		// floor and ceil, because in the setleaves function <= is used
		cint minX = (int)floor(middle - radius);
		cint maxX = (int)ceil(middle + radius);
		setLeaves(containerIn, cveci2(minX, y), cveci2(maxX, y), leavesToUse);
	}
}
bool biomeGenerator::placeTree(tickableBlockContainer *containerIn, cveci2 &pos, const woodTypeID &type, cbool &generating, std::mt19937 &randomToUse)
{
	// first: check light level
	if (!generating && ((int)type < normalTreeTypeCount) && containerIn->getVisibleLightLevel(pos) < glowInTheDarkLightLevel)
	{
		return false;
	}
	if (!containerIn->blockRangeContainsOnly(pos, pos, {blockID::air, (blockID)((int)blockID::wood_sapling + (int)type), blockID::grass, blockID::tall_grass, blockID::dead_bush}))
	{
		return false;
	}

	return placeTreeStructure(containerIn, pos, type, generating, randomToUse);
}

bool biomeGenerator::placeTreeStructure(tickableBlockContainer *containerIn, cveci2 &pos, const woodTypeID &type, cbool &generating, std::mt19937 &randomToUse)
{
	return placeTreeStructure(containerIn, pos, woodTypeDataList[type]->name, generating, randomToUse);
}

bool biomeGenerator::placeTreeStructure(tickableBlockContainer *containerIn, cveci2 &pos, const std::wstring &treeName, cbool &generating, std::mt19937 &randomToUse)
{
	jigsawPool *treePool = getJigsawPoolByPath(stdPath(L"tree") / treeName / L"random");
	structure *treeStructure = treePool->getRandomStructure(randomToUse);
	cveci2 &treeBottom = treeFeature().getStructureBottom(treeStructure);

	cbool &flipX = randChance(randomToUse, 2);

	treeFeature().placeStructure(treeStructure, containerIn, pos - cveci2(flipX ? ((int)treeStructure->blockIDArray.size.x - 1) - treeBottom.x : treeBottom.x, treeBottom.y), randomToUse, flipX, 0, false);
	return true;
}

bool biomeGenerator::placeIceSpike(tickableBlockContainer *containerIn, cveci2 &pos, std::mt19937 &randomToUse)
{
	jigsawPool *treePool = getJigsawPoolByPath(stdPath(L"ice_spike") / "random");
	structure *treeStructure = treePool->getRandomStructure(randomToUse);
	cveci2 &treeBottom = structureFeature().getStructureBottom(treeStructure);

	cbool &flipX = randChance(randomToUse, 2);

	structureFeature().placeStructure(treeStructure, containerIn, pos - cveci2(flipX ? ((int)treeStructure->blockIDArray.size.x - 1) - treeBottom.x : treeBottom.x, treeBottom.y), randomToUse, flipX, 0, false);
	return true;
}

bool biomeGenerator::placeHugeMushroom(tickableBlockContainer *containerIn, cveci2 &pos, std::mt19937 &randomToUse)
{
	const mushroomColorID &mushroomColor = (mushroomColorID)rand(randomToUse, (int)mushroomColorID::count - 1);
	return placeHugeMushroom(containerIn, pos, mushroomColor, randomToUse);
}

bool biomeGenerator::placeHugeMushroom(tickableBlockContainer *containerIn, cveci2 &pos, const mushroomColorID &mushroomColor, std::mt19937 &randomToUse)
{
	jigsawPool *mushroomPool = getJigsawPoolByPath(stdPath(L"mushroom") / mushroomColorDataList[mushroomColor]->name / L"random");
	structure *mushroomStructure = mushroomPool->getRandomStructure(randomToUse);
	cveci2 &mushroomBottom = mushroomFeature().getStructureBottom(mushroomStructure);

	cbool &flipX = randChance(randomToUse, 2);

	mushroomFeature().placeStructure(mushroomStructure, containerIn, pos - cveci2(flipX ? ((int)mushroomStructure->blockIDArray.size.x - 1) - mushroomBottom.x : mushroomBottom.x, mushroomBottom.y), randomToUse, flipX, 0, false);
	return true;
}

bool biomeGenerator::placeChorusTree(blockContainer *containerIn, cveci2 &pos, std::mt19937 &randomToUse)
{
	if (containerIn->getBlockID(pos + cveci2(0, -1)) != blockID::end_stone)
	{
		return false;
	}
	else
	{
		int topY = pos.y + rand(currentRandom, 0x8, 0x18);
		std::vector<int> xCoords = std::vector<int>({pos.x});
		for (int y = pos.y; y <= topY; y++)
		{
			std::vector<int> nextXCoords = std::vector<int>();
			for (size_t index = 0; index < xCoords.size(); index++)
			{
				cint x = xCoords[index];
				if (randChance(currentRandom, 0x8) || y == topY)
				{
					containerIn->setBlockID(cveci2(x, y), blockID::chorus_flower);
				}
				else if (randChance(currentRandom, 0x4))
				{
					containerIn->setBlockID(cveci2(x, y), blockID::chorus_plant);

					// split left
					if (index == 0 || (xCoords[index - 1] + 3 < x))
					{
						containerIn->setBlockID(cveci2(x - 1, y), blockID::chorus_plant);
						nextXCoords.push_back(x - 1);
					}
					// split right
					if (index == xCoords.size() - 1 || xCoords[index + 1] - 3 > x)
					{
						containerIn->setBlockID(cveci2(x + 1, y), blockID::chorus_plant);
						nextXCoords.push_back(x + 1);
					}
				}
				else
				{
					containerIn->setBlockID(cveci2(x, y), blockID::chorus_plant);
					nextXCoords.push_back(x);
				}
			}
			xCoords = nextXCoords;
		}
		return true;
	}
}
void biomeGenerator::generateTopping(blockContainer *containerIn, cveci2 &pos, const std::vector<blockID> &blocksToReplace, const blockID &block, std::mt19937 &randomToUse)
{
	generateTopping(containerIn, pos, blocksToReplace, block, rand(randomToUse, 3, 5));
}

void biomeGenerator::generateTopping(blockContainer *containerIn, cveci2 &pos, const std::vector<blockID> &blocksToReplace, const blockID &block, cint &dripDepth)
{
	containerIn->replaceBlockRange(cveci2(pos.x, pos.y - dripDepth), pos + cveci2(0, -1), block, blocksToReplace);
}

void biomeGenerator::generateCeilingTopping(blockContainer *containerIn, cveci2 &pos, const std::vector<blockID> &blocksToReplace, const blockID &block, cint &dripDepth)
{
	containerIn->replaceBlockRange(cveci2(pos.x, pos.y), pos + cveci2(0, dripDepth - 1), block, blocksToReplace);
}

bool biomeGenerator::placeCactus(blockContainer *containerIn, cveci2 &pos, std::mt19937 &randomToUse)
{
	int height = rand(randomToUse, 2, 3);
	if (containerIn->getBlockID(pos + cveci2(0, -1)) != blockID::sand)
	{
		return false;
	}
	if (!containerIn->blockRangeContainsOnly(cveci2(pos.x - 1, pos.y), cveci2(pos.x + 1, pos.y + height - 1), {blockID::air}))
	{
		return false;
	}
	containerIn->setBlockRange(pos, cveci2(pos.x, pos.y + (height - 1)), blockID::cactus);
	return true;
}

bool biomeGenerator::placeSugarCane(blockContainer *containerIn, cveci2 &pos, std::mt19937 &randomToUse)
{
	int height = rand(randomToUse, 1, 4);
	const blockID &blockBelow = containerIn->getBlockID(pos + cveci2(0, -1));
	if (!is_in(blockBelow, blockID::sand, blockID::dirt, blockID::grass_block))
	{
		return false;
	}
	if (!containerIn->blockRangeContainsOnly(pos, cveci2(pos.x, pos.y + height - 1), {blockID::air}))
	{
		return false;
	}
	containerIn->setBlockRange(pos, cveci2(pos.x, pos.y + (height - 1)), blockID::sugar_cane);
	return true;
}

void biomeGenerator::generateGrassTopping(blockContainer *containerIn, cveci2 &pos, std::mt19937 &randomToUse)
{
	generateGrassTopping(containerIn, pos, randomToUse, blockID::grass_block);
}

void biomeGenerator::generateGrassTopping(blockContainer *containerIn, cveci2 &pos, std::mt19937 &randomToUse, const blockID &grassType)
{
	const blockID &shortVersion = (blockID)(((int)grassType - (int)blockID::grass_block) + (int)blockID::grass);
	const blockID &tallVersion = (blockID)(((int)grassType - (int)blockID::grass_block) + (int)blockID::tall_grass);

	generateTopping(containerIn, cveci2(pos.x, pos.y - 1), {blockID::stone}, blockID::dirt, 3);
	containerIn->replaceBlock(cveci2(pos.x, pos.y - 1), grassType, {blockID::stone, blockID::dirt});

	if ((grassType == blockID::grass_block) || (grassType == blockID::podzol))
	{
		if (randChance(randomToUse, 0x8) && containerIn->blockRangeContainsOnly(pos, cveci2(pos.x, pos.y + 1), {blockID::air}))
		{
			containerIn->replaceBlock(pos, tallVersion, {blockID::air});
		}
		else if (randChance(randomToUse, 0x2))
		{
			containerIn->replaceBlock(pos, shortVersion, {blockID::air});
		}
		else if (randChance(randomToUse, 0x10) && (grassType == blockID::grass_block))
		{
			static const std::vector<blockID> flowersToChooseFrom = {blockID::dandelion, blockID::poppy};
			const blockID &flowerToUse = flowersToChooseFrom[randIndex(randomToUse, (int)flowersToChooseFrom.size())];
			containerIn->replaceBlock(pos, flowerToUse, {blockID::air});
		}
	}
}

void biomeGenerator::placeDesertPyramid(tickableBlockContainer *containerIn, cveci2 &pos, std::mt19937 &randomToUse)
{
	// middle bottom
	cveci2 pos00 = pos + cveci2(-10, -13);
	structure *desertPyramidStructure = getStructureByPath(L"desert_pyramid/desert_pyramid");
	structureFeature().placeStructure(desertPyramidStructure, containerIn, pos00, randomToUse, 0, false);
}

void biomeGenerator::placeJungleTemple(tickableBlockContainer *containerIn, cveci2 &pos, std::mt19937 &randomToUse)
{
	// middle bottom
	structure *junglePyramidStructure = getStructureByPath(L"jungle_pyramid/jungle_pyramid");
	structureFeature().placeStructure(junglePyramidStructure, containerIn, structureFeature().getStructureBottom(junglePyramidStructure), randomToUse, 0, false);
}

void biomeGenerator::placeShipWreck(tickableBlockContainer *containerIn, cveci2 &pos, std::mt19937 &randomToUse)
{
	cveci2 pos00 = pos + cveci2(-14, 0);
	decayedStructureFeature(&randomToUse, 0.95, 0.03).placeStructure(getStructureByPath(L"shipwreck/with_mast"), containerIn, pos00, randomToUse, 0, false);
}

void biomeGenerator::placeOceanRuins(tickableBlockContainer *containerIn, cveci2 &pos, std::mt19937 &randomToUse)
{
	jigsawPool *oceanRuinsPool = getJigsawPoolByPath(L"underwater_ruin/underwater_ruin");
	structure *ruinStructure = oceanRuinsPool->getRandomStructure(randomToUse);
	cveci2 pos00 = pos + cveci2((int)ruinStructure->blockIDArray.size.x / -2, 0);
	decayedStructureFeature(&randomToUse, 0.95, 0.05).placeStructure(ruinStructure, containerIn, pos00, randomToUse, 0, false);
}

void biomeGenerator::placeMineShaft(tickableBlockContainer *containerIn, cveci2 &pos, std::mt19937 &randomToUse)
{
	structure *railAreaStructure = getStructureByPath(L"mineshaft/rail_area");
	cveci2 pos00 = pos + cveci2((int)railAreaStructure->blockIDArray.size.x / -2, 0);
	decayedStructureFeature(&randomToUse, 0.95, 0.02).placeStructure(railAreaStructure, containerIn, pos00, randomToUse, 0x10, false);
}

void biomeGenerator::placeNamedStructure(tickableBlockContainer *containerIn, cveci2 &pos, const std::wstring &structureName, std::mt19937 &randomToUse)
{
	structure *structureToPlace = getStructureByPath(structureName);
	cveci2 pos00 = pos + cveci2((int)structureToPlace->blockIDArray.size.x / -2, 0);
	structureFeature().placeStructure(structureToPlace, containerIn, pos00, randomToUse, 0x10, false);
}

void biomeGenerator::placeNetherFortress(tickableBlockContainer *containerIn, cveci2 &pos, std::mt19937 &randomToUse)
{
	// start with a corridor balcony
	cint corridorBalconyLevel = 3; // the amount of levels before the floor
	structure *corridorBalconyStructure = getStructureByPath(L"nether_fortress/corridor_balcony");
	netherFortressFeature().placeStructure(corridorBalconyStructure, containerIn, pos + cveci2(-(int)corridorBalconyStructure->blockIDArray.size.x / 2, -(int)corridorBalconyLevel), randomToUse, 0x10);
}

void biomeGenerator::placeStronghold(tickableBlockContainer *containerIn, cveci2 &pos, std::mt19937 &randomToUse)
{
	// middle
	cveci2 pos00 = pos + cveci2(-8, -4);
	decayedStructureFeature(&randomToUse, 0.95, 0.02).placeStructure(getStructureByPath(L"stronghold/portal_room"), containerIn, pos00, randomToUse, 0x8, false);
}

void biomeGenerator::placeDungeon(tickableBlockContainer *containerIn, cveci2 &pos, std::mt19937 &randomToUse)
{
	// middle
	cveci2 pos00 = pos + cveci2(-8, 0);
	decayedStructureFeature(&randomToUse, 0.95, 0.05).placeStructure(getStructureByPath(L"dungeon/dungeon"), containerIn, pos00, randomToUse, 1, false);
}

void biomeGenerator::placeRuinedPortal(tickableBlockContainer *containerIn, cveci2 &pos, std::mt19937 &randomToUse)
{
}

void biomeGenerator::placeEndCity(tickableBlockContainer *containerIn, cveci2 &pos, std::mt19937 &randomToUse)
{
	structure *endCityBaseFloor = getStructureByPath(L"end_city/base_floor");
	cveci2 pos00 = pos + cveci2((int)endCityBaseFloor->blockIDArray.size.x / -2, 0);
	structureFeature().placeStructure(endCityBaseFloor, containerIn, pos00, randomToUse, 0x100, false);
}

void biomeGenerator::placeFossil(dimension *dimensionIn, cveci2 &pos, std::mt19937 &randomToUse)
{
	jigsawPool *fossilPool = getJigsawPoolByPath(stdPath(L"fossil") / L"fossil");
	structure *fossilStructure = fossilPool->getRandomStructure(randomToUse);
	cveci2 pos00 = pos + cveci2((int)fossilStructure->blockIDArray.size.x / -2, 0);

	if (dimensionIn->identifier == dimensionID::overworld)
	{
		decayedStructureFeature(&randomToUse, 0.9, 0).placeStructure(fossilStructure, dimensionIn, pos00, randomToUse, 0, false);
	}
	else
	{
		structureFeature().placeStructure(fossilStructure, dimensionIn, pos00, randomToUse, 1, false);
	}
}
void biomeGenerator::attemptgenerateStructures(dimension *dimensionIn, cveci2 &pos, std::mt19937 &randomToUse) const
{
}