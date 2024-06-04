#pragma once
enum harvestTierID : int
{
	noHarvestTier,
	woodHarvestTier,
	goldHarvestTier,
	stoneHarvestTier,//stone can break better blocks than gold_block
	ironHarvestTier,
	diamondHarvestTier,
	netheriteHarvestTier,
};

constexpr int toolTierCount = netheriteHarvestTier;