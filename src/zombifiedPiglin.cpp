#include "idAnalysis.h"
#include "zombie.h"
#include "dimension.h"
#include "zombifiedPiglin.h"
#include "humanoid.h"
#include "hurtByTargetAI.h"
#include "taskList.h"
#include "wanderAI.h"
zombifiedPiglin::zombifiedPiglin(dimension* dimensionIn, cvec2& position) :humanoid(dimensionIn, position, entityID::zombiefied_piglin)
{
	tasks = new taskList(this, {
	new hurtByTargetAI(this),
	new wanderAI(this),
		});

	initializeBodyParts(zombieHeadTextureRect, zombieBodyTextureRect, zombieLeftLegTextureRect, zombieRightLegTextureRect, zombieLeftArmTextureRect, zombieRightArmTextureRect, zombiePixelSize);
	itemHolding = new itemStack(getMaterialTool(toolTypeID::sword, harvestTierID::goldHarvestTier), 1);
}

void zombifiedPiglin::render(const gameRenderData& targetData) const
{
	humanoid::render(targetData);
}

void zombifiedPiglin::serializeValue(nbtSerializer& s)
{
	humanoid::serializeValue(s);
	itemHolding->serialize(s, std::wstring(L"item holding"));
}