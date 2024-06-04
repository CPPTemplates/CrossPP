#include "cropData.h"
#include "cropBlock.h"
#include "gameRenderData.h"
#include "math/graphics/resolutiontexture.h"
#include <memory>
#include "tickableBlockContainer.h"
#include "include/math/graphics/brush/brushes.h"
void cropBlock::render(const gameRenderData& targetData, blockData* const data, blockContainer* containerIn, cveci2& blockPosition) const
{
	if (identifier == blockID::chorus_flower)
	{
		renderCollisionData(containerIn, blockPosition, *(growthStageTextures)[dynamic_cast<cropData*>(data)->age], targetData);
	}
	else
	{
		const auto& cropTex = *(growthStageTextures)[dynamic_cast<cropData*>(data)->age];
		fillTransparentTexture(getTextureToWorldTransform(cropTex.defaultSize, targetData.worldToRenderTargetTransform, blockPosition), cropTex, targetData.renderTarget);
	}
}

bool cropBlock::canPlace(tickableBlockContainer* containerIn, cveci2 position)
{
	//check if the block below is farmland
	blockID below = containerIn->getBlockID(position + cveci2(0, -1));
	if (identifier == blockID::nether_wart)
	{
		return below == blockID::soul_sand;
	}
	else
	{
		if (below != blockID::farmland)
		{
			return false;
		}
		//can plant at night
		return containerIn->getMaximumInternalLightLevel(position) > glowInTheDarkLightLevel;
	}
}

cropBlock::~cropBlock()
{
}