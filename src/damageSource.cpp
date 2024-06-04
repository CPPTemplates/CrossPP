#include "damageSource.h"
#include "world.h"
damageSource::damageSource(damageType type) : type(type), damageTick(currentWorld->ticksSinceStart)
{

}

damageSource::~damageSource()
{

}