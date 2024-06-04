#include "gameControl.h"
#include "filesystem/textfile.h"
#include "world.h"
#include "chat.h"
#include "human.h"
#include "dimensionData.h"
#include "block.h"
#include "chunk.h"
#include "statusEffectData.h"
#include "structureData.h"
#include "itemData.h"
#include "enchantmentData.h"
#include "gameModeData.h"
#include <math.h>
#include <string>
#include <utility>
#include <vector>
#include <SFML/Network/IpAddress.hpp>
#include "biomeData.h"
#include "biomeID.h"
#include "itemID.h"
#include "chunkLoadLevel.h"
#include "constants.h"
#include "dimensionID.h"
#include "enchantment.h"
#include "enchantmentID.h"
#include "entity.h"
#include "entityID.h"
#include "gameModeID.h"
#include "humanoid.h"
#include "idAnalysis.h"
#include "application/control/control.h"
#include "array/wstringFunctions.h"
#include "globalFunctions.h"
#include "math/graphics/texture.h"
#include "math/mathFunctions.h"
#include "math/rectangle/rectangletn.h"
#include "math/vector/vectn.h"
#include "type/conversion.h"
#include "itemID.h"
#include "itemStack.h"
#include "itemTag.h"
#include "mob.h"
#include "nbt/nbtCompound.h"
#include "nbt/nbtSerializable.h"
#include "nbt/nbtSerializer.h"
#include "statusEffect.h"
#include "statusEffectID.h"
#include "structureID.h"
#include "minecraftFont.h"
#include <SFML/Network.hpp>
#include "rectangularSlotContainer.h"
#include <fstream>
#include "folderList.h"

constexpr int selectedPositionCount = 2;
vec2 selectedPositions[selectedPositionCount]{};
bool selectedPositionSet[selectedPositionCount]{};
dimensionID selectedDimensions[selectedPositionCount]{};

bool chat::getLocation(const human &sender, const wstringContainer &container, int &offset, dimensionID &dimensionIn, vec2 &position)
{
	const std::wstring &dimensionName = getWord(container, offset);

	if (dimensionName == std::wstring(L"@p"))
	{
		dimensionIn = sender.dimensionIn->identifier;
		position = sender.position;
		offset++;
		return true;
	}
	else if (dimensionName.substr(0, 3) == std::wstring(L"pos"))
	{
		int posIndex;

		if (convertToInt(dimensionName.substr(3), posIndex) && selectedPositionSet[posIndex])
		{
			dimensionIn = selectedDimensions[posIndex];
			position = selectedPositions[posIndex];
		}
		offset++;
		return true;
	}

	dimensionID dimensionToReturn = dimensionDataList.getIDByName(dimensionName);
	int newOffset = offset;
	if (dimensionToReturn == (dimensionID)-1)
	{
		dimensionToReturn = sender.dimensionIn->identifier;
	}
	else
	{
		newOffset++;
	}

	vec2 posToReturn = vec2(); // the position will not be modified if the last one fails
	for (int i = 0; i < 2; i++)
	{
		const std::wstring axisWord = getWord(container, newOffset);
		bool valid = false;
		if (axisWord.size())
		{
			int letterOffset = 0;

			fp additional;
			if (axisWord[0] == L'~')
			{
				posToReturn[i] = sender.position[i];
				letterOffset++;
				valid = true;
			}
			if (convertToDouble(axisWord.substr(letterOffset), additional))
			{
				posToReturn[i] += additional;
				valid = true;
			}
			if (valid)
			{
				newOffset++;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	dimensionIn = dimensionToReturn;
	position = posToReturn;
	offset = newOffset;
	return true;
}

bool chat::getBlockToUse(const human &sender, const wstringContainer &container, int &offset, blockID &result)
{
	std::wstring blockName = getWord(container, offset);
	if (blockName.length())
	{
		result = (blockID)blockList.getIDByName(blockName);
		if ((int)result == -1)
		{
			addLine(std::wstring(L"block id not found"));
		}
		else
		{
			return true;
		}
	}
	else if (isHumanoid(sender.entityType))
	{
		const humanoid &currentHumanoid = (humanoid &)sender;
		const itemID &item = currentHumanoid.itemHolding->stackItemID;
		if ((int)item)
		{
			result = (blockID)item;
			if (isBlockItem((itemID)result))
			{
				return true;
			}
			else
			{
				addLine(std::wstring(L"the item you are holding is not a block"));
			}
		}
		else
		{
			addLine(std::wstring(L"you're not holding an item"));
		}
	}
	return false;
}

bool chat::selectEntities(const human &sender, const wstringContainer &container, int &offset, std::vector<entity *> &result)
{
	const std::wstring &compareString = getWord(container, offset);
	if (compareString.size() && (compareString[0] == L'@'))
	{
		offset++;
		const std::vector<entity *> entityList = sender.dimensionIn->findNearEntities(sender.position, commandEntityRadius);
		std::vector<entity *> selectedEntities = std::vector<entity *>();

		for (entity *e : entityList)
		{
			if (e->compareSelector(sender, compareString))
			{
				selectedEntities.push_back(e);
			}
		}
		result = selectedEntities;
	}
	else
	{
		result = {(entity *)&sender};
	}
	return true;
}

void chat::say(human &sender, std::wstring line)
{
	addLine(line);
	if (line.length() && line[0] == L'/')
	{
		line = line.substr(1); // remove the backslash
		// it's a command
		const wstringContainer lines = split_string(line, std::wstring(L" "), std::wstring(L"[](){}\"\""));
		if (lines.size())
		{
			if (getWord(lines, 0) == std::wstring(L"fix"))
			{
				if (getWord(lines, 1) == std::wstring(L"lighting"))
				{
					for (const std::pair<veci2, chunk *> &currentPair : sender.dimensionIn->loadedChunksMap)
					{
						currentPair.second->internalSunLightLevels.fill(0);
						currentPair.second->blockLightLevels.fill(0);
						currentPair.second->addLightUpdates();
					}
				}
			}
			else
			{
				if (currentWorld->allowCheats)
				{
					if (getWord(lines, 0) == std::wstring(L"setworldspawn"))
					{
						currentWorld->worldSpawnPoint = sender.position;
						currentWorld->worldSpawnDimension = sender.dimensionIn->identifier;
					}
					else if (getWord(lines, 0) == std::wstring(L"kill"))
					{
						std::vector<entity *> listToKill;
						int index = 1;
						if (selectEntities(sender, lines, index, listToKill))
						{
							for (entity *e : listToKill)
							{
								e->onDeath();
							}
						}
					}
					else if (getWord(lines, 0) == std::wstring(L"heal"))
					{
						std::vector<entity *> listToHeal;
						int index = 1;
						if (selectEntities(sender, lines, index, listToHeal))
						{
							fp heartAmount;
							if (!convertToDouble(getWord(lines, index), heartAmount))
							{
								heartAmount = INFINITY;
							}
							for (entity *e : listToHeal)
							{
								e->heal(heartAmount);
							}
						}
					}

					else if (getWord(lines, 0) == std::wstring(L"effect"))
					{
						if (getWord(lines, 1) == std::wstring(L"clear"))
						{
							std::vector<entity *> listToRemoveEffects;
							int index = 2;
							if (selectEntities(sender, lines, index, listToRemoveEffects))
							{
								for (entity *e : listToRemoveEffects)
								{
									e->activeEffects.clear();
								}
							}
						}
						else
						{
							int index = 1;
							std::vector<entity *> listToAddEffectTo;
							selectEntities(sender, lines, index, listToAddEffectTo);
							const statusEffectID &id = statusEffectDataList.getIDByName(getWord(lines, index++));
							if ((int)id != -1)
							{
								statusEffect effectToAdd = statusEffect(id);
								if (convertToInt(getWord(lines, index++), effectToAdd.ticksDuration))
								{
									if (!convertToInt(getWord(lines, index), effectToAdd.potency))
									{
										effectToAdd.potency = 1;
									}

									for (entity *e : listToAddEffectTo)
									{
										e->addStatusEffects({effectToAdd});
									}
								}
							}
						}
					}
					else if (getWord(lines, 0) == std::wstring(L"locatebiome"))
					{
						const biomeID &biomeIndex = biomeDataList.getIDByName(getWord(lines, 1));
						if ((int)biomeIndex != -1)
						{
							vec2 biomeLocation;
							if (sender.dimensionIn->locateBiomes({biomeIndex}, sender.position, biomeLocation))
							{
								// teleport player to biome
								sender.teleportTo(sender.dimensionIn, biomeLocation, true);
							}
						}
					}
					else if (getWord(lines, 0) == std::wstring(L"locate"))
					{
						const structureID &structureIndex = structureDataList.getIDByName(getWord(lines, 1));
						if ((int)structureIndex != -1)
						{
							vec2 structureLocation = sender.position;
							if (sender.dimensionIn->locateStructure(structureIndex, sender.position, structureLocation))
							{

								// teleport player to structure
								sender.teleportTo(sender.dimensionIn, structureLocation, true);
							}
						}
					}

					else if (getWord(lines, 0) == std::wstring(L"xray"))
					{
						currentWorld->xray = getWord(lines, 1) == std::wstring(L"true");
					}
					else if (getWord(lines, 0) == std::wstring(L"pool"))
					{
						int offset = 1;
						vec2 from = selectedPositions[0];
						dimensionID poolDimension = selectedDimensions[0];
						bool loc0 = getLocation(sender, lines, offset, poolDimension, from) || selectedPositionSet[0];
						if (loc0)
						{
							const blockID &b = blockList.getIDByName(getWord(lines, offset));
							if ((int)b != -1)
							{
								int maxPoolSize;
								if (lines.size() < 3 || !convertToInt(getWord(lines, 2), maxPoolSize))
								{
									maxPoolSize = 0x1000;
								}
								if (isFluid(b))
								{
									currentWorld->dimensions[(int)poolDimension]->addFluidPool(floorVector(from), b, maxPoolSize);
								}
								else
								{
									currentWorld->dimensions[(int)poolDimension]->addPool(floorVector(from), b, maxPoolSize);
								}
							}
						}
					}
					else if (getWord(lines, 0) == std::wstring(L"summon"))
					{
						int identifier = getEntityIDByName(getWord(lines, 1));
						if (identifier != -1)
						{
							summonEntity((entityID)identifier, sender.dimensionIn, sender.position);
						}
					}
					else if (isMob(sender.entityType))
					{
						mob &currentMob = (mob &)sender;
						if (getWord(lines, 0) == std::wstring(L"fly"))
						{
							// set also in survival
							currentMob.flying = !currentMob.flying;
						}
						else if (isHumanoid(sender.entityType))
						{
							humanoid &currentHumanoid = (humanoid &)sender;

							if (getWord(lines, 0) == std::wstring(L"give"))
							{
								// give item
								const itemID &itemIDToGive = itemList.getIDByName(getWord(lines, 1));
								if ((int)itemIDToGive != -1)
								{
									int amountToGive = 1;
									if (lines.size() == 2 || !convertTo<int>(getWord(lines, 2), amountToGive))
									{
										amountToGive = 1;
									}
									while (amountToGive > 0)
									{
										itemStack stackToGive = itemStack();
										stackToGive.stackItemID = (itemID)itemIDToGive;
										stackToGive.data = createItemTag(stackToGive.stackItemID);
										stackToGive.count = math::minimum(itemList[itemIDToGive]->maxStackSize, amountToGive);

										amountToGive -= stackToGive.count;
										if (!currentHumanoid.addStack(stackToGive))
										{
											break; // inventory is full
										}
									}
								}
							}
							else if (getWord(lines, 0) == std::wstring(L"enchant"))
							{
								if (currentHumanoid.itemHolding && currentHumanoid.itemHolding->count)
								{
									enchantment e;
									const enchantmentID &index = enchantmentDataList.getIDByName(getWord(lines, 1));
									if ((int)index != -1)
									{
										e.identifier = (enchantmentID)index;
										if (lines.size() == 3)
										{
											if (!convertToInt(getWord(lines, 2), e.power))
											{
												goto enchantmentFailed;
											}
										}
										else
										{
											e.power = 1;
										}
										currentHumanoid.itemHolding->enchantments.push_back(new enchantment(e));
									enchantmentFailed:;
									}
								}
							}
							else if (sender.entityType == entityID::human)
							{
								if (getWord(lines, 0) == std::wstring(L"xp"))
								{
									int amount;
									if (convertToInt(getWord(lines, 1), amount))
									{
										sender.addExperience(amount);
									}
								}
								else if (getWord(lines, 0) == std::wstring(L"setfoodlevel"))
								{
									int newFoodLevel;
									if (convertToInt(getWord(lines, 1), newFoodLevel))
									{
										sender.foodlevel = newFoodLevel;
									}
								}
								else if (getWord(lines, 0) == std::wstring(L"gamemode"))
								{
									const gameModeID &id = gameModeDataList.getIDByName(getWord(lines, 1));
									if ((int)id != -1)
									{
										sender.setGameMode((gameModeID)id);
									}
								}
							}
						}
					}
					if (getWord(lines, 0).substr(0, 3) == std::wstring(L"pos"))
					{
						int posIndex;
						if (convertToInt(getWord(lines, 0).substr(3), posIndex))
						{
							int offset = 1;
							dimensionID pos0Dimension = sender.dimensionIn->identifier;
							vec2 pos0Position;
							pos0Position = sender.selectedUUID ? sender.exactEntityIntersection : sender.exactBlockIntersection;

							getLocation(sender, lines, offset, pos0Dimension, pos0Position);
							selectedPositionSet[posIndex] = true;
							selectedPositions[posIndex] = pos0Position;
							selectedDimensions[posIndex] = pos0Dimension;

							addLine(std::wstring(L"pos") + std::to_wstring(posIndex) + std::wstring(L" set to: ") + std::to_wstring(selectedPositions[posIndex].x) + std::wstring(L", ") + std::to_wstring(selectedPositions[posIndex].y) + std::wstring(L" (dimension: ") + dimensionDataList[selectedDimensions[posIndex]]->name + std::wstring(L")"));
							return;
						}
					}
					if (getWord(lines, 0) == std::wstring(L"data"))
					{
						nbtSerializable *selectedSerializable = nullptr;
						int wordOffset = 3;

						std::wstring outputString = std::wstring();
						std::wstring dataString = getWord(lines, 2);

						if (dataString == std::wstring(L"block"))
						{
							dimensionID getDimension;
							vec2 getPosition;
							if (getLocation(sender, lines, wordOffset, getDimension, getPosition))
							{
								selectedSerializable = currentWorld->dimensions[(int)getDimension]->getBlockData(floorVector(getPosition));
								const std::wstring positionString = std::to_wstring(getPosition.x) + std::wstring(L" ") + std::to_wstring(getPosition.y) + std::wstring(L" ");
								outputString += positionString;
							}
							else
							{
								addLine(std::wstring(L"where?"));
								return;
							}
						}
						else if (getWord(lines, 2) == std::wstring(L"entity"))
						{
							std::vector<entity *> entityList;
							if (selectEntities(sender, lines, wordOffset, entityList))
							{
								if (entityList.size())
								{
									selectedSerializable = entityList[0];
									// wordOffset++;
								}
								else
								{
									addLine(std::wstring(L"no entities found"));
									return;
								}
							}
						}
						if (selectedSerializable)
						{
							if (getWord(lines, 1) == std::wstring(L"get"))
							{
							}
							else if (getWord(lines, 1) == std::wstring(L"merge"))
							{
								nbtCompound compound = nbtCompound::fromString(getWord(lines, wordOffset));
								nbtSerializer s = nbtSerializer(compound, false, false);
								selectedSerializable->serializeValue(s);
							}
							else
							{
								addLine(std::wstring(L"the command you typed is not a valid command"));
							}
							addLine(outputString + std::wstring(L" has the following ") + dataString + std::wstring(L" data: ") + selectedSerializable->toString());
						}
						else
						{
							addLine(outputString + std::wstring(L" has no ") + dataString + std::wstring(L" data"));
						}
					}

					else if (getWord(lines, 0) == std::wstring(L"seed"))
					{
						addLine(std::wstring(L"Seed: ") + std::to_wstring(currentWorld->seed));
					}
					else if (getWord(lines, 0) == std::wstring(L"ip"))
					{
						addLine(std::wstring(L"private ip: ") + stringToWString(sf::IpAddress::getLocalAddress().toString()));
						// public ip adress takes a while as it needs to connect to an outside server to check
						addLine(std::wstring(L"public ip: ") + stringToWString(sf::IpAddress::getPublicAddress().toString()));
					}
					else if (getWord(lines, 0) == std::wstring(L"tp"))
					{
						std::vector<entity *> listToTeleport;
						int wordOffset = 1;
						if (selectEntities(sender, lines, wordOffset, listToTeleport))
						{
							dimensionID teleportDimension = sender.dimensionIn->identifier;
							vec2 teleportPosition = sender.position;
							if (getLocation(sender, lines, wordOffset, teleportDimension, teleportPosition))
							{

								for (entity *e : listToTeleport)
								{
									e->teleportTo(currentWorld->dimensions[(int)teleportDimension], teleportPosition, true);
								}
							}
						}
					}
					else if (getWord(lines, 0) == std::wstring(L"clear"))
					{
						if (getWord(lines, 1) == std::wstring(L"lag"))
						{
							for (int i = 0; i < (int)dimensionID::count; i++)
							{
								currentWorld->dimensions[i]->blockUpdatePositions.clear();
							}
						}
						else if (getWord(lines, 1) == std::wstring(L"inventory"))
						{
							sender.armorSlots->clearData();
							sender.hotbarSlots->clearData();
							sender.leftHandSlot->clearData();
							sender.inventorySlots->clearData();
						}
					}
					else if (getWord(lines, 0) == std::wstring(L"log") && getWord(lines, 1) == std::wstring(L"chat"))
					{
						const std::wstring path = getWord(lines, 2);
						std::wstring mergedString = std::wstring();
						if (path.size() > 2)
						{
							for (size_t i = 0; i < currentWorld->currentChat.linesSaid.size(); i++)
							{
								if (i)
								{
									mergedString += std::wstring(L"\n");
								}
								mergedString += currentWorld->currentChat.linesSaid[i];
							}

							writealltext(path.substr(1, path.size() - 2), WStringToString(mergedString));
						}
						else
						{
							addLine(std::wstring(L"where should the chat be saved?"));
							return;
						}
					}
					else if (getWord(lines, 0) == std::wstring(L"time") && getWord(lines, 1) == std::wstring(L"set"))
					{
						fp newTime;
						const std::wstring &timeString = getWord(lines, 2);
						if (convertToDouble(timeString, newTime))
						{
							currentWorld->currentTime = newTime;
						}
						else if (timeString == std::wstring(L"day"))
						{
							currentWorld->currentTime = 1000;
						}
						else if (timeString == std::wstring(L"noon"))
						{
							currentWorld->currentTime = 6000;
						}
						else if (timeString == std::wstring(L"night"))
						{
							currentWorld->currentTime = 13000;
						}
						else if (timeString == std::wstring(L"midnight"))
						{
							currentWorld->currentTime = 18000;
						}
						else if (timeString == std::wstring(L"sunrise"))
						{
							currentWorld->currentTime = 23000;
						}
						else if (timeString == std::wstring(L"sunset"))
						{
							currentWorld->currentTime = 12000;
						}
					}
					else if (getWord(lines, 0) == std::wstring(L"gamerule"))
					{
						if (getWord(lines, 1) == std::wstring(L"randomtickspeed"))
						{
							int newSpeed;
							if (convertToInt(getWord(lines, 2), newSpeed))
							{
								currentWorld->randomTickSpeed = newSpeed;
							}
						}
						else if (getWord(lines, 1) == std::wstring(L"domobspawning"))
						{
							currentWorld->spawnMobs = getWord(lines, 2) == std::wstring(L"true");
						}
						else if (getWord(lines, 1) == std::wstring(L"keepinventory"))
						{
							currentWorld->keepInventoryOnDeath = getWord(lines, 2) == std::wstring(L"true");
						}
						else if (getWord(lines, 1) == std::wstring(L"daynightcyclespeed"))
						{
							fp newSpeed;
							if (convertToDouble(getWord(lines, 2), newSpeed))
							{
								currentWorld->dayNightCycleSpeed = newSpeed;
							}
						}
					}
					else
					{
						int offset = 0;
						const std::wstring &commandName = getWord(lines, offset++);

						vec2 from = selectedPositions[0];
						dimensionID fromDimension = selectedDimensions[0];
						bool loc1 = getLocation(sender, lines, offset, fromDimension, from) || selectedPositionSet[0];
						if (loc1)
						{
							vec2 to = selectedPositions[1];
							dimensionID toDimension = selectedDimensions[1];
							bool loc2 = getLocation(sender, lines, offset, toDimension, to) || selectedPositionSet[1];
							if (loc2 && (fromDimension == toDimension))
							{
								blockID blockToUse;
								if (commandName == std::wstring(L"set"))
								{
									if (getBlockToUse(sender, lines, offset, blockToUse))
									{
										currentWorld->dimensions[(int)fromDimension]->setBlockRange(floorVector(from), floorVector(to), (blockID)blockToUse, chunkLoadLevel::updateLoaded);
									}
								}
								else if (commandName == std::wstring(L"replace"))
								{
									const wstringContainer &replaceContainer = split_string(getWord(lines, offset++), std::wstring(L","));

									std::vector<blockID> replaceList = std::vector<blockID>(replaceContainer.size());
									if (replaceContainer.size())
									{
										for (size_t i = 0; i < replaceContainer.size(); i++)
										{
											replaceList[i] = (blockID)blockList.getIDByName(trim_copy(replaceContainer[i]));
											if ((int)replaceList[i] == -1)
											{
												addLine(std::wstring(L"identifier not found"));
												return;
											}
										}
										if (getBlockToUse(sender, lines, offset, blockToUse))
										{
											currentWorld->dimensions[(int)fromDimension]->replaceBlockRange(floorVector(from), floorVector(to), (blockID)blockToUse, replaceList, chunkLoadLevel::updateLoaded);
										}
									}
									else
									{
										addLine(std::wstring(L"what should be replaced?"));
									}
								}
							}
						}
					}
				}
				else
				{
					addLine(std::wstring(L"you can't cheat"));
				}
			}
		}
	}
}

void chat::addLine(const std::wstring &line)
{
	timesSaid.push_back(currentWorld->ticksSinceStart);
	linesSaid.push_back(line);
	const stdPath &logPath = savesFolder / currentWorld->name / L"log.txt";
	cbool &fileExists = std::ifstream(logPath).good();
	std::wofstream logFile(logPath, std::ios_base::app | std::ios_base::out);
	if (logFile.is_open())
	{
		if (!fileExists)
		{
			logFile.imbue(std::locale(logFile.getloc(), new std::codecvt_utf8_utf16<wchar_t, 0x10ffff, std::little_endian>));
			logFile << L'\uFEFF'; // UTF-16 BOM. this tells apps that this is a wide char text file
			logFile << line;
		}
		else
		{
			logFile << "\n" << line;
		}
		logFile.flush();
		logFile.close();
	}
}

void chat::render(const texture &renderTarget, const gameControl &screen) const
{
	cint chatTime = 0x8 * ticksPerRealLifeSecond;
	cbool showMoreChat = screen.focusedChild == screen.commandLineTextbox;

	int lineCount = 0;
	cint maxLineCount = 0x10;
	minecraftFont f = minecraftFont();
	vec2 offset = cvec2(screen.commandLineTextbox->rect.pos0);
	// offset.y -= f.fontSize;
	for (int i = (int)linesSaid.size() - 1; (i >= 0) && (lineCount < maxLineCount); i--)
	{
		if (((timesSaid[i] + chatTime) > currentWorld->ticksSinceStart) || showMoreChat)
		{
			// this applies even in the top left
			cvec2 &size = f.measureStringSize(cvec2(renderTarget.size), linesSaid[i]);
			offset.y -= size.y;
			f.DrawString(linesSaid[i], crectangle2(offset, size), renderTarget);
			lineCount++;
		}
		else
		{
			return;
		}
	}
}