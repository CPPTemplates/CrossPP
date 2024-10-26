#pragma once
#include "structureFeature.h"
struct decayedStructureFeature : structureFeature
{
	fp integrity = 1;
	fp cobWebChance = 0;
	fp mossyChance = 0;
	fp crackedChance = 0;
	fp stolenChance = 0;

	std::mt19937* randomToUse = nullptr;

	decayedStructureFeature(std::mt19937* randomToUse, cfp& integrity = 1, cfp& cobWebChance = 0, cbool& replaceBlocks = true) : structureFeature(replaceBlocks), integrity(integrity), cobWebChance(cobWebChance), mossyChance(math::minimum((fp)(1 - integrity), (fp)0.3)), crackedChance(math::minimum((fp)(1 - integrity), (fp)0.1)), stolenChance(math::minimum((fp)((1 - integrity) * 2), (fp)1)), randomToUse(randomToUse) {}

	virtual blockID getBlockToPlace(blockContainer* const& containerIn, cveci2& worldPos, const blockID& structureBlock, blockData* const& data) override;

};