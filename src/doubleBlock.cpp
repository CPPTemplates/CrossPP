#include "doubleBlock.h"
#include "doubleBlockData.h"
#include "doorData.h"
#include "renderBrush.h"
void doubleBlock::render(const gameRenderData& targetData, blockData* const data, blockContainer* containerIn, cveci2& blockPosition) const
{
	const resolutionTexture& blockPartTexture = (dynamic_cast<doubleBlockData*>(data))->isPart0 ? *part0Tex : *part1Tex;
	if (isDoor(identifier))
	{
		renderCollisionData(containerIn, blockPosition, blockPartTexture, targetData);
		//if ((dynamic_cast<doorData*>(data))->isOpen)
		//{
		//	fillTransparentRectangle(crectangle2(blockPartTexture.getClientRect()), getTextureToWorldTransform(blockPartTexture.size, targetData.worldToRenderTargetTransform, blockPosition), blockPartTexture, targetData.renderTarget)
		//}
		//else
		//{
		//	directionID facing = (dynamic_cast<doorData*>(data))->directionFacing;
		//	rectangle2 textureRect;
		//	rectangle2 partialDrawRect;
		//	cfp openDoorDrawWidth = drawRect.w * (fp)openDoorDrawPixels / blockTextureSize;
		//	if (facing == directionID::negativeX)
		//	{
		//		partialDrawRect = rectangle2(drawRect.x, drawRect.y, openDoorDrawWidth, drawRect.h);
		//		textureRect = rectangle2(0, 0, openDoorDrawPixels, blockPartTexture.size.y);
		//	}
		//	else
		//	{
		//		partialDrawRect = rectangle2(drawRect.x + drawRect.w - openDoorDrawWidth, drawRect.y, openDoorDrawWidth, drawRect.h);
		//		textureRect = rectangle2(blockPartTexture.size.x - openDoorDrawPixels, 0, openDoorDrawPixels, blockPartTexture.size.y);
		//	}
		//	renderBrush(textureRect, partialDrawRect, true, directionID::positiveY, false, blockPartTexture, targetData.renderTarget);
		//}
	}
	else
	{
		block::render(blockPartTexture, crectangle2(0, 0, blockPartTexture.defaultSize.x, blockPartTexture.defaultSize.y), crectangle2(cvec2(blockPosition), cvec2(1)), targetData, data, containerIn);
	}
}