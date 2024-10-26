#pragma once
#include "facingData.h"
struct dispenserData : facingData
{
	dispenserData(const directionID& directionFacing = directionID::positiveY);
	rectangularSlotContainer* slots = nullptr;
	bool wasPowered = false;
	virtual void onBlockRemove(tickableBlockContainer* containerIn, cveci2& position) override;
	virtual ~dispenserData() override;
	virtual void serializeValue(nbtSerializer& s) override;
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;
	entity* summonEntityTouching(const entityID& entityType, tickableBlockContainer* containerIn, cveci2& position);
	virtual bool dispenseItem(const itemStack& stack, tickableBlockContainer* containerIn, cveci2& position);
	bool dropItem(const itemStack& stack, tickableBlockContainer* containerIn, cveci2& position);
};