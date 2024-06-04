#pragma once
#include "filesystem/jsonReader.h"
#include <random>
#include "constants.h"
#include "interface/idestructable.h"
struct dropCondition : IDestructable
{
	virtual bool checkCondition(const dropData& data, std::mt19937& randomToUse) const;
};
dropCondition* readCondition(const jsonContainer& container);