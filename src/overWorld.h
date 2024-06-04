#pragma once
#include "dimension.h"
struct layerNoiseSimplex;
template <typename t>
struct shuffleRandom;
struct overWorld :dimension
{
	overWorld();

	layerNoiseSimplex* caveNoise = nullptr;

	struct shuffleRandom<uint>* biomeCoordinatesRandom[2]{};

	layerNoiseSimplex* cloudThicknessNoise = nullptr;

	layerNoiseSimplex* windSpeedNoise = nullptr;
	layerNoiseSimplex* windDirectionNoise[2]{};

	virtual generationData* generateTerrain(chunk& generateIn) override;
	virtual void generateStructures(chunk& generateIn) override;
	virtual void renderSky(crectangle2& blockRect, crectangle2& drawRect, const gameRenderData& targetData) const override;
	virtual biomeID getBiome(cvec2& position) const override;
	fp getTemperature(cvec2& position) const;
	vec2 getBiomeTextureCoords(cfp& x) const;
	virtual ~overWorld() override;

	virtual void initialize() override;
	static biomeID getBiome(cfp& elevation, cfp& temperature, cfp& humidity) noexcept;


	virtual vec2 getWindSpeed(cvec2& position) override;
};