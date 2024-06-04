#pragma once
#include "particleBrush.h"
#include "math/graphics/resolutiontexture.h"
#include "math/rectangle/rectangletn.h"
#include "particleID.h"
#include "gameRenderData.h"
struct textureParticleBrush : particleBrush
{
	size_t brushIndex = 0;
	virtual void render(crectangle2& rect, const gameRenderData& targetData) const override;
	const resolutionTexture& getTexture() const;
	textureParticleBrush(const particleID& particleType) : particleBrush(particleType){}
};
