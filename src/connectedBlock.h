#pragma once
#include "block.h"
struct connectedBlock :public block
{
	resolutionTexture* topTexture = nullptr;//the texture of the topmost block
	veci2 topRelativeLocation = veci2();
	virtual void render(const gameRenderData& targetData, blockData* const data, blockContainer* containerIn, cveci2& blockPosition) const override;
	connectedBlock(const block& b, resolutionTexture* const& topTexture, cveci2& topRelativeLocation) :
		block(b), topTexture(topTexture), topRelativeLocation(topRelativeLocation) {}
};