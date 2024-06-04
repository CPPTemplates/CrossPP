#include "dimension.h"
#include "enderman.h"
#include "taskList.h"
#include "teleportOnDamageAI.h"
#include "wanderAI.h"
#include "stareDownAI.h"

enderman::enderman(dimension* dimensionIn, cvec2& position) :humanoid(dimensionIn, position, entityID::enderman)
{
	tasks = new taskList(this, {
	new teleportOnDamageAI(this),
	new stareDownAI(this),
	new wanderAI(this),
		});

	initializeBodyParts(endermanHeadTextureRect, endermanBodyTextureRect, endermanLeftLegTextureRect, endermanRightLegTextureRect, endermanLeftArmTextureRect, endermanRightArmTextureRect, endermanPixelSize);
	itemHolding = new itemStack();
}

void enderman::serializeValue(nbtSerializer& s)
{
	humanoid::serializeValue(s);
	itemHolding->serialize(s, std::wstring(L"item holding"));
}
bool enderman::addDamageSource(cfp& damage, std::shared_ptr<damageSource> source)
{
	if (source.get()->type == projectileDamage)
	{
		return false;
	}
	else
	{
		return humanoid::addDamageSource(damage, source);
	}
}