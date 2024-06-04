#include "chunkLoadLevel.h"
#include "itemData.h"
#include "fallingBlock.h"
#include "dimension.h"
#include "block.h"
#include "dropData.h"
#include "idConverter.h"
#include "include/math/graphics/brush/brushes.h"

fallingBlock::fallingBlock(dimension* dimensionIn, cvec2& position, const blockID& linkedBlockID) : entity(dimensionIn, position, entityID::falling_block), linkedBlockID(linkedBlockID)
{
}

void fallingBlock::render(const gameRenderData& targetData) const
{
	crectangle2 rect = targetData.worldToRenderTargetTransform.multRectMatrix(calculateHitBox());

	fillTransparentTexture(rect, *itemList[(int)linkedBlockID]->tex, targetData.renderTarget);
}

void fallingBlock::tick()
{
	if (onGround)
	{
		//try to create a block where it landed
		cveci2 blockPos = floorVector(position);
		block* currentBlock = dimensionIn->getBlock(blockPos, chunkLoadLevel::updateLoaded);
		if (currentBlock->canReplaceBlock || currentBlock->hardness == 0)
		{
			if (currentBlock->hardness != -1)
			{
				dimensionIn->harvestBlock(dropData(), blockPos, chunkLoadLevel::updateLoaded);
			}
			dimensionIn->setBlockID(blockPos, linkedBlockID, chunkLoadLevel::updateLoaded);
		}
		else
		{
			//drop as item
			lootTable::dropLoot({ itemStack((itemID)linkedBlockID, 1) }, dimensionIn, position);
		}
		despawn = true;
	}
}

void fallingBlock::serializeValue(nbtSerializer& s)
{
	entity::serializeValue(s);
	idConverter::serializeID(s, std::wstring(L"linked block"), (itemID&)linkedBlockID, s.converter ? &s.converter->itemIDConverter : nullptr);
}

