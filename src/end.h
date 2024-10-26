#pragma once
#include "math/LayerNoiseSimplex.h"
#include "dimension.h"
constexpr size_t obsidianPillarCount = 2;
struct end :dimension
{
	layerNoiseSimplex* terrainNoise = nullptr;

	bool dragonAlive = true;

	int exitPortalLevel = 0;
	int obsidianPillarLevel[obsidianPillarCount] = {};

	int obsidianPillarHeight[obsidianPillarCount] = {};
	int obsidianPillarRadius[obsidianPillarCount] = {};

	end();
	virtual generationData* generateTerrain(chunk& generateIn) override;
	virtual void generateStructures(chunk& generateIn) override;
	virtual void serializeValue(nbtSerializer& s) override;
	virtual void renderSky(crectangle2& blockRect, crectangle2& drawRect, const gameRenderData& targetData) const override;
	virtual biomeID getBiome(cvec2& position) const final;
	virtual void initialize() override;
	virtual ~end() override;
	virtual vec2 getWindSpeed(cvec2& position) override;
	fp getThickness(cvec2& position) const;
};