#include "settings.h"
#include "playerControlledAI.h"
#include "world.h"
#include "folderList.h"
#include "gameControl.h"

#include "overWorld.h"
#include "nether.h"
#include "end.h"

#include "ocean.h"
#include "swamp.h"
#include "plains.h"
#include "savanna.h"
#include "desert.h"

#include "taiga.h"
#include "forest.h"
#include "darkForest.h"
#include "jungle.h"

#include "snowyTundra.h"
#include "iceSpikes.h"
#include "mushroomFields.h"

#include "badLands.h"
#include "deathValley.h"
#include "mountains.h"

#include "netherWastes.h"
#include "crimsonForest.h"
#include "warpedForest.h"
#include "soulSandValley.h"
#include "basaltDeltas.h"

#include "theEnd.h"
#include "endVoid.h"
#include "endLands.h"

#include "craftingTableSlotContainer.h"
#include "smithingTableSlotContainer.h"
#include "anvilSlotContainer.h"
#include "furnaceSlotContainer.h"
#include "humanSlotContainerUI.h"
#include "brewingStandSlotContainer.h"
#include "enchantingTableSlotContainer.h"
#include "chestSlotContainer.h"
#include "dispenserSlotContainer.h"
#include "human.h"
#include "chunk.h"
#include "tag.h"
#include "nbt/serializeColor.h"

bool world::serialize(cbool& write)
{

	const stdPath path = savesFolder / currentWorld->name;
	if (!stdFileSystem::is_directory(path) || !stdFileSystem::exists(path))
	{
		if (write)
		{
			stdFileSystem::create_directory(path);
		}
		else
		{
			handleError(std::wstring(L"world not found"));
		}
	}
	nbtCompound* compound = nullptr;
	const stdPath &worldDataPath = path / saveFolderWorldData;
	if (write)
	{
		compound = new nbtCompound(name);
	}
	else
	{
		if (!nbtCompound::serialize(compound, write, worldDataPath))
		{
			return false;
		}
	}
	nbtSerializer s = nbtSerializer(*compound, write);

	//seed
	if (write)
	{
		//take screenshot
		//cveci2 screenSize = currentScreenSize;
		//as small as possible to increase refresh() performance
		//cveci2 screenShotsize = cveci2(0x40, 0x40);
		//texture screenShot(screenShotsize);
		//currentScreenSize = screenShotsize;
		//
		//renderGame(crectanglei2(screenShot.getClientRect()), screenShot, settings::renderHUD);
		//currentScreenSize = screenSize;

		//screenShot.Flip();
		//screenShot.Save(path + saveFolderScreenShot);
	}
	else
	{
		lastAutoSaveTick = ticksSinceStart;
		if (s.converter)
		{
			//todo: message box
			//if (MessageBox(NULL, L"This File Needs To Be Converter To The Current Version. Do You Still Want To Open It?",
			//	gameName.c_str(), MB_OKCANCEL | MB_ICONWARNING) == IDCANCEL)
			//{
			//	return false;
			//}
			//switch (MessageBox(NULL, L"Do You Want To Make A Backup?",
			//	gameName.c_str(), MB_YESNOCANCEL | MB_ICONQUESTION))
			//{
			//case IDYES:
			//{
			//	const std::wstring& backupFolderName = getAvailableWorldName(currentWorld->name + std::wstring(L" - Backup"));
			//	stdFileSystem::copy(path, savesFolder / backupFolderName, stdFileSystem::copy_options::recursive);
			//}
			//break;
			//case IDNO:
			//{
//
			//}
			//break;
			//case IDCANCEL:
			//{
//
			//}
			//return false;
			//default:
			//	handleError(std::wstring(L"not a messagebox option"));
			//	break;
			//}
		}
	}
	s.serializeValue(std::wstring(L"seed"), seed);
	serializeNBTValue(s, std::wstring(L"world spawnpoint"), worldSpawnPoint);
	s.serializeValue(std::wstring(L"world spawn dimension"), worldSpawnDimension);
	s.serializeValue(std::wstring(L"current time"), currentTime);
	//settings
	s.serializeValue(std::wstring(L"allow cheats"), allowCheats);
	s.serializeValue(std::wstring(L"randomtickspeed"), randomTickSpeed);
	s.serializeValue(std::wstring(L"spawn mobs"), spawnMobs);
	s.serializeValue(std::wstring(L"keep inventory on death"), keepInventoryOnDeath);
	s.serializeValue(std::wstring(L"xray"), xray);
	s.serializeValue(std::wstring(L"day night cycle speed"), dayNightCycleSpeed);
	
	if (!write)
	{
		initialize();
	}

	for (dimension* d : dimensions)
	{
		d->serialize(write);
	}

	if (write)
	{
		if (!nbtCompound::serialize(compound, write, worldDataPath))
		{
			delete compound;
			return false;
		}
	}
	else
	{
		finish();
	}

	delete compound;
	return true;
}

fp world::getTimeOfDay() const
{
	return math::mod(currentTime, (fp)ticksPerDay);
}
world::~world()
{
	for (dimension* d : dimensions)
	{
		delete d;
	}

	for (size_t i = 0; i < biomeList.size(); i++)
	{
		//new biomes don't have to exist
		if (biomeList[i])
		{
			delete biomeList[i];
		}
	}
	//if (currentWindSound)
	//{
	//	currentWindSound->stop();
	//}
}

world::world()
{
	ticksSinceStart = 0;
	currentTime = 0;
	dimensions[(int)dimensionID::overworld] = new overWorld();
	dimensions[(int)dimensionID::nether] = new nether();
	dimensions[(int)dimensionID::end] = new end();
	//each 16x16x16 minecraft chunk section gets 3 random ticks per tick
	//that evaluates in this amount of random ticks per frame
	randomTickSpeed = (int)defaultRandomTickSpeed;
	spawnMobs = true;
	allowCheats = true;
	dayNightCycleSpeed = 1;
}

void world::initialize()
{
	worldRandom = getRandomFromSeed(seed);

	

	for (dimension* const& d : dimensions)
	{
		d->initialize();
	}

	//initialize all biomes
	biomeList = std::vector<biomeGenerator*>({
		new ocean(),
		new swamp(),

		new plains(),
		new savanna(),
		new desert(),

		new taiga(),
		new forest(),
		new darkForest(),
		new jungle(),

		new snowyTundra(),
		new iceSpikes(),
		new mushroomFields(),

		new mountains(),
		new deathValley(),
		new badlands(),

		new netherWastes(),
		new crimsonForest(),
		new warpedForest(),
		new soulSandValley(),
		new basaltDeltas(),
		
		new theEnd(),
		new endVoid(),
		new endLands()
		});

}

void world::finish()
{
}

void world::tick()
{
	ticksSinceStart++;
	currentTime += dayNightCycleSpeed;

	//update entity positions before the tick to let all entities know eachother
	for (dimension* d : dimensions)
	{
		for (const auto& it : d->loadedChunksMap)
		{
			it.second->entityList.update();
		}
		d->removeUnNessecaryChunks();
	}

	csize_t& maxChunkSaveCount = 1 + (chunksToSave.size() / 0x40);//divide over a period of 3 seconds
	size_t chunksSavedThisTick = 0;
	for (size_t i = 0; i < chunksToSave.size(); i++)
	{
		chunk* c = chunksToSave[i];
		if (chunksSavedThisTick < maxChunkSaveCount)
		{
			chunksSavedThisTick++;

			for (size_t i = 0; i < c->dimensionIn->loadedChunksMap.size(); i++)
			{
				const auto& it = c->dimensionIn->loadedChunksMap[i];
				if (it.second == c)
				{
					c->dimensionIn->loadedChunksMap.erase(c->dimensionIn->loadedChunksMap.begin() + i);
					break;
				}
			}

			//c->dimensionIn->loadedChunksMap.erase(
								//std::find_if(c->dimensionIn->loadedChunksMap.begin(), c->dimensionIn->loadedChunksMap.end(), 
					//[c](const auto& a) {return a.first == c->chunkCoordinates; }));
			//memory corruption


			if (!c->dimensionIn->loadedChunksRange.expanded(-1).contains(c->chunkCoordinates))
			{
				c->dimensionIn->recalculateChunkArray();
			}
			else
			{
				c->dimensionIn->loadedChunksFastArray.setValueUnsafe(c->chunkCoordinates - c->dimensionIn->loadedChunksRange.pos0, nullptr);
			}

			c->serialize(true);

			delete c;
		}
	}

	for (dimension* d : dimensions)
	{
		//the 'loaded chunks' map will probably change, when mobs load in chunks
		//so we have to make copies
		const std::vector<std::pair<veci2, chunk*>> oldLoadedChunks = d->loadedChunksMap;
		for (const auto& it : oldLoadedChunks)
		{
			chunk* currentChunk = it.second;
			for (entity* e : currentChunk->entityList)
			{
				cveci2& chunkX = getChunkCoordinates(e->position);
				cveci2& newChunkX = getChunkCoordinates(e->newPosition);

				//move to correct chunk
				if (newChunkX != chunkX || e->newDimension != e->dimensionIn)
				{
					chunk* oldChunk = e->dimensionIn->getChunk(chunkX);
					oldChunk->entityList.erase(e);
					chunk* newChunk = e->newDimension->loadChunkIfNotLoaded(newChunkX, chunkLoadLevel::updateLoaded);
					newChunk->entityList.push_back(e);
				}

				e->position = e->newPosition;
				e->dimensionIn = e->newDimension;
			}
		}
	}

	for (dimension* d : dimensions)
	{
		for (const auto& it : d->loadedChunksMap)
		{
			it.second->entityList.update();
		}
	}

	for (dimension* d : dimensions)
	{
		d->tick();
	}

	for (dimension* d : dimensions)
	{
		//copy
		const std::vector<std::pair<veci2, chunk*>> oldChunksMap = d->loadedChunksMap;
		for (const auto& it : oldChunksMap)
		{
			it.second->changeEntityList();
		}
	}

	chunksToSave.clear();

	//rightClicked = false;
	//leftClicked = false;
}
void generateNewWorld()
{
	currentWorld->initialize();

	tag* spawnTag = tagList[getTagListIndexByName(std::wstring(L"valid_spawn"))];

	vec2 spawnBiomeLocation;

	currentWorld->worldSpawnDimension = dimensionID::overworld;

	currentWorld->dimensions[(int)currentWorld->worldSpawnDimension]->locateBiomes({ biomeID::plains,biomeID::forest,biomeID::taiga,biomeID::savanna }, cvec2(), spawnBiomeLocation);

	//seek spawn point
	int spawnTries = 0x10;
reRollSpawnPoint:
	spawnTries--;

	//cint spawnSeekRadius = 0x10;

	veci2 spawnBlock;
	spawnBlock.x = rand(worldRandom,
		(int)(spawnBiomeLocation.x > 0x10 ? spawnBiomeLocation.x : spawnBiomeLocation.x - 0x10),
		(int)(spawnBiomeLocation.x < -0x10 ? spawnBiomeLocation.x : spawnBiomeLocation.x + 0x10));

	//top to bottom
	for (spawnBlock.y = (int)chunkSize.y - 1; spawnBlock.y >= 0; spawnBlock.y--)
	{
		blockID b = currentWorld->dimensions[(int)currentWorld->worldSpawnDimension]->getBlockID(spawnBlock, chunkLoadLevel::updateLoaded);
		if (blockList[(int)b]->blockCollisionType == collisionTypeID::willCollide)
		{
			if (spawnTag->compare((itemID)b) || spawnTries == 0)
			{
				break;
			}
			else
			{
				goto reRollSpawnPoint;
			}
		}
	}
	currentWorld->worldSpawnPoint = cvec2((fp)spawnBlock.x + (fp)0.5, (fp)spawnBlock.y + 1 + math::fpepsilon);
	//currentWorld->worldSpawnPoint.y++;//to spawn on top of the block
	//to spawn in the middle of the block
	currentWorld->finish();
}
