#pragma once
#include "block.h"
struct doubleBlock :public block
{
	resolutionTexture* part0Tex = nullptr;
	resolutionTexture* part1Tex = nullptr;
	veci2 part1RelativeLocation = veci2();
	virtual void render(const gameRenderData& targetData, blockData* const data, blockContainer* containerIn, cveci2& blockPosition) const override;
	doubleBlock(const block& b, resolutionTexture* const& part0Tex, resolutionTexture* const& part1Tex, cveci2& part1RelativeLocation = veci2()) :
		block(b), part0Tex(part0Tex), part1Tex(part1Tex), part1RelativeLocation(part1RelativeLocation) {}
};