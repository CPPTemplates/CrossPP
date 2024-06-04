#include "humanoid.h"
#include "itemData.h"
#include "rectangularSlotContainer.h"
#include "human.h"
#include "mobData.h"
#include "idAnalysis.h"
#include "textureList.h"
#include "throwableData.h"
#include "throwable.h"
#include "soundList.h"
#include "durabilityData.h"
#include "slabData.h"
#include "doubleBlockData.h"
#include "attachmentDirectionData.h"
#include "upsideDownData.h"
#include "math/collisions.h"
#include "particle.h"
#include "dimension.h"
#include "dropData.h"
#include "experienceOrb.h"
#include "blockParticleBrush.h"
#include <vector>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <memory>
#include <set>
#include <string>
#include "armorTier.h"
#include "armorType.h"
#include "block.h"
#include "blockData.h"
#include "itemID.h"
#include "chunkLoadLevel.h"
#include "collisionTypeID.h"
#include "constants.h"
#include "damageSource.h"
#include "damageType.h"
#include "dimensionID.h"
#include "enchantmentID.h"
#include "entity.h"
#include "entityData.h"
#include "entityID.h"
#include "facingData.h"
#include "gameModeID.h"
#include "harvestType.h"
#include "globalFunctions.h"
#include "math/axis.h"
#include "math/bodypart2d.h"
#include "math/direction.h"
#include "math/graphics/brush/brushes.h"
#include "math/mathFunctions.h"
#include "math/mattnxn.h"
#include "math/random/random.h"
#include "math/rectangle/rectangletn.h"
#include "math/graphics/resolutiontexture.h"
#include "math/swingsynchronizer.h"
#include "math/timemath.h"
#include "math/vector/vectn.h"
#include "itemID.h"
#include "itemStack.h"
#include "mob.h"
#include "nbt/nbtSerializer.h"
#include "gameRenderData.h"
#include "slabType.h"
#include "statusEffectID.h"
#include "toolTypeID.h"
#include "gameControl.h"
#include "nbt/nbtSerializer.h"
#include "world.h"
void humanoid::resetDigProgress()
{
	selectedBlockDamage = 0;
	timeWaitingToDig = waitTime;
}

fp humanoid::getAttackDamage() const
{
	return math::maximum((int)itemHolding->stackItemID ? itemList[(int)itemHolding->stackItemID]->attackDamageModifier : 0, mob::getAttackDamage());
}

bool humanoid::substractStack(itemStack& stack)
{
	return armorSlots->substractStack(stack);
}

bool humanoid::addStack(itemStack& stack)
{
	return itemHolding->addStack(stack);
}

void humanoid::updateBodyParts() const
{
	cbool sitting = UUIDRidingOn;

	//update bodyparts
	if (sitting)
	{
		mainBodyPart->translate = vec2(position.x, position.y + mainBodyPart->size.y * 0.5);
	}
	else if (sleeping)
	{
		mainBodyPart->translate = vec2(position.x, position.y + mainBodyPart->size.x * 0.5);
	}
	else
	{
		mainBodyPart->translate = vec2(position.x, position.y + leftLeg->size.y + mainBodyPart->size.y * 0.5);
	}

	//draw laying down
	//flipping is done above
	//rotate 90 degrees
	mainBodyPart->angle = sleeping ? math::PI * -0.5 : 0;

	if (sleeping)//looking to the left
	{
		//flip body
		mainBodyPart->flipX = lookingAt.y < position.y;;
	}

	fp rightArmWalkingAngle;
	if (((entityType != entityID::human || !((human*)this)->flying)) && ((!walking && onGround && (abs(speed.x) > legBrakeSpeed.x)) || (abs(speed.y) > legBrakeSpeed.y)))
	{
		constexpr fp legBrakeAngle = -20 * math::degreesToRadians;
		//brakes animation
		rightLeg->angle = legBrakeAngle;
		leftLeg->angle = legBrakeAngle;

		constexpr fp wavingStrength = 20;//the angle in degrees of the maximum difference between the hands while waving
		constexpr fp waveSpeed = 0.5;//the time in seconds until the hands have the same position and speed again
		//arms waving in the air
		constexpr swingSynchronizer armWavingSynchronizer = swingSynchronizer(waveSpeed, wavingStrength * -0.5 * math::degreesToRadians, wavingStrength * -0.5 * math::degreesToRadians);
		cfp& armAngleOffset = armWavingSynchronizer.getSwingAngle(currentWorld->ticksSinceStart * secondsPerTick);
		//cfp armAngleOffset = sin(microsectosec(getmicroseconds()) * math::PI2 / waveSpeed) * wavingStrength * 0.5 * math::degreesToRadians;
		rightArmWalkingAngle = math::PI + legBrakeAngle + armAngleOffset;
		leftArm->angle = math::PI + legBrakeAngle - armAngleOffset;
	}
	else
	{
		cfp rightLegAngle = ((mobData*)entityDataList[(int)entityType])->legSwingSynchronizer.getSwingAngle(totalLegDistance);
		rightLeg->angle = rightLegAngle;
		leftLeg->angle = -rightLegAngle;
		rightArmWalkingAngle = -rightLegAngle;
		leftArm->angle = rightLegAngle;
	}
	if (sitting)
	{
		//bend legs forward
		rightLeg->angle = math::degreesToRadians * -80;
		leftLeg->angle = math::degreesToRadians * -80;
	}

	cfp angleIfHoldingItem = math::degreesToRadians * -30;

	cfp& armAngleOffset = armSwingSynchronizer.getSwingAngle(currentWorld->ticksSinceStart * secondsPerTick);
	rightArm->angle = digging ? armAngleOffset : (!walking && itemHolding && (int)itemHolding->stackItemID) ? angleIfHoldingItem : rightArmWalkingAngle;

	if (sleeping)
	{
		head->angle = 0;
	}
	else
	{
		updateHeadAngle();//at last, update the head angle
	}
	mainBodyPart->changed = true;
	rightLeg->changed = true;
	leftLeg->changed = true;
	rightArm->changed = true;
	leftArm->changed = true;
	head->changed = true;
}

void humanoid::render(const gameRenderData& targetData) const
{
	updateBodyParts();
	//draw the currently held itemStack
	if (itemHolding && itemHolding->count)
	{
		//calculate transforms to use for the item
		mainBodyPart->CalculateTransform();
		rightArm->CalculateTransform();

		const resolutionTexture& itemTex = replaceIfMissing(itemList[(int)itemHolding->stackItemID]->tex);
		crectangle2 textureRect = crectangle2(itemTex.getClientRect());

		mat3x3 itemTransform;
		fp itemHandSize;
		if (itemList[(int)itemHolding->stackItemID]->hasHilt)
		{
			itemHandSize = humanArmSize.y;
			//rotate after scaling
			itemTransform = mat3x3::rotate(vec2(itemHandSize * 0.5), math::degreesToRadians * 45);
			//mirror after rotating
			itemTransform = mat3x3::cross(mat3x3::mirror(axisID::y, itemHandSize * 0.5), itemTransform);
		}
		else
		{
			itemTransform = mat3x3();
			itemHandSize = humanHeadSize.x;
		}
		//transform so the size appears natural
		itemTransform = mat3x3::cross(itemTransform, mat3x3::fromRectToRect(crectangle2(textureRect), rectangle2(0, 0, itemHandSize, itemHandSize)));

		fp handHoldingPart = 0.6;//the part of the texture the hand 'holds'
		fp handFingerThickness = humanArmSize.x * 0.2;//the thickness of the finger that blocks a part of the item
		mat3x3 moveToHand = mat3x3::translate(-rightArm->rotationCentre + vec2(handFingerThickness, itemHandSize * (-1 + handHoldingPart)));
		//get right arm transform
		mat3x3 rightArmTransform = mat3x3::cross(mainBodyPart->applied, rightArm->applied);

		itemTransform = mat3x3::combine({ itemTransform, moveToHand ,rightArmTransform, targetData.worldToRenderTargetTransform });//, });
		//first rescaled, then transformed to the body, then to the right arm

		//transform rectangle
		itemHolding->renderSingleItem(targetData.clone(itemTransform));
	}

	renderBodyPart(mainBodyPart, targetData.worldToRenderTargetTransform, targetData);

	//the amount of body parts
	cint armorPartCount = 2 + 2 + 1 + 3;

	//from bottom to top
	bodyPart2D* parts[armorPartCount]
	{
		leftLeg,
		leftLeg,
		leftArm,

		mainBodyPart,
		head,

		rightLeg,
		rightLeg,
		rightArm,
	};

	//draw armor
	armorTypeID drawIndexes[armorPartCount]
	{
		bootsArmorType,leggingsArmorType,chestplateArmorType, chestplateArmorType, helmetArmorType, bootsArmorType,leggingsArmorType, chestplateArmorType
	};

	//just the skin rects, but for armor
	crectangle2 textureRects[armorTypeCount]
	{
		crectangle2(0, 0, 4, 12),
		crectangle2(0, 0, 4, 12),
		crectangle2(16, 0, 4, 12),
		crectangle2(0, 16, 8, 8),
	};

	for (int i = 0; i < armorPartCount; i++)
	{
		armorTypeID drawIndex = drawIndexes[i];
		const itemStack& currentArmorSlot = armorSlots->slots[drawIndex - bootsArmorType];
		if (currentArmorSlot.count)
		{
			//itemData* currentArmorItem = itemList[(int)currentArmorSlot.stackItemID];
			bodyPart2D* b = parts[i];
			crectangle2 textureRect = textureRects[drawIndex - bootsArmorType];
			if (isMaterialArmor(currentArmorSlot.stackItemID))
			{
				const armorTierID& tier = getMaterialArmorTier(currentArmorSlot.stackItemID);
				const resolutionTexture& tex = drawIndex == leggingsArmorType ? *armorLegTextures[tier - turtleArmorTier] : *armorTextures[tier - turtleArmorTier];

				if (b == rightArm)
				{
					//draw again to make sure the leg armor does not overlap
					mainBodyPart->CalculateTransform();
					cmat3x3 armTransform = mat3x3::cross(targetData.worldToRenderTargetTransform, mainBodyPart->applied);

					renderBodyPart(b, armTransform, targetData);
				}
				cmat3x3 armorTransform = mat3x3::combine({ mat3x3::fromRectToRect((crectangle2)textureRect, crectangle2(-b->rotationCentre, b->size).expanded(armorExpansion)),b->getCumulativeTransform() ,targetData.worldToRenderTargetTransform });
				//expand 0.1f
				//const mat3x3 armorTransform = mat
				fillTransparentRectangle((crectangle2)textureRect, armorTransform, tex, targetData.renderTarget);
			}
		}
	}
	if (UUIDRidingOn)
	{
		//render boat after the player
		entity* entityRidingOn = dimensionIn->findUUID(position, ridingEntitySearchRadius, UUIDRidingOn);
		if (entityRidingOn)
		{
			entityRidingOn->render(targetData);
		}
	}

	//renderTarget.fillCircle(targetData.worldToRenderTargetTransform.multRectMatrix(crectangle2(getHeadPosition(), vec2()).expanded(0.1)), brushes::red);

	entity::render(targetData);
}

void humanoid::updateSelection()
{
	cveci2 oldBlockPos = selectedBlockPosition;//don't make a const reference, copy the value
	tickableBlockContainer* const oldContainer = selectedBlockContainer;//warning! oldContainer might be deleted
	mob::updateSelection();
	if ((selectedBlockPosition != oldBlockPos) && oldContainer)
	{
		//can't compare blocks, as the old container might be deleted
		if (oldContainer == selectedBlockContainer)// || (selectedBlockContainer->getBlockID(selectedBlockPosition) != oldContainer->getBlockID(oldBlockPos))
		{
			resetDigProgress();
			if (entityType == entityID::human)
			{
				human* h = (human*)this;
				if (h->selectedContainerContainer)
				{
					//check if the player still is aiming at the container
					if (!hasGUI(selectedBlockContainer->getBlockID(selectedBlockPosition)))
					{
						h->closeBlockGUI();
					}
				}
			}
		}
	}
}
fp humanoid::getTotalCoolDownTime() const
{
	cfp attackSpeed = itemHolding && itemHolding->count ? math::minimum(itemList[(int)itemHolding->stackItemID]->attackSpeedModifier, getMobData(entityType)->attackSpeed) : getMobData(entityType)->attackSpeed;
	return ticksPerRealLifeSecond / attackSpeed;
}

//https://minecraft.gamepedia.com/Armor#Defense_points
bool humanoid::addDamageSource(cfp& damage, std::shared_ptr<damageSource> source)
{
	//reduce damage by armor
	int defencePoints = getDefencePoints();
	int toughness = getToughnessPoints();
	cfp reducedDamageByArmor = damage * (1 - (math::minimum((fp)20.0, math::maximum((fp)(defencePoints * 0.2), (fp)(defencePoints - (damage / (2 + toughness * 0.25))))) * 0.04));

	int enchantmentProtectionFactor = 0;
	for (int i = 0; i < armorTypeCount; i++)
	{
		const itemStack* s = &armorSlots->slots[i];
		if (s->count)
		{
			enchantmentProtectionFactor += s->getEnchantmentLevel(enchantmentID::protection);
			switch (source.get()->type)
			{
			case fallDamage:
				enchantmentProtectionFactor += s->getEnchantmentLevel(enchantmentID::featherFalling) * 3;
				break;
			case projectileDamage:
				enchantmentProtectionFactor += s->getEnchantmentLevel(enchantmentID::projectileProtection) * 2;
				break;
			case fireDamage:
				enchantmentProtectionFactor += s->getEnchantmentLevel(enchantmentID::fireProtection) * 2;
				break;
			case blastDamage:
				enchantmentProtectionFactor += s->getEnchantmentLevel(enchantmentID::blastProtection) * 2;
				break;
                default:
                    break;
			}
		}
	}

	cfp reducedDamageByEnchantments = reducedDamageByArmor *
		(1.0 - (math::minimum(enchantmentProtectionFactor, 20) * 0.04));

	for (int index = 0; index < armorTypeCount; index++)
	{
		itemStack& currentArmorPiece = armorSlots->slots[index];
		if (currentArmorPiece.count)
		{
			const armorTierID& protectionTier = getArmorTier(currentArmorPiece.stackItemID);
			if (protectionTier)
			{
				decreaseDurability(currentArmorPiece, (damage / 4));
			}
		}
	}

	return mob::addDamageSource(reducedDamageByEnchantments, source);
}

int humanoid::getDefencePoints() const
{
	//boots to helmet
	cint defenceTable[armorTierCount * armorTypeCount]
	{
		0,0,0,2,
		1,2,3,1,
		1,3,5,2,
		1,4,5,2,
		2,5,6,2,
		3,6,8,3,
		3,6,8,3,
	};
	int totalDefence = 0;
	for (int index = 0; index < armorTypeCount; index++)
	{
		const itemID& currentArmorPiece = armorSlots->slots[index].stackItemID;
		if ((int)currentArmorPiece)
		{
			const armorTierID& armorTierID = getArmorTier(currentArmorPiece);
			if (armorTierID)
			{
				totalDefence += defenceTable[(armorTierID - turtleArmorTier) * armorTypeCount + index];
			}
		}
	}
	return totalDefence;
}
int humanoid::getToughnessPoints()
{
	int totalToughness = 0;
	for (int index = 0; index < armorTypeCount; index++)
	{
		const itemID& currentArmorPiece = armorSlots->slots[index].stackItemID;
		if ((int)currentArmorPiece)
		{
			const armorTierID& armorTierID = getArmorTier(currentArmorPiece);
			if (armorTierID >= diamondArmorTier)
			{
				//3 for netherite
				totalToughness += (armorTierID == diamondArmorTier) ? 2 : 3;
			}
		}
	}
	return totalToughness;
}
fp humanoid::getWeight() const
{
	fp weightDivider = 1;
	for (int i = 0; i < armorTypeCount; i++)
	{
		const itemStack* s = &armorSlots->slots[i];
		if (s->count)
		{
			if (getArmorTier(s->stackItemID) == netheriteArmorTier)
			{
				weightDivider -= 0.1;
			}
		}
	}
	return entity::getWeight() / weightDivider;
}
humanoid::humanoid(dimension* dimensionIn, cvec2& position, const entityID& entityType) :mob(dimensionIn, position, entityType)
{
	armorSlots = new rectangularSlotContainer(cveci2(1, 4));
}

void humanoid::initializeBodyParts(crectangle2& headTextureRect, crectangle2& bodyTextureRect, crectangle2& leftLegTextureRect, crectangle2& rightLegTextureRect, crectangle2& leftArmTextureRect, crectangle2& rightArmTextureRect, cfp& pixelSize)
{
	cfp legHeight = leftLegTextureRect.h * pixelSize;
	cfp legWidth = leftLegTextureRect.w * pixelSize;
	cfp bodyHeight = bodyTextureRect.h * pixelSize;
	cfp bodyWidth = bodyTextureRect.w * pixelSize;
	cfp armHeight = leftArmTextureRect.h * pixelSize;
	cfp armWidth = leftArmTextureRect.w * pixelSize;
	cfp headHeight = headTextureRect.h * pixelSize;
	cfp headWidth = headTextureRect.w * pixelSize;
	mainBodyPart = new bodyPart2D(bodyTextureRect, nullptr, vec2(position.x, position.y + legHeight + bodyHeight * 0.5), vec2(bodyWidth, bodyHeight), vec2(bodyWidth, bodyHeight) * 0.5);

	rightLeg = new bodyPart2D(rightLegTextureRect, mainBodyPart, vec2(0, bodyHeight * -0.5), vec2(legWidth, legHeight), vec2(legWidth * 0.5, legHeight));
	leftLeg = new bodyPart2D(leftLegTextureRect, mainBodyPart, vec2(0, bodyHeight * -0.5), vec2(legWidth, legHeight), vec2(legWidth * 0.5, legHeight));

	rightArm = new bodyPart2D(rightArmTextureRect, mainBodyPart, vec2(0, (bodyHeight - armWidth) * 0.5), vec2(armWidth, armHeight), vec2(armWidth * 0.5, armHeight - armWidth * 0.5));
	leftArm = new bodyPart2D(leftArmTextureRect, mainBodyPart, vec2(0, (bodyHeight - armWidth) * 0.5), vec2(armWidth, armHeight), vec2(armWidth * 0.5, armHeight - armWidth * 0.5));

	head = new bodyPart2D(headTextureRect, mainBodyPart, vec2(0, bodyHeight * 0.5), vec2(headWidth, headHeight), vec2(headWidth * 0.5, 0));

	//add bodyparts
	//ordered by draw order
	//left parts at the back, right parts at front
	mainBodyPart->children.push_back(leftLeg);
	mainBodyPart->children.push_back(leftArm);
	mainBodyPart->drawOrderIndex = 2;
	mainBodyPart->children.push_back(head);
	mainBodyPart->children.push_back(rightLeg);
	mainBodyPart->children.push_back(rightArm);
}
fp humanoid::getGravityForce() const
{
	return (climbing ? 0 : mob::getGravityForce());
}

void humanoid::launchItem(const itemID& itemType)
{
	updateSelection();
	cvec2 headPos = getHeadPosition();
	cvec2 nearestIntersection = selectedUUID ? exactEntityIntersection : exactBlockIntersection;
	cvec2 difference = nearestIntersection - headPos;
	cfp intersectionDistance = difference.length();
	cvec2 differenceNormalized = difference / intersectionDistance;
	if (intersectionDistance >= 1)
	{
		//throw item
		const entityID& entityType = getThrowableEntityID(itemType);
		entity* e = summonEntity(entityType, dimensionIn, headPos + differenceNormalized);
		fp throwPower = ((throwableData*)entityDataList[(int)entityType])->throwPower;
		if (itemType == itemID::arrow)
		{
			throwPower *= math::minimum(bowAnimationTime * secondsPerTick, (fp)1);
			throwPower *= (1 + itemHolding->getEnchantmentLevel(enchantmentID::power));
		}//speed *=

		e->speed = speed + differenceNormalized * throwPower;
		e->onGround = false;
		((throwable*)e)->thrownFrom = identifier;
		if (entityType == entityID::ender_eye)
		{
			eyeOfEnderLaunchSound->playRandomSound(dimensionIn, headPos);
		}
		else
		{
			shootSound->playRandomSound(dimensionIn, headPos);
		}
		return;
	}
}
vec2 humanoid::getHandPosition() const
{
	return rightArm->getCumulativeTransform().multPointMatrix(cvec2(humanArmSize.x * 0.5, humanArmSize.y - humanArmSize.x * 0.5) - rightArm->rotationCentre);
}
void humanoid::decreaseDurability(itemStack& stack, cfp& amount)
{
	updateBodyParts();
	decreaseDurability(stack, amount, getHandPosition());
}
void humanoid::decreaseDurability(itemStack& stack, cfp& amount, cvec2& soundPositionIfBroken)
{
	if (stack.count)
	{
		cfp multipliedByWeakness = amount * getItemWeakness(stack.stackItemID);

		cfp multipliedByEnchantments = multipliedByWeakness / (1 + stack.getEnchantmentLevel(enchantmentID::unBreaking));

		durabilityData* toDurabilityData = (durabilityData*)stack.data;
		toDurabilityData->durability -= multipliedByEnchantments;
		if (toDurabilityData->durability <= 0)
		{
			stack.add(-1);
			toolBreakSound->playRandomSound(dimensionIn, soundPositionIfBroken);
		}
	}
}
void humanoid::respawn()
{
	mob::respawn();
	//humanoid* respawnedHumanoid = (humanoid*)mob::respawn();
	//armorSlots->clone(*respawnedHumanoid->armorSlots);
	//return respawnedHumanoid;
}

humanoid::~humanoid()
{
	if (itemHolding)
	{
		delete itemHolding;
	}

	delete armorSlots;
}
void humanoid::serializeValue(nbtSerializer& s)
{
	mob::serializeValue(s);
	s.serializeValue(std::wstring(L"selected block damage"), selectedBlockDamage);
	s.serializeValue(std::wstring(L"time waiting to dig"), timeWaitingToDig);
	s.serializeValue(std::wstring(L"digging"), digging);
	s.serializeValue(std::wstring(L"sleeping"), sleeping);
	s.serializeValue(std::wstring(L"climbing"), climbing);
	armorSlots->serialize(s, std::wstring(L"armor slots"));
}
void humanoid::tick()
{
	//clear decisions
	wantsToDig = false;
	mob::tick();

	//if this is a player
	fp exhaustionIncrease = 0;

	if (wantsToHit || wantsToDig)
	{
		//for performance reasons
		updateSelection();
	}
	//digging
	digging = false;
	if (!selectedUUID)
	{
		if (wantsToDig)
		{
			block* b = selectedBlockContainer->getBlock(selectedBlockPosition);
			if (canDig(b->identifier))
			{
				fp damageRequired = b->hardness * 10;
				cbool inCreative = entityType == entityID::human && ((human*)this)->currentGameMode == gameModeID::creative;
				if (b->hardness >= 0 || inCreative)
				{
					ticksSinceToolUsed = 0;
					fp damagePerTick = 1.0 / ticksPerRealLifeSecond;

					cint hasteLevel = getEffectLevel(statusEffectID::haste);
					damagePerTick *= (1 + (hasteLevel * 0.2));

					bool canHarvest = b->canHarvest(itemHolding->stackItemID);
					damagePerTick /= (canHarvest ? 0.15 : 0.5);
					//tool tier multipliers
					const int toolTierMultipliers[]{ 1,2,12,4,6,8,9 };

					cbool& increaseMiningSpeed = canHarvest && itemHolding->count && itemList[(int)itemHolding->stackItemID]->harvestType == b->bestTool;
					if (increaseMiningSpeed)
					{
						fp speedMultiplier = toolTierMultipliers[itemList[(int)itemHolding->stackItemID]->harvestTier];
						int efficiencyLevel = itemHolding->getEnchantmentLevel(enchantmentID::efficiency);
						if (efficiencyLevel)
						{
							speedMultiplier += 1 + math::squared(efficiencyLevel);
						}
						damagePerTick *= speedMultiplier;
					}
					if (fluidArea > 0)
					{
						itemStack* helmetStack = armorSlots->getSlot(cveci2(0, helmetArmorType - bootsArmorType));
						if (helmetStack->getEnchantmentLevel(enchantmentID::aquaAffinity) == 0)
						{
							damagePerTick *= 0.2;
						}
					}
					if (!onGround)
					{
						damagePerTick *= 0.2;
					}

					//if you can instamine, you have no waiting time.
					if (damagePerTick < damageRequired && timeWaitingToDig > 0 && !inCreative)
					{
						timeWaitingToDig--;
					}
					else
					{
						//can dig
						digging = true;

						selectedBlockDamage += damagePerTick;

						cbool& breakingBlock = selectedBlockDamage >= damageRequired || inCreative;
						if (breakingBlock)
						{
							//harvest block
							if (b->canHarvest(itemHolding->stackItemID) && (!inCreative))
							{
								//cvec2 harvestPosition = vec2(selectedBlockPosition.x + 0.5, selectedBlockPosition.y);
								selectedBlockContainer->harvestBlock(dropData(nullptr, itemHolding), selectedBlockPosition, chunkLoadLevel::updateLoaded);

								//add experience
								int experience = rand(currentRandom, b->experienceWhenBroken.minimum, b->experienceWhenBroken.maximum);
								for (int amount : divideExperience(experience))
								{
									experienceOrb* orb = (experienceOrb*)summonEntity(entityID::experience_orb, selectedBlockContainer, getrandomPosition(currentRandom, rectangle2(selectedBlockPosition, vec2(1)), vec2(experienceSize)) - relativeExperienceHitbox.pos0);
									orb->value = amount;
									orb->speed = vec2::getrotatedvector(randFp(currentRandom, math::PI2)) * randFp(maxFloatingSlotSpeed);
								}
							}
							cint particleCount = rand(currentRandom, minBlockBreakParticleCount, maxBlockBreakParticleCount);
							//show particles
							for (int i = 0; i < particleCount; i++)
							{
								summonParticle(selectedBlockContainer, getrandomPosition(currentRandom, rectangle2(selectedBlockPosition, vec2(1)), cvec2(particleSize)), new blockParticleBrush(b->identifier), maxParticleSpeed);
							}
							//break block
							selectedBlockContainer->setBlockID(selectedBlockPosition, blockID::air, chunkLoadLevel::entityLoaded);
							b->breakSound->playRandomSound(dimensionIn, exactBlockIntersection);
							resetDigProgress();
							exhaustionIncrease += 0.005;

						}
						else
						{
							//play digging sound if the arm is on its furthest point
							//cfp furthestPoint = 0.5;
							
							if (armSwingSynchronizer.maximumBetween(currentWorld->ticksSinceStart * secondsPerTick, (currentWorld->ticksSinceStart + 1) * secondsPerTick))
							{
								//start playing the sound just before 'hitting'
								b->hitSound->playRandomSound(dimensionIn, exactBlockIntersection);
							}
						}

						if (increaseMiningSpeed && (b->bestTool != withHand))
						{
							fp durabilityDecrease = breakingBlock ? 1 * secondsPerTick : 0.5 * secondsPerTick;

							if (isMaterialTool(itemHolding->stackItemID) && getMaterialToolType(itemHolding->stackItemID) == toolTypeID::sword)
							{
								durabilityDecrease *= 2;
							}

							decreaseDurability(*itemHolding, durabilityDecrease);
						}
					}
				}
			}
		}
	}

	if (entityType == entityID::human)
	{
		((human*)this)->addExhaustion(exhaustionIncrease);
	}

	crectangle2& hitbox = calculateHitBox();
	//else you will have friction while you are just walking past a ladder
	climbing = dimensionIn->blockRangeContains(floorVector(hitbox.pos0),
		floorVector(hitbox.pos1()),
		{ blockID::ladder, blockID::vine, blockID::twisting_vines, blockID::weeping_vines });

}
bool humanoid::placeBlock(blockID blockToPlace)
{
	if (dimensionIn->identifier == dimensionID::nether)
	{
		if (blockToPlace == blockID::water)
		{
			extinguishSound->playRandomSound(selectedBlockContainer, cvec2(selectedBlockPosition) + 0.5);
			return true;
		}
		else if (blockToPlace == blockID::wet_sponge)
		{
			blockToPlace = blockID::sponge;
		}
		else
		{
			goto dontExtinguish;
		}
		extinguishSound->playRandomSound(selectedBlockContainer, cvec2(selectedBlockPosition) + 0.5);
	dontExtinguish:;
	}

	block* posessingBlock = blockList[(int)selectedBlockContainer->getBlockID(adjacentBlockPosition)];
	if (posessingBlock->canReplaceBlock)
	{
		if (selectedBlockContainer->inBounds(adjacentBlockPosition) &&
			blockList[(int)blockToPlace]->canPlace(selectedBlockContainer, adjacentBlockPosition))
		{
			if (blockList[(int)blockToPlace]->blockCollisionType == collisionTypeID::willCollide)
			{
				crectangle2 checkRect = selectedBlockContainer->containerToRootTransform.multRectMatrix(crectangle2(adjacentBlockPosition, vec2(1)));

				std::vector<entity*> entityList = dimensionIn->findNearEntities(crectangle2(cvec2(adjacentBlockPosition), cvec2(1)).expanded(mobSizeMargin));

				//check for all mobs if it collides
				for (entity* e : entityList)
				{
					if ((e->entityType != entityID::item) && (e->entityType != entityID::particle) && (e->entityType != entityID::pollen))
					{
						if (collides2d(e->calculateHitBox(e->position), checkRect))
						{
							//entity in the way
							return false;
						}
					}
				}
			}

			//place one block
			blockData* placedBlockData = createBlockData(blockToPlace);

			cvec2 newBlockMiddle = cvec2(adjacentBlockPosition) + cvec2(0.5);
			cvec2 relativePosition = exactBlockIntersection - getHeadPosition();
			if (hasUpsideDownData(blockToPlace))
			{
				bool upsideDown = exactBlockIntersection.y < newBlockMiddle.y;
				if (isStairs(blockToPlace))
				{
					upsideDown = !upsideDown;
				}
				dynamic_cast<upsideDownData*> (placedBlockData)->upsideDown = upsideDown;
			}
			directionID direction;
			if (hasFacingData(blockToPlace))
			{
				if (canFaceUp(blockToPlace))
				{

					if (relativePosition.x < 0)
					{
						direction = directionID::negativeX;
					}
					else
					{
						direction = directionID::positiveX;
					}
					if (abs(relativePosition.y) > abs(relativePosition.x))
					{
						if (relativePosition.y < 0)
						{
							direction = directionID::negativeY;
						}
						else
						{
							direction = directionID::positiveY;
						}
					}
				}
				else
				{
					if (exactBlockIntersection.x < newBlockMiddle.x)
					{
						direction = directionID::negativeX;
					}
					else
					{
						direction = directionID::positiveX;
					}
				}
				if (invertPlacementDirection(blockToPlace))
				{
					direction = flipDirection(direction);
				}
				dynamic_cast<facingData*> (placedBlockData)->directionFacing = direction;
			}
			else
			{
				direction = standardUpFacingBlockDirection;
			}

			if (hasAttachmentDirectionData(blockToPlace))
			{
				int nearestAttachment = -1;
				fp nearestAttachmentDistanceSquared = INFINITY;

				cvec2 relativeAttachmentPosition = exactBlockIntersection - cvec2(adjacentBlockPosition);

				//check which block to attach to
				for (fsize_t i = 0; i < directionCount2D; i++)
				{
					if ((!isTorch(blockToPlace)) || ((directionID)i != directionID::positiveY))
					{
						cveci2 attachmentPosition = adjacentBlockPosition + directionVectors2D[i];

						if (selectedBlockContainer->canAttachTo(attachmentPosition, flipDirection((directionID)i)))
						{
							cvec2 currentRelativeAttachmentPosition = cvec2(0.5) + cvec2(directionVectors2D[i]) * 0.5;
							cfp attachmentDistanceSquared = (currentRelativeAttachmentPosition - relativeAttachmentPosition).lengthSquared();

							if (attachmentDistanceSquared < nearestAttachmentDistanceSquared)
							{
								nearestAttachment = i;
								nearestAttachmentDistanceSquared = attachmentDistanceSquared;
							}
						}
					}
				}
				if (nearestAttachment == -1)
				{

					//can't place
					return false;
				}
				else
				{
					dynamic_cast<attachmentDirectionData*>(placedBlockData)->attachmentDirection = (directionID)nearestAttachment;
				}
			}

			if (isDoubleBlockWhenPlaced(blockToPlace))
			{
				blockData* otherPartData = createBlockData(blockToPlace);
				dynamic_cast<doubleBlockData*>(placedBlockData)->isPart0 = true;
				dynamic_cast<doubleBlockData*>(otherPartData)->isPart0 = false;

				if (hasFacingData(blockToPlace))
				{
					dynamic_cast<facingData*>(otherPartData)->directionFacing = direction;
				}

				cveci2 part1Pos = adjacentBlockPosition + getOtherPartRelativeLocation(blockToPlace, true, direction);
				selectedBlockContainer->setBlockWithData(part1Pos, blockToPlace, otherPartData, chunkLoadLevel::updateLoaded);
			}
			if (isSlab(blockToPlace))
			{
				dynamic_cast<slabData*> (placedBlockData)->type = exactBlockIntersection.y > newBlockMiddle.y ? slabType::topSlab : slabType::bottomSlab;
			}
			selectedBlockContainer->setBlockWithData(adjacentBlockPosition, blockToPlace, placedBlockData, chunkLoadLevel::updateLoaded);

			blockList[(int)blockToPlace]->placeSound->playRandomSound(selectedBlockContainer, exactBlockIntersection);
			return true;
		}
	}
	return false;
}
swingSynchronizer getLegSwingSynchronizer(cfp& legHeight, cfp& maxLegAngle)
{
	cfp speedMultiplier = 2;
	cfp distanceBetweenLegsSpread = sin(maxLegAngle) * legHeight * 2 * speedMultiplier;
	return swingSynchronizer(distanceBetweenLegsSpread, -maxLegAngle, maxLegAngle);
}
std::vector<vec3> humanoid::getFrictions() const
{
	std::vector<vec3> frictions = mob::getFrictions();
	if (climbing)
		frictions.push_back(vec3(0, 0, ladderFrictionMultiplier));
	return frictions;
}