#pragma once
#include "itemID.h"
#include "rectParticleBrush.h"

struct itemParticleBrush : rectParticleBrush
{
	itemID brushItemID = itemID();
	itemParticleBrush();
	itemParticleBrush(const itemID& brushItemID);
	virtual void serializeValue(nbtSerializer& s) override;
	virtual const resolutionTexture& getTexture() const final;
};