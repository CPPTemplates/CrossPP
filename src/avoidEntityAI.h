#pragma once
#include "taskAI.h"
struct avoidEntityAI :taskAI
{
	entityID entityTypeToAvoid;
	avoidEntityAI(entity* connectedEntity, const entityID& entityTypeToAvoid) : taskAI(connectedEntity), entityTypeToAvoid(entityTypeToAvoid) {}
	virtual void serializeValue(nbtSerializer& s) override;
};