#include "block.h"
#include "math/graphics/brush/brushes.h"
#include "itemData.h"
#include "fluidData.h"
#include "snowLayerData.h"
#include "attachmentDirectionData.h"
#include "redstoneTorchData.h"
#include "trapDoorData.h"
#include "renderBrush.h"
#include "brewingStandData.h"
#include "repeaterData.h"
#include "comparatorData.h"
#include "endPortalFrameData.h"
#include "pistonData.h"
#include "spawnerData.h"
#include "mobList.h"
#include "stairsData.h"
#include "fenceGateData.h"
#include "buttonData.h"
#include "slabData.h"
#include "textureList.h"
#include "tickableBlockContainer.h"
#include "renderBrewingStand.h"
#include <algorithm>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include "biomeData.h"
#include "blockContainer.h"
#include "blockData.h"
#include "itemID.h"
#include "collisionData.h"
#include "collisionDataCollection.h"
#include "collisionTypeID.h"
#include "constants.h"
#include "entityID.h"
#include "experienceDrop.h"
#include "facingData.h"
#include "harvestTier.h"
#include "harvestType.h"
#include "idAnalysis.h"
#include "array/arraynd/arraynd.h"
#include "array/fastarray.h"
#include "globalFunctions.h"
#include "math/axis.h"
#include "math/direction.h"
#include "math/graphics/color/color.h"
#include "math/graphics/resolutiontexture.h"
#include "math/graphics/texture.h"
#include "math/mathFunctions.h"
#include "math/mattnxn.h"
#include "math/rectangle/rectangletn.h"
#include "math/vector/vectn.h"
#include "itemID.h"
#include "levelID.h"
#include "lightLevelID.h"
#include "openData.h"
#include "gameRenderData.h"
#include "settings.h"
#include "slabType.h"
#include "soundCollection.h"
#include "rectangularSlotContainer.h"
#include "dimension.h"
#include "math/graphics/brush/brushes/repeatingBrush.h"
#include "include/math/graphics/brush/brushes/colorMultiplier.h"
#include "gameColors.h"

block::block(blockID identifier, fp hardness, fp blastResistance, cfp &weightPerCubicMeter, resolutionTexture *tex, std::wstring name, std::shared_ptr<soundCollection> fallSound, std::shared_ptr<soundCollection> stepSound, std::shared_ptr<soundCollection> hitSound, std::shared_ptr<soundCollection> breakSound, std::shared_ptr<soundCollection> placeSound, cint (&filterStrength)[(size_t)levelID::count], harvestTypeID bestTool, harvestTierID itemTier, collisionTypeID collisiontype, cint &fireEncouragement, cint &flammability, cbool &canCatchFireFromLava, cbool &canReplaceBlock, cint (&emittanceLevel)[(size_t)levelID::count], const experienceDrop &experienceWhenBroken, std::shared_ptr<soundCollection> ambientSound)
	: INamable(name),
	  identifier(identifier),
	  itemTier(itemTier),
	  bestTool(bestTool),
	  hardness(hardness),
	  blastResistance(blastResistance),
	  fireEncouragement(fireEncouragement),
	  flammability(flammability),
	  canCatchFireFromLava(canCatchFireFromLava),
	  blockCollisionType(collisiontype),
	  weightPerCubicMeter(weightPerCubicMeter),
	  tex(tex),
	  fallSound(fallSound),
	  stepSound(stepSound),
	  hitSound(hitSound),
	  breakSound(breakSound),
	  placeSound(placeSound),
	  ambientSound(ambientSound),
	  canReplaceBlock(canReplaceBlock),
	  experienceWhenBroken(experienceWhenBroken)
{
	// we check for block light filter strength, because there are solid blocks which don't allow block light, but do allow sunlight
	if (filterStrength[(int)levelID::light + (int)lightLevelID::blockLight] != maxLightLevel &&
		!(isTreeType(identifier) && is_in(getTreeItemType(identifier), treeItemTypeID::log, treeItemTypeID::strippedLog)))
	{
		willFillSquare = false;
	}

	std::copy(filterStrength, filterStrength + (size_t)levelID::count, this->filterStrength);
	std::copy(emittanceLevel, emittanceLevel + (size_t)levelID::count, this->emittanceLevel);
}
bool block::correctTool(const itemID &tool) const
{
	return bestTool == withHand || ((int)tool && bestTool == itemList[(int)tool]->harvestType);
}

bool block::canHarvest(const itemID &tool)
{
	return (int)tool ? itemTier <= itemList[(int)tool]->harvestTier && (itemTier == noHarvestTier || correctTool(tool)) : itemTier == noHarvestTier;
}
bool block::canPlace(tickableBlockContainer *containerIn, cveci2 position)
{
	if (identifier == blockID::structure_void)
	{
		return true;
	}
	// find solid block adjacent
	cint checkPositionCount = 0x4;
	cveci2 relativeCheckpositions[checkPositionCount]{
		cveci2(-1, 0),
		cveci2(1, 0),
		cveci2(0, -1),
		cveci2(0, 1)};
	if (identifier == blockID::cactus)
	{
		const blockID below = containerIn->getBlockID(position + cveci2(0, -1));
		return below == blockID::sand || below == blockID::cactus;
	}
	else if (identifier == blockID::sugar_cane)
	{
		const blockID below = containerIn->getBlockID(position + cveci2(0, -1));
		return below == blockID::sand || below == blockID::sugar_cane;
	}
	else if (identifier == blockID::kelp)
	{
		blockID blockIn = containerIn->getBlockID(position);
		if (blockIn != blockID::water || fluidData::getFluidLevel(containerIn, position, blockID::water) < maxFluidLevel)
		{
			return false;
		}
		blockID blockBelow = containerIn->getBlockID(position + cveci2(0, -1));
		return blockBelow == blockID::sand || blockBelow == blockID::gravel || blockBelow == blockID::kelp;
	}
	for (int i = 0; i < checkPositionCount; i++)
	{
		const blockID &adjacentBlock = containerIn->getBlockID(position + relativeCheckpositions[i]);
		if ((!blockList[adjacentBlock]->canReplaceBlock) || (adjacentBlock == blockID::structure_void))
		{
			return true;
		}
	}
	return false;
}

void block::use()
{
}

void block::render(const gameRenderData &targetData, blockData *const data, blockContainer *containerIn, cveci2 &blockPosition) const
{
	if (identifier == blockID::air)
	{
		// do nothing
	}
	else if (isFluid(identifier))
	{
		fluidData *currentData = ((fluidData *)data);
		const fluidLevel valueLeft = fluidData::getFluidLevel(containerIn, blockPosition + cveci2(-1, 0), identifier);
		const fluidLevel valueRight = fluidData::getFluidLevel(containerIn, blockPosition + cveci2(1, 0), identifier);
		const fluidLevel valueTop = fluidData::getFluidLevel(containerIn, blockPosition + cveci2(0, 1), identifier);
		blockID blockBelow = containerIn->getBlockID(blockPosition + cveci2(0, -1));

		cfp drawHeightLeft = valueTop > 0 || blockBelow == blockID::air ? 1 : (math::maximum(currentData->currentFluidLevel, valueLeft)) / (fp)maxFluidLevel;
		cfp drawHeightRight = valueTop > 0 || blockBelow == blockID::air ? 1 : (math::maximum(currentData->currentFluidLevel, valueRight)) / (fp)maxFluidLevel;

		color c = identifier == blockID::water ? waterColor : lavaColor;

		cfp partFilled = (drawHeightLeft + drawHeightRight) * 0.5;
		cfp transparencyMultiplier = (currentData->currentFluidLevel / (fp)maxFluidLevel) / partFilled;

		const solidColorBrush b = solidColorBrush(color(c, (colorChannel)(c.a() * transparencyMultiplier)));
		const auto m = colorMixer<solidColorBrush, texture>(b, targetData.renderTarget);
		if (drawHeightLeft == drawHeightRight)
		{
			fillTransformedRectangle(targetData.renderTarget, crectangle2(cvec2(blockPosition), cvec2(1, drawHeightLeft)), targetData.worldToRenderTargetTransform, m);
		}
		else
		{
			fastArray<vec2> positions({cvec2(),
									   cvec2(0, drawHeightLeft),
									   cvec2(1, drawHeightRight),
									   cvec2(1, 0)});
			for (int i = 0; i < 4; i++)
			{
				positions[i] = targetData.worldToRenderTargetTransform.multPointMatrix(positions[i] + blockPosition);
			}
			fillPolygon(targetData.renderTarget, positions, m);
		}
		return;
	}
	else if (isTorch(identifier))
	{
		attachmentDirectionData *toAttachmentDirectionData = dynamic_cast<attachmentDirectionData *>(data);
		const resolutionTexture &textureToUse = ((identifier == blockID::redstone_torch) && (dynamic_cast<redstoneTorchData *>(data)->lastAttachedBlockPowerLevel > 0)) ? *unLitRedstoneTorchTexture : *tex;

		if (toAttachmentDirectionData->attachmentDirection == directionID::negativeY)
		{
			renderTorch(blockPosition, cvec2(0.5, 0), 0, textureToUse, targetData);
		}
		else
		{
			constexpr fp attachmentHeight = (1 - torchSize.getY()) / 2;
			if (toAttachmentDirectionData->attachmentDirection == directionID::negativeX)
			{
				renderTorch(blockPosition, cvec2(0, attachmentHeight), 30 * math::degreesToRadians, textureToUse, targetData);
			}
			else
			{
				renderTorch(blockPosition, cvec2(1, attachmentHeight), -30 * math::degreesToRadians, textureToUse, targetData);
			}
		}
	}
	else if (isDispenser(identifier))
	{
		const directionID &directionFacing = dynamic_cast<facingData *>(data)->directionFacing;
		if (getAxis(directionFacing) == axisID::x)
		{
			mat3x3 transform = block::getTextureToWorldTransform(tex->defaultSize, targetData.worldToRenderTargetTransform, blockPosition);
			if (directionFacing != standardSideFacingBlockDirection)
			{
				transform = mat3x3::cross(transform, mat3x3::mirror(axisID::x, blockTextureSize * 0.5));
			}
			fillTransformedTexture(transform, *tex, targetData.renderTarget);
		}
		else
		{
			render(*verticalDispenserTextures[(int)identifier - (int)blockID::dispenser], crectangle2(tex->getClientRect()), crectangle2(cvec2(blockPosition), cvec2(1)), targetData, data, containerIn);
			// renderTexture(drawRect, false, directionFacing, false, *verticalDispenserTextures[(int)identifier - (int)blockID::dispenser], targetData.renderTarget);
		}
	}
	else if (isRail(identifier))
	{
		cvecb2 topConnection = containerIn->railTopConnection(blockPosition);

		// stretch out and rotate the rails
		if (topConnection[0])
		{
			// from top x0 to bottom x1
			cmat3x3 transform = mat3x3::cross(getTextureToWorldTransform(tex->defaultSize, targetData.worldToRenderTargetTransform, blockPosition),
											  mat3x3::cross(mat3x3::rotate(cvec2(blockTextureSize * math::sqrt2, 0), math::PI2 / 8.0),
															mat3x3::scale(cvec2(math::sqrt2, 1))));

			fillTransparentRectangle(crectangle2(tex->getClientRect()), transform, *tex, targetData.renderTarget);
		}
		else if (topConnection[1])
		{
			// from bottom x0 to top x1
			// because of the stretching, the rails is now at another point
			cmat3x3 transform = mat3x3::cross(
				getTextureToWorldTransform(tex->defaultSize, targetData.worldToRenderTargetTransform, blockPosition),
				mat3x3::cross(mat3x3::rotate(cvec2(), math::PI2 / -8.0),
							  mat3x3::scale(cvec2(math::sqrt2, 1))));

			fillTransparentRectangle(crectangle2(tex->getClientRect()), transform, *tex, targetData.renderTarget);
		} // else: render normally
	}
	else if (identifier == blockID::brewing_stand)
	{
		bool hasBottle[brewingStandPotionCapacity]{};
		brewingstandData *toBrewingStandData = (brewingstandData *)data;
		for (int i = 0; i < brewingStandPotionCapacity; i++)
		{
			hasBottle[i] = toBrewingStandData->potionSlots[i]->slots[0].count;
		}

		renderBrewingStand(mat3x3::cross(targetData.worldToRenderTargetTransform, mat3x3::translate(cvec2(blockPosition))), hasBottle, targetData.renderTarget);
	}
	else if ((identifier == blockID::repeater) || (identifier == blockID::comparator))
	{
		constexpr fp relativeMiddle = 0.5;

		cint directionX = directionVectors2D[(int)dynamic_cast<facingData *>(data)->directionFacing].x;

		constexpr fp cutSlabPart = 1.0 / 8;
		constexpr rectangle2 cutSlabRelativeRect = crectangle2(0, 0, 1, cutSlabPart);

		cfp normalTorchDepth = cutSlabPart + torchSize.x * 2 - torchSize.y;
		cfp deepTorchDepth = cutSlabPart + torchSize.x - torchSize.y;

		const powerLevel outputLevel = containerIn->getEmittanceLevel(blockPosition, levelID::powerLevel);

		if (identifier == blockID::repeater)
		{
			repeaterData *toRepeaterData = dynamic_cast<repeaterData *>(data);

			// todo: check if it had to be emittanceLevel[2]
			renderTorch(blockPosition, cvec2(relativeMiddle + (directionX * 0.25), normalTorchDepth), 0, outputLevel ? *blockList[blockID::redstone_torch]->tex : *unLitRedstoneTorchTexture, targetData);

			cfp torchOffset = relativeMiddle + (directionX * (0.25 - torchSize.x - (((toRepeaterData->delayArrayIndex) / (fp)delayArraySize) * 0.5)));

			if (toRepeaterData->lastPowerLevelBelow == 0)
			{
				renderTorch(blockPosition, cvec2(torchOffset, normalTorchDepth), 0, outputLevel ? *blockList[blockID::redstone_torch]->tex : *unLitRedstoneTorchTexture, targetData);
			}
			else
			{
				// render a small blockage instead of the torch
				crectangle2 relativeRect = crectangle2(torchOffset - torchSize.x * 0.5, cutSlabPart, torchSize.x, torchSize.x);
				fillTransformedBrushRectangle(getAbsoluteRect(crectangle2(tex->getClientRect()), relativeRect), getTextureToWorldTransform(tex->defaultSize, targetData.renderTargetToWorldTransform, cvec2(blockPosition)), *blockList[blockID::bedrock]->tex, targetData.renderTarget);
			}
		}
		else
		{
			comparatorData *toComparatorData = dynamic_cast<comparatorData *>(data);

			renderTorch(blockPosition, cvec2(relativeMiddle + (directionX * 0.25), deepTorchDepth), 0, toComparatorData->substractionMode ? *blockList[blockID::redstone_torch]->tex : *unLitRedstoneTorchTexture, targetData);
			renderTorch(blockPosition, cvec2(relativeMiddle + (directionX * -0.25), normalTorchDepth), 0, outputLevel ? *blockList[blockID::redstone_torch]->tex : *unLitRedstoneTorchTexture, targetData);
		}
		fillTransformedBrushRectangle(getAbsoluteRect(crectangle2(tex->getClientRect()), cutSlabRelativeRect), getTextureToWorldTransform((veci2)blockList[blockID::stone_slab]->tex->defaultSize, targetData.worldToRenderTargetTransform, blockPosition), *blockList[blockID::stone_slab]->tex, targetData.renderTarget);
	}
	else if (identifier == blockID::end_portal)
	{
		// render a static texture
		fillUnTransformedRepeatingTexture(crectangle2(cvec2(blockPosition), cvec2(1)), targetData.worldToRenderTargetTransform, *tex, cvec2(tex->defaultSize), targetData.renderTarget);
	}
	else if (identifier == blockID::end_portal_frame)
	{
		endPortalFrameData *toEndPortalFrameData = dynamic_cast<endPortalFrameData *>(data);
		// direction does not matter
		fillTransformedBrushRectangle(getAbsoluteRect(tex->getClientRect(), endPortalBlockRect), getTextureToWorldTransform(tex->defaultSize, targetData.worldToRenderTargetTransform, blockPosition), *tex, targetData.renderTarget);

		if (toEndPortalFrameData->hasEye)
		{
			// render eye
			fillTransformedBrushRectangle(getAbsoluteRect(tex->getClientRect(), endPortalFrameEyeBlockRect), getTextureToWorldTransform(tex->defaultSize, targetData.worldToRenderTargetTransform, blockPosition), *endPortalFrameEyeTexture, targetData.renderTarget);
		}
	}
	else if (identifier == blockID::end_rod)
	{
		// TODO: pre-render end rod texture correctly, so this complex transformation doensn't have to be done every time
		const directionID &attachmentDirection = dynamic_cast<attachmentDirectionData *>(data)->attachmentDirection;
		cmat3x3 &rotationTransform =
			mat3x3::rotateDegrees(cvec2(blockPosition) + cvec2(0.5), getAngle2DDegrees(flipDirection(attachmentDirection)));
		crectangle2 &endRodBaseTextureRect = crectangle2(endRodBaseTextureRelativeRect.pos0 * (fp)tex->defaultSize.x, endRodBaseTextureRelativeRect.size * (fp)tex->defaultSize.x);
		crectangle2 &endRodPoleTextureRect = crectangle2(endRodPoleTextureRelativeRect.pos0 * (fp)tex->defaultSize.x, endRodPoleTextureRelativeRect.size * (fp)tex->defaultSize.x);
		fillTransformedBrushRectangle(crectangle2(endRodBaseTextureRect),
									  mat3x3::cross(targetData.worldToRenderTargetTransform,
													mat3x3::cross(rotationTransform,
																  mat3x3::fromRectToRect(crectangle2(endRodBaseTextureRect), getAbsoluteRect(crectangle2(cvec2(blockPosition), cvec2(1)),
																																			 endRodBaseRelativeRect)))),
									  *tex, targetData.renderTarget);
		fillTransformedBrushRectangle(crectangle2(endRodPoleTextureRect),
									  mat3x3::cross(targetData.worldToRenderTargetTransform,
													mat3x3::cross(rotationTransform,
																  mat3x3::fromRectToRect(crectangle2(endRodPoleTextureRect), getAbsoluteRect(crectangle2(cvec2(blockPosition), cvec2(1)),
																																			 endRodPoleRelativeRect)))),
									  *tex, targetData.renderTarget);
	}
	else if ((identifier == blockID::redstone_lamp) && containerIn->getEmittanceLevel(blockPosition, (levelID)((int)levelID::light + (int)lightLevelID::blockLight)))
	{
		render(*redstoneLampOnTexture, crectangle2(redstoneLampOnTexture->getClientRect()), crectangle2(cvec2(blockPosition), cvec2(1)), targetData, data, containerIn);
	}
	else if (isFurnace(identifier) && containerIn->getEmittanceLevel(blockPosition, (levelID)((int)levelID::light + (int)lightLevelID::blockLight)))
	{
		resolutionTexture *textureToUse = furnaceOnTextures[(size_t)identifier - (size_t)blockID::furnace];
		render(*textureToUse, crectangle2(textureToUse->getClientRect()), crectangle2(cvec2(blockPosition), cvec2(1)), targetData, data, containerIn, true);
	}
	else if (isSlab(identifier) || isButton(identifier) || isPressurePlate(identifier) || isFence(identifier) || isFenceGate(identifier) || isWall(identifier) || isStairs(identifier) || isTrapDoor(identifier) || isChest(identifier) ||
			 is_in(identifier, blockID::chorus_plant, blockID::chorus_flower, blockID::iron_bars, blockID::snow, blockID::enchanting_table))
	{
		renderCollisionData(containerIn, blockPosition, targetData);
	}
	else if (isPiston(identifier))
	{
		pistonData *toPistonData = dynamic_cast<pistonData *>(data);

		if (toPistonData->isPart0)
		{
			cfp pushProgress0To1 = std::ranges::clamp(toPistonData->pushProgress + toPistonData->getPushDistancePerTick() * targetData.tickPartOffset, (fp)0, (fp)1);

			crectangle2 &relativeRect = crectangle2(0, 0, 1, 1 - pistonTopSize);
			// bottom
			render(*tex, getAbsoluteRect(blockTextureRect, relativeRect), crectangle2(cvec2(blockPosition), relativeRect.size), targetData, data, containerIn, false);

			if (pushProgress0To1 > 0)
			{
				// arm
				crectangle2 &relativePushRect = crectangle2((1 - pistonTopSize) * 0.5, (2 - pistonTopSize) - pushProgress0To1, pistonTopSize, pushProgress0To1);

				render(*tex, getAbsoluteRect(blockTextureRect, relativePushRect), crectangle2((fp)blockPosition.x + relativePushRect.x, (fp)blockPosition.y + (1 - pistonTopSize), relativePushRect.w, relativePushRect.h), targetData, data, containerIn, false);
			}
			// top
			render(*tex, crectangle2(0, blockTextureSize * 2 - pistonTopPixelSize, blockTextureSize, pistonTopPixelSize), crectangle2((fp)blockPosition.x + 0, (fp)blockPosition.y + (1 - pistonTopSize) + pushProgress0To1, 1, pistonTopSize), targetData, data, containerIn, false, cvec2(blockPosition) + vec2(0.5));
		}
	}
	else if (identifier == blockID::redstone_wire)
	{
		const solidColorBrush &wireColor = solidColorBrush(color(colorf(0, 0, math::lerp((fp)0.5, (fp)1, containerIn->getPowerLevel(blockPosition) / (fp)maxPowerLevel))));
		const auto &mult = colorMultiplier<resolutionTexture, solidColorBrush>(*tex, wireColor);
		render(mult, crectangle2(tex->getClientRect()), crectangle2((vec2)blockPosition, cvec2(1)), targetData, data, containerIn, false);
	}
	else if (identifier == blockID::grass_block && isSnow(containerIn->getBlockID(veci2(blockPosition.x, blockPosition.y + 1))))
	{
		fillTransformedBrushRectangle(blockTextureRect, getTextureToWorldTransform(veci2(blockTextureRect.size), targetData.worldToRenderTargetTransform, blockPosition), *snowyGrassBlockTexture, targetData.renderTarget);
	}
	else
	{
		// if (multiplyByBiomeColor(identifier))
		//{
		//	if (tickableBlockContainer* c = dynamic_cast<tickableBlockContainer*>(containerIn)) {
		//
		//		cvec2& rootPosition = c->containerToRootTransform.multPointMatrix(cvec2(blockPosition) + 0.5);
		//
		//		//multiply by the biome color
		//		const solidColorBrush biomeColorBrush = solidColorBrush(biomeDataList[c->rootDimension->getBiome(rootPosition.x)]->grassColor);
		//		if (identifier == blockID::grass_block)
		//		{
		//			//fill back texture
		//			render(*tex, crectangle2(tex->getClientRect()), crectangle2((vec2)blockPosition, cvec2(1)), targetData, data, containerIn, true);
		//
		//			//transparent overlay
		//			render(colorMultiplier<resolutionTexture, solidColorBrush>(*grassOverlay, biomeColorBrush), crectangle2(tex->getClientRect()), crectangle2((vec2)blockPosition, cvec2(1)), targetData, data, containerIn, true);
		//		}
		//		else
		//		{
		//			render(colorMultiplier<resolutionTexture, solidColorBrush>(*tex, biomeColorBrush), crectangle2(tex->getClientRect()), crectangle2((vec2)blockPosition, cvec2(1)), targetData, data, containerIn, true);
		//			return;
		//		}
		//	}
		//
		// }
		render(*tex, crectangle2(tex->getClientRect()), crectangle2((vec2)blockPosition, cvec2(1)), targetData, data, containerIn, true);

		// render afterwards
		if (identifier == blockID::spawner)
		{
			const entityID &entityIDToRender = dynamic_cast<spawnerData *>(containerIn->getBlockData(blockPosition))->entityToSpawn;
			const auto it = std::find(mobList, mobList + mobTypeCount, entityIDToRender);
			if (it != mobList + mobTypeCount)
			{
				const auto &spawnEggTex = *itemList[(int)itemID::spawn_egg + (int)(it - mobList)]->tex;
				// render spawn egg
				crectangle2 &relativeRect = crectangle2(vec2(0.5), vec2()).expanded(0.4);
				crectangle2 &clientRect = getAbsoluteRect(crectangle2(spawnEggTex.getClientRect()), relativeRect);
				crectangle2 &blockRect = crectangle2(cvec2(blockPosition) + relativeRect.pos0, relativeRect.size);
				render(spawnEggTex, clientRect, blockRect, targetData, data, containerIn);
			}
		}
	}
}

mat3x3 block::getTextureToWorldTransform(cvec2 &textureSize, cmat3x3 &blockToRenderTargetTransform, cvec2 &blockPosition)
{
	return getBrushRectToWorldTransform(crectangle2(textureSize), blockToRenderTargetTransform, blockPosition);
}

mat3x3 block::getBrushRectToWorldTransform(crectangle2 &brushRect, cmat3x3 &blockToRenderTargetTransform, cvec2 &blockPosition)
{
	return mat3x3::cross(blockToRenderTargetTransform, mat3x3::fromRectToRect(brushRect, crectangle2(blockPosition, cvec2(1))));
}

collisionDataCollection block::getCollisionData(blockContainer *containerIn, cveci2 &position) const
{
	collisionDataCollection collisionCollection = collisionDataCollection();

	if (this->blockCollisionType == collisionTypeID::willNotCollide)
	{
		return collisionCollection;
	}
	else if (isRail(identifier))
	{
		cvecb2 topConnection = containerIn->railTopConnection(position);
		cfp from = topConnection.x ? position.y + 1 : position.y;
		cfp to = topConnection.y ? position.y + 1 : position.y;
		constexpr int sleeperCount = 4;
		constexpr int sleeperTextureSize = 4;
		constexpr fp sleeperHitboxSize = sleeperTextureSize / (fp)blockTextureSize;
		for (int i = 0; i < sleeperCount; i++)
		{
			cfp lerpValue = (i + 0.5) / sleeperCount;
			collisionCollection.hitboxes.push_back(collisionData(crectangle2(cvec2(position.x + lerpValue - sleeperHitboxSize * 0.5, math::lerp(from, to, lerpValue)), cvec2(sleeperHitboxSize)), collisionTypeID::willCollideTop));
		}
		return collisionCollection;
	}
	else if (isStairs(identifier))
	{
		stairsData *data = dynamic_cast<stairsData *>(containerIn->getBlockData(position));
		collisionData broadCollision = collisionData(rectangle2(position.x, position.y, 1, 0.5), blockCollisionType);
		collisionData narrowCollision = collisionData(rectangle2(position.x, position.y, 0.5, 0.5), blockCollisionType);
		if (data->upsideDown)
		{
			broadCollision.hitboxCollidingWith.y += 0.5;
		}
		else
		{
			narrowCollision.hitboxCollidingWith.y += 0.5;
		}
		if (data->directionFacing == directionID::positiveX)
		{
			narrowCollision.hitboxCollidingWith.x += 0.5;
		}
		collisionCollection.hitboxes.push_back(narrowCollision);
		collisionCollection.hitboxes.push_back(broadCollision);
		return collisionCollection;
	}
	else if (isFence(identifier) || isFenceGate(identifier) || isWall(identifier) || identifier == blockID::iron_bars)
	{
		const blockData *const data = containerIn->getBlockData(position);
		bool connect[fenceConnectionPossibilityCount]{};
		for (size_t connectionIndex = 0; connectionIndex < fenceConnectionPossibilityCount; connectionIndex++)
		{
			connect[connectionIndex] = containerIn->canConnect(position, fenceConnectionCheckPositions[connectionIndex]);
		}

		if (isFence(identifier) || isFenceGate(identifier))
		{
			collisionTypeID poleCollisionType = collisionTypeID::willCollideTop;
			if (isFenceGate(identifier))
			{
				const fenceGateData *toFenceGateData = dynamic_cast<const fenceGateData *>(data);

				if (toFenceGateData->isOpen)
				{
					connect[(toFenceGateData->directionFacing == directionID::negativeX) ? 0 : 1] = true;

					rectangle2 relativeHandleRect = crectangle2(0, fenceConnectionPoleY[0] + fenceConnectionPoleWidth, fenceConnectionPoleWidth, (fenceConnectionPoleY[1] - fenceConnectionPoleY[0]) - fenceConnectionPoleWidth);

					if (toFenceGateData->directionFacing == directionID::positiveX)
					{
						relativeHandleRect.x = 1 - fenceConnectionPoleWidth;
					}

					collisionCollection.hitboxes.push_back(collisionData(crectangle2(relativeHandleRect.pos0 + cvec2(position), relativeHandleRect.size), collisionTypeID::willCollideTop));
				}
				else
				{
					poleCollisionType = collisionTypeID::willCollide;
				}
			}

			collisionCollection.hitboxes.push_back(collisionData(crectangle2(position.x + fencePoleX, position.y, fencePoleWidth, fenceGateCollisionHeight), poleCollisionType));
			for (size_t connectionIndex = 0; connectionIndex < fenceConnectionPossibilityCount; connectionIndex++)
			{
				if (connect[connectionIndex])
				{
					for (int i = 0; i < fenceConnectionPoleCount; i++)
					{
						rectangle2 connectionPoleBlockRect = crectangle2(connectionIndex == 0 ? 0 : 1 - fencePoleX, fenceConnectionPoleY[i], fencePoleX, fenceConnectionPoleWidth);

						collisionCollection.hitboxes.push_back(collisionData(crectangle2(connectionPoleBlockRect.pos0 + cvec2(position), connectionPoleBlockRect.size), collisionTypeID::willCollideTop));
					}
				}
			}
		}
		else if (isWall(identifier))
		{
			bool wallConnectionAbove[fenceConnectionPossibilityCount] = {};
			cveci2 topPosition = cveci2(0, 1);
			if (isWall(containerIn->getBlockID(position + topPosition)))
			{
				for (int connectionIndex = 0; connectionIndex < fenceConnectionPossibilityCount; connectionIndex++)
				{
					wallConnectionAbove[connectionIndex] = containerIn->canConnect(position, fenceConnectionCheckPositions[connectionIndex]);
				}
			}

			// render pole
			crectangle2 poleBlockRect = crectangle2(wallPoleX, 0, wallPoleWidth, 1);

			collisionCollection.hitboxes.push_back(collisionData(crectangle2(poleBlockRect.pos0 + cvec2(position), poleBlockRect.size), collisionTypeID::willCollideTop));

			for (int connectionIndex = 0; connectionIndex < fenceConnectionPossibilityCount; connectionIndex++)
			{
				if (connect[connectionIndex])
				{
					rectangle2 connectionPoleBlockRect = crectangle2(connectionIndex == 0 ? 0 : 1 - wallPoleX, 0, wallPoleX, wallConnectionAbove[connectionIndex] ? 1 : wallConnectionHeight);

					collisionCollection.hitboxes.push_back(collisionData(crectangle2(connectionPoleBlockRect.pos0 + cvec2(position), connectionPoleBlockRect.size), collisionTypeID::willCollideTop));
				}
			}
		}
		else if (identifier == blockID::iron_bars)
		{
			cfp minX = connect[0] ? 0 : ironBarsPoleX;
			cfp maxX = connect[1] ? 1 : 1 - ironBarsPoleX;
			crectangle2 ironBarsBlockRect = crectangle2(minX, 0, maxX - minX, 1);

			collisionCollection.hitboxes.push_back(collisionData(crectangle2(ironBarsBlockRect.pos0 + cvec2(position), ironBarsBlockRect.size), collisionTypeID::willCollideTop));
		}

		return collisionCollection;
	}
	else if (identifier == blockID::chorus_plant || identifier == blockID::chorus_flower)
	{
		// blob-like middle hitbox
		constexpr fp chorusPlantMiddleHitboxOffset = 3.0 / 0x10;
		constexpr fp chorusPlantMiddleHitboxSize = 1 - (chorusPlantMiddleHitboxOffset * 2);

		constexpr fp chorusFlowerMiddleHitboxOffset = 2.0 / 0x10;
		constexpr fp chorusFlowerMiddleHitboxSize = 1 - (chorusFlowerMiddleHitboxOffset * 2);

		constexpr fp extensionMinimalLength = 1.0 / 0x10;

		const fp currentMiddleHitboxSize = identifier == blockID::chorus_plant ? chorusPlantMiddleHitboxSize : chorusFlowerMiddleHitboxSize;
		const fp currentMiddleHitboxOffset = identifier == blockID::chorus_plant ? chorusPlantMiddleHitboxOffset : chorusFlowerMiddleHitboxOffset;

		collisionCollection.hitboxes.push_back(collisionData(crectangle2(cvec2(position) + currentMiddleHitboxOffset, cvec2(currentMiddleHitboxSize)), collisionTypeID::willCollideTop));

		// add 4 exstensions
		for (fsize_t i = 0; i < directionCount2D; i++)
		{
			blockID connectingBlock = containerIn->getBlockID(position + directionVectors2D[i]);
			cfp extensionLength = (connectingBlock == blockID::chorus_plant || connectingBlock == blockID::chorus_flower || connectingBlock == blockID::end_stone) ? currentMiddleHitboxOffset : extensionMinimalLength;
			//+y connection
			rectangle2 relativeHitbox = rectangle2(1 - currentMiddleHitboxOffset, currentMiddleHitboxOffset, extensionLength, currentMiddleHitboxSize);
			if (!isPositive((directionID)i))
			{
				relativeHitbox.x = currentMiddleHitboxOffset - extensionLength;
			}
			if (getAxis((directionID)i) == axisID::y)
			{
				relativeHitbox = relativeHitbox.switchAxes();
			}
			relativeHitbox.pos0 += cvec2(position);
			collisionCollection.hitboxes.push_back(collisionData(relativeHitbox, collisionTypeID::willCollideTop));
		}
		return collisionCollection;
	}
	else if (identifier == blockID::end_rod)
	{
		directionID attachmentDirection = dynamic_cast<attachmentDirectionData *>(containerIn->getBlockData(position))->attachmentDirection;
		directionID directionFacing = flipDirection(attachmentDirection);
		crectangle2 baseRelativeHitbox = endRodBaseRelativeRect.rotatedInRectangle(crectangle2(0, 0, 1, 1), directionFacing);
		crectangle2 poleRelativeHitbox = endRodPoleRelativeRect.rotatedInRectangle(crectangle2(0, 0, 1, 1), directionFacing);
		collisionCollection.hitboxes.push_back(collisionData(crectangle2(cvec2(position) + baseRelativeHitbox.pos0, baseRelativeHitbox.size), collisionTypeID::willCollideTop));
		collisionCollection.hitboxes.push_back(collisionData(crectangle2(cvec2(position) + poleRelativeHitbox.pos0, poleRelativeHitbox.size), collisionTypeID::willCollideTop));
		return collisionCollection;
	}
	collisionData collision = collisionData(rectangle2(position.x, position.y, 1, 1), blockCollisionType);

	if (isDoor(identifier))
	{
		const openData *toOpenData = dynamic_cast<openData *>(containerIn->getBlockData(position));
		if (toOpenData->isOpen)
		{
			collision.type = collisionTypeID::willCollideTop;
		}
		else
		{
			const facingData *toFacingData = dynamic_cast<facingData *>(containerIn->getBlockData(position));
			collision.hitboxCollidingWith = rectangle2(position.x, position.y, openDoorBlockSize, 1);
			if (toFacingData->directionFacing == directionID::positiveX)
			{
				collision.hitboxCollidingWith.pos0.x += 1 - openDoorBlockSize;
			}
			collision.type = collisionTypeID::willCollide;
		}
	}
	else if (isButton(identifier))
	{
		buttonData *data = dynamic_cast<buttonData *>(containerIn->getBlockData(position));
		cfp buttonHeight = data->ticksToPress > 0 ? pressedButtonHeight : unPressedButtonHeight;
		//+y connection
		rectangle2 relativeHitbox = rectangle2(0.5 - (buttonWidth * 0.5), 0, buttonWidth, buttonHeight);
		relativeHitbox = relativeHitbox.rotatedInRectangle(crectangle2(0, 0, 1, 1), flipDirection(data->attachmentDirection));
		relativeHitbox.pos0 += cvec2(position);
		collision.hitboxCollidingWith = relativeHitbox;
	}
	else if (isTrapDoor(identifier))
	{
		const trapDoorData *toTrapDoorData = dynamic_cast<trapDoorData *>(containerIn->getBlockData(position));
		if (toTrapDoorData->isOpen)
		{
			collision.hitboxCollidingWith.w = openDoorBlockSize;
			if (toTrapDoorData->directionFacing == directionID::positiveX)
			{
				collision.hitboxCollidingWith.x += 1 - openDoorBlockSize;
			}
		}
		else
		{
			collision.hitboxCollidingWith.h = openDoorBlockSize;
			if (!toTrapDoorData->upsideDown)
			{
				collision.hitboxCollidingWith.y += 1 - openDoorBlockSize;
			}
		}
	}
	else if (isSlab(identifier))
	{
		const slabData *data = dynamic_cast<slabData *>(containerIn->getBlockData(position));
		if (data->type == slabType::topSlab)
		{
			collision.hitboxCollidingWith = crectangle2(position.x, position.y + 0.5, 1, 0.5);
		}
		else if (data->type == slabType::bottomSlab)
		{
			collision.hitboxCollidingWith = crectangle2(position.x, position.y, 1, 0.5);
		}
	}
	else if (isPressurePlate(identifier))
	{
		collision.hitboxCollidingWith.h = containerIn->getEmittanceLevel(position, levelID::powerLevel) > 0 ? pressedPressurePlateHeight : unPressedPressurePlateHeight;
	}
	else if (isChest(identifier))
	{
		collision.hitboxCollidingWith = getAbsoluteRect(collision.hitboxCollidingWith, chestBlockRect);
	}
	else if (isBed(identifier))
	{
		collision.hitboxCollidingWith.h = bedHitboxHeight;
	}
	else if (identifier == blockID::snow)
	{
		collision.hitboxCollidingWith.h = dynamic_cast<snowLayerData *>(containerIn->getBlockData(position))->layerThickness;
	}
	else if (identifier == blockID::soul_sand)
	{
		collision.hitboxCollidingWith.h = soulSandHeight;
	}
	else if (identifier == blockID::grass_path)
	{
		collision.hitboxCollidingWith.h = grassPathHeight;
	}
	else if (identifier == blockID::enchanting_table)
	{
		collision.hitboxCollidingWith.h = enchantingTableHeight;
	}
	else if (identifier == blockID::end_portal_frame)
	{
		collision.hitboxCollidingWith.h = endPortalBlockRect.h;
	}
	collisionCollection.hitboxes.push_back(collision);
	return collisionCollection;
}

template <typename brush0Type>
void block::render(const brush0Type &currentBrush, rectangle2 brushRect, crectangle2 &blockRect, const gameRenderData &targetData, blockData *const data, blockContainer *containerIn, cbool &renderAnimation, const std::optional<vec2> &rotationCentre) const
{
	mat3x3 transform;
	if (renderAnimation)
	{
		transform = mat3x3::fromRectToRect(crectangle2(brushRect.x, brushRect.y, brushRect.w, brushRect.w), blockRect);
	}
	else
	{
		transform = mat3x3::fromRectToRect(brushRect, blockRect);
	}
	transform = mat3x3::cross(targetData.worldToRenderTargetTransform, transform);

	// rotation centre on screen because then all other transformations, like animation etc, are done already
	cvec2 &rotationCentreOnScreen = targetData.worldToRenderTargetTransform.multPointMatrix(rotationCentre ? rotationCentre.value() : floorVector<fp>(blockRect.pos0) + cvec2(0.5));

	if (settings::videoSettings::multiplyBiomeColors && multiplyByBiomeColor(identifier))
	{
		if (tickableBlockContainer *c = dynamic_cast<tickableBlockContainer *>(containerIn))
		{
			cvec2 &rootPosition = c->containerToRootTransform.multPointMatrix(blockRect.getCenter());
			// multiply by the biome color
			const solidColorBrush biomeColorBrush = solidColorBrush(biomeDataList[c->rootDimension->getBiome(rootPosition)]->grassColor);
			// const solidColorBrush biomeColorBrush = solidColorBrush(biomeDataList[c->getBiome(blockRect.getCenter().x)]->grassColor);

			constexpr bool mipmap = std::is_same<brush0Type, resolutionTexture>::value;
			using finalBrushType = std::conditional<mipmap, texture, brush0Type>::type;

			auto mipmapfunc = [&currentBrush, &transform, &brushRect]() -> const finalBrushType &
			{
				if constexpr (mipmap)
				{
					// modify transform and change brush
					// rectangle2 mipmappedRect = brushRect;
					return (const finalBrushType &)(((const resolutionTexture &)currentBrush).mipmap(transform, brushRect));
					// renderBrush(mipmappedRect, transform, rotationCentreOnScreen, true, facingDirection, tex, targetData.renderTarget, renderAnimation);
				}
				else
				{
					return (const finalBrushType &)currentBrush;
				}
			};

			// if constexpr (std::is_same<brush0Type, resolutionTexture>::value)
			//{
			//	//modify transform and change brush
			//	rectangle2 mipmappedRect = brushRect;
			//	const texture& tex = ((const resolutionTexture&)currentBrush).mipmap(transform, mipmappedRect);
			//	renderBrush(mipmappedRect, transform, rotationCentreOnScreen, true, facingDirection, tex, targetData.renderTarget, renderAnimation);
			// }
			// else {
			//	renderBrush(brushRect, transform, rotationCentreOnScreen, true, facingDirection, currentBrush, targetData.renderTarget, renderAnimation);
			// }

			if (identifier == blockID::grass_block)
			{

				mat3x3 overlayTransform = transform;
				rectangle2 overlayBrushRect = brushRect;

				const finalBrushType &finalBrush = mipmapfunc();
				// fill back texture
				fillTransformedBrushRectangle(brushRect, transform, finalBrush, targetData.renderTarget);
				// renderBrush(brushRect, transform, rotationCentreOnScreen, true, standardUpFacingBlockDirection, mult, targetData.renderTarget, true);
				const texture &overlayTex = grassOverlay->mipmap(overlayTransform, overlayBrushRect);
				const auto &mult = colorMultiplier<texture, solidColorBrush>(overlayTex, biomeColorBrush);

				// transparent overlay
				fillTransparentRectangle(overlayBrushRect, overlayTransform, mult, targetData.renderTarget);
				return;
			}
			else
			{
				const finalBrushType &finalBrush = mipmapfunc();
				const auto &mult = colorMultiplier<finalBrushType, solidColorBrush>(finalBrush, biomeColorBrush);
				renderBrush(brushRect, transform, rotationCentreOnScreen, true, standardUpFacingBlockDirection, mult, targetData.renderTarget, true);
				return;
			}
		}
	}

	directionID facingDirection = standardUpFacingBlockDirection;
	if (hasFacingData(identifier))
	{
		facingData *toFacingData = dynamic_cast<facingData *>(data);
		if (canFaceUp(identifier))
		{
			facingDirection = toFacingData->directionFacing;
		}
		else if (toFacingData->directionFacing != standardSideFacingBlockDirection)
		{
			transform = mat3x3::cross(mat3x3::mirror(axisID::x, rotationCentreOnScreen.x), transform);
		}
	}
	renderBrush(brushRect, transform, rotationCentreOnScreen, true, facingDirection, currentBrush, targetData.renderTarget, renderAnimation);
}

void renderTorch(cveci2 &blockPosition, cvec2 &relativeRotationCentre, cfp &angle, const resolutionTexture &tex, const gameRenderData &targetData)
{
	mat3x3 transform = mat3x3::fromRectToRect(crectangle2(torchTextureRect), crectangle2(cvec2(blockPosition) + relativeRotationCentre + cvec2(torchSize.x * -0.5, 0), torchSize));
	if (angle != 0)
	{
		transform = mat3x3::cross(transform, mat3x3::rotate(cvec2(torchTextureRect.pos0) + cvec2(torchTextureRect.size.x * 0.5, 0), angle));
	}
	fillTransparentRectangle(crectangle2(torchTextureRect), mat3x3::cross(targetData.worldToRenderTargetTransform, transform), tex, targetData.renderTarget);
}

void renderTexture(crectangle2 &rectangleToTransform, cmat3x3 &transform, cbool &hasTransparency, const directionID &directionFacing, const resolutionTexture &tex, const texture &renderTarget, cbool &renderAnimation)
{
	mat3x3 finalTransform = mat3x3::cross(transform, mat3x3::fromRectToRect(crectangle2(0, 0, tex.defaultSize.x, tex.defaultSize.x), rectangleToTransform));
	rectangle2 brushRect = crectangle2(0, 0, tex.defaultSize.x, tex.defaultSize.y);
	const texture &appropriateTexture = tex.mipmap(finalTransform, brushRect);
	renderBrush(brushRect, finalTransform, transform.multPointMatrix(rectangleToTransform.getCenter()), hasTransparency, directionFacing, appropriateTexture, renderTarget, renderAnimation);
}
void renderTexture(crectangle2 &drawRect, cbool &hasTransparency, const directionID &directionFacing, const resolutionTexture &tex, const texture &renderTarget, cbool &renderAnimation)
{
	const texture &appropriateTexture = tex.getMipmapTexture(drawRect.size.x);
	renderBrush(crectangle2(appropriateTexture.getClientRect()), drawRect, hasTransparency, directionFacing, appropriateTexture, renderTarget, renderAnimation);
}
void renderBlockRect(crectangle2 &blockRect, const gameRenderData &targetData)
{
	renderBlockRect(blockRect, targetData, colorPalette::white);
}
void renderBlockRect(crectangle2 &blockRect, const gameRenderData &targetData, const color &c)
{
	fillRectangleBorders(targetData.renderTarget, ceilRectangle(targetData.worldToRenderTargetTransform.multRectMatrix(blockRect)), (int)(settings::videoSettings::guiScale * 2), solidColorBrush(c));
}