#pragma once
#include "damageSource.h"
#include "math/uuid.h"
struct mobDamageSource : public damageSource
{
	mobDamageSource(const uuid& uuidFrom);
	uuid uuidFrom;
};