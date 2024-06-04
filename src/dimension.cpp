#include "cpuUsageID.h"
#include "filesystem/filemanager.h"
#include "dimension.h"
#include "dimensionData.h"
#include "world.h"
#include "folderList.h"
#include "chunk.h"
#include "benchMark.h"
#include "mob.h"
#include <vector>
#include <math.h>
#include <map>
#include <string>
#include <utility>
#include "arrayDataType.h"
#include "biomeID.h"
#include "itemID.h"
#include "chunkLoadLevel.h"
#include "collisionData.h"
#include "collisionDataCollection.h"
#include "collisionTypeID.h"
#include "constants.h"
#include "dimensionID.h"
#include "entity.h"
#include "entityID.h"
#include "idAnalysis.h"
#include "array/arraynd/arraynd.h"
#include "array/fastlist.h"
#include "globalFunctions.h"
#include "math/collisions.h"
#include "math/graphics/brush/brushes.h"
#include "math/graphics/color/color.h"
#include "math/mathFunctions.h"
#include "math/mattnxn.h"
#include "math/random/random.h"
#include "math/rectangle/rectangletn.h"
#include "math/graphics/texture.h"
#include "math/uuid.h"
#include "math/vector/vectn.h"
#include "optimization/benchmark.h"
#include "levelID.h"
#include "lightLevel.h"
#include "lightLevelID.h"
#include "nbt/nbtCompound.h"
#include "nbt/nbtData.h"
#include "nbt/nbtDataTag.h"
#include "nbt/nbtSerializer.h"
#include "structureID.h"
#include "tickableBlockContainer.h"
#include "math/algorithm/findInCircles.h"
#include "math/graphics/brush/brushes/bilinearInterpolator.h"
#include "array/arraynd/arrayndFunctions.h"
#include "nbt/nbtSerializer.h"
#include "include/filesystem/fileFunctions.h"
#include "nbt/serializeColor.h"

color dimension::getColorMultiplier(cfp& sunLight, cfp& blockLight) const
{
	const texture& lightMapTexture = *dimensionDataList[identifier]->lightMapTexture->scaledTextures[0];
	cfp preciseSunlightLevel = (timeToLightLevel.getValue(currentWorld->getTimeOfDay()) / maxLightLevel) * (fp)(lightMapTexture.size.x - 2);//-2 for thunder

	cfsize_t& quarter = lightMapTexture.size.y / 4;

	cfp sunLightYRow = lightMapTexture.size.y - (sunLight / maxLightLevel) * (quarter - 1) - 1;
	cfp blockLightYRow = (lightMapTexture.size.y - quarter) - ((blockLight / maxLightLevel) * (quarter - 1) + 1);
	const auto& interpolator = bilinearInterpolator(lightMapTexture);

	return color::maximizeColors(
		interpolator.getValue(cvec2(currentWorld->ticksSinceStart % (int)lightMapTexture.size.x, blockLightYRow)),
		interpolator.getValue(cvec2(preciseSunlightLevel, sunLightYRow))
	);
}

veci2 dimension::searchPortal(cveci2& positionNear)
{
	cfp netherPortalSearchRange = identifier == dimensionID::nether ? netherPortalSearchRangeNether : netherPortalSearchRangeOverWorld;
	fp minimumDistanceSquared = INFINITY;
	size_t portalIndex = 0;
	for (size_t i = 0; i < portalPositions.size(); i++)
	{
		veci2 location = portalPositions[i];
		cint distanceSquared = (location - positionNear).lengthSquared();
		if (distanceSquared < minimumDistanceSquared)
		{
			portalIndex = i;
			minimumDistanceSquared = distanceSquared;
		}
	}
	if (minimumDistanceSquared < math::squared(netherPortalSearchRange))
	{
		return portalPositions[portalIndex];
	}
	else
	{
		//create new portal
		//check random positions if it is a good place for a nether portal

		cint portalTryCount = 0x100;
		veci2 portalPos = positionNear;
		cint netherSpawnSearchRange = (int)(netherPortalSearchRange * 0.8);
		for (int i = 0; i < portalTryCount; i++)
		{
			cveci2 pos = veci2(positionNear.x + rand(currentRandom, -netherSpawnSearchRange, netherSpawnSearchRange),
				rand(currentRandom, netherLavaLevel, netherCeilingStart));
			if (getBlockID(pos + cveci2(0, -1), chunkLoadLevel::updateLoaded) == blockID::air &&
				getBlock(pos + cveci2(0, -2), chunkLoadLevel::updateLoaded)->blockCollisionType == collisionTypeID::willCollide)
			{
				portalPos = pos;
				break;
			}
		}
		//build portal
		//build border
		setBlockRange(veci2(portalPos + cveci2(-1, -1)), portalPos + cveci2(minimumPortalWidth, minimumPortalHeight), blockID::obsidian);
		//fill with portal frames
		setBlockRange(veci2(portalPos + cveci2(0, 0)), portalPos + cveci2(minimumPortalWidth - 1, minimumPortalHeight - 1), blockID::portal, chunkLoadLevel::updateLoaded);
		portalPositions.push_back(portalPos);
		return portalPos;
	}
}

bool dimension::serialize(cbool& write)
{
	const stdPath& worldFolder = savesFolder / currentWorld->name;
	const stdPath& dimensionFolder = worldFolder / dimensionDataList[identifier]->name;
	if (write)
	{
		createFolderIfNotExists(worldFolder);
		createFolderIfNotExists(dimensionFolder);
	}

	const stdPath& path = dimensionFolder / (std::wstring(L"dimensionData") + nbtFileExtension);

	nbtCompound* compound = nullptr;
	if (write)
	{
		compound = new nbtCompound(std::wstring(L"dimension"));
	}
	else
	{
		if (!nbtCompound::serialize(compound, write, path))
		{
			return false;
		}
	}
	nbtSerializer s = nbtSerializer(*compound, write);
	serializeValue(s);
	if (write)
	{
		if (!nbtCompound::serialize(compound, write, path))
		{

			delete compound;
			return false;
		}
	}

	delete compound;
	return true;
}

void dimension::serializeValue(nbtSerializer& s)
{
	if (s.push<nbtDataTag::tagList>(std::wstring(L"portal positions")))
	{
		if (s.write)
		{
			for (veci2& portalPosition : portalPositions)
			{
				if (s.push<nbtDataTag::tagCompound>())
				{
					serializeNBTValue(s, std::wstring(L"position"), portalPosition);
					s.pop();
				}
			}
		}
		else
		{
			const std::vector<nbtData*>& positionDataList = s.getChildren();
			for (nbtData* positionData : positionDataList)
			{
				if (s.push(positionData))
				{
					veci2 pos;
					if (serializeNBTValue(s, std::wstring(L"position"), pos))
					{
						portalPositions.push_back(pos);
					}
					s.pop();
				}
			}
		}
		s.pop();
	}

	if (s.push<nbtDataTag::tagList>(std::wstring(L"loaded chunks")))
	{
		if (s.write)
		{
			for (const auto& it : loadedChunksMap)
			{
				if (s.push<nbtDataTag::tagCompound>())
				{
					serializeNBTValue(s, std::wstring(L"chunk coordinates"), it.second->chunkCoordinates);
					it.second->serialize(s.write);
					s.pop();
				}
			}
		}
		else
		{
			const std::vector<nbtData*>& chunkDataList = s.getChildren();
			loadedChunksMap = std::vector<std::pair< veci2, chunk*>>();
			for (nbtData* chunkData : chunkDataList)
			{
				if (s.push(chunkData))
				{
					veci2 chunkCoordinates;
					serializeNBTValue(s, std::wstring(L"chunk coordinates"), chunkCoordinates);
					chunk* c = new chunk(this, chunkCoordinates);
					c->generateArrays();
					loadedChunksMap.push_back(std::pair<veci2, chunk*>(chunkCoordinates, c));
					recalculateChunkArray();

					c->increaseLoadLevel(chunkLoadLevel::worldGenerationLoaded);
					s.pop();
				}
			}
		}
		s.pop();
	}
	if (s.push<nbtDataTag::tagList>(std::wstring(L"blockupdate positions")))
	{
		//list of block update positions
		if (s.write)
		{
			for (fsize_t i = 0; i < blockUpdatePositions.size; i++)
			{
				if (s.push())
				{
					serializeNBTValue(s, std::wstring(L"position"), blockUpdatePositions[i]);
					s.pop();
				}
			}
		}
		else
		{
			const std::vector<nbtData*>& dataList = s.getChildren();
			blockUpdatePositions = fastList<veci2>((int)dataList.size());
			int i = 0;
			for (nbtData* data : dataList)
			{
				if (s.push(data))
				{
					serializeNBTValue(s, std::wstring(L"position"), blockUpdatePositions[i]);
					s.pop();
				}
				i++;
			}
		}
		s.pop();
	}
}

void dimension::tick()
{
	const auto copy = loadedChunksMap;

	currentBenchmark->addBenchmarkPoint(cpuUsageID::entities);

	for (const auto& it : copy)
	{
		it.second->tick();
	}

	currentBenchmark->addBenchmarkPoint(cpuUsageID::physics);

	for (const auto& it : copy)
	{
		it.second->physics();
	}

	currentBenchmark->addBenchmarkPoint(cpuUsageID::blockUpdates);
	for (const auto& it : copy)
	{
		it.second->randomTick();
	}

	tickableBlockContainer::tick();
	currentBenchmark->addBenchmarkPoint(cpuUsageID::lightUpdates);
	//FIRST BLOCK UPDATES, THEN LIGHT LEVELS
	reCalculateLevels();
	currentBenchmark->addBenchmarkPoint(cpuUsageID::miscellaneous);
}
void dimension::recalculateChunkArray()
{
	//list all loaded chunks again
	if (loadedChunksMap.size())
	{
		loadedChunksRange = crectanglei2(loadedChunksMap.begin()->first, cveci2(1));
		//calculate minimum and maximum
		for (const auto& it : loadedChunksMap)
		{
			loadedChunksRange.expandToContain(crectanglei2(it.first, cveci2(1)));
		}
	}
	else
	{
		loadedChunksRange = crectanglei2();
	}
	loadedChunksFastArray = array2d<chunk*>(loadedChunksRange.size, true);
	for (const auto& it : loadedChunksMap)
	{
		loadedChunksFastArray.setValueUnsafe(it.first - loadedChunksRange.pos0, it.second);
	}
}
dimension::dimension(const dimensionID& identifier) :identifier(identifier), loadedChunksMap(std::vector<std::pair< veci2, chunk*>>())
{
	rootDimension = this;
}
void dimension::initialize()
{
}
bool dimension::locateBiomes(const std::vector<biomeID>& biomesToLocate, cvec2& initialPosition, vec2& resultingPosition) const
{
	constexpr fp step = 0x20;
	vec2 searchPosition[2] = { initialPosition, initialPosition };
	constexpr fp searchStep[2] = { -step, step };
	int chunkSearchRadius = 0x1000;
	for (int i = 0; i < chunkSearchRadius; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			searchPosition[j].x += searchStep[j];
			cvec2& currentPosition = searchPosition[j];
			if (std::find(biomesToLocate.begin(), biomesToLocate.end(), getBiome(currentPosition)) != biomesToLocate.end())
			{
				resultingPosition = currentPosition;
				return true;
			}
		}
	}
	return false;
}

bool dimension::locateStructureChunkCoordinates(const structureID& id, cveci2& initialChunkCoordinates, veci2& resultingChunkCoordinates)
{
	//expand outwards from position
	constexpr int chunkSearchRadius = 0x100;
	auto checkFunction = [this, &id, &initialChunkCoordinates](cveci2& pos) {
		const chunk& c = chunk(this, pos + initialChunkCoordinates);
		return std::find(c.generatedStructures.begin(), c.generatedStructures.end(), id) != c.generatedStructures.end();
		};
	if (findInCircles(chunkSearchRadius, checkFunction, resultingChunkCoordinates)) {
		resultingChunkCoordinates += initialChunkCoordinates;
		return true;
	}
	return false;
}

bool dimension::locateStructure(const structureID& id, cvec2& initialPosition, vec2& resultingPosition)
{
	cveci2& initialChunkPosition = getChunkCoordinates(initialPosition);
	veci2 resultingChunkCoordinates;
	if (locateStructureChunkCoordinates(id, initialChunkPosition, resultingChunkCoordinates)) {
		resultingPosition = ((vec2)resultingChunkCoordinates * ((vec2)chunkSize) + 0.5);
		return true;
	}
	else {
		return false;
	}
}
bool dimension::meetsSpawningConditions(entity* const& e)
{

	//check if the mob is not outside the chunk

	//---
	//-w-
	//?-?
	//?#?
	cbool& inWater = e->getFluidArea(e->calculateHitBox(), { blockID::water }) > 0;

	cvec2& absoluteHitboxTopLeft = e->position + e->relativeHitbox.pos0;
	if (inWater)
	{
		if (!canSpawnInWater(e->entityType))
		{
			return false;
		}
	}
	else
	{
		if (!canSpawnOutsideWater(e->entityType))
		{
			return false;
		}

		//check if the floor does collide
		if ((e->entityType != entityID::blaze) && (e->entityType != entityID::ghast) && (e->entityType != entityID::drowned))
		{
			cvec2 posStandingOn = absoluteHitboxTopLeft + cvec2(e->relativeHitbox.w * 0.5, -1);
			crectangle2 floorRectangle = crectangle2(absoluteHitboxTopLeft.x, posStandingOn.y, e->relativeHitbox.size.x, 1);
			collisionTypeID floorCollision = getHitboxCollisionType(floorRectangle);
			if (floorCollision == collisionTypeID::willCollide)
			{
				const blockID& blockStandingOn = getBlockID(floorVector(posStandingOn));
				if (blockStandingOn == blockID::bedrock)
				{
					return false;
				}
				else if (isGrassEatingMob(e->entityType) && (blockStandingOn != blockID::grass_block))
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}

	//check if the room does not collide
	collisionTypeID topCollision = getHitboxCollisionType(crectangle2(absoluteHitboxTopLeft, e->relativeHitbox.size));
	if (topCollision == collisionTypeID::willCollide)
	{
		return false;
	}

	if (isMob(e->entityType))
	{
		mob* mobToSpawn = (mob*)e;
		//check if the room above has water at head level
		cveci2 flooredHeadPosition = floorVector(mobToSpawn->getHeadPosition());
		const lightLevel visibleLightLevel = getVisibleLightLevel(flooredHeadPosition);
		const lightLevel visibleSunLightLevel = getVisibleSunLightLevel(getInternalSunLightLevel(flooredHeadPosition));
		//room to spawn more of those mobs
		if (isUndeadMob(e->entityType) || (e->entityType == entityID::enderman) || (e->entityType == entityID::creeper))
		{
			//feet sunlightlevel = head sunlightlevel
			//> to allow hostile mobs to spawn in spawners
			if (visibleLightLevel > hostileMobSpawnTreshold)
			{
				return false;
			}
		}
		else if (isPassiveMob(e->entityType) || e->entityType == entityID::wolf)
		{
			if (visibleSunLightLevel <= hostileMobSpawnTreshold
				)
			{
				return false;
			}
		}
	}
	if (!isImmuneToFire(e->entityType) && e->getFluidArea(e->calculateHitBox(), { blockID::lava }))
	{
		return false;
	}
	return true;
}
dimension::~dimension()
{
	for (const auto& it : loadedChunksMap)
	{
		delete it.second;
	}
}

std::vector<entity*> dimension::findNearEntities(cvec2& position, cfp& radius)
{
	std::vector<entity*> list = std::vector<entity*>();
	std::vector<chunk*> nearChunks = getLoadedChunks(getTouchingChunkCoordinateRadius(position, radius));
	cfp radiusSquared = radius * radius;
	for (chunk* c : nearChunks)
	{
		for (entity* e : c->entityList) {
			if ((e->position - position).lengthSquared() < radiusSquared)
			{
				list.push_back(e);
			}
		}
	}
	return list;
}

std::vector<entity*> dimension::findNearEntities(crectangle2& searchBox)
{
	std::vector<entity*> list = std::vector<entity*>();
	std::vector<chunk*> nearChunks = getLoadedChunks(getTouchingChunkCoordinateRange(searchBox));
	for (chunk* c : nearChunks)
	{
		for (entity* e : c->entityList) {
			if (searchBox.contains(e->position))
			{
				list.push_back(e);
			}
		}
	}
	return list;
}

std::vector<chunk*> dimension::getLoadedChunks(crectanglei2& range)
{
	std::vector<chunk*> list = std::vector<chunk*>();

	for (const auto& it : loadedChunksMap)
	{
		chunk* c = it.second;

		if (range.contains(c->chunkCoordinates))
		{
			list.push_back(c);
		}

	}
	return list;
}
crectanglei2 dimension::getTouchingChunkCoordinateRadius(cvec2& position, cfp& radius)
{
	return getTouchingChunkCoordinateRange(crectangle2(position - radius, cvec2(radius * 2)));
}
crectanglei2 dimension::getTouchingChunkCoordinateRange(crectangle2& rect)
{
	cveci2& coords0 = getChunkCoordinates(rect.pos0);
	return crectanglei2(coords0, (getChunkCoordinates(rect.pos1()) - coords0) + 1);
}
std::vector<entity*> dimension::getCollidingEntities(crectangle2& rect)
{
	std::vector<entity*> entities = findNearEntities(rect.expanded(mobSizeMargin));
	std::vector<entity*> collidingEntities = std::vector<entity*>();
	for (entity* e : entities)
	{
		if (collides2d(rect, e->calculateHitBox()))
		{
			collidingEntities.push_back(e);
		}
	}
	return collidingEntities;
}
bool dimension::canAddUpdates(cveci2& position)
{
	if (!inBounds(position))
	{
		return false;
	}
	else
	{
		chunk* c = getChunk(getChunkCoordinates(cvec2(position)));
		return c && c->loadLevel >= chunkLoadLevel::updateLoaded;
	}
}

collisionDataCollection dimension::getRecursiveHitboxCollisionData(crectangle2& box, cvec2& hitboxSpeed)
{
	collisionDataCollection collection = getHitboxCollisionData(box, hitboxSpeed);

	for (tickableBlockContainer* container : childContainers)
	{
		cmat3x3& transform = container->containerToRootTransform;
		cmat3x3& inverse = transform.inverse();

		cvec2& rootSpeed = container->speedInRoot;

		cvec2& substractedSpeed = hitboxSpeed - rootSpeed;

		crectangle2& transformedBox = inverse.multRectMatrix(box);

		collisionDataCollection additionalCollection = container->getHitboxCollisionData(transformedBox, substractedSpeed);

		for (collisionData& data : additionalCollection.hitboxes)
		{
			data.speed += rootSpeed;
			data.hitboxCollidingWith = transform.multRectMatrix(data.hitboxCollidingWith);
		}
		collection.addCollisionData(additionalCollection);

	}
	return collection;
}
lightLevel dimension::getVisibleLightLevel(cveci2& pos)
{
	const lightLevel internalSunLight = getInternalSunLightLevel(pos);
	const lightLevel visibleSunLight = getVisibleSunLightLevel(internalSunLight);
	if (visibleSunLight == maxLightLevel)
	{
		return maxLightLevel;
	}
	else
	{
		const lightLevel blockLight = getBlockLightLevel(pos);
		return math::maximum(blockLight, visibleSunLight);
	}
}
void dimension::generateStructures(chunk& generateIn)
{
}

chunk* dimension::getChunk(cveci2& chunkCoordinates) const
{
	return loadedChunksFastArray.getValue(chunkCoordinates - loadedChunksRange.pos0);
}

chunk* dimension::loadChunkIfNotLoaded(cveci2& chunkCoordinates, const chunkLoadLevel& loadLevel)
{
	chunk* c = getChunk(chunkCoordinates);
	if (!c)
	{
		c = new chunk(this, chunkCoordinates);

		loadedChunksMap.push_back(std::pair<veci2, chunk*>(chunkCoordinates, c));
		//update the fast array
		if (loadedChunksRange.contains(chunkCoordinates))
		{
			loadedChunksFastArray.setValueUnsafe(chunkCoordinates - loadedChunksRange.pos0, c);
		}
		else
		{
			recalculateChunkArray();
		}
	}
	c->increaseLoadLevel(loadLevel);
	c->ticksSinceNecessity = 0;
	return c;
}

void dimension::keepPlayerLoaded(crectanglei2& chunkRange)
{
	cveci2 pos1 = chunkRange.pos1();
	for (veci2 position = chunkRange.pos0; position.y < pos1.y; position.y++)
	{
		for (position.x = chunkRange.pos0.x; position.x < pos1.x; position.x++)
		{
			chunk* c = loadChunkIfNotLoaded(position, chunkLoadLevel::entityLoaded);
			c->ticksSincePlayer = 0;
		}
	}
}

void dimension::removeUnNessecaryChunks()
{
	for (auto it = loadedChunksMap.begin(); it != loadedChunksMap.end(); it++)
	{
		chunk* c = it->second;
		if (c->ticksSinceNecessity >= chunkSaveTreshold)
		{
			currentWorld->chunksToSave.push_back(c);
		}
		else
		{
			if (c->loadLevel > chunkLoadLevel::worldGenerationLoaded)
			{
				if (c->ticksSincePlayer >= chunkEntityFreezeTreshold)
				{
					//to prevent not generated chunks from jumping over the generation phase
					c->decreaseLoadLevel(chunkLoadLevel::updateLoaded);
				}
				else
				{
					c->increaseLoadLevel(chunkLoadLevel::entityLoaded);
				}
			}
			c->ticksSincePlayer++;
			c->ticksSinceNecessity++;
		}
	}
}



entity* dimension::findUUID(cvec2& position, cfp& range, const uuid& id)
{
	std::vector<entity*> entities = findNearEntities(position, range);
	for (entity* e : entities)
	{
		if (e->identifier == id)
		{
			return e;
		}
	}
	return nullptr;
}
void* dimension::getArrayValuePointerUnsafe(cveci2& position, const arrayDataType& dataType, const chunkLoadLevel& minimalLoadLevel)
{
	cveci2& chunkCoordinates = getChunkCoordinates(position);
	chunk* chunkToGet = loadChunkIfNotLoaded(chunkCoordinates, minimalLoadLevel);

	csize_t& arrayIndex = (size_t)(position.x - chunkToGet->worldPos.x) + ((size_t)(position.y - chunkToGet->worldPos.y) * chunkSize.x);
	switch (dataType)
	{
	case arrayDataType::blockIDType:
		return chunkToGet->blockIDArray.baseArray + arrayIndex;
	case arrayDataType::blockDataType:
		return chunkToGet->blockDataArray.baseArray + arrayIndex;
	default:
	{
		if (isLevelDataType(dataType))
		{
			const levelID& levelType = (levelID)((int)dataType - (int)arrayDataType::levelType);
			switch (levelType)
			{
			case levelID::powerLevel:
				return chunkToGet->powerLevels.baseArray + arrayIndex;
			default:
				if (isLightLevel(levelType))
				{
					const lightLevelID& lightLevelType = (lightLevelID)((int)levelType - (int)levelID::light);
					switch (lightLevelType)
					{
					case lightLevelID::blockLight:
						return chunkToGet->blockLightLevels.baseArray + arrayIndex;
					case lightLevelID::internalSunLight:
						return chunkToGet->internalSunLightLevels.baseArray + arrayIndex;
					default:
						break;
					}
				}
				break;
			}
		}
		else
		{
			handleError(std::wstring(L"array not found"));
		}
	}
	}
	return nullptr;
}
bool dimension::inBounds(cveci2& position) const
{
	return true;
}