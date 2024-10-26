#include "lootTable.h"
#include "textureList.h"
#include "sheep.h"
#include "eatGrassAI.h"
#include "taskList.h"
#include "colors.h"
#include "dimension.h"
#include "math/graphics/brush/brushes/colorMultiplier.h"
sheep::sheep(dimension* dimensionIn, cvec2& position) :mob(dimensionIn, position, entityID::sheep), fourLeggedPassiveMob(dimensionIn, position, entityID::sheep)
{
	((taskList*)tasks)->addTask((int)((taskList*)tasks)->tasks.size() - 1, new eatGrassAI(this));
	hasWool = true;
	cfp value = randFp(currentRandom);
	if (value < 0.82)
	{
		woolColor = colorID::white;
	}
	else if (value < 0.87)
	{
		woolColor = colorID::black;
	}
	else if (value < 0.92)
	{
		woolColor = colorID::gray;
	}
	else if (value < 0.97)
	{
		woolColor = colorID::lightGray;
	}
	else if (value < 0.998)
	{
		woolColor = colorID::brown;
	}
	else
	{
		woolColor = colorID::pink;
	}

	//intialize body parts
	mainBodyPart = new bodyPart2D(sheepBodyTextureRect, nullptr, vec2(position.x, position.y + sheepLegSize.y), sheepBodySize, sheepBodyRotationCentre, 0, 90);

	frontRightLeg = new bodyPart2D(sheepFrontRightLegTextureRect, mainBodyPart, sheepFrontLegOffset, sheepLegSize, sheepLegRotationCentre);
	frontLeftLeg = new bodyPart2D(sheepFrontLeftLegTextureRect, mainBodyPart, sheepFrontLegOffset, sheepLegSize, sheepLegRotationCentre);

	backRightLeg = new bodyPart2D(sheepFrontRightLegTextureRect, mainBodyPart, sheepBackLegOffset, sheepLegSize, sheepLegRotationCentre);
	backLeftLeg = new bodyPart2D(sheepFrontLeftLegTextureRect, mainBodyPart, sheepBackLegOffset, sheepLegSize, sheepLegRotationCentre);

	head = new bodyPart2D(sheepHeadTextureRect, mainBodyPart, vec2(sheepBodySize.x - sheepBodyRotationCentre.x - sheepHeadOffset + sheepHeadRotationCentre.x, sheepBodySize.y + sheepHeadOffset - sheepHeadRotationCentre.y), sheepHeadSize, sheepHeadRotationCentre);
	connectBodyParts();
}
void sheep::serializeValue(nbtSerializer& s)
{
	mob::serializeValue(s);

	s.serializeValue(std::wstring(L"has wool"), hasWool);
	s.serializeValue(std::wstring(L"wool color"), woolColor);
}
void sheep::render(const gameRenderData& targetData) const
{
	fourLeggedMob::render(targetData);

	if (hasWool)
	{
		//from bottom to top
		cint bodyPartCount = 6;
		bodyPart2D* parts[bodyPartCount]
		{
			backLeftLeg,
			frontLeftLeg,

			mainBodyPart,
			head,

			backRightLeg,
			frontRightLeg
		};

		cint drawIndexes[bodyPartCount]
		{
			0,0, 2, 3,1, 1
		};

		cint bodyPartTypeCount = 4;
		//just the skin rects, but for wool
		crectanglei2 textureRects[bodyPartTypeCount]
		{
			crectanglei2(0, 6, 4, 6),
			crectanglei2(8, 6, 4, 6),
			crectanglei2(42, 2, 6, 16),
			crectanglei2(0, 20, 6, 6)
		};

		crectangle2 legRect = crectangle2(-backLeftLeg->rotationCentre + cvec2(0.0, backLeftLeg->size.y * 0.5), cvec2(backLeftLeg->size.x, backLeftLeg->size.y * 0.5));
		cfp thickness = 0.03;
		crectangle2 bodyPartRects[bodyPartTypeCount]
		{
			legRect,
			legRect,
			crectangle2(-mainBodyPart->rotationCentre,mainBodyPart->size).expanded(thickness),
			//only fill the square part
			crectangle2(-head->rotationCentre,cvec2(head->size.y,head->size.y)).expanded(thickness),
		};

		const auto& woolColorBrush = solidColorBrush(rgbColorValues[(int)woolColor]);
		const auto& woolBrush = colorMultiplier<resolutionTexture, solidColorBrush>(*woolOverlay, woolColorBrush);

		for (int i = 0; i < bodyPartCount; i++)
		{
			cint drawIndex = drawIndexes[i];
			bodyPart2D* b = parts[i];
			crectanglei2 textureRect = textureRects[drawIndex];

			cmat3x3 woolTransform = mat3x3::combine({ mat3x3::fromRectToRect((crectangle2)textureRect, bodyPartRects[drawIndex].expanded(armorExpansion)),b->getCumulativeTransform() ,targetData.worldToRenderTargetTransform });
			//expand 0.1f
			//const mat3x3 armorTransform = mat
			fillTransparentRectangle((crectangle2)textureRect, woolTransform, woolBrush, targetData.renderTarget);
		}
	}
}
void sheep::onDeath()
{
	passiveMob::onDeath();
	if (hasWool)
	{
		//drop wool depending on color
		lootTable::dropLoot({ itemStack((itemID)((int)blockID::wool + (int)woolColor), 1) }, dimensionIn, position, maxFloatingSlotSpeed);
	}
}