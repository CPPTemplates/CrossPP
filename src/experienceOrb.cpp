#include "experienceOrb.h"
#include "dimension.h"
#include "textureList.h"
#include "math/graphics/brush/brushes/colorMultiplier.h"
#include "nbt/nbtSerializer.h"
#include "math/graphics/brush/brushes.h"

fp getExperienceLevel(cfp& experience)
{
	cfp levelCalc1 = -3.0 + sqrt(experience + 9);
	cfp levelCalc2 = (81 + sqrt(40 * experience - 7839)) / 10.0;
	cfp levelCalc3 = (325 + sqrt(72 * experience - 54215)) / 18.0;
	return levelCalc1 < experienceCalculation0Treshold ? levelCalc1 :
		levelCalc2 < experienceCalculation1Treshold ? levelCalc2 : levelCalc3;
}
fp getExperience(cfp& level)
{
	return level < experienceCalculation0Treshold ?
		math::squared(level) + 6 * level :
		level < experienceCalculation1Treshold ?
		2.5 * math::squared(level) - 40.5 * level + 360 :
		4.5 * math::squared(level) - 162.5 * level + 2220;
}
std::vector<int> divideExperience(int amount)
{
	std::vector<int> amounts = std::vector<int>();
	int spawnedAmount = 0;
	int sizeIndex = experienceSizeCount - 1;
	while (spawnedAmount < amount)
	{
		int partAmount;
		do {
			partAmount = sizeIndex ? experienceSizeTable[sizeIndex] :
				math::minimum(experienceSizeTable[sizeIndex], amount - spawnedAmount);
			if (spawnedAmount + partAmount > amount)
			{
				sizeIndex--;
			}
		} while (spawnedAmount + partAmount > amount);
		amounts.push_back(partAmount);
		spawnedAmount += partAmount;
	}
	return amounts;
}
int getExperienceTableIndex(cint& amount)
{
	for (int i = 0; i < experienceSizeCount; i++)
	{
		if (amount < experienceSizeTable[i])
		{
			return i;
		}
	}
	return -1;
}
void dropExperienceOrbs(tickableBlockContainer * containerIn, cvec2& position, cint& amount)
{
	std::vector<int> experienceTable = divideExperience(amount);
	for (int i : experienceTable)
	{
		experienceOrb* orb = (experienceOrb*)summonEntity(entityID::experience_orb, containerIn, position - relativeExperienceHitbox.pos0 - vec2(relativeExperienceHitbox.size.x, 0.0));
		orb->value = i;
		orb->speed = vec2::getrotatedvector(randFp(currentRandom, math::PI2)) * maxExperienceSpeed;
	}
}
experienceOrb::experienceOrb(dimension* dimensionIn, cvec2& pos) : entity(dimensionIn, pos, entityID::experience_orb)
{
}

void experienceOrb::tick()
{
	//despawns after 5 minutes
	if (ticksFloating++ >= floatingSlotDespawnTicks)
	{
		despawn = true;
	}
	entity::tick();
}

void experienceOrb::render(const gameRenderData& targetData) const
{
	cint experienceTableIndex = getExperienceTableIndex(value);
	//calculate draw offset
	cveci2& off = veci2(experienceTableIndex % 4, experienceTableIndex / 4);
	cvec2& experienceTextureSize = experienceTexture->defaultSize / 4;
	crectangle2& textureRect = rectangle2(off * experienceTextureSize, experienceTextureSize);

	//0: yellow, 10: green
	cint transitionPart = ticksFloating % ticksPerRealLifeSecond;
	colorChannel redAmount = (colorChannel)(abs(transitionPart / 10.0 - 1) * color::maxValue);

	const color currentColor = color(redAmount, color::maxValue, 0);
	const solidColorBrush& b = solidColorBrush(currentColor);
	const auto& multiplier = colorMultiplier<resolutionTexture, solidColorBrush>(*experienceTexture, b);

	fillTransparentRectangle(textureRect, this->calculateHitBox(), targetData.worldToRenderTargetTransform, multiplier, targetData.renderTarget);

	entity::render(targetData);
}

void experienceOrb::serializeValue(nbtSerializer& s)
{
	entity::serializeValue(s);
	s.serializeValue(std::wstring(L"value"), value);
	s.serializeValue(std::wstring(L"ticks floating"), ticksFloating);
}