#include "ghastFireBallAI.h"
#include "soundList.h"
#include "dimension.h"
#include "mob.h"
#include "nbt/nbtSerializer.h"
bool ghastFireBallAI::shouldExecute()
{
	//not in the constructor
	if (fireBallTicks)
	{
		fireBallTicks--;
	}
	if (fireBallAI::shouldExecute())
	{
		fireBallTicks = math::maximum(fireBallTicks, warningTick);
		return true;
	}
	else
	{
		return false;
	}
}

bool ghastFireBallAI::continueExecuting()
{
	if (fireBallTicks)
	{
		fireBallTicks--;
	}
	return fireBallAI::shouldExecute();
}

void ghastFireBallAI::updateTask()
{
	if (fireBallTicks == warningTick)
	{
		ghastWarningSound->playRandomSound(connectedEntity->dimensionIn, ((mob*)connectedEntity)->getHeadPosition());
	}
	if (entity* targetEntity = findTarget())
	{
		//TODO: add evading when too close
		if ((connectedEntity->position - targetEntity->position).lengthSquared() > math::squared(ghastSightRange * 0.5))
		{
			(((mob*)connectedEntity)->goToPosition(targetEntity->position));
		}
		if (fireBallTicks == 0)
		{
			shootFireBall(targetEntity);
			fireBallTicks = ghastFireballCoolDown;
		}

	}
}

void ghastFireBallAI::serializeValue(nbtSerializer& s)
{
	fireBallAI::serializeValue(s);
	s.serializeValue(std::wstring(L"fireball ticks"), fireBallTicks);
}