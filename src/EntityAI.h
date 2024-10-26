#pragma once
#include "nbt/nbtSerializable.h"
#include "interface/idestructable.h"
#include "entity.h"
struct entityAI : nbtSerializable, IDestructable
{
	entityAI(entity* const& connectedEntity) :connectedEntity(connectedEntity) {}
	entity* connectedEntity = nullptr;
	virtual void execute() = 0;
};