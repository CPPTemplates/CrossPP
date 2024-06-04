#pragma once
#include "blockData.h"
#include "constants.h"
struct dropData
{
	// 0 when there was no explosion
	blockData *harvestedBlockData = nullptr;
	const itemStack *toolUsed = nullptr;
	int explosionPower = 0;
	dropData(blockData *const &harvestedBlockData = nullptr, const itemStack *toolUsed = nullptr, cint &explosionPower = 0)
		: harvestedBlockData(harvestedBlockData),
		  toolUsed(toolUsed),
		  explosionPower(explosionPower) {}
};