#pragma once
#include "block.h"
struct cropBlock :public block
{
	int growthStageCount = 0;
	std::vector<resolutionTexture* > growthStageTextures = std::vector<resolutionTexture* >();
	fp chanceToGrow = 0;

	virtual void render(const gameRenderData& targetData, blockData* const data, blockContainer* containerIn, cveci2& blockPosition) const override;
	bool canPlace(tickableBlockContainer* containerIn, cveci2 position) override;
	virtual ~cropBlock() override;
	cropBlock(const block& b, cint& growthStageCount, const std::vector<resolutionTexture* >& growthStageTextures, cfp& chanceToGrow = 0) :
		block(b), growthStageCount(growthStageCount), growthStageTextures(growthStageTextures), chanceToGrow(chanceToGrow) {}
};
