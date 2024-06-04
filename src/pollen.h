#pragma once
#include "entity.h"
//pollen
constexpr fp pollenSize = 0.1;
constexpr rectangle2 relativePollenHitbox = crectangle2(cvec2(pollenSize * -0.5), cvec2(pollenSize));
struct pollen : public entity
{
	int ticksToExist = 0x200;
	blockID blockToSpread = blockID::grass;
	virtual void render(const gameRenderData& targetData) const override;
	pollen(dimension* dimensionIn, cvec2& position) : entity(dimensionIn, position, entityID::pollen) {}
	virtual void tick() override;
	virtual void serializeValue(nbtSerializer& s) override;
};