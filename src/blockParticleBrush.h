#pragma once
#include "itemID.h"
#include "rectParticleBrush.h"
struct blockParticleBrush : rectParticleBrush
{
	blockID brushBlockID = blockID();
	blockParticleBrush();
	blockParticleBrush(const blockID& brushBlockID);
	virtual void serializeValue(nbtSerializer& s) override;
	virtual const resolutionTexture& getTexture() const final;
};