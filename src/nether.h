#pragma once
#include "dimension.h"
#include "math/LayerNoiseSimplex.h"

struct nether :dimension
{
	layerNoiseSimplex* terrainNoise = nullptr;
	layerNoiseSimplex* biomeLocationNoise = nullptr;
	layerNoiseSimplex* biomeHumidityNoise = nullptr;
	nether();
	virtual generationData* generateTerrain(chunk& generateIn) override;
	virtual void generateStructures(chunk& generateIn) override;
	virtual void renderSky(crectangle2& blockRect, crectangle2& drawRect, const gameRenderData& targetData) const override;
	virtual biomeID getBiome(cvec2& position) const override;
	virtual void initialize() override;
	virtual ~nether() override;
	virtual vec2 getWindSpeed(cvec2& position) override;
};