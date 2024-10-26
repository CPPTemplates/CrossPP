#pragma once
#include "math/graphics/resolutiontexture.h"
#include "particleBrush.h"
struct rectParticleBrush : particleBrush
{
	rectangle2 brushRect = rectangle2();
	rectParticleBrush(const particleID& particleType, crectangle2& brushRect = crectangle2()) :particleBrush(particleType), brushRect(brushRect) {}
	virtual void serializeValue(nbtSerializer& s) override;
	virtual void render(crectangle2& rect, const gameRenderData& targetData) const override;
	virtual const resolutionTexture& getTexture() const = 0;
};
rectangle2 getParticleBrushRect(const resolutionTexture& tex);