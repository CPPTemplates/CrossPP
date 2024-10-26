#pragma once
#include "blockData.h"
#include "math/uuid.h"
struct pressurePlateData : blockData
{
	int lastUpdateTick = -1;

	pressurePlateData(cint lastUpdateTick = -1) : lastUpdateTick(lastUpdateTick) {}
	std::vector<uuid> entitiesStandingOn = std::vector<uuid>();
	void addEntityStandingOn(tickableBlockContainer* containerIn, cveci2& position, entity* const& e);

	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;

	virtual void serializeValue(nbtSerializer& s) override;
};