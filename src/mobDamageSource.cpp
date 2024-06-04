#include "mobDamageSource.h"
mobDamageSource::mobDamageSource(const uuid& uuidFrom) :damageSource(mobDamage)
{
	this->uuidFrom = uuidFrom;
}