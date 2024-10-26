#include "math/collisions.h"
#include "jigsawPool.h"
#include "attachmentDirectionData.h"
#include "facingData.h"
#include "idAnalysis.h"
#include "structureFeature.h"
#include "jigsawData.h"
#include "array/arrayFunctions/arrayFunctions.h"
#include "tickableBlockContainer.h"
#include "doubleBlockData.h"
#include "entity.h"
#include "block.h"
#include <filesystem>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include "blockContainer.h"
#include "blockData.h"
#include "itemID.h"
#include "constants.h"
#include "globalFunctions.h"
#include "math/axis.h"
#include "math/direction.h"
#include "math/mattnxn.h"
#include "math/random/random.h"
#include "math/rectangle/rectangletn.h"
#include "math/vector/vectn.h"
#include "structure.h"
#include "structurePlacementBlueprint.h"
#include "include/optimization/handleError.h"
blockData* structureFeature::cloneBlockData(const blockID& id, blockData* data, cbool& flipX)
{
	blockData* clonedBlockData = createBlockData(id);
	if (data && clonedBlockData)
	{
		data->clone(*clonedBlockData);
		if (flipX)
		{
			if (hasFacingData(id))
			{
				facingData* clonedFacingData = dynamic_cast<facingData*>(clonedBlockData);
				if (getAxis(clonedFacingData->directionFacing) == axisID::x)
				{
					clonedFacingData->directionFacing = flipDirection(clonedFacingData->directionFacing);
				}
			}

			if (hasAttachmentDirectionData(id))
			{
				attachmentDirectionData* clonedAttachmentDirectionData = dynamic_cast<attachmentDirectionData*>(clonedBlockData);
				if (getAxis(clonedAttachmentDirectionData->attachmentDirection) == axisID::x)
				{
					clonedAttachmentDirectionData->attachmentDirection = flipDirection(clonedAttachmentDirectionData->attachmentDirection);
				}
			}
		}
	}
	return clonedBlockData;
}
blockID structureFeature::getBlockToPlace(blockContainer* const& containerIn, cveci2& worldPos, const blockID& structureBlock, blockData* const& data)
{
	blockID blockToPlace = structureBlock;
	if (replaceBlocks && blockToPlace == blockID::jigsaw)
	{
		jigsawData* currentJigsawData = dynamic_cast<jigsawData*>(data);
		if (!currentJigsawData->keepJigsaws)
		{
			blockToPlace = currentJigsawData->turnsInto;//can also be structure void
		}
	}
	return blockToPlace;
}
bool structureFeature::placeStructure(structure* firstStructure, tickableBlockContainer* containerIn, cveci2& firstStructurePos00, std::mt19937& randomToUse, cint& expansionLevels, cbool& includeEntities, cbool& replaceStructureVoid)
{
	return placeStructure(firstStructure, containerIn, firstStructurePos00, randomToUse, randChance(randomToUse, 2), expansionLevels, includeEntities, replaceStructureVoid);
}

bool structureFeature::placeStructure(structure* firstStructure, tickableBlockContainer* containerIn, cveci2& firstStructurePos00, std::mt19937& randomToUse, cbool& flipX, cint& expansionLevels, cbool& includeEntities, cbool& replaceStructureVoid)
{
	std::vector<structurePlacementBlueprint*> placedStructures = std::vector<structurePlacementBlueprint*>(
		{
			new structurePlacementBlueprint(firstStructure, firstStructurePos00, expansionLevels, flipX)
		}
	);

	if (!canPlace(*placedStructures[0], containerIn))
	{
		delete placedStructures[0];
		return false;
	}

	for (size_t structureIndex = 0; structureIndex < placedStructures.size();structureIndex++)
	{
        //we cannot iterate over it in the way of for (structureplacementdata* : placedStructures) because this vector keeps being modified
        //also we have to make a copy of the pointer, as the vector of pointers can get deleted and a bigger array allocated
		const structurePlacementBlueprint* expandFrom = placedStructures[structureIndex];
		placeStructurePart(expandFrom->placedStructure, containerIn, expandFrom->structurePos00, expandFrom->flipX, expandFrom->structureLevel, includeEntities, replaceStructureVoid);
		if (expandFrom->structureLevel >= 0)
		{
			cmat3x3i structureToWorldTransform = expandFrom->placedStructure->getBlocksToWorldTransform(expandFrom->structurePos00, expandFrom->flipX);

			for (cveci2& pos : expandFrom->positionsToConnect)
			{
				jigsawData* currentJigsawData = dynamic_cast<jigsawData*>(expandFrom->placedStructure->blockDataArray.getValueUnsafe(pos));
				if (currentJigsawData->targetName != std::wstring())
				{
					//find fitting piece for this jigsaw
					cveci2 currentJigsawWorldPos = structureToWorldTransform.multPointMatrix(pos);

					directionID worldDirection = currentJigsawData->directionFacing;
					if (expandFrom->flipX && getAxis(worldDirection) == axisID::x)
					{
						worldDirection = flipDirection(worldDirection);
					}

					stdPath pathWithoutExtension = firstStructure->pathWithoutExtension;
					pathWithoutExtension.remove_filename();
					pathWithoutExtension /= currentJigsawData->targetPoolName;
					jigsawPool* targetPool = getJigsawPoolByPath(pathWithoutExtension);

					std::vector<structure*> checkStructures;
					std::vector<fp> checkStructureWeights;

					if (targetPool)
					{
						checkStructures = targetPool->targetedStructures;
						checkStructureWeights = targetPool->structureWeights;
					}
					else
					{
						structure* s = getStructureByPath(pathWithoutExtension);
						if (s)
						{
							checkStructures = { s };
							checkStructureWeights = { 1 };
						}
						else
						{
							checkStructures = { };
							checkStructureWeights = { };
						}
					}

					std::vector<structurePlacementBlueprint*> possibleStructures = {};
					std::vector<fp> possibleStructureWeights = {};

					for (size_t checkStructureIndex = 0; checkStructureIndex < checkStructures.size(); checkStructureIndex++)
					{
						structure* checkStructure = checkStructures[checkStructureIndex];
						std::vector<veci2> connectedStructureJigsawPositions = checkStructure->findBlocks(blockID::jigsaw);

						std::vector<size_t> possibleConnectingJigsawIndexes = {  };
						std::vector<structurePlacementBlueprint*> possibleConnectingJigsawIndexData = {};

						//check all jigsaw positions

						bool foundJigsawWithName = false;

						for (size_t jigsawPositionIndex = 0; jigsawPositionIndex < connectedStructureJigsawPositions.size(); jigsawPositionIndex++)
						{
							//find target block to connect to
							cveci2 connectedJigsawStructurePos = connectedStructureJigsawPositions[jigsawPositionIndex];

							jigsawData* connectedJigsawData = dynamic_cast<jigsawData*>(checkStructure->getBlockData(connectedJigsawStructurePos));

							if (connectedJigsawData->jigsawName == currentJigsawData->targetName)
							{
								foundJigsawWithName = true;

								cbool& flipConnectingStructure = getAxis(connectedJigsawData->directionFacing) == axisID::y ?
									randChance(randomToUse, 2) :
									isPositive(connectedJigsawData->directionFacing) == isPositive(worldDirection);

								veci2 positionToPlace = veci2();

								caxisID& jigsawFacingAxis = getAxis(worldDirection);
								caxisID& jigsawNotFacingAxis = (axisID)(1 - (int)jigsawFacingAxis);

								positionToPlace[jigsawNotFacingAxis] = currentJigsawWorldPos[jigsawNotFacingAxis];

								cveci2& connectedJigsawRelativeWorldPos = cveci2(flipConnectingStructure ?
									(int)checkStructure->blockIDArray.size.x - connectedJigsawStructurePos.x - 1 :
									connectedJigsawStructurePos.x,
									connectedJigsawStructurePos.y
								);

								if (isPositive(worldDirection))
								{
									positionToPlace[jigsawFacingAxis] = currentJigsawWorldPos[jigsawFacingAxis] + 1;
								}
								else
								{
									positionToPlace[jigsawFacingAxis] = currentJigsawWorldPos[jigsawFacingAxis] - 1;
								}

								positionToPlace -= connectedJigsawRelativeWorldPos;

								//cveci2 connectedJigsawWorldPos = currentJigsawWorldPos + directionVectors2D[(int)worldDirection];

								structurePlacementBlueprint* connectingStructureData = new structurePlacementBlueprint(
                                        checkStructure, positionToPlace, connectedStructureJigsawPositions,
                                        expandFrom->structureLevel - 1, flipConnectingStructure
								);

								cmat3x3i connectedStructureTransform = checkStructure->getBlocksToWorldTransform(connectingStructureData->structurePos00, connectingStructureData->flipX);

								if (!canPlace(*connectingStructureData, containerIn))
								{
									delete connectingStructureData;
									goto structureCollided;
								}

								if (checkStructureCollisions)
								{
									//check if the connected structure does not collide
									for (const structurePlacementBlueprint* const& placedStructureData : placedStructures)
									{
										if (collides2d(
											crectangle2(placedStructureData->structurePos00, placedStructureData->placedStructure->blockIDArray.size),
											crectangle2(connectingStructureData->structurePos00, connectingStructureData->placedStructure->blockIDArray.size)))
										{
											//check if there is structure void in the colliding places
											//transform new structure into colliding structure
											cmat3x3i& connectingToColliding = mat3x3::cross(
												placedStructureData->placedStructure->getBlocksToWorldTransform(placedStructureData->structurePos00, placedStructureData->flipX).inverse(),
												connectedStructureTransform
											);

											cmat3x3i& collidingToConnecting = connectingToColliding.inverse();

											cveci2& transformedConnectingStructurePos00 = connectingToColliding.multPointMatrix(cveci2());
											//substracting 1 to prevent loop errors. for example pos00 = 1, 1 and pos11 = 10, 10. then you would loop from 1 to 9. but when you flip it (pos00 = 10, pos11 = 1), then you would start the loop at 10
											cveci2& transformedConnectingStructurePos11 = connectingToColliding.multPointMatrix(cveci2(checkStructure->blockIDArray.size) - 1);

											crectanglei2& otherRect = crectanglei2(cveci2(), (veci2)placedStructureData->placedStructure->blockIDArray.size - 1);
											crectanglei2& collisionRectMinus1 = otherRect.cropClientRectUnsafe(crectanglei2::fromOppositeCorners(transformedConnectingStructurePos00, transformedConnectingStructurePos11));
											//adding the 1 back for the loop
											crectanglei2& collisionRect = crectanglei2(collisionRectMinus1.pos0, collisionRectMinus1.size + 1);

											//crectanglei2 collidingStructureRect = crectanglei2(connectingStructureData->structurePos00 - data->structurePos00, connectingStructureData->placedStructure->blocks->size);

											for (cveci2& checkPos : collisionRect)
											{
												const blockID& a = placedStructureData->placedStructure->blockIDArray.getValueUnsafe(checkPos);
												if (a != blockID::structure_void)
												{
													if (a == blockID::jigsaw)
													{
														if (dynamic_cast<jigsawData*>(placedStructureData->placedStructure->blockDataArray.getValueUnsafe(checkPos))->turnsInto == blockID::structure_void)
														{
															continue;
														}
													}
													//check if the connecting structure part also does not have structure void
													cveci2 connectingStructurePos = collidingToConnecting.multPointMatrix(checkPos);
													const blockID b = checkStructure->blockIDArray.getValueUnsafe(connectingStructurePos);
													if (b != blockID::structure_void)
													{
														if (b == blockID::jigsaw)
														{
															if (dynamic_cast<jigsawData*>(checkStructure->blockDataArray.getValueUnsafe(connectingStructurePos))->turnsInto == blockID::structure_void)
															{
																continue;
															}
														}
														delete connectingStructureData;
														goto structureCollided;
													}
												}
											}
										}
									}
								}
								possibleConnectingJigsawIndexes.push_back(jigsawPositionIndex);
								possibleConnectingJigsawIndexData.push_back(connectingStructureData);
							structureCollided:;
							}
						}

						if (!foundJigsawWithName)
						{
							handleError(std::wstring(L"did not find a jigsaw block with the name ") + currentJigsawData->targetName);
						}

						if (possibleConnectingJigsawIndexes.size())
						{
							cint possibleConnectionIndex = randIndex(randomToUse, (int)possibleConnectingJigsawIndexes.size());
							csize_t& connectionIndex = possibleConnectingJigsawIndexes[possibleConnectionIndex];

							connectedStructureJigsawPositions.erase(connectedStructureJigsawPositions.begin() + connectionIndex);

							possibleStructures.push_back(possibleConnectingJigsawIndexData[possibleConnectionIndex]);
							possibleStructureWeights.push_back(checkStructureWeights[checkStructureIndex]);
						}

					}

					if (possibleStructures.size())
					{
						structurePlacementBlueprint* connectedStructureData = possibleStructures[randIndex(randomToUse, possibleStructureWeights, getSum<fp>(possibleStructureWeights))];
						placedStructures.push_back(connectedStructureData);
					}
				}
			}
		}
	}
	for (structurePlacementBlueprint* data : placedStructures)
	{
		delete data;
	}
	return true;
}

void structureFeature::placeStructurePart(structure* s, tickableBlockContainer* containerIn, cveci2& pos00, cbool& flipX, cint& levels, cbool& includeEntities, cbool& replaceStructureVoid)
{
	cmat3x3i& structureToWorldTransform = s->getBlocksToWorldTransform(pos00, flipX);

	std::vector<veci2> connectedStructurePositions;
	for (veci2 pos = veci2(); pos.y < (int)s->blockIDArray.size.y; pos.y++)
	{
		for (pos.x = 0; pos.x < (int)s->blockIDArray.size.x; pos.x++)
		{
			cveci2& worldPos = structureToWorldTransform.multPointMatrix(pos);

			const blockID& structureBlock = s->blockIDArray.getValue(pos);

			blockData* dataToClone = s->blockDataArray.getValueUnsafe(pos);
			blockID blockToPlace = structureBlock;

			blockToPlace = getBlockToPlace(containerIn, worldPos, blockToPlace, dataToClone);

			if (blockToPlace != structureBlock)
			{
				dataToClone = nullptr;
			}

			if ((blockToPlace != blockID::structure_void) || (!replaceBlocks))
			{
				blockData* clonedBlockData = cloneBlockData(blockToPlace, dataToClone, flipX);
				if (dataToClone)
				{
					if (isDoubleBlock(blockToPlace))
					{
						doubleBlockData* structureDoubleBlockData = dynamic_cast<doubleBlockData*>(dataToClone);
						if (structureDoubleBlockData->isPart0)
						{
							directionID structureDirection;
							if (hasFacingData(blockToPlace))
							{
								structureDirection = dynamic_cast<facingData*>(dataToClone)->directionFacing;
							}
							else
							{
								structureDirection = standardUpFacingBlockDirection;
							}
							cveci2 structurePart1Pos = pos + getOtherPartRelativeLocation(blockToPlace, structureDoubleBlockData->isPart0, structureDirection);
							blockData* part1Data = s->blockDataArray.getValue(structurePart1Pos);
							//you never know if the other part of the block is outside of the structure
							if (part1Data)
							{
								containerIn->setBlockWithData(worldPos, blockToPlace, clonedBlockData);
								cveci2 worldPart1Pos = structureToWorldTransform.multPointMatrix(structurePart1Pos);
								containerIn->setBlockWithData(worldPart1Pos, blockToPlace, cloneBlockData(blockToPlace, part1Data, flipX));
							}
						}
						continue;
					}
				}
				containerIn->setBlockWithData(worldPos, blockToPlace, clonedBlockData);
			}
		}
	}
	cmat3x3& entitiesToWorldTransform = s->getEntitiesToWorldTransform(pos00, flipX);
	if (includeEntities)
	{
		//add entities
		for (entity* e : s->entities)
		{
			entity* clonedEntity = createEntity(e->entityType, containerIn->rootDimension, entitiesToWorldTransform.multPointMatrix(e->position));
			e->clone(*clonedEntity);
			clonedEntity->newPosition = entitiesToWorldTransform.multPointMatrix(e->newPosition);
			if (flipX)
			{
				e->speed.x = -e->speed.x;
			}
			clonedEntity->addToWorld();
		}
	}
}

veci2 structureFeature::getStructureBottom(structure* s)
{
	for (fsize_t height = 0; height < s->blockIDArray.size.y; height++)
	{
		std::vector<veci2> bottomPositions = s->getAffectedPositions(crectanglei2(0, height, (int)s->blockIDArray.size.x, 1), [this, s](cveci2& pos)
			{
				return isBottomBlock(s->getBlockID(pos));
			});

		if (bottomPositions.size())
		{
			return bottomPositions[bottomPositions.size() / 2];
		}
	}

	return veci2((int)s->blockDataArray.size.x / 2, 0);
}

bool structureFeature::isBottomBlock(const blockID& b)
{
	return !blockList[b]->canReplaceBlock;
}

bool structureFeature::canPlace(const structurePlacementBlueprint& bluePrint, tickableBlockContainer* containerIn)
{
	return true;
}

