#pragma once
#include "interface/idestructable.h"
#include "entityID.h"
#include <string>
#include "math/rectangle/rectangletn.h"
#include "idList.h"
#include "interface/inamable.h"
struct entityData :INamable, IDestructable
{
	entityData(const entityID& identifier, const std::wstring name, cfp& maxHealth, crectangle2& initialialRelativeHitbox, cfp& volume = 0, cfp& weight = 0, cbool& canGoThroughPortals = true);
	entityID identifier = (entityID)0;
	fp maxHealth = 0;
	fp weight = 0;//weight in kilograms
	fp volume = 0;//the volume of the actual entity (not the hitbox volume)
	rectangle2 initialRelativeHitbox = crectangle2();
	bool canGoThroughPortals = true;
};
extern idList<entityData*, entityID> entityDataList;