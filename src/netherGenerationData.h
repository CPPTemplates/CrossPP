#pragma once
#include "generationData.h"
#include "math/vector/vectn.h"
#include <vector>
struct netherGenerationData :generationData
{
	netherGenerationData() {}
	virtual void serializeValue(nbtSerializer& s) override;
	virtual ~netherGenerationData() override;
	std::vector<veci2> groundPositions = std::vector<veci2>();
};