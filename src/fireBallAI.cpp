#include "fireBall.h"
#include "fireBallAI.h"
#include "dimension.h"
#include "soundList.h"
#include <string>
#include "constants.h"
#include "entity.h"
#include "math/vector/vectn.h"
#include "mob.h"
#include "nbt/nbtSerializer.h"
#include "nbt/serializeUUID.h"
constexpr fp fireBallSpeed = 0x10;

void fireBallAI::shootFireBall(const entity* shootFireBallAt) const
{
	mob* connectedMob = (mob*)connectedEntity;
	//shoot fireball
	cvec2 headPos = connectedMob->getHeadPosition();
	fireBall* f = (fireBall*)summonEntity(fireBallType, connectedEntity->dimensionIn, headPos);
	f->onGround = false;
	f->speed = (shootFireBallAt->position - headPos).normalized() * fireBallSpeed;
	f->thrownFrom = connectedMob->identifier;

	fireBallShootSound->playRandomSound(connectedMob->dimensionIn, headPos);
}

void fireBallAI::serializeValue(nbtSerializer& s)
{
	s.serializeValue(std::wstring(L"fireball type"), fireBallType);
	serializeNBTValue(s, std::wstring(L"target"), target);
}