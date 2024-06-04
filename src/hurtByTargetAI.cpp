#include "hurtByTargetAI.h"
#include "mobDamageSource.h"
bool hurtByTargetAI::shouldExecute()
{
	for (std::shared_ptr<damageSource> source : connectedEntity->lastDamageSources)
	{
		if (source.get()->type == mobDamage)
		{
			target = std::static_pointer_cast<mobDamageSource>(source).get()->uuidFrom;
			ticksAngry = maxTicksAngry;
		}
	}
	return target && targetAI::shouldExecute();
}