#include "mobData.h"
#include "creeper.h"
#include "taskList.h"
#include "explodeAI.h"
#include "wanderAI.h"
#include "attackOnCollideAI.h"
#include "nbt/nbtSerializer.h"
creeper::creeper(dimension* dimensionIn, cvec2& position) :mob(dimensionIn, position, entityID::creeper), fourLeggedMob(dimensionIn, position, entityID::creeper)
{
	tasks = new taskList(this, {
	new explodeAI(this, entityID::human),
	new attackOnCollideAI(this, entityID::human),
	new wanderAI(this), });

	//intialize body parts
	mainBodyPart = new bodyPart2D(creeperBodyTextureRect, nullptr, vec2(position.x, position.y + creeperLegSize.y), creeperBodySize, creeperBodyRotationCentre);

	frontRightLeg = new bodyPart2D(creeperFrontRightLegTextureRect, mainBodyPart, creeperFrontLegOffset, creeperLegSize, creeperFrontLegRotationCentre);
	frontLeftLeg = new bodyPart2D(creeperFrontLeftLegTextureRect, mainBodyPart, creeperFrontLegOffset, creeperLegSize, creeperFrontLegRotationCentre);

	backRightLeg = new bodyPart2D(creeperFrontRightLegTextureRect, mainBodyPart, creeperBackLegOffset, creeperLegSize, creeperBackLegRotationCentre);
	backLeftLeg = new bodyPart2D(creeperFrontLeftLegTextureRect, mainBodyPart, creeperBackLegOffset, creeperLegSize, creeperBackLegRotationCentre);

	head = new bodyPart2D(creeperHeadTextureRect, mainBodyPart, vec2(0, creeperBodySize.y), creeperHeadSize, creeperHeadRotationCentre);
	connectBodyParts();

}

void creeper::render(const gameRenderData& targetData) const
{
	if ((ticksUntilFuse < creeperFuseLength) && (ticksUntilFuse % ticksPerRealLifeSecond < (ticksPerRealLifeSecond / 2)))
	{
		updateBodyParts();
		//render white
		const solidColorBrush flashColorBrush = solidColorBrush(color(color::halfMaxValue, color::maxValue, color::maxValue, color::maxValue));
		mat3x3 transform = targetData.worldToRenderTargetTransform;//TODO: antialiasing for creeper skin
		auto flashingBrush = colorMixer<solidColorBrush, resolutionTexture>(flashColorBrush, *((mobData*)entityDataList[(int)entityType])->skin);

		renderBodyPart(mainBodyPart, transform, flashingBrush, targetData);
	}
	else
	{
		fourLeggedMob::render(targetData);
	}
}

void creeper::serializeValue(nbtSerializer& s)
{
	fourLeggedMob::serializeValue(s);
	s.serializeValue(std::wstring(L"powered"), powered);
	s.serializeValue(std::wstring(L"explosion strength"), explosionStrength);
	s.serializeValue(std::wstring(L"ticks until fuse"), ticksUntilFuse);
	s.serializeValue(std::wstring(L"ignited"), ignited);
}