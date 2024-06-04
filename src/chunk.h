#include "generationData.h"
#include "interface/idestructable.h"
#include "itemID.h"
#include "blockData.h"
#include "chunkLoadLevel.h"
#include "entity.h"
#include "structureID.h"
#include "array/fastlist.h"
#include <vector>
#include <random>
#include "constants.h"
#include "dimension.h"
#include "array/arraynd/arraynd.h"
#include "globalFunctions.h"
#include "math/vector/vectn.h"

constexpr int chunkSaveTreshold = 0x10;//the amount of ticks a chunk needs to have no update for it to be saved if no chunk is saved that tick
constexpr int chunkEntityFreezeTreshold = 0x10;//the amount of ticks a chunk needs to have no player around to only calculate block updates
constexpr size_t maxChunkMobCount = (chunkSize.getX() * chunkSize.getY() * 3) / 0x1000;


struct chunk :IDestructable, nbtSerializable
{
	//position
	veci2 chunkCoordinates = veci2();
	veci2 worldPos = veci2();
	dimension* dimensionIn = nullptr;

	//worldgen
	chunkLoadLevel loadLevel = chunkLoadLevel::notLoaded;
	std::mt19937 chunkRandom;
	generationData* terrainData = nullptr;
	std::vector<structureID> generatedStructures = std::vector<structureID>();

	//arrays
	array2d<blockID> blockIDArray = array2d<blockID>();
	array2d<lightLevel> internalSunLightLevels = array2d<lightLevel>();
	array2d<lightLevel> blockLightLevels = array2d<lightLevel>();
	array2d<blockData*> blockDataArray = array2d<blockData*>();
	array2d<powerLevel> powerLevels = array2d<powerLevel>();

	//entities
	fastList<entity*> entityList = fastList<entity*>();

	//other data
	//since getchunk() with this coordinate was called
	int ticksSinceNecessity = 0;

	//since a player loaded this chunk
	int ticksSincePlayer = chunkEntityFreezeTreshold;

	//ticks to wait before double spawnrates
	int spawnCooldown = 0;


	bool inBounds(cveci2& position) const;
	void generateArrays();
	void generateTerrain();
	void generateStructures();
	virtual void serializeValue(nbtSerializer& s) override;
	bool serialize(cbool& write);
	chunk(dimension* dimensionIn, cveci2& chunkCoordinates);
	void changeEntityList();
	void spawnMobs();
	void tick() const;
	void physics() const;
	void randomTick();
	virtual ~chunk() override;
	void setLoadLevel(const chunkLoadLevel& level);
	void increaseLoadLevel(const chunkLoadLevel& level);
	void decreaseLoadLevel(const chunkLoadLevel& level);
	void addLightUpdates();
};


ull getChunkSeed(cveci2& chunkCoordinates);