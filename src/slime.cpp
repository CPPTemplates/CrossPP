#include "humanoid.h"
#include "slime.h"
#include "taskList.h"
#include "attackOnCollideAI.h"
#include "wanderAI.h"
#include "dimension.h"
slime::slime(dimension* dimensionIn, cvec2& position, const entityID& entityType) : mob(dimensionIn, position, entityType)
{
	tasks = new taskList(this, {
	new attackOnCollideAI(this, entityID::human),
	new wanderAI(this),
		});

	size = math::lerp(minimumSlimeSize, maximumSlimeSize, math::squared(randFp()));
	if (entityType == entityID::slime)
	{
		resize();
	}
}

void slime::updateBodyParts() const
{
	mainBodyPart->translate = position;
	mainBodyPart->changed = true;
}

void slime::serializeValue(nbtSerializer& s)
{
	mob::serializeValue(s);
	s.serializeValue(std::wstring(L"size"), size);
	if (!s.write)
	{
		resize();
	}
}

void slime::resize()
{
	health = getMaxHealth();
	relativeHitbox = crectangle2().expanded(size * 0.5);
	if (mainBodyPart)
	{

		delete mainBodyPart;
	}
	createBodyParts();
}

void slime::createBodyParts()
{
	mainBodyPart = new bodyPart2D(editedSlimeTextureRect, nullptr, position, relativeHitbox.size, -relativeHitbox.pos0, 0, 0, true);
	head = mainBodyPart;
}

bool slime::goToPosition(cvec2& destination)
{
	//jump to the destination
	if (randChance(currentRandom, 3 * ticksPerRealLifeSecond))
	{
		mob::goToPosition(destination);
		wantsToJump = true;
	}
	else if (!onGround)
	{
		mob::goToPosition(destination);
	}
	return true;
}

vec2 slime::getJumpPower() const
{
	return cvec2(humanJumpPower.x * 0x2, humanJumpPower.y) * (size / humanStandHeight);
}

fp slime::getAttackDamage() const
{
	cfp& mult = size * 0.5;
	return mult * mult * mult;
}

fp slime::getWeight() const
{
	cfp& mult = size * size * size;
	return mult * 20;
}

void slime::onDeath()
{
	cint splitCount1D = 2;
	cint splitCount = math::squared(splitCount1D);//two dimensional
	cfp splitSize = size / splitCount1D;

	crectangle2 hitboxOnDeath = calculateHitBox(position);

	if (splitSize > minimumSlimeSize)
	{
		for (int i = 0; i < splitCount; i++)
		{
			entity* splitEntity = summonEntity(entityType, dimensionIn, hitboxOnDeath.pos0 + cvec2(((i % splitCount1D) + 0.5) / splitCount1D, ((i / splitCount1D) + 0.5) / splitCount1D) * hitboxOnDeath.size);
			splitEntity->speed = speed;
			slime* splitSlime = (slime*)splitEntity;
			splitSlime->size = splitSize;
			splitSlime->resize();
		}
	}
	mob::onDeath();
}

fp slime::getMaxHealth() const
{
	return size * size * size;
}