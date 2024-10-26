#include "decayedStructureFeature.h"
#include "block.h"
#include "include/math/random/random.h"
blockID decayedStructureFeature::getBlockToPlace(blockContainer* const& containerIn, cveci2& worldPos, const blockID& structureBlock, blockData* const& data)
{
	blockID blockToPlace = structureBlock;
	//hardness >= 0 will prevent bedrock from getting replaced, for example
	if ((integrity < 1) && (((blockList[blockToPlace]->hardness >= 0)) || blockList[blockToPlace]->canReplaceBlock))
	{
		//to reduce unnecessary rolls
		if ((blockToPlace != blockID::air) && (blockToPlace != blockID::structure_void))
		{
			if (randFp(*randomToUse) > integrity)
			{
				blockToPlace = blockID::structure_void;
			}
		}

		if ((blockToPlace == blockID::gold_block) || (blockToPlace == blockID::diamond_block))
		{
			if (randFp(*randomToUse) < stolenChance)
			{
				blockToPlace = blockID::structure_void;
			}
		}

		if (blockToPlace == blockID::smooth_stone)
		{
			if (randFp(*randomToUse) > integrity)
			{
				blockToPlace = blockID::stone;
			}
		}
		if (blockToPlace == blockID::smooth_stone_slab)
		{
			if (randFp(*randomToUse) > integrity)
			{
				blockToPlace = blockID::stone_slab;
			}
		}

		if (blockToPlace == blockID::stone)
		{
			if (randFp(*randomToUse) > integrity)
			{
				blockToPlace = blockID::cobblestone;
			}
		}

		if (blockToPlace == blockID::stone_bricks)
		{
			if (randFp(*randomToUse) < crackedChance)
			{
				blockToPlace = blockID::cracked_stone_bricks;
			}
		}

		if ((blockToPlace == blockID::cobblestone) || (blockToPlace == blockID::stone_bricks))
		{
			const blockID oldBlock = containerIn->getBlockID(worldPos);
			if (oldBlock == blockID::water)
			{
				if (randFp(*randomToUse) < mossyChance)
				{
					if (blockToPlace == blockID::cobblestone)
					{
						blockToPlace = blockID::mossy_cobblestone;
					}
					else if (blockToPlace == blockID::stone_bricks)
					{
						blockToPlace = blockID::mossy_stone_bricks;
					}
				}
			}
		}

		if ((blockToPlace == blockID::air) && (cobWebChance > 0))
		{
			if (randFp(*randomToUse) < cobWebChance)
			{
				blockToPlace = blockID::cobweb;
			}
		}
	}
	return structureFeature::getBlockToPlace(containerIn, worldPos, blockToPlace, data);
}