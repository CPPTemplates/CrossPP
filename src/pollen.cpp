#include "idAnalysis.h"
#include "pollen.h"
#include "dimension.h"
#include "idConverter.h"
#include "math/graphics/brush/brushes/solidColorBrush.h"
#include "include/math/graphics/graphicsFunctions.h"

void pollen::render(const gameRenderData& targetData) const
{
	fillEllipse(targetData.renderTarget, targetData.worldToRenderTargetTransform.multRectMatrix(calculateHitBox()), solidColorBrush(colorPalette::gray));
}

void pollen::tick()
{
	entity::tick();
	if (axisCollided.x || axisCollided.y)
	{
		cveci2& spreadPosition = floorVector(position + cvec2(0, -0.2));

		const blockID& b = dimensionIn->getBlockID(spreadPosition);

		if (canSpreadOn(blockToSpread, b))
		{
			if (isGrassBlock(blockToSpread))
			{
				dimensionIn->setBlockID(spreadPosition, blockToSpread);
			}
			else if (dimensionIn->getBlockID(spreadPosition + cveci2(0, 1)) == blockID::air)
			{
				dimensionIn->setBlockID(spreadPosition + cveci2(0, 1), blockToSpread);
			}
		}

		despawn = true;
	}
	else
	{
		ticksToExist--;
		if (ticksToExist <= 0)
		{
			despawn = true;
		}
	}
}

void pollen::serializeValue(nbtSerializer& s)
{
	idConverter::serializeID(s, std::wstring(L"block to spread"), (itemID&)blockToSpread, s.converter ? &s.converter->itemIDConverter : nullptr);
	s.serializeValue(std::wstring(L"ticks to exist"), ticksToExist);
}