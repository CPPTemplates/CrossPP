#include "endCrystal.h"
#include "math/bodypart2d.h"
#include "dimension.h"
#include "enderDragon.h"
#include "textureList.h"
#include "math/bodypart2d.h"
#include "world.h"

endCrystal::endCrystal(dimension* dimensionIn, cvec2& position) :entity(dimensionIn, position, entityID::end_crystal)
{
	cvec2 outerSquareSize = cvec2(endCrystalOuterSquareTextureRect.size) * endCrystalPixelSize;
	outerPart = new bodyPart2D(endCrystalOuterSquareTextureRect, nullptr, vec2(), outerSquareSize, outerSquareSize * 0.5, 0, 0, true);
	cvec2 innerSquareSize = cvec2(endCrystalInnerSquareTextureRect.size) * endCrystalPixelSize;
	innerPart = new bodyPart2D(endCrystalInnerSquareTextureRect, outerPart, vec2(), innerSquareSize, innerSquareSize * 0.5);
	outerPart->children.push_back(innerPart);
	outerPart->drawOrderIndex = outerPart->children.size();
}

void endCrystal::render(const gameRenderData& targetData) const
{
	outerPart->angle = (targetData.getSecondsSinceStart(currentWorld->ticksSinceStart) / 8) * math::PI2;

	//two times as fast
	innerPart->angle = (targetData.getSecondsSinceStart(currentWorld->ticksSinceStart) / 6) * math::PI2;

	//0.5 to 1.5
	outerPart->translate = position + cvec2(0, math::mapValue((fp)sin(targetData.getSecondsSinceStart(currentWorld->ticksSinceStart) * math::PI2 * 0.5), (fp)-1, (fp)1, endCrystalBaseSize.y + 0.5, endCrystalHitboxSize.y - 0.5));

	outerPart->changed = true;
	innerPart->changed = true;
	outerPart->renderRecursive(targetData.renderTarget, targetData.worldToRenderTargetTransform, *endCrystalTexture);

	cveci2 blockPosFloatingOn = cveci2((int)floor(position.x), (int)floor(position.y - 1.5));
	if (dimensionIn->getBlockID(blockPosFloatingOn) == blockID::bedrock)
	{
		crectangle2 blockRect = targetData.worldToRenderTargetTransform.multRectMatrix(crectangle2(blockPosFloatingOn.x, blockPosFloatingOn.y, endCrystalBaseSize.x, endCrystalBaseSize.y));
		fillTransformedBrushRectangle(crectangle2(endCrystalBaseTextureRect), blockRect, targetData.worldToRenderTargetTransform, *endCrystalTexture, targetData.renderTarget);
	}
}

void endCrystal::explode()
{
	dimensionIn->createExplosion(position, 6);
	despawn = true;

	//seek nearby dragons
	std::vector<entity*> nearByEntities = dimensionIn->findNearEntities(position, dragonNoticeDistance);

	for (size_t i = 0; i < nearByEntities.size(); i++)
	{
		if (nearByEntities[i]->entityType == entityID::ender_dragon)
		{
			//let them become mad at the player
			((enderDragon*)nearByEntities[i])->endCrystalBroken = true;
		}
	}
}