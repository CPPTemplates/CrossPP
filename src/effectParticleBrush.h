#pragma once
#include "textureParticleBrush.h"
struct effectParticleBrush : textureParticleBrush {
	color effectColor;
	effectParticleBrush(const color& effectColor) : textureParticleBrush(particleID::effect), effectColor(effectColor)
	{
	}
	virtual void render(crectangle2& rect, const gameRenderData& targetData) const override;
};