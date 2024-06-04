#include "idAnalysis.h"
#include "settings.h"
#include "recipe.h"
#include "block.h"

bool canBeDestroyedByFluids(const blockID& identifier)
{
	return identifier == blockID::air || (identifier != blockID::kelp && blockList[(int)identifier]->hardness == 0) || (identifier == blockID::snow) || (identifier == blockID::vine);
}

blockID getBlockToPlace(const itemID& identifier)
{
	if (isBlockItem(identifier))
	{
		return (blockID)identifier;
	}
	else
	{
		switch (identifier)
		{
		case itemID::redstone:
			return blockID::redstone_wire;
		case itemID::wheat_seeds:
			return blockID::wheat;
		case itemID::carrot:
			return blockID::carrots;
		case itemID::potato:
			return blockID::potatoes;
		case itemID::melon_seeds:
			return blockID::melon_stem;
		case itemID::pumpkin_seeds:
			return blockID::pumpkin_stem;
		case itemID::sweet_berries:
			return blockID::sweet_berry_bush;
		default:
			return (blockID)0;
		}
	}
}