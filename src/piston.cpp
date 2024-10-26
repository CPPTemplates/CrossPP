#include "piston.h"
#include "movableBlockContainer.h"
#include "block.h"
#include "nbt/serializeColor.h"
piston::piston(dimension* dimensionIn, cvec2& position) : entity(dimensionIn, position, entityID::piston)
{
}

void piston::tick()
{
	if (!despawn)
	{
		movingBlocks->tick();
	}
}

void piston::render(const gameRenderData& targetData) const
{
	cmat3x3& multipliedTransform = mat3x3::cross(targetData.worldToRenderTargetTransform, movingBlocks->containerToRootTransform);
	cmat3x3& screenToBlocks = multipliedTransform.inverse();

	crectangle2& blockRect = screenToBlocks.multRectMatrix(crectangle2(targetData.renderTarget.getClientRect()));
	cveci2& unCroppedPos00 = floorVector(blockRect.pos0);
	crectanglei2& flooredBlockRect = crectanglei2(unCroppedPos00, floorVector((blockRect.pos1()) + 1) - unCroppedPos00);
	crectanglei2& croppedBlockRect = crectanglei2(movingBlocks->arrayPos00Offset, veci2(movingBlocks->blockIDArray.size)).cropClientRectUnsafe(flooredBlockRect);
	cveci2& pos11 = croppedBlockRect.pos1();
	const gameRenderData& incrementedRenderData = gameRenderData(multipliedTransform, targetData.renderTarget, targetData.screen, targetData.secondsOffset);
	for (veci2 currentPosition = croppedBlockRect.pos0; currentPosition.y < pos11.y; currentPosition.y++)
	{
		for (currentPosition.x = croppedBlockRect.x; currentPosition.x < pos11.x; currentPosition.x++)
		{
			crectangle2& transformedRect = multipliedTransform.multRectMatrix(crectangle2(currentPosition, cveci2(1)));
			movingBlocks->getBlock(currentPosition)->render(incrementedRenderData, movingBlocks->getBlockData(currentPosition), movingBlocks, currentPosition);
		}
	}
}
piston::~piston()
{
	delete movingBlocks;
}

fp piston::getGravityForce() const
{
	return 0;
}

void piston::serializeValue(nbtSerializer& s)
{
	serializeNBTValue(s, std::wstring(L"connected piston position"), connectedPistonPosition);
	if (!s.write)
	{
		movingBlocks = new movableBlockContainer(veci2());
	}
	if (s.push(L"moving blocks"))
	{
		movingBlocks->serializeValue(s);
		s.pop();
	}
	entity::serializeValue(s);
}
