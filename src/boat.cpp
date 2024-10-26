#include "boat.h"
#include "lootTable.h"
#include "textureList.h"
#include "itemStack.h"
#include "dimension.h"
#include "math/bodypart2d.h"
#include "block.h"
#include "nbt/nbtSerializer.h"

fp boat::getPaddleAngle(cfp& paddleRotations)
{
	cfp maxPaddleAngle = 30;
	return sin(paddleRotations * math::PI2) * maxPaddleAngle * math::degreesToRadians;
}

boat::boat(dimension* dimensionIn, cvec2& position) : hollowRidableEntity(dimensionIn, position, entityID::boat, 2)
{

	//boat texture
	constexpr rectangle2 rightPaddleTextureRect = crectangle2(140, 102, 14, 12);
	constexpr rectangle2 leftPaddleTextureRect = crectangle2(140, 62, 14, 12);
	constexpr rectangle2 rightPaddleHandleTextureRect = crectangle2(164, 88, 36, 4);
	constexpr rectangle2 leftPaddleHandleTextureRect = crectangle2(164, 48, 36, 4);
	constexpr rectangle2 boatTextureRect = crectangle2(4, 26, 56, 0x10);
	cvec2 handleOffset = cvec2(0, boatHitboxSize.y);

	cfp& boatScale = boatHitboxSize.x / (fp)boatTextureRect.w;

	mainBodyPart = new bodyPart2D(boatTextureRect, nullptr, vec2(position.x, position.y), vec2(boatHitboxSize.x, boatScale * boatTextureRect.h), vec2(boatHitboxSize.x * 0.5, 0), 0, 180);

	cvec2 paddleHandleSize = cvec2(rightPaddleHandleTextureRect.h * boatScale, rightPaddleHandleTextureRect.w * boatScale);
	cvec2 paddleHandleRotationCentre = cvec2(paddleHandleSize.x * 0.5, paddleHandleSize.y - paddleHandleSize.x * 0.5);

	rightPaddleHandle = new bodyPart2D(rightPaddleHandleTextureRect, mainBodyPart, handleOffset, paddleHandleSize, paddleHandleRotationCentre, 0, 90);
	leftPaddleHandle = new bodyPart2D(leftPaddleHandleTextureRect, mainBodyPart, handleOffset, paddleHandleSize, paddleHandleRotationCentre, 0, 90);

	cvec2 paddleSize = (cvec2)rightPaddleTextureRect.size * boatScale;
	cvec2 paddleRotationCentre = cvec2(paddleSize.x * 0.5, paddleSize.y);
	cvec2 paddleOffset = cvec2(0, -paddleHandleRotationCentre.y);

	bodyPart2D* rightPaddle = new bodyPart2D(rightPaddleTextureRect, rightPaddleHandle, paddleOffset, paddleSize, paddleRotationCentre);
	bodyPart2D* leftPaddle = new bodyPart2D(leftPaddleTextureRect, leftPaddleHandle, paddleOffset, paddleSize, paddleRotationCentre);

	//add bodyparts
	//ordered by draw order
	//left parts at the back, right parts at front
	mainBodyPart->children.push_back(leftPaddleHandle);
	leftPaddleHandle->children.push_back(leftPaddle);
	mainBodyPart->drawOrderIndex = 1;
	mainBodyPart->children.push_back(rightPaddleHandle);
	rightPaddleHandle->children.push_back(rightPaddle);
}

void boat::render(const gameRenderData& targetData) const
{
	//paddle
	cfp paddleRotation = getPaddleAngle(paddleRotations);
	rightPaddleHandle->angle = paddleRotation;
	leftPaddleHandle->angle = paddleRotation;

	mat3x3 boatTransform = targetData.worldToRenderTargetTransform;

	if (speed.x < 0)
	{
		boatTransform = mat3x3::cross(boatTransform, mat3x3::mirror(axisID::x, position.x));
	}
	//the boat itself

	leftPaddleHandle->changed = true;
	rightPaddleHandle->changed = true;
	mainBodyPart->translate = position;
	mainBodyPart->changed = true;
	mainBodyPart->renderRecursive(targetData.renderTarget, boatTransform, *boatTextures[(int)boatType]);
}

void boat::tick()
{
	heal(0.1);
	ridableEntity::tick();
}

void boat::onDeath()
{
	ridableEntity::onDeath();
	//drop as item
	lootTable::dropLoot({ itemStack((itemID)((int)itemID::wood_boat + (int)boatType), 1) }, dimensionIn, position, maxFloatingSlotSpeed);
}

void boat::serializeValue(nbtSerializer& s)
{
	ridableEntity::serializeValue(s);
	s.serializeValue(std::wstring(L"paddle rotations"), paddleRotations);
	s.serializeValue(std::wstring(L"boat type"), boatType);
}

vec2 boat::getRelativeSeatPosition(csize_t& seatIndex) const
{
	bool flipped = speed.x < 0;

	vec2 frontPosition = vec2(boatHitboxSize.x * 0.25, boatHitboxSize.y * 0.5);
	if (flipped == (seatIndex == 1))
	{
		frontPosition.x = -frontPosition.x;
	}
	return frontPosition;
}

fp boat::getHollowVolume() const
{
	return getVolume() * 0.9;
}

fp boat::getWeightPerCubicMeter() const
{
	return blockList[getTreeBlock(boatType, treeItemTypeID::planks)]->weightPerCubicMeter;
}