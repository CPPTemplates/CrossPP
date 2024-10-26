#include "explosionDrop.h"
#include "dropData.h"
#include "math/random/random.h"
bool explosionDrop::checkCondition(const dropData& data, std::mt19937& randomToUse) const
{
	return (data.explosionPower < 2) || randChance(randomToUse, data.explosionPower);
}