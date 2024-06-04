#pragma once
#include "generationData.h"
#include <numeric>
#include "biomeID.h"
#include "constants.h"
#include "nbt/nbtSerializer.h"
#include "constants/vectorConstants.h"
struct overworldGenerationData :generationData
{
	int* heights = nullptr;
	biomeID* biomes = nullptr;
	overworldGenerationData() :heights(new int[chunkSize.x]), biomes(new biomeID[chunkSize.x]) {}
	virtual void serializeValue(nbtSerializer& s) override;
	virtual ~overworldGenerationData() override;
};