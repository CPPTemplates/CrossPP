#include "netherBiomeGenerator.h"
#include "overWorldOreList.h"
#include "netherOreList.h"
#include "nether.h"
#include "netherGenerationData.h"
#include "chunk.h"
#include "world.h"
#include "gameControl.h"
nether::nether() : dimension(dimensionID::nether)
{

}

generationData* nether::generateTerrain(chunk& generateIn)
{
	netherGenerationData* data = new netherGenerationData();
	transition<fp> thicknessAt = transition<fp>(std::vector<keyFrame<fp>>({
			keyFrame<fp>(netherLavaOceanBedEnd,0.6),
			keyFrame<fp>(netherLavaOceanBottomLevel,1),
			keyFrame<fp>(netherLavaLevel,0.5),
			keyFrame<fp>(netherCeilingStart,0.25),
			keyFrame<fp>(netherCeilingEnd, 0.6)
		}));


	crectanglei2& rect = crectanglei2(generateIn.worldPos, (veci2)chunkSize);
	for (cveci2& pos : rect)
	{
		cfp thickness = thicknessAt.getValue(pos.y);
		cfp noiseValue = terrainNoise->evaluate(cvec2(pos));

		if (noiseValue < thickness)
		{
			setBlockID(pos, blockID::netherrack);
		}
		else if (pos.y < netherLavaLevel && pos.y >= netherLavaOceanBottomLevel)
		{
			setBlockID(pos, blockID::lava);
		}
	}
	return data;
}

void nether::generateStructures(chunk& generateIn)
{
	netherGenerationData* data = (netherGenerationData*)generateIn.terrainData;
	cfp rarity[netherOreTypeCount]
	{
		0x10 * chunkOreMultiplier,
		10 * chunkOreMultiplier,
		4 * chunkOreMultiplier,
		2 * chunkOreMultiplier
	};
	cint maxVeinSize[overWorldOreTypeCount]
	{
		14,
		10,
		33,
		3
	};

	//Scan for ceiling and ground positions

	std::vector<veci2> ceilingPositions = std::vector<veci2>();

	crectanglei2& chunkRect = crectanglei2(generateIn.worldPos, (veci2)chunkSize);
	for (cveci2& pos : chunkRect)
	{
		if (getBlockID(pos) == blockID::netherrack)
		{
			//on chunk borders, this could change if something is already placed, but it is only a small difference
			if (getBlockID(pos + cveci2(0, 1)) == blockID::air)
			{
				data->groundPositions.push_back(pos + cveci2(0, 1));
			}

			if (getBlockID(pos + cveci2(0, -1)) == blockID::air)
			{
				ceilingPositions.push_back(pos);
			}
		}
	}

	for (int i = 0; i < netherOreTypeCount; i++)
	{
		const blockID currentOre = netherOreList[i];
		csize_t& count = roundRandom(generateIn.chunkRandom, rarity[i]);
		cveci2& depositPosition = generateIn.worldPos + veci2(randIndex(generateIn.chunkRandom, (int)chunkSize.x), randIndex(generateIn.chunkRandom, (int)chunkSize.y));
		for (size_t j = 0; j < count; j++)
		{
			addOres(depositPosition, currentOre, rand(generateIn.chunkRandom, maxVeinSize[i]), generateIn.chunkRandom, { blockID::netherrack });
		}
	}

	//for the soul sand to not fall down
	for (cveci2 pos : ceilingPositions)
	{
		((netherBiomeGenerator*)currentWorld->biomeList[(int)getBiome(pos)])->attemptgenerateCeilingStructures(this, pos, generateIn.chunkRandom);
	}

	for (cveci2 pos : data->groundPositions)
	{
		currentWorld->biomeList[(int)getBiome(pos)]->attemptgenerateStructures(this, pos, generateIn.chunkRandom);
	}
}

void nether::renderSky(crectangle2& blockRect, crectangle2 & drawRect, const gameRenderData& targetData) const
{
	//TODO: stop the biome colors from looking like cubes
	cfp& value = biomeLocationNoise->evaluate(blockRect.getCenter());
	cfp& humidity = biomeHumidityNoise->evaluate(blockRect.getCenter());

	//0 to 1 will be converted to 0 to 0xff
	//BGR
	const color& skyColor = color(colorf(value, math::maximum(((value + (1 - humidity)) - 1), (fp)0), 1 - value));

	fillRectangle(targetData.renderTarget, ceilRectangle(drawRect), colorMixer(targetData.renderTarget, solidColorBrush(skyColor)));
}

biomeID nether::getBiome(cvec2& position) const
{
	cfp value = biomeLocationNoise->evaluate(position);
	cfp humidity = biomeHumidityNoise->evaluate(position);

	if (humidity < 0.6)
	{
		if (value > 0.6)
		{
			return biomeID::soulsand_valley;
		}
		if (humidity < 0.3)
		{
			if (value > 0.4)
			{
				return biomeID::basalt_deltas;
			}
		}
		return biomeID::nether_wastes;
	}
	else
	{
		if (value < 0.5)
		{
			return biomeID::crimson_forest;
		}
		else
		{
			return biomeID::warped_forest;
		}
	}
}

void nether::initialize()
{
	const std::vector<fp>& octaveWeights
	{
		6,
		5,
		4,
		3,
		2,
		1,
	};

	cfp noiseHillWidth = 0x800;

	const std::vector<vec3>& octaveScales
	{
		{noiseHillWidth, noiseHillWidth / 0b100},
		{noiseHillWidth / 0b10, noiseHillWidth / 0b1000},
		{noiseHillWidth / 0b100, noiseHillWidth / 0b10000},
		{noiseHillWidth / 0b1000, noiseHillWidth / 0b100000},
		{noiseHillWidth / 0b10000, noiseHillWidth / 0b1000000},
		{noiseHillWidth / 0b100000, noiseHillWidth / 0b10000000},
	};


	terrainNoise = new layerNoiseSimplex(worldRandom, octaveWeights, octaveScales);

	std::vector<fp> biomeLocationNoiseOctaveWeights
	{
		1
	};

	cfp biomeHumidityNoiseHillWidth = 0x400;
	cfp biomeLocationNoiseHillWidth = 0x200;

	biomeLocationNoise = new layerNoiseSimplex(worldRandom, biomeLocationNoiseOctaveWeights, biomeLocationNoiseHillWidth, crectangle1::fromOppositeCorners(cvec1(0), cvec1(1)));

	std::vector<fp> biomeHumidityNoiseOctaveWeights
	{
		1
	};

	biomeHumidityNoise = new layerNoiseSimplex(worldRandom, biomeHumidityNoiseOctaveWeights, biomeHumidityNoiseHillWidth, crectangle1::fromOppositeCorners(cvec1(0), cvec1(1)));
}

nether::~nether()
{

	if (terrainNoise)
	{

		delete terrainNoise;
	}
}

vec2 nether::getWindSpeed(cvec2& position)
{
	return vec2();
}

