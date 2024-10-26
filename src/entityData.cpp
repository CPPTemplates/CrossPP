#include "entityData.h"
idList<entityData*, entityID> entityDataList = idList<entityData*, entityID>();
entityData::entityData(const entityID& identifier, const std::wstring name, cfp& maxHealth, crectangle2& initialialRelativeHitbox, cfp& volume, cfp& weight, cbool& canGoThroughPortals)
	: INamable(name), identifier(identifier), maxHealth(maxHealth), weight(weight), volume(volume), initialRelativeHitbox(initialialRelativeHitbox), canGoThroughPortals(canGoThroughPortals)
{
}