#pragma once
#include "nbt/nbtSerializable.h"
#include "tickableBlockContainer.h"
#include "dimensionID.h"
#include <utility>
#include <vector>
#include "arrayDataType.h"
#include "biomeID.h"
#include "chunkLoadLevel.h"
#include "collisionDataCollection.h"
#include "constants.h"
#include "entity.h"
#include "gameRenderData.h"
#include "array/arraynd/arraynd.h"
#include "globalFunctions.h"
#include "math/graphics/color/color.h"
#include "math/mathFunctions.h"
#include "math/rectangle/rectangletn.h"
#include "math/uuid.h"
#include "math/vector/vectn.h"
#include "nbt/nbtSerializer.h"
#include "constants/vectorConstants.h"
struct chunk;
enum class BiomeID : int;
enum class structureID;

//constexpr fp GravityForce30Fps = 9.8f / 465;//9.8m/s2 with 30 fps, 1 + 2 + 3 + ... 30 = 465
//constexpr fp SwimUpspeed = GravityForcePerTick + (0.5f * NormalTickTime * (1 - FluidFriction));//0.2



struct dimension : nbtSerializable, tickableBlockContainer
{
	dimensionID identifier;
	//list of loaded chunks, access chunks with chunk coordinates
	std::vector<std::pair<veci2, chunk*>> loadedChunksMap;

	//the range of loaded chunks with minimum being the x coordinate of the chunk with the lowest x, and the maximum vice versa.
	rectanglei2 loadedChunksRange = rectanglei2();
	//array of loaded chunks, for extremely high-performance accessing
	array2d<chunk*> loadedChunksFastArray = array2d<chunk*>();

	void recalculateChunkArray();

	//get max(blocklight,sunlight)

	//return wind speed in meters per second
	virtual vec2 getWindSpeed(cvec2& position) = 0;

	dimension(const dimensionID& identifier);
	virtual void initialize();
	color getColorMultiplier(cfp& sunLight, cfp& blockLight) const;
	veci2 searchPortal(cveci2& positionNear);
	bool serialize(cbool& write);
	void tick() override;

	virtual struct generationData* generateTerrain(chunk& generateIn) = 0;
	virtual void serializeValue(nbtSerializer& s) override;
	virtual void generateStructures(chunk& generateIn);
	virtual biomeID getBiome(cvec2& position) const = 0;
	virtual void renderSky(crectangle2& blockRect, crectangle2 & drawRect, const gameRenderData& targetData) const = 0;

	chunk* getChunk(cveci2& chunkCoordinates) const;
	chunk* loadChunkIfNotLoaded(cveci2& chunkCoordinates, const chunkLoadLevel& loadLevel);
	void keepPlayerLoaded(crectanglei2& chunkRange);

	void removeUnNessecaryChunks();

	lightLevel getVisibleLightLevel(cveci2& pos) override;

	virtual ~dimension() override;
	std::vector<chunk*> getLoadedChunks(crectanglei2& range);
	entity* findUUID(cvec2& position, cfp& range, const uuid& id) final;
	static crectanglei2 getTouchingChunkCoordinateRadius(cvec2& position, cfp& radius);
	static crectanglei2 getTouchingChunkCoordinateRange(crectangle2& rect);

	std::vector<entity*> getCollidingEntities(crectangle2& rect);

	//returns the position of the nearest biome with this id
	bool locateBiomes(const std::vector<biomeID>& biomesToLocate, cvec2& initialPosition, vec2& resultingPosition) const;
	
	//returns the chunk coordinates of the nearest structure with this id
	bool locateStructureChunkCoordinates(const structureID& id, cveci2& initialChunkCoordinates, veci2& resultingChunkCoordinates);
	//returns the position of the nearest structure with this id
	bool locateStructure(const structureID& id, cvec2& initialPosition, vec2& resultingPosition);

	bool meetsSpawningConditions(entity* const& e);

	virtual bool inBounds(cveci2& position) const final;

	template<typename t>
	array2d<t> getArrayValues(crectanglei2& rect, const arrayDataType& dataType, const chunkLoadLevel& minimalLoadLevel);

	virtual void* getArrayValuePointerUnsafe(cveci2& position, const arrayDataType& dataType, const chunkLoadLevel& minimalLoadLevel) override;

	virtual bool canAddUpdates(cveci2& position) final;

	collisionDataCollection getRecursiveHitboxCollisionData(crectangle2& box, cvec2& hitboxSpeed);

	virtual std::vector<entity*> findNearEntities(cvec2& position, cfp& radius) final;
	virtual std::vector<entity*> findNearEntities(crectangle2& searchBox) final;
};

template<typename t>
inline array2d<t> dimension::getArrayValues(crectanglei2& rect, const arrayDataType& dataType, const chunkLoadLevel& minimalLoadLevel)
{
	array2d<t> values(rect.size);

	//array elements that are out of bounds


	cveci2& pos1 = rect.pos1();

	veci2 firstChunkCoordinates = getChunkCoordinates(vec2(rect.pos0));
	veci2 lastChunkCoordinates = getChunkCoordinates(vec2(pos1));

	for (size_t i = 0; i < 2; i++)
	{
		if (pos1[i] != (lastChunkCoordinates[i] * (int)chunkSize[i]))
		{
			//exclusive
			lastChunkCoordinates[i]++;
		}
	}
	//divide in chunks
	for (veci2 currentChunkCoordinates = firstChunkCoordinates; currentChunkCoordinates.y < lastChunkCoordinates.y; currentChunkCoordinates.y++)
	{
		for (currentChunkCoordinates.x = firstChunkCoordinates.x; currentChunkCoordinates.x < lastChunkCoordinates.x; currentChunkCoordinates.x++)
		{
			cveci2 currentChunkWorldPos = currentChunkCoordinates * veci2(chunkSize);

			veci2 from, to;

			for (size_t i = 0; i < 2; i++)
			{
				from[i] = math::maximum(rect.pos0[i], currentChunkWorldPos[i]);
				to[i] = math::minimum(pos1[i], (int)(currentChunkWorldPos[i] + chunkSize[i]));
			}

			cveci2& chunkArraySize = to - from;

			t* chunkYPtr = (t*)getArrayValuePointerUnsafe(from, dataType, minimalLoadLevel);
			const t* const& chunkYEndPtr = chunkYPtr + chunkArraySize.y * chunkSize.x;
			t* arrayYPtr = values.baseArray + ((from.x - rect.x) + ((from.y - rect.y) * rect.w));
			while (chunkYPtr != chunkYEndPtr)
			{
				std::copy(chunkYPtr, chunkYPtr + chunkArraySize.x, arrayYPtr);
				arrayYPtr += rect.w;
				chunkYPtr += chunkSize.x;
			}
		}
	}
	return values;
}

