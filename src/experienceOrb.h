#pragma once
#include "entity.h"
#include <limits.h>

//experience
constexpr fp experienceSize = 0.2;
constexpr rectangle2 relativeExperienceHitbox = crectangle2(vec2(experienceSize * -0.5), vec2(experienceSize));

struct experienceOrb : public entity
{
	int value = 0;
	int ticksFloating = 0;
	experienceOrb(dimension* dimensionIn, cvec2& pos);
	virtual void tick() override;
	virtual void render(const gameRenderData& targetData) const override;
	virtual void serializeValue(nbtSerializer& s) override;
};

constexpr int experienceSizeCount = 11;
constexpr int experienceSizeTable[experienceSizeCount]
{
	2,
	6,
	16,
	36,
	72,
	148,
	306,
	616,
	1236,
	2476,
	INT_MAX
};

fp getExperience(cfp& level);
fp getExperienceLevel(cfp& experience);
std::vector<int> divideExperience(int amount);
int getExperienceTableIndex(cint& amount);
void dropExperienceOrbs(tickableBlockContainer * containerIn, cvec2& position, cint& amount);