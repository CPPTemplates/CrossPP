#include "chunk.h"
#include "blockData.h"
#include "chunkLoadLevel.h"
#include "dimension.h"
#include "overWorldGenerationData.h"
#include "biomeGenerator.h"
#include "netherGenerationData.h"
#include "endGenerationData.h"
#include "world.h"
#include "math/hashing.h"
#include "idAnalysis.h"
#include "mobList.h"
#include "mobData.h"
#include "dimensionData.h"
#include "block.h"
#include "folderList.h"
#include "filesystem/filemanager.h"
#include "idConverter.h"
#include <vector>
#include <cstdint>
#include <map>
#include <random>
#include <string>
#include "arrayDataType.h"
#include "biomeID.h"
#include "blockContainer.h"
#include "itemID.h"
#include "constants.h"
#include "dimensionID.h"
#include "entity.h"
#include "entityData.h"
#include "entityID.h"
#include "array/arraynd/arraynd.h"
#include "globalFunctions.h"
#include "math/mathFunctions.h"
#include "math/random/random.h"
#include "math/vector/vectn.h"
#include "levelID.h"
#include "nbt/nbtCompound.h"
#include "nbt/nbtData.h"
#include "nbt/nbtDataTag.h"
#include "nbt/nbtSerializer.h"
#include "structureID.h"
#include "server.h"
#include "human.h"
#include "nbt/nbtSerializer.h"
#include "nbt/serializeColor.h"
#include "include/filesystem/fileFunctions.h"
#include "rectangularSlotContainer.h"

bool chunk::inBounds(cveci2 &position) const
{
    return blockIDArray.inBounds(position - worldPos);
}

void chunk::generateArrays()
{
    blockIDArray = array2d<blockID>(chunkSize, true);
    blockDataArray = array2d<blockData *>(chunkSize, true);
    internalSunLightLevels = array2d<lightLevel>(chunkSize, true);
    blockLightLevels = array2d<lightLevel>(chunkSize, true);
    powerLevels = array2d<powerLevel>(chunkSize, true);
}

void chunk::generateTerrain()
{
    loadLevel = chunkLoadLevel::worldGenerationLoaded;

    terrainData = dimensionIn->generateTerrain(*this);
}

void chunk::generateStructures()
{
    loadLevel = chunkLoadLevel::updateLoaded;
    dimensionIn->generateStructures(*this);

    // generate structures
    for (structureID id : generatedStructures)
    {
        if (dimensionIn->identifier == dimensionID::overworld)
        {
            cint &height = ((overworldGenerationData *)terrainData)->heights[halfChunkSize.x];

            cbool &hasTopPosition =
                (height >= worldPos.y) && (height < (worldPos.y + (int)chunkSize.y));

            cveci2 &topPosition = cveci2(worldPos.x + (int)halfChunkSize.x, height);

            cbool &hasUnderGround = height >= (worldPos.y + (int)chunkSize.y);

            cveci2 &randomChunkPosition = cveci2(
                worldPos.x + randIndex(currentRandom, (int)chunkSize.x),
                worldPos.y + randIndex(currentRandom, (int)chunkSize.y));

            switch (id)
            {
            case structureID::mineshaft:
            {
                if (hasUnderGround)
                {
                    biomeGenerator::placeMineShaft(dimensionIn, randomChunkPosition,
                                                   chunkRandom);
                }
            }
            break;
            case structureID::hot_air_balloon:
            {
                if (worldPos.y > 0)
                {
                    biomeGenerator::placeNamedStructure(dimensionIn, randomChunkPosition,
                                                        L"hot_air_balloon/base", chunkRandom);
                }
            }
            break;
            case structureID::floating_ship:
            {
                if (worldPos.y > 0)
                {
                    biomeGenerator::placeNamedStructure(dimensionIn, randomChunkPosition,
                                                        L"floating_ship/floating_ship",
                                                        chunkRandom);
                }
            }
            break;
            case structureID::wind_mill:
            {
                if (hasTopPosition)
                {
                    biomeGenerator::placeNamedStructure(dimensionIn, topPosition,
                                                        L"wind_mill/wind_mill", chunkRandom);
                }
            }
            break;
            case structureID::stronghold:
            {
                if (hasUnderGround)
                {
                    biomeGenerator::placeStronghold(dimensionIn, randomChunkPosition,
                                                    chunkRandom);
                }
            }
            break;
            case structureID::ocean_ruins:
            {
                if (hasTopPosition)
                {
                    biomeGenerator::placeOceanRuins(dimensionIn, topPosition, chunkRandom);
                }
            }
            break;
            case structureID::shipwreck:
            {
                if (hasTopPosition)
                {
                    biomeGenerator::placeShipWreck(dimensionIn, topPosition, chunkRandom);
                }
            }
            break;
            case structureID::desert_pyramid:
            {
                if (hasTopPosition)
                {
                    biomeGenerator::placeDesertPyramid(dimensionIn, topPosition, chunkRandom);
                }
            }
            break;
            case structureID::jungle_pyramid:
            {
                if (hasTopPosition)
                {
                    biomeGenerator::placeJungleTemple(dimensionIn, topPosition, chunkRandom);
                }
            }
            break;
            case structureID::fossil:
            {
                if (hasUnderGround)
                {
                    biomeGenerator::placeFossil(dimensionIn, randomChunkPosition, chunkRandom);
                }
            }
            break;

            case structureID::dungeon:
            {
                if (hasUnderGround)
                {
                    biomeGenerator::placeDungeon(dimensionIn, randomChunkPosition, chunkRandom);
                }
            }
            break;
            default:
                break;
            }
        }
        else if (dimensionIn->identifier == dimensionID::nether)
        {
            netherGenerationData *data = (netherGenerationData *)terrainData;
            switch (id)
            {
            case structureID::ruined_portal:
            {
            }
            break;
            case structureID::nether_fortress:
            {
                if (data->groundPositions.size())
                {
                    cveci2 positionToPlace = data->groundPositions[randIndex(chunkRandom,
                                                                             (int)data->groundPositions.size())];
                    biomeGenerator::placeNetherFortress(dimensionIn, positionToPlace,
                                                        chunkRandom);
                }
            }
            break;
            case structureID::bastion_remnant:
            {
            }
            break;
            case structureID::nether_fossil:
            {
                if (data->groundPositions.size())
                {
                    cveci2 positionToPlace = data->groundPositions[randIndex(chunkRandom,
                                                                             (int)data->groundPositions.size())];
                    biomeGenerator::placeFossil(dimensionIn, positionToPlace, chunkRandom);
                }
            }
            break;
            default:
                break;
            }
        }
        else if (dimensionIn->identifier == dimensionID::end)
        {
            switch (id)
            {

            case structureID::end_city:
            {
                endGenerationData *data = (endGenerationData *)terrainData;
                if (data->groundPositions.size())
                {
                    cveci2 positionToPlace = data->groundPositions[randIndex(chunkRandom,
                                                                             (int)data->groundPositions.size())];
                    biomeGenerator::placeEndCity(dimensionIn, positionToPlace, chunkRandom);
                }
            }
            break;
            default:
                break;
            }
        }
    }
    delete terrainData;
    terrainData = nullptr;

    // set it to update loaded here, to add light updates
    loadLevel = chunkLoadLevel::updateLoaded;
    addLightUpdates();
}

void chunk::serializeValue(nbtSerializer &s)
{
    serializeBlocks(s, blockIDArray, blockDataArray, powerLevels);

    s.serializeArray(std::wstring(L"internal sunlight levels"), internalSunLightLevels.baseArray,
                     chunkArraySize);
    s.serializeArray(std::wstring(L"blocklight levels"), blockLightLevels.baseArray,
                     chunkArraySize);

    s.serializeValue(std::wstring(L"load level"), loadLevel);
    s.serializeValue(std::wstring(L"ticks since player"), ticksSincePlayer);
    s.serializeValue(std::wstring(L"spawn cooldown"), spawnCooldown);
    if (loadLevel == chunkLoadLevel::worldGenerationLoaded)
    {
        if (!s.write)
        {
            switch (dimensionIn->identifier)
            {
            case dimensionID::overworld:
            {
                terrainData = new overworldGenerationData();
            }
            break;
            case dimensionID::nether:
            {
                terrainData = new netherGenerationData();
            }
            break;
            case dimensionID::end:
            {
                terrainData = new endGenerationData();
            }
            break;
            default:
                break;
            }
        }
        terrainData->serialize(s, std::wstring(L"terrain data"));
    }
    if (s.push<nbtDataTag::tagList>(std::wstring(L"entities")))
    {
        if (s.write)
        {
            entityList.update();
            // mob types
            for (fsize_t i = 0; i < entityList.size; i++)
            {
                if (isSerializable(entityList[i]->entityType))
                {
                    if (s.push<nbtDataTag::tagCompound>())
                    {
                        s.serializeValue(std::wstring(L"entity id"),
                                         entityList[i]->entityType);
                        serializeNBTValue(s, std::wstring(L"position"), entityList[i]->position);
                        entityList[i]->serializeValue(s);
                        s.pop();
                    }
                }
            }
        }
        else
        {
            const std::vector<nbtData *> &entityDataList = s.getChildren();

            // mob types
            for (nbtData *serializedEntityData : entityDataList)
            {
                if (s.push(serializedEntityData))
                {
                    entityID entityType;
                    if (idConverter::serializeID(s, std::wstring(L"entity id"), entityType,
                                                 s.converter ? &s.converter->entityIDConverter
                                                             : nullptr))
                    {
                        vec2 position;
                        serializeNBTValue(s, std::wstring(L"position"), position);
                        if (isSerializable(entityType))
                        {

                            entity *e = createEntity(entityType, dimensionIn, position);
                            e->serializeValue(s);
                            if (!e->identifier)
                            {
                                e->identifier = randomUUID(currentRandom);
                            }
                            entityList.push_back(e);

                            // TODO: add players in through separate files
                            // if (e->identifier == currentWorld->playableCharachterUUID)
                            //{
                            //	e->setAsPlayableCharachter();
                            // }
                        }
                        else if (entityType == entityID::human)
                        {
                            // save human separate

                            gameControl *fakeControl = nullptr;
                            human h = human(dimensionIn, position, *fakeControl, L"joe");
                            h.serializeValue(s);
                            h.hotbarSlots->dropContent(dimensionIn, position);
                            h.inventorySlots->dropContent(dimensionIn, position);
                            h.leftHandSlot->dropContent(dimensionIn, position);
                            // rectangularSlotContainer c =rectangularSlotContainer(cveci2());
                            // c.serialize(s, std::wstring(L"hotbar slots"));
                            // c.dropContent(dimensionIn, position);
                            // c.serialize(s, std::wstring(L"inventory slots"));
                            // c.dropContent(dimensionIn, position);
                            // c.serialize(s, std::wstring(L"left hand slot"));
                            // c.dropContent(dimensionIn, position);
                            // inventorySlots->serialize(s, std::wstring(L"inventory slots"));
                            // leftHandSlot->serialize(s, std::wstring(L"left hand slot"));
                            // human h = human(dimensionIn, position, gameControl(), L"jamey");
                        }
                    }
                    s.pop();
                }
            }
            entityList.update();
        }
        s.pop();
    }
}

chunk::chunk(dimension *dimensionIn, cveci2 &chunkCoordinates) : chunkCoordinates(chunkCoordinates),
                                                                 dimensionIn(dimensionIn)
{
    worldPos = chunkCoordinates * cveci2(chunkSize);

    chunkRandom = getRandomFromSeed(getChunkSeed(chunkCoordinates));
    const biomeID biomeIn = dimensionIn->getBiome(cvec2(worldPos + cveci2(chunkSize / 2)));
    if (dimensionIn->identifier == dimensionID::overworld)
    {
        if (chunkCoordinates.y < 0)
        {

            // add structures
            // underground
            if (randChance(chunkRandom, 0x40))
            {
                generatedStructures.push_back(structureID::stronghold);
            }
            else if (randChance(chunkRandom, 0x10))
            {
                generatedStructures.push_back(structureID::mineshaft);
            }
            if (randChance(chunkRandom, 0x10))
            {
                generatedStructures.push_back(structureID::dungeon);
            }
            if (randChance(chunkRandom, 0x10))
            {
                generatedStructures.push_back(structureID::fossil);
            }
            if (biomeIn == biomeID::ocean && worldPos.y > -100)
            {
                if (randChance(chunkRandom, 0x20))
                {
                    generatedStructures.push_back(structureID::shipwreck);
                }
                else if (randChance(chunkRandom, 0x20))
                {
                    generatedStructures.push_back(structureID::ocean_ruins);
                }
            }
        }
        else if (worldPos.y < 10)
        {
            // above ground
            // biome dependent structures
            if ((biomeIn == biomeID::desert) && (randChance(chunkRandom, 0x20)))
            {
                generatedStructures.push_back(structureID::desert_pyramid);
            }
            else if ((biomeIn == biomeID::jungle) && (randChance(chunkRandom, 0x20)))
            {
                generatedStructures.push_back(structureID::jungle_pyramid);
            }
            else if ((biomeIn == biomeID::plains) && randChance(chunkRandom, 0x10))
            {
                generatedStructures.push_back(structureID::wind_mill);
            }
            else if (randChance(chunkRandom, 0x20))
            {
                generatedStructures.push_back(structureID::village);
            }
        }
        else if (worldPos.y > 50)
        {
            if (randChance(chunkRandom, 0x10))
                generatedStructures.push_back(structureID::hot_air_balloon);
            if (randChance(chunkRandom, 0x10))
                generatedStructures.push_back(structureID::floating_ship);
        }
    }
    else if (dimensionIn->identifier == dimensionID::nether)
    {
        if (randChance(chunkRandom, 0x8))
        {
            generatedStructures.push_back(structureID::nether_fortress);
        }
        else if ((biomeIn == biomeID::soulsand_valley) && randChance(chunkRandom, 0x10))
        {
            generatedStructures.push_back(structureID::nether_fossil);
        }
    }
    else if (dimensionIn->identifier == dimensionID::end)
    {
        if (biomeIn == biomeID::end_lands)
        {
            if (randChance(chunkRandom, 0x40))
            {
                generatedStructures.push_back(structureID::end_city);
            }
        }
    }
    loadLevel = chunkLoadLevel::notLoaded;
}

void chunk::changeEntityList()
{
    for (fsize_t i = 0; i < entityList.size; i++)
    {
        entity *e = entityList[i];
        if (e->despawn)
        {

            delete e;
            entityList.erase(i);
        }
    }
    // call update before spawnmobs because spawnmobs checks the entitytype
    entityList.update();
    if (loadLevel == chunkLoadLevel::entityLoaded && currentWorld->spawnMobs)
    {
        spawnMobs();
    }
    entityList.update();
}

ull getChunkSeed(cveci2 &chunkCoordinates)
{
    return perfectlyHashToDoubleSizedType(
        (int32_t)perfectlyHashToDoubleSizedType(chunkCoordinates.x, chunkCoordinates.y),
        (int32_t)currentWorld->seed);
}

void chunk::spawnMobs()
{
    cint spawnTries =
        spawnCooldown <= spawnTreshold ? spawnRate : spawnCooldown <= doubleSpawnRateTreshold ? (spawnRate + spawnRate)
                                                                                              : 0;
    if (spawnCooldown > 0)
    {
        spawnCooldown--;
    }
    // this amount of tries per time
    for (int i = 0; i < spawnTries; i++)
    {
        // check mob cap
        size_t mobCount = 0;
        for (entity *e : entityList)
        {
            if (isMob(e->entityType))
            {
                mobCount++;
            }
        }
        if (mobCount >= maxChunkMobCount)
        {
            spawnCooldown += mobCapSpawnCoolDown;
            return;
            // no need to try to spawn more mobs
        }

        // choose a random position in the chunk
        veci2 blockSpawningIn = worldPos + veci2(rand(currentRandom, (int)chunkSize.x - 1),
                                                 rand(currentRandom, (int)chunkSize.y - 1));
        cfp &distance = currentServer->distanceToNearestPlayer(dimensionIn, cvec2(blockSpawningIn));
        if (distance < minimalSpawnDistance || distance > maximumSpawnDistance)
        {
            continue;
        }

        fp spawnWeights[(int)entityID::count] = {};
        // get a random mob
        fp totalWeight = 0;
        for (const entityID &id : mobList)
        {
            spawnWeights[(int)id] = ((mobData *)entityDataList[(int)id])->getSpawnWeight(dimensionIn, blockSpawningIn);
            totalWeight += spawnWeights[(int)id];
        }
        if (totalWeight == 0)
        {
            continue;
        }
        // this is the chosen mob
        const entityID entityType = (entityID)randIndex(currentRandom,
                                                        std::vector<fp>(spawnWeights,
                                                                        spawnWeights +
                                                                            mobTypeCount),
                                                        totalWeight);
        cvec2 spawnPosition = cvec2(blockSpawningIn) + 0.5;
        if (trySummonEntity(entityType, dimensionIn, spawnPosition))
        {
            spawnCooldown += mobSpawnCoolDown;
        }
    }
}

void chunk::tick() const
{
    if (loadLevel >= chunkLoadLevel::entityLoaded)
    {
        for (size_t index = 0; index < entityList.size; index++)
        {
            entityList[index]->tick();
        }
    }
}

void chunk::physics() const
{
    if (loadLevel >= chunkLoadLevel::entityLoaded)
    {
        for (size_t index = 0; index < entityList.size; index++)
        {
            entityList[index]->physics();
        }
    }
}

void chunk::randomTick()
{
    if (loadLevel >= chunkLoadLevel::entityLoaded)
    {
        for (int i = 0; i < currentWorld->randomTickSpeed; i++)
        {
            cveci2 &pos = worldPos + cveci2(randIndex(currentRandom, (int)chunkSize.x),
                                            randIndex(currentRandom, (int)chunkSize.y));
            blockData *data = dimensionIn->getBlockData(pos);
            //const blockID &id = dimensionIn->getBlockID(pos);
            if (data)
            {
                data->randomTick(dimensionIn, pos);
            }
        }
    }
}

chunk::~chunk()
{
    if (blockDataArray.size.x)
    {
        for (size_t index = 0; index < chunkSize.x * chunkSize.y; index++)
        {
            if (blockDataArray.baseArray[index])
            {
                delete blockDataArray.baseArray[index];
            }
        }
        for (fsize_t i = 0; i < entityList.size; i++)
        {
            delete entityList[i];
        }
    }

    if (terrainData)
    {
        delete terrainData;
        terrainData = nullptr;
    }
}

void chunk::setLoadLevel(const chunkLoadLevel &level)
{
    // WARNING: DO NOT SET IT TO A HIGHER LOADLEVEL WHILE TRANSITIONING TO A HIGHER LEVEL

    // example: chunk 0 with loadlevel 1 places a structure with loadlevel 2

    // but the chunk is already loading to loadlevel 1

    if (loadLevel < level)
    {
        if (loadLevel < chunkLoadLevel::worldGenerationLoaded &&
            level >= chunkLoadLevel::worldGenerationLoaded)
        {
            generateArrays();
            if (serialize(false))
            {
                // if a file is opened with a higher loadlevel
                if (loadLevel >= level)
                {
                    return;
                }
            }
            else
            {
                // CHANGES LOADLEVEL
                generateTerrain();
            }
        }
        if (loadLevel < chunkLoadLevel::updateLoaded && level >= chunkLoadLevel::updateLoaded)
        {
            generateStructures();
        }
    }
    else if (level <= chunkLoadLevel::worldGenerationLoaded &&
             loadLevel > chunkLoadLevel::worldGenerationLoaded)
    {
        handleError(std::wstring(L"can't decrease below world generation"));
    }
    loadLevel = level;
}

void chunk::increaseLoadLevel(const chunkLoadLevel &level)
{
    if (loadLevel < level)
    {
        setLoadLevel(level);
    }
}

void chunk::decreaseLoadLevel(const chunkLoadLevel &level)
{
    if (level < loadLevel)
    {
        setLoadLevel(level);
    }
}

void chunk::addLightUpdates()
{
    // flood fill with light and block updates
    const blockID *blockPtr = blockIDArray.baseArray;

    for (size_t j = 0; j < chunkSize.y; j++)
    {
        for (size_t i = 0; i < chunkSize.x; i++, blockPtr++)
        {
            cveci2 worldPosition = this->worldPos + cveci2((int)i, (int)j);
            const blockID &currentID = *blockPtr;
            if (needsBlockUpdate(currentID))
            {
                // dimensionIn->addBlockUpdatePosition(cveci2(i + worldX, j));
            }
            bool shouldAdd = false;
            if (dimensionDataList[dimensionIn->identifier]->hasSunLight &&
                ((j + worldPos.y) == sunLightHeight))
            {
                shouldAdd = true;
                goto addToList;
            }
            if ((i == 0) || (i == (chunkSize.x - 1)) || (j == 0) || (j == (chunkSize.y - 1)))
            {
                for (int arrayLevelTypeIndex = 0;
                     arrayLevelTypeIndex < (int)levelID::count; arrayLevelTypeIndex++)
                {
                    if (blockList[(int)currentID]->filterStrength[arrayLevelTypeIndex] !=
                        maxArrayValue[(size_t)arrayDataType::levelType +
                                      (size_t)arrayLevelTypeIndex])
                    {
                        shouldAdd = true;
                        goto addToList;
                    }
                }
            }
            for (int arrayLevelTypeIndex = 0;
                 arrayLevelTypeIndex < (int)levelID::count; arrayLevelTypeIndex++)
            {
                if (dimensionIn->getEmittanceLevel(worldPosition, (levelID)arrayLevelTypeIndex))
                {
                    shouldAdd = true;
                    goto addToList;
                }
            }

        addToList:
            if (shouldAdd)
            {
                dimensionIn->addLevelUpdatePosition(worldPos + cveci2((int)i, (int)j));
            }
        }
    }
}

bool chunk::serialize(cbool &write)
{
    const stdPath chunksFolder =
        savesFolder / currentWorld->name / dimensionDataList[dimensionIn->identifier]->name /
        saveFolderChunks / std::to_wstring(chunkCoordinates.x);
    if (write)
    {
        createFoldersIfNotExists(chunksFolder);
    }
    else
    {
        loadLevel = math::maximum(loadLevel, chunkLoadLevel::worldGenerationLoaded);
    }
    const stdPath &chunkPath =
        chunksFolder / (std::to_wstring(chunkCoordinates.y) + nbtFileExtension);

    return nbtSerializable::serialize(L"chunk", chunkPath, write);
}