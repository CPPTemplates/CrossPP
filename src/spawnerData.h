#pragma once
#include "blockData.h"
struct spawnerData : blockData
{
	entityID entityToSpawn = entityID::zombie;
	int spawnCoolDown = 0;
	spawnerData(const entityID &mobToSpawn = entityID::zombie, cint &spawnCoolDown = 0) : blockData(), entityToSpawn(mobToSpawn), spawnCoolDown(spawnCoolDown) {}
	virtual void randomTick(tickableBlockContainer *containerIn, cveci2 &position) override;
	virtual void serializeValue(nbtSerializer &s) override;
};