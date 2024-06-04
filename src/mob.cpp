#include "mob.h"
#include "dimension.h"
#include "mobData.h"
#include "human.h"
#include "idAnalysis.h"
#include "soundList.h"
#include "mobDamageSource.h"
#include "math/collisions.h"
#include "block.h"
#include "particle.h"
#include "blockParticleBrush.h"
#include "endCrystal.h"
#include "dropData.h"
#include "math/raycast.h"
#include "ridableEntity.h"
#include "furnaceRecipe.h"
#include "world.h"
#include "server.h"
#include "textureParticleBrush.h"
#include "statusEffectData.h"
#include "effectParticleBrush.h"
#include "nbt/serializeUUID.h"
#include "nbt/serializeColor.h"

constexpr fp verticalSwimSpeedSeconds = 1.0f;
constexpr fp verticalSwimSpeed = verticalSwimSpeedSeconds;
constexpr fp SwimUpspeed = verticalSwimSpeed; // 0.2;
constexpr fp swimDownSpeed = -SwimUpspeed;
constexpr fp ladderSpeedSeconds = 2.5f;
constexpr fp ladderSpeed = ladderSpeedSeconds; // 0.2;
constexpr fp climbUpSpeed = ladderSpeed;
constexpr fp climbDownSpeed = ladderSpeed;
constexpr fp swimSpeed = 5.612f;

entity *mob::getSelectedEntity()
{
	return selectedUUID ? dimensionIn->findUUID(position, getArmRange() + mobSizeMargin, selectedUUID) : nullptr;
}

int mob::getDefencePoints() const
{
	return 0;
}

void mob::tick()
{
	// clear decisions
	wantsToSprint = false;
	wantsToJump = false;
	wantsToSneak = false;
	wantsToGoUp = false;
	wantsToGoDown = false;
	wantsToGoLeft = false;
	wantsToGoRight = false;
	wantsToHit = false;

	entity::tick();

	ticksSinceToolUsed++;

	walking = wantsToGoRight ^ wantsToGoLeft;
	cbool moving = abs(speed.x) > 0;
	fp lastLegDistance;
	if (!walking)
	{
		lastLegDistance = 0;
		totalLegDistance = 0;
	}
	else
	{
		lastLegDistance = totalLegDistance;
		totalLegDistance += speed.x * secondsPerTick;
	}

	if (((mobData *)entityDataList[(int)entityType])->ambientSound)
	{
		if (ambientSoundCoolDown <= 0)
		{
			((mobData *)entityDataList[(int)entityType])->ambientSound->playRandomSound(dimensionIn, getHeadPosition());
			resetAmbientSoundCoolDown();
		}
		else
		{
			ambientSoundCoolDown--;
		}
	}
	// if this is a player
	fp exhaustionIncrease = 0;
	if (jumpStamina)
	{
		jumpStamina--;
	}

	fp MovementSpeed;
	// the power of this step

	if (collideLevel != collisionTypeID::willNotCollide)
	{
		collideLevel = wantsToGoDown ? collisionTypeID::willCollide : collisionTypeID::willCollideTop;
	}

	if (wantsToHit)
	{
		if (selectedUUID)
		{
			entity *entityToAttack = dimensionIn->findUUID(position, getArmRange() + mobSizeMargin, selectedUUID);
			if (entityToAttack)
			{
				// cooldown
				cfp totalCoolDownTime = getTotalCoolDownTime();
				cfp coolDownMultiplier = ticksSinceToolUsed < totalCoolDownTime ? math::lerp(1 - cooldownTreshold, (fp)1.0, math::squared((this->ticksSinceToolUsed + 0.5) / totalCoolDownTime)) : 1.0;
				fp attackDamage = getAttackDamage();
				attackDamage *= coolDownMultiplier;

				attackDamage += getEffectLevel(statusEffectID::strength) * 3;

				fp durabilityDecrease = 1;

				// crits
				bool criticalDamage = !onGround && (speed.y < -3) && (speed.x <= getMobData(entityType)->walkingSpeed) && (coolDownMultiplier > 0.848);
				if (criticalDamage)
				{
					attackDamage *= 1.5;
					durabilityDecrease *= 1.5;
				}
				// enchantments
				if (isHumanoid(entityType) && (int)((humanoid *)this)->itemHolding->stackItemID)
				{
					int sharpnessLevel = ((humanoid *)this)->itemHolding->getEnchantmentLevel(enchantmentID::sharpness);
					fp enchantmentDamage = sharpnessLevel ? 0.5 + sharpnessLevel * 0.5 : 0;
					// undead mobs
					if (isUndeadMob(entityToAttack->entityType))
					{
						enchantmentDamage += 2.5 * ((humanoid *)this)->itemHolding->getEnchantmentLevel(enchantmentID::smite);
					}
					// not squared
					cfp enchantmentsCoolDownMultiplier = ticksSinceToolUsed < totalCoolDownTime ? math::lerp(1 - cooldownTreshold, (fp)1.0, (this->ticksSinceToolUsed + 0.5) / totalCoolDownTime) : 1.0;
					enchantmentDamage *= enchantmentsCoolDownMultiplier;
					attackDamage += enchantmentDamage;

					entityToAttack->setOnFire(80 * ((humanoid *)this)->itemHolding->getEnchantmentLevel(enchantmentID::fireAspect));

					const itemID &idHolding = ((humanoid *)this)->itemHolding->stackItemID;

					if (hasDurability(idHolding))
					{
						if (isMaterialTool(idHolding))
						{
							toolTypeID toolTypeHolding = getMaterialToolType(idHolding);
							switch (toolTypeHolding)
							{
							case toolTypeID::pickaxe:
							case toolTypeID::axe:
							case toolTypeID::shovel:
								durabilityDecrease *= 2;
							default:
								break;
							}
						}
						if (isHumanoid(entityType))
						{
							((humanoid *)this)->decreaseDurability(*((humanoid *)this)->itemHolding, durabilityDecrease);
						}
					}
				}

				if (entityType == entityID::human) // play attack sounds
				{
					if (ticksSinceToolUsed < totalCoolDownTime)
					{
						weakAttackSound->playRandomSound(dimensionIn, exactEntityIntersection);
					}
					else
					{
						if (criticalDamage)
						{
							criticalAttackSound->playRandomSound(dimensionIn, exactEntityIntersection);
							for (int i = 0; i < 5; i++)
							{
								summonParticle(dimensionIn, exactEntityIntersection, new textureParticleBrush(particleID::crit), 4);
							}
						}
						else
						{
							strongAttackSound->playRandomSound(dimensionIn, exactEntityIntersection);
						}
					}
				}
				// mob is in the way, hit the mob instead

				cint knockBackLevel = isHumanoid(entityType) ? ((humanoid *)this)->itemHolding->getEnchantmentLevel(enchantmentID::knockback) : 0;
				cfp difference = entityToAttack->position.x - position.x;
				cfp knockbackDistance = 0.5; // the mob has to be this amount of blocks away to recieve knockback
				cfp baseKnockBackSpeed = 5;
				cvec2 sideWaysBonusSpeed = cvec2((knockBackLevel + 1) * baseKnockBackSpeed *
													 (difference < -knockbackDistance ? -1 : difference > knockbackDistance ? 1
																															: 0),
												 0);
				// cvec2 knockBack = speed + sideWaysBonusSpeed;

				speed = entityToAttack->handleCollision(speed, getWeight());
				entityToAttack->speed += sideWaysBonusSpeed; // add arm knockback. for now, that's free.

				entityToAttack->addDamageSource(attackDamage, std::make_shared<mobDamageSource>(identifier));
				exhaustionIncrease += 0.1;
			}
		}
		ticksSinceToolUsed = 0;
	}

	vec2 forceToAdd = vec2();

	fp terminalVelocityMultiplier = 1;

	if (!flying)
	{
		if (wantsToSprint)
		{
			MovementSpeed = humanSprintingSpeed;
		}
		else
		{
			MovementSpeed = getMobData(entityType)->walkingSpeed;
		}
		// even when falling this multiplier should be there
		terminalVelocityMultiplier = groundTerminalVelocityMultiplier;

		// MovementSpeed = MovementSpeed;

		if (isHumanoid(entityType) && ((humanoid *)this)->climbing)
		{
			if (wantsToGoUp || wantsToJump)
			{
				forceToAdd.y += climbUpSpeed;
			}
			else if (wantsToGoDown)
			{
				forceToAdd.y += -climbDownSpeed;
			}
			terminalVelocityMultiplier = getTerminalVelocityMultiplier(ladderFrictionMultiplier);
		}
		else if (fluidArea > 0)
		{
			// TODO: swimming animation
			terminalVelocityMultiplier = getTerminalVelocityMultiplier(fluidFrictionMultiplier);
			if (wantsToGoUp || wantsToJump)
			{
				forceToAdd.y += SwimUpspeed + gravityForce / terminalVelocityMultiplier;
			}
			else if (wantsToGoDown)
			{
				forceToAdd.y += swimDownSpeed + gravityForce / terminalVelocityMultiplier;
			}
			MovementSpeed = swimSpeed;
		}

		if (onGround)
		{
			cbool jump = wantsToJump && (jumpStamina == 0);
			sneaking = wantsToSneak && (!jump);
			if (jump)
			{
				cvec2 jumpPower = getJumpPower();
				// Jumpheight = 1.252203 -> vanilla mc: 1.25
				cfp Jumpspeed = (jumpPower.y * pow(1.5, getEffectLevel(statusEffectID::jumpBoost))) + gravityForce; // gravity also gets substracted
				fp power = Jumpspeed;
				speed.y += power;
				jumpStamina = 9;
				MovementSpeed *= jumpPower.x; // horizontal jump power
				exhaustionIncrease += wantsToSprint ? 0.2 : 0.05;
			}
			else
			{
				if (sneaking)
				{
					MovementSpeed = humanSneakingSpeed;
				}
			}
		}
	}
	else // flying
	{
		MovementSpeed =
			(entityType == entityID::human && ((human *)this)->currentGameMode == gameModeID::spectator)
				? ((human *)this)->spectatorSpeed
				: ((mobData *)entityDataList[entityType])->flyingSpeed;

		if (wantsToSprint)
		{
			MovementSpeed *= 2;
		}

		terminalVelocityMultiplier = airTerminalVelocityMultiplier;
		if (wantsToGoUp || wantsToJump)
		{
			forceToAdd.y += MovementSpeed;
		}
		if (wantsToGoDown || wantsToSneak)
		{
			forceToAdd.y -= MovementSpeed;
		}
	}

	if (wantsToGoRight)
	{
		forceToAdd.x += MovementSpeed;
	}
	if (wantsToGoLeft)
	{
		forceToAdd.x -= MovementSpeed;
	}

	cfp &effectMultiplier = 1 + 0.2 * getEffectLevel(statusEffectID::speed);
	cfp &totalMultiplier = effectMultiplier * terminalVelocityMultiplier;
	cvec2 &multiplied = forceToAdd * totalMultiplier;
	speed += multiplied;

	if (forceToAdd.lengthSquared())
	{
		if (walking && wantsToSprint)
		{
			exhaustionIncrease += 0.1 * humanSprintingSpeed * secondsPerTick; // 0.1 / meter
		}
		if (fluidArea > 0)
		{
			exhaustionIncrease += 0.01 * swimSpeed * secondsPerTick; // 0.01 / meter
		}
	}

	// going the wrong way?

	if (onGround)
	{
		if ((speed.x > 0) ? (!wantsToGoRight) : (!wantsToGoLeft))
		{
			// use feet as brakes
			cfp feetFriction = 0.5;
			speed.x = speed.x * feetFriction;
		}
	}
	else if (flying)
	{
		cbool &noSpeed = speed.lengthSquared() == 0;
		cbool &noSpeedToAdd = forceToAdd.lengthSquared() == 0;

		if (!noSpeedToAdd)
		{
			if (!noSpeed)
			{
				cfp &angle = angleBetween(forceToAdd.normalized(), speed.normalized());

				if (angle < (math::PI2 * 0.25))
				{
					goto dontBrake;
				}
			}
			else
			{
				goto dontBrake;
			}
		}
		{
			// use wings as brakes
			cfp wingFriction = 0.8;
			speed.x = speed.x * wingFriction;
		}
	dontBrake:;
	}

	// dont play sounds if sneaking or falling
	if (!sneaking && onGround && moving)
	{
		if (((mobData *)entityDataList[(int)entityType])->legSwingSynchronizer.maximumBetween(lastLegDistance, totalLegDistance))
		{
			// check block
			block *b = blockList[(int)dimensionIn->getBlockID(veci2((int)floor(position.x), (int)floor(position.y - 0.05)))];
			if (b->blockCollisionType != collisionTypeID::willNotCollide)
			{
				std::shared_ptr<soundCollection> mobStepSound = getMobData(entityType)->stepSound;
				std::shared_ptr<soundCollection> stepSound = mobStepSound ? mobStepSound : b->stepSound;
				fp volume = mobStepSound ? 0.5 : 1;
				if (walking)
				{
					stepSound->playRandomSound(dimensionIn, position, volume);
					// add particle
					summonParticle(dimensionIn, position, new blockParticleBrush(b->identifier));
				}
				else if (abs(speed.x) > legBrakeSpeed.x)
				{
					stepSound->playRandomSound(dimensionIn, position, volume, mobStepSound ? 1 : 0.5);
					// add particle
					summonParticle(dimensionIn, position, new blockParticleBrush(b->identifier));
				}
			}
			// play block touch sound
		}
	}

	if (entityType == entityID::human)
	{
		human *currentHuman = (human *)this;
		currentHuman->addExhaustion(exhaustionIncrease);
	}

	entity *selectedEntity = selectedUUID ? dimensionIn->findUUID(position, getArmRange() + mobSizeMargin, selectedUUID) : nullptr;
	if (UUIDRidingOn)
	{
		entity *entityRidingOn = dimensionIn->findUUID(position, ridingEntitySearchRadius, UUIDRidingOn);
		if (!entityRidingOn)
		{
			// exit boat
			UUIDRidingOn = uuid();
		}
	}
	if (wantsToHit)
	{
		if (selectedEntity)
		{
			if (selectedEntity->entityType == entityID::end_crystal)
			{
				((endCrystal *)selectedEntity)->explode();
			}
		}
	}

	// add particles for potions
	for (size_t i = 0; i < activeEffects.size(); i++)
	{
		if (randChance(currentRandom, (ticksPerRealLifeSecond / activeEffects[i].potency)))
		{
			summonParticle(dimensionIn, getrandomPosition(currentRandom, calculateHitBox()), new effectParticleBrush(statusEffectDataList[activeEffects[i].identifier]->particleColor));
		}
	}

	if ((!isBossMob(entityType)) && isHostileMob(entityType) && randChance(currentRandom, 0x400))
	{
		// human* nearestPlayer = currentServer->distanceToNearestPlayer(dimensionIn, position);
		// cfp distanceToPlayer = (position - nearestPlayer->position).lengthSquared();
		if (currentServer->distanceToNearestPlayer(dimensionIn, position) > randomDespawnDistance)
		{
			despawn = true; // despawn
		}
	}
}

void mob::onDeath()
{
	if (((mobData *)entityDataList[(int)entityType])->deathSound)
	{
		((mobData *)entityDataList[(int)entityType])->deathSound->playRandomSound(dimensionIn, position);
	}
	dropData data = dropData();

	if (lastDamageSources.size())
	{
		auto it = lastDamageSources.end();
		it--;

		if (it->get()->type == mobDamage)
		{

			mobDamageSource *source = (mobDamageSource *)it->get();

			if (entity *entityFrom = dimensionIn->findUUID(position, 0x40, source->uuidFrom))
			{
				if (entityFrom->entityType == entityID::human)
				{
					human *currentHuman = (human *)entityFrom;
					((mobData *)entityDataList[(int)entityType])->experienceWhenKilled->dropLoot(dimensionIn, position);

					data.toolUsed = currentHuman->itemHolding;
				}
			}
		}
	}

	std::vector<itemStack> currentDrops = ((mobData *)entityDataList[(int)entityType])->dropsWhenKilled->roll(data, currentRandom);
	if (fireTicks)
	{
		// drop cooked version
		for (itemStack &stack : currentDrops)
		{
			stack.stackItemID = getCookedItem(stack.stackItemID);
		}
	}
	lootTable::dropLoot(currentDrops, dimensionIn, position);

	entity::onDeath();
}

bool mob::addDamageSource(cfp &damage, std::shared_ptr<damageSource> source)
{
	if (entity::addDamageSource(damage, source))
	{
		if (health >= 0)
		{
			if (entityType == entityID::human && source.get()->type == fireDamage)
			{
				burningDamageSound->playRandomSound(dimensionIn, position);
			}
			else
			{
				std::shared_ptr<soundCollection> hurtSound = getMobData(entityType)->hurtSound;
				if (hurtSound)
				{
					hurtSound->playRandomSound(dimensionIn, position);
					resetAmbientSoundCoolDown();
				}
			}
		}
		return true;
	}
	return false;
}

fp mob::getTotalCoolDownTime() const
{
	return ticksPerRealLifeSecond / getMobData(entityType)->attackSpeed;
}

void mob::updateBodyParts() const
{
}

void mob::onCollisionWithGround(cfp &verticalSpeed)
{
	// play block sound of block below
	rectangle2 h = calculateHitBox(position);
	block *below = blockList[(int)dimensionIn->getBlockID(cveci2(math::floor(position.x), math::floor(position.y) - 1))];
	cfp damage = CalculateFallDamage(verticalSpeed);
	if (damage > 0)
	{
		addDamageSource(damage, std::make_shared<damageSource>(fallDamage));
	}

	// wont play the sound if you dont fall right onto the block
	if (below->blockCollisionType != collisionTypeID::willNotCollide)
	{
		if (-verticalSpeed > legBrakeSpeed.y)
		{
			below->fallSound->playRandomSound(dimensionIn, position, 1, 0.5);
		}
		else if (damage > 0)
		{
			below->fallSound->playRandomSound(dimensionIn, position);
		}
	}
}
void mob::serializeValue(nbtSerializer &s)
{
	// body parts are not serialized, as they change shape
	entity::serializeValue(s);
	s.serializeValue(std::wstring(L"ambient sound cooldown"), ambientSoundCoolDown);
	s.serializeValue(std::wstring(L"flying"), flying);
	s.serializeValue(std::wstring(L"walking"), walking);
	s.serializeValue(std::wstring(L"jump stamina"), jumpStamina);
	s.serializeValue(std::wstring(L"total leg distance"), totalLegDistance);
	serializeNBTValue(s, std::wstring(L"uuid riding on"), UUIDRidingOn);
	serializeNBTValue(s, std::wstring(L"selected uuid"), selectedUUID);
	serializeNBTValue(s, std::wstring(L"looking at"), lookingAt);
	serializeNBTValue(s, std::wstring(L"selected block position"), selectedBlockPosition);
	serializeNBTValue(s, std::wstring(L"adjacent block position"), adjacentBlockPosition);
	serializeNBTValue(s, std::wstring(L"exact block intersection"), exactBlockIntersection);
	serializeNBTValue(s, std::wstring(L"exact entity intersection"), exactEntityIntersection);
	s.serializeValue(std::wstring(L"ticks since tool used"), ticksSinceToolUsed);
}
void mob::render(const gameRenderData &targetData) const
{
	updateBodyParts();
	renderBodyPart(mainBodyPart, targetData.worldToRenderTargetTransform, targetData);
	entity::render(targetData);
}
bool mob::shouldJump(bool &wantsToGoLeft, bool &wantsToGoRight) const
{
	// check if there is a block in front
	cfp speedX = ((mobData *)entityDataList[(int)entityType])->walkingSpeed * (wantsToGoLeft ? -1 : wantsToGoRight ? 1
																												   : 0);
	crectangle2 testHitbox = calculateHitBox(position);
	constexpr fp timeBeforeJump = 0.5; // 0.5 seconds
	cvec2 testSpeed = cvec2(speedX, 0);
	collisionDataCollection data = dimensionIn->getHitboxCollisionData(testHitbox, testSpeed * timeBeforeJump);

	// filter on 'willcollide'
	cbool allowCollisionType[3] = {false, false, true};

	data.evaluate(testHitbox, testSpeed, timeBeforeJump);
	const auto &noJumpData = data.getFirstCollision(allowCollisionType);

	//cbool &willCollideIfNotJumping = data.getMaximumCollision() == collisionTypeID::willCollide;

	if (noJumpData.collisionTime < timeBeforeJump)
	{
		// check if the mob will still collide when jumping
		cvec2 &positionJump = position + cvec2(0, JumpHeight);
		crectangle2 &testHitboxJump = calculateHitBox(positionJump);
		collisionDataCollection jumpData = dimensionIn->getHitboxCollisionData(testHitboxJump, testSpeed);
		jumpData.evaluate(testHitboxJump, testSpeed, timeBeforeJump);

		const auto &firstJumpCollision = jumpData.getFirstCollision(allowCollisionType);
		if (firstJumpCollision.collisionTime > noJumpData.collisionTime)
		{

			// cbool allowCollisionType[3] = { false, false, true };
			// if(jumpData.getFirstCollision(allowCollisionType).collisionTime < 0.1)//
			return true;
		}
		else
		{ // jumping won't help
			if (noJumpData.collisionTime < 0.1)
			{
				wantsToGoLeft = wantsToGoRight = false;
			}
		}
	}
	return false;
}
void mob::lookForward()
{
	lookingAt = getHeadPosition() + vec2(mainBodyPart->flipX ? -2 : 2, 0);
}
void mob::lookAtEntity(entity *const &e)
{
	lookingAt = isMob(e->entityType) ? ((mob *)e)->getHeadPosition() : e->position;
}
void mob::flipBodyToLookingDirection()
{
	mainBodyPart->flipX = lookingAt.x < position.x;
}
void mob::flipBodyToWalkingDirection()
{
	mainBodyPart->flipX = wantsToGoLeft;
}
void mob::flipBodyToSpeedDirection()
{
	mainBodyPart->flipX = speed.x < 0;
}
bool mob::goToPosition(cvec2 &destination)
{
	if (destination.x < position.x)
	{
		wantsToGoLeft = true;
	}
	else if (destination.x > position.x)
	{
		wantsToGoRight = true;
	}

	if (destination.y < position.y)
	{
		wantsToGoDown = true;
	}
	else if (destination.y > position.y)
	{
		wantsToGoUp = true;
	}
	if (wantsToGoLeft || wantsToGoRight)
	{
		// if the mob is flying but is on the ground we can still evaluate this
		// also if the collision level is willNotCollide, it won't set onGround to true
		wantsToJump = onGround && shouldJump(wantsToGoLeft, wantsToGoRight);
		if (!wantsToGoLeft && !wantsToGoRight)
		{
			return false; // cannot reach target point
		}
	}
	return true;
}

void mob::updateHeadAngle() const
{
	mainBodyPart->CalculateAllTransforms();
	// look at 'lookingAt'
	cvec2 headJoint = getHeadPosition();
	vec2 headLookingAtDifference = lookingAt - headJoint;
	headLookingAtDifference.normalize();
	cvec2 headLookingAtDifferenceFlipped = vec2(abs(headLookingAtDifference.x), headLookingAtDifference.y);
	head->angle = headLookingAtDifferenceFlipped.getRotation() - math::PI * 0.5;
	if ((headLookingAtDifference.x < 0) ^ mainBodyPart->flipX)
	{
		head->flipX = true;
		// head->angle = -head->angle;
	}
	else
	{
		head->flipX = false;
	}
	head->changed = true;
}
fp mob::getArmRange() const
{
	constexpr fp mobArmRange = 3.5;
	switch (entityType)
	{
	case entityID::human:
		return humanArmRange;
		break;
	default:
		return mobArmRange;
		break;
	}
	return mobArmRange;
}
void mob::updateSelection()
{
	updateBodyParts();
	cvec2 eyePosition = getHeadPosition();
	cvec2 relative = lookingAt - eyePosition;
	cvec2 relativeNormalized = relative.normalized();
	cfp distance = math::minimum(relative.length(), getArmRange());
	cvec2 lookingAtShortened = eyePosition + relativeNormalized * distance;
	if (!dimensionIn->findRaycastRecursive(eyePosition, lookingAtShortened, selectedBlockPosition, adjacentBlockPosition, exactBlockIntersection, selectedBlockContainer))
	{
		if (!selectedBlockContainer)
		{
			selectedBlockContainer = dimensionIn;
		}
	}
	// findRayCast(eyePosition, lookingAtShortened, selectedBlockPosition, adjacentBlockPosition, exactBlockIntersection,
	//	[dimensionIn = dimensionIn](cveci2& position)
	//	{
	//		return dimensionIn->getBlock(position)->blockCollisionType == collisionTypeID::willCollide;
	//	}
	//);
	cfp minimalBlockCollideDistance = (exactBlockIntersection - eyePosition).length();
	selectedUUID = uuid();
	fp minimalMobCollideDistance = minimalBlockCollideDistance;

	cfp marginRange = (fp)chunkSize.x; // for if the 'position' is their feet and you want to hit their head

	std::vector<entity *> nearEntities = dimensionIn->findNearEntities(position, getArmRange() + marginRange);
	for (entity *e : nearEntities)
	{
		if (canHit(e->entityType))
		{
			if (e != this)
			{
				fp tmin, tmax;
				crectangle2 hitBox = e->calculateHitBox(e->position);
				// check for intersection
				if (collidedistance2dDirection(eyePosition, relativeNormalized, hitBox, tmin, tmax))
				{
					if (tmin < minimalMobCollideDistance)
					{
						minimalMobCollideDistance = tmin > 0 ? tmin : 0;
						selectedUUID = e->identifier;
					}
				}
			}
		}
	}
	if (selectedUUID)
	{
		exactEntityIntersection = eyePosition + relativeNormalized * minimalMobCollideDistance;
	}
	else
	{
		exactEntityIntersection = exactBlockIntersection;
	}
}

void mob::resetAmbientSoundCoolDown()
{
	ambientSoundCoolDown = rand(currentRandom, 0x80, 0x100);
}

fp mob::getGravityForce() const
{
	return flying ? 0 : entity::getGravityForce();
}

vec2 mob::getJumpPower() const
{
	return humanJumpPower;
}

fp mob::getAttackDamage() const
{
	return getMobData(entityType)->attackDamage;
}
mob::mob(dimension *dimensionIn, cvec2 &position, const entityID &entityType) : entity(dimensionIn, position, entityType)
{
	totalLegDistance = 0;
}

void mob::exitRodeEntity()
{
	if (UUIDRidingOn)
	{
		ridableEntity *entityRodeOn = (ridableEntity *)dimensionIn->findUUID(position, ridingEntitySearchRadius, UUIDRidingOn);
		for (size_t i = 0; i < entityRodeOn->seats.size(); i++)
		{
			if (entityRodeOn->seats[i] == identifier)
			{
				entityRodeOn->seats[i] = uuid();
			}
		}
		UUIDRidingOn = uuid();
	}
}
void mob::renderBodyPart(bodyPart2D *const &b, cmat3x3 &transform, const gameRenderData &targetData) const
{
	renderBodyPart(b, transform, *((mobData *)entityDataList[(int)entityType])->skin, targetData);
}
// just do it here:
// https://elsenaju.eu/Calculator/online-curve-fit.htm

// sources:
// https://gaming.stackexchange.com/questions/178726/what-is-the-terminal-velocity-of-a-sheep
fp mob::CalculateFallDamage(cfp &vertspeed)
{
	// fall 3 blocks without taking damage,
	// at 10 blocks take 7 fall damage
	// more accurate than minecraft, because mobs can push you down, so the calculation will output a much higher fall distance.
	// the leash up-and-down trick also does not work this way
	return -9.8 - (0.7 * vertspeed);
}

vec2 mob::getHeadPosition() const
{
	return head->getRotationCentrePosition();
}
mob::~mob()
{
	delete mainBodyPart;
	mainBodyPart = nullptr;
}