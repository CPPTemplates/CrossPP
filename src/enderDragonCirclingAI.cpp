#include "math/vector/vectn.h"
#include "world.h"
#include "end.h"
#include "enderDragon.h"
#include "enderDragonCirclingAI.h"
#include "math/vector/vectorrandom.h"
#include "nbt/nbtSerializer.h"
#include "nbt/serializeColor.h"
void enderDragonCirclingAI::startExecuting()
{
	chooseTargetPoint();
}

void enderDragonCirclingAI::updateTask()
{
	enderDragon* connectedEnderDragon = ((enderDragon*)connectedEntity);

	cfp distance = (connectedEnderDragon->position - targetPoint).lengthSquared();
	if (distance < math::squared(0x10))
	{
		chooseTargetPoint();
	}

	connectedEnderDragon->flying = true;

	connectedEnderDragon->goToPosition(targetPoint);
	connectedEnderDragon->flipBodyToWalkingDirection();
}

void enderDragonCirclingAI::chooseTargetPoint()
{
	end* currentEnd = (end*)currentWorld->dimensions[(int)dimensionID::end];
	//choose the farthest pillar

	csize_t& targetIndex = connectedEntity->position.x < 0 ? 1 : 0;

	targetPoint = cvec2(targetIndex ? obsidianPillarDistance : -obsidianPillarDistance,
		(currentEnd->obsidianPillarLevel[targetIndex] + currentEnd->obsidianPillarHeight[targetIndex] + 2));

	crectangle2 randomRect = crectangle2().expanded(0x10);
	targetPoint += getRandomPointOnEllipse(currentRandom, randomRect);
}

void enderDragonCirclingAI::serializeValue(nbtSerializer& s)
{
	serializeNBTValue(s, std::wstring(L"target point"), targetPoint);
}