#include "human.h"
#include "dimension.h"
#include "idAnalysis.h"
#include "keyID.h"
#include "application/interaction.h"
#include "rectangularSlotContainer.h"
#include "ridableEntity.h"
#include "endCrystal.h"
#include "gameControl.h"
#include "rectangularSlotContainer.h"

#include "gameForm.h"
#include "furnaceData.h"
#include "jigsawData.h"
#include "structureBlockData.h"
#include "chestData.h"
#include "bedData.h"
#include "openData.h"
#include "buttonData.h"
#include "leverData.h"
#include "repeaterData.h"
#include "comparatorData.h"
#include "noteBlockData.h"
#include "jukeBoxData.h"
#include "experienceOrb.h"
#include "mobData.h"
#include "gameModeData.h"
#include "floatingSlot.h"
#include "itemData.h"
#include "snowLayerData.h"
#include "boat.h"
#include "mobList.h"
#include "spawnerData.h"
#include "endPortalFrameData.h"
#include "sheep.h"
#include "fluidData.h"
#include "bucketData.h"
#include "potionData.h"
#include "itemParticleBrush.h"
#include "particle.h"
#include "soundList.h"
#include "world.h"
#include "resourcePack.h"
#include "math/sound/sound.h"
#include "playerControlledAI.h"
#include "folderList.h"
#include <filesystem/filemanager.h>
#include "minecraftFont.h"
// #include <minwindef.h>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <SFML/Audio/SoundSource.hpp>
#include "anvilSlotContainer.h"
#include "armorType.h"
#include "block.h"
#include "blockData.h"
#include "itemID.h"
#include "brewingStandData.h"
#include "brewingStandSlotContainer.h"
#include "chestSlotContainer.h"
#include "chunkLoadLevel.h"
#include "collisionDataCollection.h"
#include "collisionTypeID.h"
#include "constants.h"
#include "craftingTableSlotContainer.h"
#include "damageSource.h"
#include "damageType.h"
#include "dimensionID.h"
#include "dispenserData.h"
#include "dispenserSlotContainer.h"
#include "doubleBlockData.h"
#include "enchantingTableSlotContainer.h"
#include "enchantmentID.h"
#include "entity.h"
#include "entityData.h"
#include "entityID.h"
#include "furnaceSlotContainer.h"
#include "gameModeID.h"
#include "harvestType.h"
#include "humanoid.h"
#include "humanSlotContainerUI.h"
#include "globalFunctions.h"
#include "interface/inamable.h"
#include "math/collisions.h"
#include "math/direction.h"
#include "math/graphics/brush/brushes.h"
#include "math/graphics/color/color.h"
#include "math/graphics/resolutiontexture.h"
#include "math/keyframe.h"
#include "math/mathFunctions.h"
#include "math/mattnxn.h"
#include "math/random/random.h"
#include "math/rectangle/rectangletn.h"
#include "math/uuid.h"
#include "math/vector/vectn.h"
#include "inventory.h"
#include "itemID.h"
#include "itemStack.h"
#include "lightLevel.h"
#include "lootTable.h"
#include "mob.h"
#include "nbt/nbtSerializable.h"
#include "nbt/nbtSerializer.h"
#include "gameRenderData.h"
#include "smithingTableSlotContainer.h"
#include "soundCollection.h"
#include "statusEffect.h"
#include "statusEffectID.h"
#include "treeItemTypeID.h"
#include "woodtypeID.h"
#include "math/graphics/brush/brushes/alphaMask.h"
#include "nbt/nbtSerializer.h"
#include "include/filesystem/fileFunctions.h"
// std::shared_ptr<audio2d> currentWindSound;

void human::tick()
{
	// clear decisions
	wantsToStartUsing = false;
	wantsToKeepUsing = false;
	wantsToStopUsing = false;
	humanoid::tick();
	updateSelection();

	entity *selectedEntity = selectedUUID ? dimensionIn->findUUID(position, humanArmRange + mobSizeMargin, selectedUUID) : nullptr;
	if (currentGameMode != gameModeID::spectator)
	{
		pickUpFloatingSlots();
	}

	mainBodyPart->flipX = lookingAt.x < position.x;

	const bool &pressedExit = screen.pressedKey((vk)keyID::exit);
	if (pressedExit)
	{
		if (UUIDRidingOn)
		{
			exitRodeEntity();
		}
	}

	if (sleeping)
	{
		if (!canSleep() || !isBed(dimensionIn->getBlockID(floorVector(position))) || pressedExit)
		{
			sleeping = false;
			// spawn next to the bed
			newPosition.y -= bedHitboxHeight;
		}
	}

	if (wantsToStartUsing)
	{
		if (!sneaking)
		{
			if (selectedEntity)
			{
				if (isRidable(selectedEntity->entityType))
				{
					ridableEntity *selectedRidable = (ridableEntity *)selectedEntity;
					uuid uuidRodeOn = UUIDRidingOn;
					if (uuidRodeOn)
					{
						exitRodeEntity();
					}
					if (UUIDRidingOn != selectedRidable->identifier)
					{
						//'ride' the boat
						selectedRidable->addPassenger(this);
					}
					goto rightClickUsed;
				}
				else if (selectedEntity->entityType == entityID::end_crystal)
				{
					((endCrystal *)selectedEntity)->explode();
					goto rightClickUsed;
				}
			}
			// check for interactable block
			blockID selectedBlockID = selectedBlockContainer->getBlockID(selectedBlockPosition);
			blockData *selectedBlockData = selectedBlockContainer->getBlockData(selectedBlockPosition);
			if (hasGUI(selectedBlockID))
			{
				switch (selectedBlockID)
				{
				case blockID::crafting_table:
				{
					// interact with crafting table
					screen.inventoryUI->linkUp(craftingTableSlots);
					break;
				}
				case blockID::smithing_table:
				{
					// interact with crafting table
					screen.inventoryUI->linkUp(smithingTableSlots);
					break;
				}
				case blockID::anvil:
				{
					// interact with crafting table
					screen.inventoryUI->linkUp(anvilSlots);
					break;
				}
				case blockID::brewing_stand:
				{
					screen.inventoryUI->linkUp(brewingStandSlots);
					brewingstandData *selectedBrewingStandData = (brewingstandData *)selectedBlockData;
					brewingStandSlots->selectedBrewingStandData = selectedBrewingStandData;
					brewingStandSlots->blazePowderSlot->linkedContainer = selectedBrewingStandData->blazePowderSlot;
					brewingStandSlots->ingredientSlot->linkedContainer = selectedBrewingStandData->ingredientSlot;
					for (int i = 0; i < brewingStandPotionCapacity; i++)
					{
						brewingStandSlots->potionSlots[i]->linkedContainer = selectedBrewingStandData->potionSlots[i];
					}
					break;
				}
				case blockID::enchanting_table:
				{
					// interact with enchanting table
					screen.inventoryUI->linkUp(enchantmentSlots);
					break;
				}
				case blockID::structure_block:
				{
					screen.focusChild(screen.structureBlockOptions);
					screen.structureBlockOptions->temporaryStructureBlockData = new structureBlockData(*(structureBlockData *)selectedBlockData);
					screen.structureBlockOptions->serializeWithTemporaryData(false);
					break;
				}
				case blockID::jigsaw:
				{
					screen.focusChild(screen.jigsawOptions);
					screen.jigsawOptions->temporaryJigsawData = new jigsawData(*dynamic_cast<jigsawData *>(selectedBlockData));
					screen.jigsawOptions->serializeWithTemporaryData(false);
					break;
				}
				default:
				{
					if (isDispenser(selectedBlockID))
					{
						screen.inventoryUI->linkUp(dispenserSlots);
						// link containers
						dispenserData *selectedDispenserData = dynamic_cast<dispenserData *>(selectedBlockData);
						dispenserSlots->selectedDispenserData = selectedDispenserData;
						dispenserSlots->dispenserSlots->linkedContainer = selectedDispenserData->slots;
					}
					else if (isFurnace(selectedBlockID))
					{
						// interact with furnace
						screen.inventoryUI->linkUp(furnaceSlots);
						// link containers
						furnaceData *selectedFurnaceData = (furnaceData *)selectedBlockData;
						furnaceSlots->selectedFurnaceData = selectedFurnaceData;

						furnaceSlots->furnaceInputSlot->linkedContainer = selectedFurnaceData->inputSlot;
						furnaceSlots->furnaceOutputSlot->linkedContainer = selectedFurnaceData->outputSlot;
						furnaceSlots->furnaceFuelSlot->linkedContainer = selectedFurnaceData->fuelSlot;
					}
					else if (hasChestData(selectedBlockID))
					{
						screen.inventoryUI->linkUp(chestSlots);
						// link containers
						chestData *selectedChestData = (chestData *)selectedBlockData;
						selectedChestData->generateChestLoot();
						chestSlots->selectedChestData = selectedChestData;
						chestSlots->chestSlots->linkedContainer = selectedChestData->slots;

						(isChest(selectedBlockID) ? chestOpenSound : barrelOpenSound)->playRandomSound(selectedBlockContainer, selectedBlockPosition + cvec2(0.5));
					}
					else
					{
						currentWorld->currentChat.addLine(std::wstring(L"interaction not implemented"));
					}
				}
				break;
				}
				if (!hasCustomGUI(selectedBlockID))
				{
					screen.focusChild(screen.inventoryUI);
				}
				selectedContainerContainer = selectedBlockContainer;
				selectedContainerPosition = selectedBlockPosition;
				screen.focusedChild->visible = true;
				goto rightClickUsed;
			}
			else
			{
				if (isBed(selectedBlockID))
				{
					bedData *data = dynamic_cast<bedData *>(selectedBlockData);
					if (dimensionIn->identifier == dimensionID::overworld)
					{
						if (canSleep())
						{
							sleeping = true;
							cvec2 middlePosition = cvec2(selectedBlockPosition.x + getOtherPartRelativeLocation(selectedBlockID, data->isPart0, data->directionFacing).x * 0.5 + 0.5, selectedBlockPosition.y + bedHitboxHeight + math::fpepsilon);
							cvec2 &absolute = selectedBlockContainer->containerToRootTransform.multPointMatrix(middlePosition);
							// set respawn point
							currentWorld->worldSpawnPoint = absolute;
							currentWorld->worldSpawnDimension = dimensionIn->identifier;
							teleportTo(dimensionIn, absolute, false);
							goto rightClickUsed;
						}
					}
					else
					{
						cvec2 explosionPosition = cvec2(selectedBlockPosition) + cvec2(getOtherPartRelativeLocation(selectedBlockID, data->isPart0, data->directionFacing)) * 0.5;
						selectedBlockContainer->createExplosion(explosionPosition, 5, true);
						goto rightClickUsed;
					}
				}
				else if (((isDoor(selectedBlockID) && (selectedBlockID != blockID::iron_door)) || isFenceGate(selectedBlockID) || (isTrapDoor(selectedBlockID) && (selectedBlockID != blockID::iron_trapdoor))))
				{
					selectedBlockContainer->openDoorType(selectedBlockPosition, !dynamic_cast<openData *>(selectedBlockContainer->getBlockData(selectedBlockPosition))->isOpen);
					goto rightClickUsed;
				}
				else if (isButton(selectedBlockID))
				{
					const collisionDataCollection &buttonCollision = blockList[(int)selectedBlockID]->getCollisionData(selectedBlockContainer, selectedBlockPosition);
					if (buttonCollision.hitboxes[0].hitboxCollidingWith.contains(exactBlockIntersection))
					{
						dynamic_cast<buttonData *>(selectedBlockData)->ticksToPress = isWoodenButton(selectedBlockID) ? (ticksPerRealLifeSecond * 3) / 2 : ticksPerRealLifeSecond;
						dimensionIn->addUpdatePosition(selectedBlockPosition);
					}
					goto rightClickUsed;
				}
				else if (selectedBlockID == blockID::lever)
				{
					leverData *toLeverData = dynamic_cast<leverData *>(selectedBlockData);
					toLeverData->active = !toLeverData->active;
					dimensionIn->addUpdatePosition(selectedBlockPosition);
					goto rightClickUsed;
				}
				else if (selectedBlockID == blockID::repeater)
				{
					repeaterData *toRepeaterData = dynamic_cast<repeaterData *>(selectedBlockData);
					toRepeaterData->delayArrayIndex = (toRepeaterData->delayArrayIndex + 1) % delayArraySize;
					dimensionIn->addUpdatePosition(selectedBlockPosition);
					goto rightClickUsed;
				}
				else if (selectedBlockID == blockID::comparator)
				{
					comparatorData *toComparatorData = dynamic_cast<comparatorData *>(selectedBlockData);
					toComparatorData->substractionMode = !toComparatorData->substractionMode;
					dimensionIn->addUpdatePosition(selectedBlockPosition);
					goto rightClickUsed;
				}
				else if (selectedBlockID == blockID::jukebox)
				{
					jukeBoxData *toJukeBoxData = dynamic_cast<jukeBoxData *>(selectedBlockData);
					if (toJukeBoxData->recordSlot->slots[0].count)
					{
						// remove music disc from jukebox
						addStackOrDrop(toJukeBoxData->recordSlot->slots[0]);

						//handler->stopAudio(toJukeBoxData->musicPlaying);

						// for safety
						//toJukeBoxData->musicPlaying = nullptr;

						goto rightClickUsed;
					}
				}
				else if (selectedBlockID == blockID::note_block)
				{
					noteBlockData *toNoteBlockData = dynamic_cast<noteBlockData *>(selectedBlockData);

					toNoteBlockData->note = (toNoteBlockData->note + 1) % maxNoteBlockNote;

					toNoteBlockData->playNote(dimensionIn, selectedBlockPosition);
				}
			}
		}
		// TODO: right-click an item to pick it up from the ground
	}
	if ((wantsToStartUsing || wantsToKeepUsing) && itemHolding->count)
	{
		// place or use
		// place the hotbar slot
		onItemRightClick(*itemHolding);
		goto rightClickUsed;
	}
rightClickUsed:;
	if (wantsToStopUsing && itemHolding->count)
	{
		onItemRightClickReleased(*itemHolding);
	}

	calculateFood();

	// if (currentWindSound)
	//{
	//	if (currentWindSound->getStatus() == sf::SoundSource::Stopped) {
	//		currentWindSound->play();
	//	}
	// }
	// else {
	//	currentWindSound = windSound->playRandomSound();
	// }

	// cfp& windSpeed = dimensionIn->getWindSpeed(getHeadPosition()).length();
	// static const transition<fp> speedToVolume = transition<fp>({
	//	keyFrame<fp>(2,0),
	//	keyFrame<fp>(20,1),
	//	});
	// currentWindSound->setVolume(math::lerp(currentWindSound->volume, speedToVolume.getValue(windSpeed), 0.2));
	// static const transition<fp> speedToPitch = transition<fp>({
	//	keyFrame<fp>(6,1),
	//	keyFrame<fp>(15,2),
	//	});
	//
	// currentWindSound->setPitch(math::lerp(currentWindSound->pitch, speedToPitch.getValue(windSpeed), 0.2));
}

void human::applyStatusEffect(const statusEffect &effect)
{
	if (effect.identifier == statusEffectID::hunger)
	{
		addExhaustion(0.005 * effect.potency);
	}
	else
	{
		humanoid::applyStatusEffect(effect);
	}
}

void human::onDeath()
{
	if (!currentWorld->keepInventoryOnDeath)
	{
		std::vector<rectangularSlotContainer *> containers =
			{
				hotbarSlots,
				leftHandSlot,
				inventorySlots,
				armorSlots,
				leftHandSlot,
			};
		// drop all items
		for (rectangularSlotContainer *container : containers)
		{
			// container->dropContent(getDropPosition(), floatingSlotSpeedOnDeath);
			cint &slotCount = container->rowsAndColumns.x * container->rowsAndColumns.y;
			cvec2 &dropPosition = getDropPosition();
			for (int i = 0; i < slotCount; i++)
			{
				if (container->slots[i].count)
				{
					if (container->slots[i].getEnchantmentLevel(enchantmentID::curseOfVanishing) == 0)
					{
						lootTable::dropLoot({container->slots[i]}, dimensionIn, dropPosition, floatingSlotSpeedOnDeath);
					}
					container->slots[i].clearData();
				}
			}
		}
		// drop experience
		cint &levels = math::floor(getExperienceLevel(experience));
		cint &experienceToDrop = math::minimum(levels * 7, 100);
		dropExperienceOrbs(dimensionIn, position, experienceToDrop);

		experience = 0;
		score = 0;
	}
	// dont call mob::ondeath because it will look for the loot table of a human
	entity::onDeath();
	despawn = false; // don't let the human despawn!
	// create a copy that respawns
	respawn();
}

human::~human()
{
	delete leftHandSlot;

	delete inventorySlots;

	delete hotbarSlots;

	itemHolding = nullptr;
}

void human::serializeValue(nbtSerializer &s)
{
	humanoid::serializeValue(s);

	hotbarSlots->serialize(s, std::wstring(L"hotbar slots"));
	inventorySlots->serialize(s, std::wstring(L"inventory slots"));
	leftHandSlot->serialize(s, std::wstring(L"left hand slot"));

	s.serializeValue(std::wstring(L"experience"), experience);
	s.serializeValue(std::wstring(L"score"), score);
	s.serializeValue(std::wstring(L"food level"), foodlevel);
	s.serializeValue(std::wstring(L"food exhaustion level"), foodExhaustionlevel);
	s.serializeValue(std::wstring(L"food saturation level"), foodsaturationlevel);
	s.serializeValue(std::wstring(L"food tick timer"), foodticktimer);
	s.serializeValue(std::wstring(L"food animation ticks"), foodAnimationTime);
	s.serializeValue(std::wstring(L"right hand slot index"), rightHandSlotIndex);
	s.serializeValue(std::wstring(L"gamemode"), currentGameMode);
	s.serializeValue(std::wstring(L"has seen credits"), seenCredits);
	s.serializeValue(std::wstring(L"spectator speed"), spectatorSpeed);
	s.serializeValue(std::wstring(L"visible range"), visibleRangeXWalk);
}

bool human::serialize(cbool &write)
{
	const stdPath &playersFolder = savesFolder / currentWorld->name / L"players";
	if (write)
	{
		createFoldersIfNotExists(playersFolder);
	}
	const stdPath &path = playersFolder / (std::wstring)identifier;
	// to point out which 'serialize' function to use. this might cause errors. is mob::serialize calling human::serializeValue?
	return mob::serialize(L"player", path, write);
}

bool human::addDamageSource(cfp &damage, std::shared_ptr<damageSource> source)
{
	if (currentGameMode == gameModeID::survival || source.get()->type == voidDamage)
	{
		if (currentGameMode == gameModeID::survival)
		{
			addExhaustion(0.1 * damage);
		}
		return humanoid::addDamageSource(damage, source);
	}
	else
	{
		return false;
	}
}

bool human::compareSelector(const human &sender, const std::wstring &selectorString) const
{
	return entity::compareSelector(sender, selectorString) ||
		   ((&sender == this) && (selectorString == std::wstring(L"@p"))) || (selectorString == std::wstring(L"@a"));
}

bool human::addStack(itemStack &stack)
{
	return hotbarSlots->addStack(stack) || inventorySlots->addStack(stack);
}

void human::addStackOrDrop(itemStack &stack)
{
	if (!addStack(stack))
	{
		drop(stack);
	}
}

bool human::substractStack(itemStack &stack)
{
	return hotbarSlots->substractStack(stack) || inventorySlots->substractStack(stack);
}

std::vector<vec3> human::getFrictions() const
{
	return flying ? std::vector<vec3>({noCollisionFriction}) : humanoid::getFrictions();
}

void human::render(const gameRenderData &targetData) const
{
	if (currentGameMode == gameModeID::spectator)
	{
		if (targetData.screen.player->currentGameMode != gameModeID::spectator)
			return;

		updateBodyParts();

		const auto &transparencyBrush = solidColorBrush(color(color::halfMaxValue, 0));
		const auto &transparentSkin = alphaMask<solidColorBrush, resolutionTexture>(transparencyBrush, *((mobData *)entityDataList[(int)entityType])->skin);

		head->hasTransparency = true;
		renderBodyPart(head, mat3x3::cross(targetData.worldToRenderTargetTransform, head->getCumulativeParentTransform()), transparentSkin, targetData);
	}
	else
	{
		head->hasTransparency = false;
		humanoid::render(targetData);
	}
	if (!sneaking)
	{
		// render nametag
		crectangle2 &hitbox = calculateHitBox();
		constexpr fp averageLetterRelativeWidth = 0.7;
		constexpr fp averageNameLetterCount = 7.0;
		constexpr fp averageNameTagSize = 1.0;
		constexpr fp letterSize = averageNameTagSize / (averageNameLetterCount * averageLetterRelativeWidth);
		constexpr fp maxNameLength = 2.0;
		minecraftFont clonedFont = minecraftFont(letterSize);
		cvec2 &nameTagMiddleBottom = hitbox.pos0 + cvec2(hitbox.size.x * 0.5, hitbox.size.y);
		cvec2 &movedBottom = nameTagMiddleBottom + cvec2(clonedFont.measureStringSize(cvec2(maxNameLength, letterSize), name) * -0.5, 0);

		rectangle2 nameTagRect = crectangle2(movedBottom, cvec2(maxNameLength, letterSize));
		clonedFont.DrawString(name, nameTagRect, targetData.renderTarget, targetData.worldToRenderTargetTransform);
	}
}

void human::setGameMode(const gameModeID &newGameMode)
{
	if (newGameMode == gameModeID::spectator)
	{
		collideLevel = collisionTypeID::willNotCollide;
		// ensure to fly, to stop falling out of the world
		flying = true;
		speed = cvec2();
	}
	else if (currentGameMode == gameModeID::spectator)
	{
		collideLevel = collisionTypeID::willCollideTop;
	}
	flying = flying && gameModeDataList[(int)newGameMode]->canFly;

	currentGameMode = newGameMode;
}

void human::closeBlockGUI()
{
	screen.inventoryUI->unLink();

	selectedContainerContainer = nullptr;
	selectedContainerPosition = veci2();
}

void human::pickUpFloatingSlots()
{
	bool pickedUpExperience = false;
	// pick items up
	crectangle2 hitbox = calculateHitBox(position);
	crectangle2 floatingSlotAttractionBox = hitbox.expanded(1.0f);	// attract items from 1 block away
	crectangle2 experienceOrbAttractionBox = hitbox.expanded(7.0f); // attract experience from 7 blocks away

	std::vector<entity *> entityList = dimensionIn->findNearEntities(experienceOrbAttractionBox);
	for (entity *const &currentEntity : entityList)
	{
		// entity* currentEntity = entityList[i];
		if (currentEntity->entityType == entityID::item)
		{
			floatingSlot *slot = (floatingSlot *)currentEntity;
			if ((slot->ticksAvailable >= 0) && floatingSlotAttractionBox.contains(slot->position))
			{
				if (collides2d(hitbox, slot->calculateHitBox()))
				{
					// pick item up
					if (addStack(slot->stack))
					{
						slot->despawn = true;
						popSound->playRandomSound(dimensionIn, getDropPosition());
					}
				}
				else
				{
					// attract towards the player
					vec2 difference = position - slot->position;
					fp length = difference.length();
					if (length > 0) // else NAN
					{
						// faster than gravity, so the player has a chance to pick up an item from a block broken below.
						slot->speed += difference / length * 0.2;
					}
				}
			}
		}
		else if (currentEntity->entityType == entityID::experience_orb)
		{
			experienceOrb *orb = (experienceOrb *)currentEntity;
			if (!pickedUpExperience && collides2d(hitbox, orb->calculateHitBox()))
			{
				orb->despawn = true;
				pickedUpExperience = true;
				// add experience
				addExperience(orb->value);
			}
			else
			{
				// attract towards the player
				vec2 difference = position - orb->position;
				fp length = difference.length();
				if (length > 0) // else NAN
				{
					// faster than gravity, so the player has a chance to pick up an item from a block broken below.
					orb->speed += difference / length * 0.2;
				}
			}
		}
	}
}
void human::drop(itemStack &s, cint &amount)
{
	constexpr fp humanDropSpeed = 10;
	cvec2 &dropPosition = getDropPosition();
	floatingSlot *slot = (floatingSlot *)summonEntity(entityID::item, dimensionIn, dropPosition);
	slot->speed = (lookingAt - getHeadPosition()).normalized() * humanDropSpeed;
	popSound->playRandomSound(dimensionIn, dropPosition);

	// wait 2 seconds until it can be picked up
	slot->ticksAvailable = ticksPerRealLifeSecond * -2;

	if (amount == 0) // drop the whole stack
	{
		slot->stack.addStack(s);
	}
	else
	{
		slot->stack.addStack(s, amount);
	}
}
vec2 human::getDropPosition()
{
	return cvec2(position.x, position.y + humanLegSize.y);
}

bool human::canSleep() const
{
	return timeToLightLevel.getValue(currentWorld->getTimeOfDay()) < maxLightLevel && immunityFrameCount == 0;
}
human::human(dimension *dimensionIn, cvec2 &position, gameControl &screen, const std::wstring &name) : humanoid(dimensionIn, position, entityID::human), INamable(name),
																									   screen(screen)
{
	initializeBodyParts(humanHeadTextureRect, humanBodyTextureRect, humanLeftLegTextureRect, humanRightLegTextureRect, humanLeftArmTextureRect, humanRightArmTextureRect);
	hotbarSlots = new rectangularSlotContainer(cveci2(StandardInventoryColumnCount, 1));
	leftHandSlot = new rectangularSlotContainer(cveci2(1));
	inventorySlots = new rectangularSlotContainer(cveci2(StandardInventoryColumnCount, StandardInventoryRowCount));

	// TODO: make enchantment slot random save
	// add slotcontainers. initialized here because the enchantment slots depend on world random (else you can save and reload to change the enchantment roll)
	craftingTableSlots = new craftingTableSlotContainer();
	smithingTableSlots = new smithingTableSlotContainer();
	anvilSlots = new anvilSlotContainer();

	furnaceSlots = new furnaceSlotContainer();
	chestSlots = new chestSlotContainer();
	brewingStandSlots = new brewingStandSlotContainer();
	enchantmentSlots = new enchantingTableSlotContainer();
	dispenserSlots = new dispenserSlotContainer();
	humanSlots = new humanSlotContainerUI();
	linkUpInventories();
	tasks = new playerControlledAI(this);
	// enchantmentSlots->enchantmentSeed = rand(worldRandom);
}
void human::onItemRightClick(itemStack &stackIn)
{
	blockID selectedBlock = selectedBlockContainer->getBlockID(selectedBlockPosition);
	entity *selectedEntity = getSelectedEntity();

	if (itemList[(int)stackIn.stackItemID]->harvestType == harvestTypeID::withHoe)
	{
		std::vector<blockID> hoeArray = {blockID::dirt, blockID::grass_block};
		if (std::find(hoeArray.begin(), hoeArray.end(), selectedBlock) != hoeArray.end())
		{
			selectedBlockContainer->setBlockID(selectedBlockPosition, blockID::farmland, chunkLoadLevel::updateLoaded);
			tillSound->playRandomSound(selectedBlockContainer, exactBlockIntersection);

			decreaseDurability(stackIn, 1, exactBlockIntersection);

			return;
		}
	}
	else if (itemList[(int)stackIn.stackItemID]->harvestType == harvestTypeID::withAxe)
	{
		// check if the selected block is a log
		if (isTreeType(selectedBlock) && getTreeItemType(selectedBlock) == treeItemTypeID::log)
		{
			// caution! data will be deleted once we setBlockWithData
			facingData *data = dynamic_cast<facingData *>(selectedBlockContainer->getBlockData(selectedBlockPosition));
			selectedBlockContainer->setBlockWithData(selectedBlockPosition, (blockID)((int)selectedBlock + 1), new facingData(data->directionFacing), chunkLoadLevel::updateLoaded);
			;
			stripLogSound->playRandomSound(selectedBlockContainer, exactBlockIntersection);

			decreaseDurability(stackIn, 1, exactBlockIntersection);
			return;
		}
	}
	else if (isMusicDisc(stackIn.stackItemID) && wantsToStartUsing)
	{
		if (selectedBlock == blockID::jukebox)
		{
			jukeBoxData *data = (jukeBoxData *)selectedBlockContainer->getBlockData(selectedBlockPosition);
			if (data->recordSlot->slots[0].count == 0)
			{
				data->recordSlot->addStack(stackIn);
				data->playMusic(selectedBlockContainer, selectedBlockPosition);
				return;
			}
		}
	}
	else if (wantsToStartUsing)
	{
		const blockID &blockToPlace = getBlockToPlace(stackIn.stackItemID);
		if ((int)blockToPlace && placeBlock(blockToPlace))
		{
			if (currentGameMode != gameModeID::creative)
			{
				stackIn.add(-1);
			}
			return;
		}
	}
	if (stackIn.stackItemID == itemID::end_crystal && wantsToStartUsing)
	{
		if (selectedBlock == blockID::obsidian || selectedBlock == blockID::bedrock)
		{
			cvec2 endCrystalPosition = cvec2(selectedBlockPosition) + cvec2(0.5, 1 + math::fpepsilon);
			rectangle2 hitboxToTest = endCrystalRelativeHitbox;
			hitboxToTest.pos0 += endCrystalPosition;

			if (selectedBlockContainer->getHitboxCollisionType(hitboxToTest) == collisionTypeID::willNotCollide)
			{
				if (currentGameMode != gameModeID::creative)
				{
					stackIn.add(-1);
				}
				summonEntity(entityID::end_crystal, selectedBlockContainer, endCrystalPosition);
				return;
			}
		}
	}
	else if ((stackIn.stackItemID == itemID::flint_and_steel) && wantsToStartUsing)
	{
		if (selectedBlock == blockID::air)
		{
			if (blockList[(int)blockID::fire]->canPlace(selectedBlockContainer, selectedBlockPosition))
			{
				// set to fire
				selectedBlockContainer->setBlockID(selectedBlockPosition, blockID::fire, chunkLoadLevel::updateLoaded);

				decreaseDurability(stackIn, 1, exactBlockIntersection);

				flintAndSteelSound->playRandomSound(selectedBlockContainer, exactBlockIntersection);
				return;
			}
		}
		else if (selectedBlock == blockID::tnt)
		{
			selectedBlockContainer->fuseTNT(selectedBlockPosition);
			return;
		}
	}

	if (getArmorType(stackIn.stackItemID))
	{
		// switch or equip armor
		// equip armor
		itemStack *armorSlot = &armorSlots->slots[getArmorType(stackIn.stackItemID) - bootsArmorType];
		if (!wantsToStartUsing && armorSlot->count)
		{
			return; // to prevent continuous swapping of armor, but to allow putting all armor on while holding RMB and scrolling
		}
		std::swap(*armorSlot, stackIn);
		getEquipSound(stackIn.stackItemID)->playRandomSound(dimensionIn, getHeadPosition());
	}
	else if (wantsToStartUsing && stackIn.stackItemID == (itemID)blockID::snow)
	{
		if (selectedBlock == blockID::snow)
		{
			snowLayerData *data = dynamic_cast<snowLayerData *>(selectedBlockContainer->getBlockData(selectedBlockPosition));
			if (data->layerThickness < 1)
			{
				data->layerThickness = math::minimum(data->layerThickness + minimalSnowLayerThickness, (fp)1.0);
				if (currentGameMode != gameModeID::creative)
				{
					stackIn.add(-1);
				}
				return;
			}
		}
	}
	else if (wantsToStartUsing && isBoatOrMinecart(stackIn.stackItemID))
	{
		const entityID &entityTypeToSpawn = isBoat(stackIn.stackItemID) ? entityID::boat : entityID::minecart;

		// place boat
		vec2 positionToPlace = exactBlockIntersection;
		crectangle2 initialRelativeHitbox = entityDataList[(int)entityTypeToSpawn]->initialRelativeHitbox;
		vec2 boatPos00 = positionToPlace - initialRelativeHitbox.size * 0.5;
		ridableEntity *r = (ridableEntity *)createEntity(entityTypeToSpawn, dimensionIn, boatPos00 - initialRelativeHitbox.pos0);

		// check if hitbox collides
		collisionTypeID top = dimensionIn->getHitboxCollisionType(r->calculateHitBox());
		collisionTypeID bottom = dimensionIn->getHitboxCollisionType(crectangle2(boatPos00.x, boatPos00.y - 1, initialRelativeHitbox.size.x, initialRelativeHitbox.size.y));
		if (top != collisionTypeID::willCollide && (bottom > top || (isBoat(stackIn.stackItemID) && selectedBlockContainer->getBlockID(selectedBlockPosition) == blockID::water && selectedBlockContainer->getBlockID(selectedBlockPosition + cveci2(0, 1)) == blockID::air)))
		{
			r->addToWorld();
			if (isBoat(stackIn.stackItemID))
			{
				((boat *)r)->boatType = (woodTypeID)((int)stackIn.stackItemID - (int)itemID::wood_boat);
			}
			r->addCollidingEntities();
			if (currentGameMode != gameModeID::creative)
			{
				stackIn.add(-1);
			}
			return;
		}
	}
	else if (stackIn.stackItemID == itemID::bone_meal)
	{
		blockID identifier = selectedBlockContainer->getBlockID(selectedBlockPosition);
		if (isSapling(identifier) || isCrop(identifier) || isMushroom(identifier))
		{
			// schedule growth for the next blockupdate
			selectedBlockContainer->getBlockData(selectedBlockPosition)->randomTick(selectedBlockContainer, selectedBlockPosition);

			if (currentGameMode != gameModeID::creative)
			{
				stackIn.add(-1);
			}
			return;
		}
	}
	else if (isSpawnEgg(stackIn.stackItemID))
	{
		if (wantsToStartUsing)
		{
			const entityID mobToSpawn = mobList[(int)stackIn.stackItemID - (int)itemID::spawn_egg];
			if (selectedBlock == blockID::spawner)
			{
				dynamic_cast<spawnerData *>(selectedBlockContainer->getBlockData(selectedBlockPosition))->entityToSpawn = mobToSpawn;
			}
			else
			{
				// spawn mob
				entity *summonedEntity = summonEntity(mobToSpawn, selectedBlockContainer, exactBlockIntersection);
				if (selectedUUID)
				{
					entity *e = selectedBlockContainer->findUUID(position, humanArmRange + mobSizeMargin, selectedUUID);
					if (e && isRidable(e->entityType))
					{
						ridableEntity *selectedRidable = (ridableEntity *)e;
						selectedRidable->addPassenger((mob *)summonedEntity);
					}
				}
			}
			if (currentGameMode != gameModeID::creative)
			{
				stackIn.add(-1);
			}
			return;
		}
	}
	else if (stackIn.stackItemID == itemID::ender_eye && selectedBlock == blockID::end_portal_frame)
	{
		endPortalFrameData *data = dynamic_cast<endPortalFrameData *>(selectedBlockContainer->getBlockData(selectedBlockPosition));
		if (!data->hasEye && endPortalFrameEyeBlockRect.contains(lookingAt - selectedBlockPosition))
		{
			int checkDirection = data->directionFacing == directionID::negativeX ? -1 : 1;
			for (int i = 0; i < maxEndPortalSize; i++)
			{
				cveci2 currentPosition = selectedBlockPosition + cveci2(checkDirection * i, 0);
				blockID currentBlock = selectedBlockContainer->getBlockID(currentPosition);
				if (currentBlock == blockID::end_portal_frame)
				{
					// found the other frame
					endPortalFrameData *otherSideData = dynamic_cast<endPortalFrameData *>(selectedBlockContainer->getBlockData(currentPosition));
					// check direction
					if (otherSideData->directionFacing != data->directionFacing)
					{
						data->hasEye = true;
						if (currentGameMode != gameModeID::creative)
						{
							stackIn.add(-1);
						}
						eyeOfEnderPlaceSound->playRandomSound(selectedBlockContainer, exactBlockIntersection);
						// the portal has both eyes
						if (otherSideData->hasEye)
						{
							// fill portal with end portal blocks
							selectedBlockContainer->setBlockRange(selectedBlockPosition + cveci2(checkDirection, 0), selectedBlockPosition + cveci2(checkDirection * (i - 1), 0), blockID::end_portal);
							// dragon sound
							endPortalOpenSound->playRandomSound(selectedBlockContainer, selectedBlockPosition + cvec2(0.5) + cveci2((i - 1) / 2 + 1, 0));
						}
					}
				}
				else if (currentBlock != blockID::air)
				{
					break;
				}
			}
			return;
		}
	}
	// an ender pearl can be placed or thrown, so no 'else'
	if (isThrowable(stackIn.stackItemID))
	{
		if (stackIn.stackItemID == itemID::bow)
		{
			bowAnimationTime++;
		}
		else if (wantsToStartUsing)
		{
			if (stackIn.stackItemID != itemID::ender_eye || dimensionIn->identifier == dimensionID::overworld)
			{
				launchItem(stackIn.stackItemID);
				if (currentGameMode != gameModeID::creative)
				{
					stackIn.add(-1);
				}
			}
		}
	}
	else if (stackIn.stackItemID == itemID::shears && wantsToStartUsing)
	{
		if (selectedEntity)
		{
			if (selectedEntity->entityType == entityID::sheep)
			{
				sheep *selectedSheep = dynamic_cast<sheep *>(selectedEntity);
				if (selectedSheep->hasWool)
				{
					cvec2 shearPosition = selectedSheep->mainBodyPart->translate;
					sheepShearingSound->playRandomSound(dimensionIn, shearPosition);

					// shearing a sheep takes a long time
					decreaseDurability(stackIn, 0x8, shearPosition);

					selectedSheep->hasWool = false;
					// drop wool at location of the main body part of the sheep
					lootTable::dropLoot({itemStack((itemID)((int)blockID::wool + (int)selectedSheep->woolColor), rand(currentRandom, 1, 3))}, dimensionIn, shearPosition, maxFloatingSlotSpeed);
					return;
				}
			}
		}
		else if (selectedBlock == blockID::pumpkin)
		{
			cvec2 &shearPosition = cvec2(selectedBlockPosition) + 0.5;
			lootTable::dropLoot({itemStack(itemID::pumpkin_seeds, rand(currentRandom, 1, 3))}, selectedBlockContainer, shearPosition, maxFloatingSlotSpeed);
			selectedBlockContainer->setBlockID(selectedBlockPosition, blockID::carved_pumpkin, chunkLoadLevel::updateLoaded);
			pumpkinCarveSound->playRandomSound(selectedBlockContainer, shearPosition);
			return;
		}
	}
	else if (stackIn.stackItemID == itemID::bucket && wantsToStartUsing)
	{
		if (selectedEntity && isMob(selectedEntity->entityType))
		{
			mob *selectedMob = (mob *)selectedEntity;
			if (selectedMob->entityType == entityID::cow)
			{
				stackIn.add(-1);
				itemStack milkBucketStack = itemStack(itemID::milk_bucket, 1);
				addStack(milkBucketStack);

				milkSound->playRandomSound(dimensionIn, exactEntityIntersection);
				return;
			}
		}

		// scoop up
		if (isFluid(selectedBlock))
		{
			fluidLevel fluidLevel = ((fluidData *)selectedBlockContainer->getBlockData(selectedBlockPosition))->currentFluidLevel;
			if (fluidLevel)
			{
				if (currentGameMode != gameModeID::creative)
				{
					stackIn.add(-1);
					itemStack fullBucketStack = itemStack(selectedBlock == blockID::water ? itemID::water_bucket : itemID::lava_bucket, 1);
					fullBucketStack.data = new bucketData(fluidLevel);

					addStack(fullBucketStack);
				}
				cvec2 soundPosition = selectedBlockPosition + cvec2(0.5);
				if (selectedBlock == blockID::water)
				{
					waterBucketFillSound->playRandomSound(selectedBlockContainer, soundPosition);
				}
				else
				{
					lavaBucketFillSound->playRandomSound(selectedBlockContainer, soundPosition);
				}
				selectedBlockContainer->setBlockID(selectedBlockPosition, blockID::air, chunkLoadLevel::updateLoaded);
				return;
			}
		}
	}
	else if ((stackIn.stackItemID == itemID::water_bucket || stackIn.stackItemID == itemID::lava_bucket))
	{
		blockID fluidID = stackIn.stackItemID == itemID::water_bucket ? blockID::water : blockID::lava;
		if ((currentGameMode != gameModeID::creative) && ((bucketData *)stackIn.data)->fillLevel != maxFluidLevel &&
			selectedBlockContainer->getBlockID(selectedBlockPosition) == fluidID)
		{
			// scoop up more
			fluidData *selectedFluidData = (fluidData *)selectedBlockContainer->getBlockData(selectedBlockPosition);
			fluidLevel transferAmount = selectedFluidData->currentFluidLevel;

			cint totalAmount = selectedFluidData->currentFluidLevel + ((bucketData *)stackIn.data)->fillLevel;
			if (totalAmount > maxFluidLevel)
			{
				transferAmount = maxFluidLevel - ((bucketData *)stackIn.data)->fillLevel;
			}

			selectedFluidData->currentFluidLevel -= transferAmount;
			((bucketData *)stackIn.data)->fillLevel += transferAmount;
			if (selectedFluidData->currentFluidLevel == 0)
			{
				selectedBlockContainer->setBlockID(selectedBlockPosition, blockID::air, chunkLoadLevel::updateLoaded);
			}
			else
			{
				selectedBlockContainer->addBlockUpdatePositionsAround(selectedBlockPosition);
			}
			return;
		}
		else if (wantsToStartUsing)
		{
			// place fluid
			if (selectedBlockContainer->getBlock(selectedBlockPosition)->canReplaceBlock)
			{
				selectedBlockContainer->setBlockID(selectedBlockPosition, fluidID, chunkLoadLevel::updateLoaded);
				fluidData *selectedFluidData = (fluidData *)selectedBlockContainer->getBlockData(selectedBlockPosition);
				selectedFluidData->currentFluidLevel = ((bucketData *)stackIn.data)->fillLevel;

				cvec2 soundPosition = selectedBlockPosition + cvec2(0.5);
				if (fluidID == blockID::water)
				{
					waterBucketEmptySound->playRandomSound(selectedBlockContainer, soundPosition);
				}
				else
				{
					lavaBucketEmptySound->playRandomSound(selectedBlockContainer, soundPosition);
				}
				if (currentGameMode != gameModeID::creative)
				{
					stackIn.add(-1);
					itemStack emptyBucketStack = itemStack(itemID::bucket, 1);

					addStackOrDrop(emptyBucketStack);
				}
				return;
			}
		}
	}
	else if (stackIn.stackItemID == itemID::glass_bottle && wantsToStartUsing)
	{
		if (selectedBlockContainer->getBlockID(selectedBlockPosition) == blockID::water)
		{
			fluidData *data = dynamic_cast<fluidData *>(selectedBlockContainer->getBlockData(selectedBlockPosition));
			if (data->currentFluidLevel >= bottleFluidLevel)
			{
				if (currentGameMode != gameModeID::creative)
				{
					data->currentFluidLevel -= bottleFluidLevel;
					if (data->currentFluidLevel == 0)
					{
						selectedBlockContainer->setBlockID(selectedBlockPosition, blockID::air, chunkLoadLevel::updateLoaded);
					}
					else
					{
						selectedBlockContainer->addBlockUpdatePositionsAround(selectedBlockPosition);
					}
				}
				// scoop up
				bottleFillSound->playRandomSound(selectedBlockContainer, exactBlockIntersection);
				stackIn.add(-1);

				itemStack fullBottleStack = itemStack(itemID::potion, 1, new potionData());
				addStackOrDrop(fullBottleStack);

				return;
			}
		}
	}
	else if (stackIn.stackItemID == itemID::potion && wantsToStartUsing && (((potionData *)stackIn.data)->effectsToAdd.size() == 0))
	{
		if (selectedBlock == blockID::water || selectedBlock == blockID::air)
		{
			if (selectedBlock == blockID::air)
			{
				selectedBlockContainer->setBlockWithData(selectedBlockPosition, blockID::water, new fluidData(bottleFluidLevel), chunkLoadLevel::updateLoaded);
			}
			else if (selectedBlock == blockID::water)
			{
				fluidData *data = dynamic_cast<fluidData *>(selectedBlockContainer->getBlockData(selectedBlockPosition));
				data->currentFluidLevel = math::minimum(data->currentFluidLevel, maxFluidLevel);
			}
			if (currentGameMode != gameModeID::creative)
			{
				stackIn.add(-1);
			}
			return;
		}
	}
	else if (itemList[stackIn.stackItemID]->canEat)
	{
		if ((foodlevel < maxhumanfoodlevel) || canEatIfFull(stackIn.stackItemID))
		{
			// do food animation
			foodAnimationTime++;

			if ((foodAnimationTime % biteInterval) == 0)
			{
				// bite
				// add particle
				summonParticle(dimensionIn, getHeadPosition(), new itemParticleBrush(stackIn.stackItemID), maxEatingParticleSpeed);
				eatSound->playRandomSound(dimensionIn, getHeadPosition());
			}

			cint requiredFoodAnimationTicks = (stackIn.stackItemID == itemID::dried_kelp) ? driedKelpAnimationTicks : foodAnimationTicks;
			if (foodAnimationTime >= requiredFoodAnimationTicks)
			{
				foodAnimationTime = 0;
				// add hunger and saturation
				foodlevel = math::minimum(maxhumanfoodlevel, foodlevel + itemList[stackIn.stackItemID]->foodPoints);
				foodsaturationlevel += itemList[stackIn.stackItemID]->extraSaturation;
				if (stackIn.stackItemID == itemID::chorus_fruit)
				{
					teleportRandomly();
				}
				else if (stackIn.stackItemID == itemID::golden_apple)
				{
					addStatusEffects(std::vector<statusEffect>({statusEffect(statusEffectID::absorption, 2 * ticksPerRealLifeMinute),
																statusEffect(statusEffectID::regeneration, 5 * ticksPerRealLifeSecond, 2)}));
				}
				else if (stackIn.stackItemID == itemID::enchanted_golden_apple)
				{
					addStatusEffects(std::vector<statusEffect>({statusEffect(statusEffectID::absorption, 2 * ticksPerRealLifeMinute, 4),
																statusEffect(statusEffectID::regeneration, 20 * ticksPerRealLifeSecond, 2),
																statusEffect(statusEffectID::fireResistance, 5 * ticksPerRealLifeMinute),
																statusEffect(statusEffectID::resistance, 5 * ticksPerRealLifeMinute)}));
				}
				else if (stackIn.stackItemID == itemID::rotten_flesh)
				{
					if (!randChance(currentRandom, 5))
					{
						addStatusEffects({statusEffect(statusEffectID::hunger, 600, 1)});
					}
				}
				else if (stackIn.stackItemID == itemID::chicken)
				{
					if (randIndex(currentRandom, 10) < 3) // 30 % chance
					{
						addStatusEffects({statusEffect(statusEffectID::hunger, 600, 1)});
					}
				}
				else if (stackIn.stackItemID == itemID::pufferfish)
				{
					addStatusEffects({statusEffect(statusEffectID::hunger, 300, 3),
									  statusEffect(statusEffectID::poison, ticksPerRealLifeMinute, 2),
									  statusEffect(statusEffectID::nausea, 15 * ticksPerRealLifeSecond, 1)});
				}
				else if (stackIn.stackItemID == itemID::spider_eye)
				{
					addStatusEffects({statusEffect(statusEffectID::poison, 4 * ticksPerRealLifeSecond, 1)});
				}
				else if ((stackIn.stackItemID == itemID::poisonous_potato) && (randIndex(currentRandom, 5) < 3))
				{
					addStatusEffects({statusEffect(statusEffectID::poison, 5 * ticksPerRealLifeSecond, 1)});
				}
				if (currentGameMode != gameModeID::creative)
				{
					if (hasBowl(stackIn.stackItemID))
					{
						stackIn.add(-1); // first remove the full bowl, then add the empty bowl
						itemStack bowlStack = itemStack(itemID::bowl, 1);
						addStack(bowlStack);
					}
					else
					{
						stackIn.add(-1);
					}
				}
				burpSound->playRandomSound(dimensionIn, getHeadPosition());
			}
			return;
		}
	}
	else if (canDrink(stackIn.stackItemID))
	{
		foodAnimationTime++;
		if (((foodAnimationTime % slurpInterval) == 0) || (foodAnimationTime >= drinkAnimationTicks))
		{
			// slurp
			std::shared_ptr<soundCollection> soundToPlay = ((stackIn.stackItemID == itemID::honey_bottle) || (stackIn.stackItemID == itemID::thick_potion)) ? honeyDrinkingSound : drinkingSound;
			soundToPlay->playRandomSound(dimensionIn, getHeadPosition());
		}
		if (foodAnimationTime >= drinkAnimationTicks)
		{
			foodAnimationTime = 0;
			// apply effects
			if (stackIn.stackItemID == itemID::potion)
			{
				potionData *data = (potionData *)stackIn.data;
				addStatusEffects(data->effectsToAdd);
			}
			else if (stackIn.stackItemID == itemID::milk_bucket)
			{
				activeEffects.clear();
			}
			if (currentGameMode != gameModeID::creative)
			{
				if (stackIn.stackItemID == itemID::milk_bucket)
				{
					stackIn.add(-1);
					itemStack emptyBucketstack = itemStack(itemID::bucket, 1);
					addStackOrDrop(emptyBucketstack);
				}
				else
				{
					stackIn.add(-1);
				}
			}
		}
	}
}

void human::onItemRightClickReleased(itemStack &stackIn)
{
	if (stackIn.stackItemID == itemID::bow)
	{
		// shoot arrow
		// check for arrows in inventory
		itemStack arrowStack = itemStack(itemID::arrow, 1);
		if (currentGameMode == gameModeID::creative || stackIn.getEnchantmentLevel(enchantmentID::infinity) || substractStack(arrowStack))
		{
			launchItem(itemID::arrow);
			decreaseDurability(stackIn, bowAnimationTime * secondsPerTick);
			bowAnimationTime = 0;
		}
	}
}

void human::linkUpInventories() const
{
	// link all containers up
	const std::vector<inventory *> inventories =
		{humanSlots, craftingTableSlots, smithingTableSlots, anvilSlots, furnaceSlots, chestSlots, enchantmentSlots, brewingStandSlots, dispenserSlots};

	for (inventory *const &inventoryToLinkUp : inventories)
	{
		inventoryToLinkUp->hotbarSlots->linkedContainer = hotbarSlots;
		inventoryToLinkUp->inventorySlots->linkedContainer = inventorySlots;
	}

	humanSlots->armorSlots->linkedContainer = armorSlots;
	humanSlots->leftHandSlot->linkedContainer = leftHandSlot;
}

void human::respawn()
{
	foodlevel = maxhumanfoodlevel;
	humanoid::respawn();

	// if (!currentWorld->keepInventoryOnDeath)
	//{
	//	inventorySlots->clone(*respawnedHuman->inventorySlots);
	//	hotbarSlots->clone(*respawnedHuman->hotbarSlots);
	//	leftHandSlot->clone(*respawnedHuman->leftHandSlot);
	// }

	// respawnedHuman->seenCredits = seenCredits;
}
void human::addExperience(cint &amount)
{
	if (amount)
	{
		cint oldLevel = (int)getExperienceLevel(experience);
		cint newExperience = experience + amount;
		cint newLevel = (int)getExperienceLevel(newExperience);
		cvec2 soundPosition = getHeadPosition();
		if ((newLevel / 5) > (oldLevel / 5))
		{
			levelUpSound->playRandomSound(dimensionIn, soundPosition);
		}
		else
		{
			cint note = getExperienceTableIndex(amount) + rand(currentRandom, 0, 2);
			if (note != -1)
			{
				cfp pitch = getNotePitch(note);
				experienceSound->playRandomSound(dimensionIn, soundPosition, 1, pitch);
			}
		}
		experience += amount;
		score += amount;
	}
}
// https://minecraft.gamepedia.com/Hunger#Mechanics
// https://www.programcreek.com/java-api-examples/?class=net.minecraft.entity.player.EntityPlayer&method=shouldHeal
void human::calculateFood()
{
	// this.prevFoodLevel = this.foodLevel;

	if (foodExhaustionlevel > 4.0F)
	{
		foodExhaustionlevel -= 4.0F;

		if (foodsaturationlevel > 0.0F)
		{
			foodsaturationlevel = math::maximum(foodsaturationlevel - (fp)1.0, (fp)0.0);
		}
		else
		{
			foodlevel = math::maximum(foodlevel - 1, 0);
		}
	}

	if (foodlevel >= 18 && health < maxhumanhealth)
	{
		foodticktimer++;

		if (foodticktimer >= 80)
		{
			heal(1);
			addExhaustion(3);
			foodticktimer = 0;
		}
	}
	else if (foodlevel <= 0)
	{
		foodticktimer++;
		if (foodticktimer >= 80)
		{
			if (health > 1)
			{
				addDamageSource(1.0F, std::make_shared<damageSource>(hungerDamage));
			}
			foodticktimer = 0;
		}
	}
	else
	{
		foodticktimer = 0;
	}
}
void human::addExhaustion(cfp &exhaustion)
{
	foodExhaustionlevel += exhaustion;
}

rectangle2 human::calculateHitBox(cvec2 &position) const
{
	return sleeping ? crectangle2(position.x - humanHitboxSize.y / 2, position.y, humanHitboxSize.y, humanHitboxSize.x) : humanoid::calculateHitBox(position);
}
