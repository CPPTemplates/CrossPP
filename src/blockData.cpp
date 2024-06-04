#include "spawnerData.h"
#include "spongeData.h"
#include "brewingStandData.h"
#include "snowLayerData.h"
#include "sugarCaneData.h"
#include "fireData.h"
#include "endPortalFrameData.h"
#include "structureBlockData.h"
#include "jigsawData.h"
#include "redstoneLampData.h"
#include "ironTrapDoorData.h"
#include "ironDoorData.h"
#include "dropperData.h"
#include "repeaterData.h"
#include "comparatorData.h"
#include "noteBlockData.h"
#include "jukeBoxData.h"
#include "redstoneTorchData.h"
#include "tntData.h"
#include "leverData.h"
#include "shortGrassData.h"
#include "fungusData.h"
#include "doubleAttachedBlockPollenData.h"
#include "idAnalysis.h"
#include "grassBlockData.h"
#include "furnaceData.h"
#include "chestData.h"
#include "stemPlantData.h"
#include "pressurePlateData.h"
#include "attachedCropData.h"
#include "bedData.h"
#include "buttonData.h"
#include "stairsData.h"
#include "slabData.h"
#include "fenceGateData.h"
#include "pistonData.h"
#include "mushroomData.h"
#include <map>
#include "ageData.h"
#include "attachedBlockData.h"
#include "attachedBlockPollenData.h"
#include "attachmentDirectionData.h"
#include "blockData.h"
#include "itemID.h"
#include "constants.h"
#include "cropData.h"
#include "dispenserData.h"
#include "doorData.h"
#include "doubleBlockData.h"
#include "facingData.h"
#include "fallingBlockData.h"
#include "fluidData.h"
#include "math/vector/vectn.h"
#include "lastPowerLevelData.h"
#include "nbt/nbtSerializer.h"
#include "pollenData.h"
#include "saplingData.h"
#include "trapDoorData.h"
#include "leavesData.h"
blockData* createBlockData(const blockID& block)
{
	if (block == blockID::spawner)
	{
		return new spawnerData();
	}
	else if (block == blockID::wet_sponge)
	{
		return new spongeData();
	}
	else if (block == blockID::brewing_stand)
	{
		return new brewingstandData();
	}
	else if (block == blockID::snow)
	{
		return new snowLayerData();
	}
	else if (block == blockID::sugar_cane)
	{
		return new sugarCaneData();
	}
	else if (block == blockID::fire)
	{
		return new fireData();
	}
	else if (block == blockID::end_portal_frame)
	{
		return new endPortalFrameData();
	}
	else if (block == blockID::structure_block)
	{
		return new structureBlockData();
	}
	else if (block == blockID::jigsaw)
	{
		return new jigsawData();
	}
	else if (block == blockID::redstone_lamp)
	{
		return new redstoneLampData();
	}
	else if (block == blockID::iron_trapdoor)
	{
		return new ironTrapDoorData();
	}
	else if (block == blockID::iron_door)
	{
		return new ironDoorData();
	}
	else if (block == blockID::dropper)
	{
		return new dropperData();
	}
	else if (block == blockID::repeater)
	{
		return new repeaterData();
	}
	else if (block == blockID::comparator)
	{
		return new comparatorData();
	}
	else if (block == blockID::note_block)
	{
		return new noteBlockData();
	}
	else if (block == blockID::jukebox)
	{
		return new jukeBoxData();
	}
	else if (block == blockID::redstone_torch)
	{
		return new redstoneTorchData();
	}
	else if (block == blockID::tnt)
	{
		return new tntData();
	}
	else if (block == blockID::lever)
	{
		return new leverData();
	}
	else if (isShortGrass(block))
	{
		return new shortGrassData();
	}
	else if (isFungus(block))
	{
		return new fungusData();
	}
	else if (hasPollenData(block))
	{
		if (hasAttachedBlockData(block))
		{
			if (isDoubleBlock(block))
			{
				return new doubleAttachedBlockPollenData();
			}
			else
			{
				return new attachedBlockPollenData();
			}
		}
		else if (hasLeavesData(block))
		{
			return new leavesData();
		}
		else
		{
			return new pollenData();
		}
	}
	else if (hasGrassBlockData(block))
	{
		return new grassBlockData();
	}
	else if (hasChestData(block))
	{
		return new chestData();
	}
	else if (isFurnace(block))
	{
		return new furnaceData();
	}
	else if (isStemPlant(block))
	{
		return new stemPlantData();
	}
	else if (isPressurePlate(block))
	{
		return new pressurePlateData();
	}
	else if (isPiston(block))
	{
		return dynamic_cast<blockData*>(new pistonData());
	}
	else if (isDispenser(block))
	{
		return new dispenserData();
	}
	else if (isStemPlant(block))
	{
		return new stemPlantData();
	}
	else if (isCrop(block))
	{
		if (attachedToBottomBlock(block))
		{
			return new attachedCropData();
		}
		else
		{
			return new cropData();
		}
	}
	else if (isBed(block))
	{
		return new bedData();
	}
	else if (isButton(block))
	{
		return new buttonData();
	}
	else if (isStairs(block))
	{
		return new stairsData();
	}
	else if (isSlab(block))
	{
		return new slabData();
	}
	else if (isFenceGate(block))
	{
		return new fenceGateData();
	}
	else if (hasFluidData(block))
	{
		return new fluidData();
	}
	else if (isMushroom(block))
	{
		return new mushroomData();
	}
	else if (isSapling(block))
	{
		return new saplingData();
	}
	else if (isDoor(block))
	{
		return dynamic_cast<blockData*>(new doorData());
	}
	else if (isTrapDoor(block))
	{
		return dynamic_cast<blockData*>(new trapDoorData());
	}
	else if (hasAttachmentDirectionData(block))
	{
		return new attachmentDirectionData();
	}
	else if (hasPowerLevelData(block))
	{
		return new lastPowerLevelData();
	}
	else if (hasFacingData(block))
	{
		return new facingData();
	}
	else if (hasAgeData(block))
	{
		return new ageData();
	}
	else if (isDoubleBlock(block))
	{
		return new doubleBlockData();
	}
	else if (isFallingBlock(block))
	{
		return new fallingBlockData();
	}
	else if (hasAttachedBlockData(block))
	{
		return new attachedBlockData();
	}
	return nullptr;
}
bool blockData::tick(tickableBlockContainer* containerIn, cveci2& position)
{
	return false;
	//handleError(std::wstring(L"not implemented or this block is not supposed to tick");
}

void blockData::randomTick(tickableBlockContainer* containerIn, cveci2& position)
{

}

void blockData::onBlockRemove(tickableBlockContainer* containerIn, cveci2& position)
{
}

void blockData::serializeValue(nbtSerializer& s)
{
	//position will be serialized in the chunk::serialize functions
	//some blocks do not store block data, but use it for random ticks or normal ticks
}