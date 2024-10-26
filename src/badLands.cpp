
#include "math/hashing.h"
#include "badLands.h"
#include "dimension.h"
#include "world.h"
#include "math/random/random.h"
badlands::badlands() : overWorldBiomeGenerator(biomeID::badlands)
{
	std::vector<fp> octaveWeights
	{
		//big scale noise: 40%
		0.8,
		//small scale noise: 60%
		0.2,
	};
	cfp averageHillWidth = 0x80;
	elevationNoise = new layerNoiseSimplex(worldRandom, octaveWeights, averageHillWidth, crectangle1::fromOppositeCorners(cvec1(0), cvec1(0x30)));

	std::vector<fp> carrotOctaveWeights
	{
		//big scale noise: 100%
		1,
		//small scale noise: 0%
	};
	cfp averageNoiseHillWidth = 1;
	terracottaNoise = new layerNoiseSimplex(worldRandom, carrotOctaveWeights, averageNoiseHillWidth, crectangle1::fromOppositeCorners(cvec1(0), cvec1(terracottaNoiseInfluence)));
}

void badlands::attemptgenerateStructures(dimension* dimensionIn, cveci2& pos, std::mt19937& randomToUse) const
{
	constexpr fp elevationInfluence = 1.0 / 4;
	constexpr vec2 coordInfluence = cvec2(1.0 / 0x100, 1.0 / 4);
	constexpr vec2 noiseStretch = coordInfluence / terracottaNoiseInfluence;
	constexpr int redSandHeight = seaLevel + 0x10;

	//fill with colored layers
	cint paletteSize = 0x8;
	const colorID palette[paletteSize]
	{
		colorID::red,
		colorID::orange,
		colorID::yellow,
		colorID::white,
		colorID::lightGray,
		colorID::gray,
		colorID::brown,
		colorID::pink
	};
	for (int height = pos.y - 0x10; height < pos.y; height++)
	{
		blockID blockToPlace;
		if ((height == pos.y - 1) && (height < redSandHeight))
		{
			blockToPlace = blockID::red_sand;
		}
		else
		{
			fp noiseValue = terracottaNoise->evaluate<2>(cvec2(pos.x * noiseStretch.x, height * noiseStretch.y)) + height * elevationInfluence;
			cint seedValue1 = (int)floor(noiseValue);
			cint seedValue2 = (int)currentWorld->seed;
			cint value = (int)perfectlyHashToDoubleSizedType(seedValue1, seedValue2);
			cint paletteIndex = math::mod(value, paletteSize);
			blockToPlace = (blockID)((int)blockID::terracotta + (int)palette[paletteIndex]);
		}
		if (dimensionIn->replaceBlock(cveci2(pos.x, height), blockToPlace, { blockID::stone })) {
			if (blockToPlace == blockID::red_sand && randChance(randomToUse, 0x10))
			{
				dimensionIn->replaceBlock(pos, blockID::dead_bush, { blockID::air });
			}
		}
	}
}

badlands::~badlands()
{

	delete terracottaNoise;
}