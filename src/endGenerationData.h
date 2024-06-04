#pragma once
#include "generationData.h"
#include <vector>
#include "math/vector/vectn.h"
struct endGenerationData :generationData
{
	endGenerationData() {}
	virtual void serializeValue(nbtSerializer& s) override;
	virtual ~endGenerationData() override;
	std::vector<veci2> groundPositions = std::vector<veci2>();
};