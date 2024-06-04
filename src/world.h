#pragma once
#include "interface/idestructable.h"
#include "entityID.h"
#include <vector>
#include "biomeGenerator.h"
#include "chat.h"
struct chunk;
struct world :IDestructable, nbtSerializable
{
	world();
	void initialize();
	void finish();
	void tick();
	//save or load world including dimensions and chunks
	bool serialize(cbool& write);

	std::vector<biomeGenerator*> biomeList = std::vector<biomeGenerator*>();

	std::vector<chunk*> chunksToSave = std::vector<chunk*>();

	chat currentChat = chat();
	dimension* dimensions[(int)dimensionID::count];
	int ticksSinceStart = 0;
	int lastAutoSaveTick = 0;
	fp currentTime = 0;

	fp getTimeOfDay()const;

	//will be set in the 'resetWorldSettings' function
	int randomTickSpeed = 0;
	bool spawnMobs = false;
	bool allowCheats = false;
	bool keepInventoryOnDeath = false;
	bool xray = false;

	fp dayNightCycleSpeed = 0;

	vec2 worldSpawnPoint = vec2();
	dimensionID worldSpawnDimension = dimensionID();

	std::wstring name = std::wstring();

	ull seed = 0;
	virtual ~world() override;
};
extern world* currentWorld;

std::wstring getAvailableWorldName(std::wstring worldName);

void generateNewWorld();