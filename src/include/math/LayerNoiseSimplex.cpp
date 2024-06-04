#include "LayerNoiseSimplex.h"
#include <numeric>
#include "math/random/random.h"
constexpr fp MAX_NOISE_OUTPUT = 1;
constexpr fp NOISE_MULT = 1.0f / MAX_NOISE_OUTPUT; // multiply output by this to map from -1 to 1

constexpr int noiseSampleSize = 0x10;
constexpr fp noiseOutputChance[3][noiseSampleSize]{
	{}, // 0.000000, 0.031250, 0.062500, 0.093750, 0.125000, 0.156250, 0.187500, 0.218750, 0.250000, 0.281250, 0.312500, 0.343750, 0.375000, 0.406250, 0.437500, 0.468750, 0.500000, 0.531250, 0.562500, 0.593750, 0.625000, 0.656250, 0.687500, 0.718750, 0.750000, 0.781250, 0.812500, 0.843750, 0.875000, 0.906250, 0.937500, 0.968750},
	{},
	{}};

layerNoiseSimplex::layerNoiseSimplex(std::mt19937 &randomToUse, const std::vector<fp> &octaveWeights, fp noiseScale, crectangle1 &outputRange) : layerNoiseSimplex(randomToUse, octaveWeights, outputRange)
{
	for (size_t i = 0; i < octaveWeights.size(); i++)
	{
		octaveFrequencies[i] = vec3(1.0 / noiseScale);
		noiseScale *= 0.5;
	}
}

layerNoiseSimplex::layerNoiseSimplex(std::mt19937 &randomToUse, const std::vector<fp> &octaveWeights, const std::vector<fp> &noiseScales, crectangle1 &outputRange) : layerNoiseSimplex(randomToUse, octaveWeights, outputRange)
{
	for (size_t i = 0; i < octaveWeights.size(); i++)
	{
		this->octaveFrequencies[i] = cvec3(1.0 / noiseScales[i]);
	}
}

layerNoiseSimplex::layerNoiseSimplex(std::mt19937 &randomToUse, const std::vector<fp> &octaveWeights, const std::vector<vec3> &noiseScales, crectangle1 &outputRange) : layerNoiseSimplex(randomToUse, octaveWeights, outputRange)
{
	for (size_t i = 0; i < octaveWeights.size(); i++)
	{
		this->octaveFrequencies[i] = 1.0 / noiseScales[i];
	}
}

layerNoiseSimplex::layerNoiseSimplex(std::mt19937 &randomToUse, const std::vector<fp> &octaveWeights, crectangle1 &outputRange) : baseNoise(simplexNoise(randomToUse))
{
	// initialize base noise
	this->octaveWeights = std::vector<fp>(octaveWeights);
	octaveFrequencies = std::vector<vec3>(octaveWeights.size());
	// sum weights
	cfp &TotalWeight = std::accumulate(octaveWeights.begin(), octaveWeights.end(), (fp)0);

	// map weights
	cfp &mid = outputRange.getCenter().x;
	this->OutputPlus = mid;
	cfp &amplitude = outputRange.pos1().x - mid;
	cfp &mult = amplitude * NOISE_MULT / TotalWeight;
	octaveOffsets = std::vector<vec3>(octaveWeights.size());
	for (size_t i = 0; i < octaveWeights.size(); i++)
	{
		this->octaveWeights[i] *= mult;
		octaveOffsets[i] = vec3(randFp(randomToUse), randFp(randomToUse), randFp(randomToUse));
	}
}

layerNoiseSimplex::~layerNoiseSimplex()
{
}
