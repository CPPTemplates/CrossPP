#include "itemDrop.h"
#include <random>
#include <vector>
#include "constants.h"
#include "itemStack.h"
#include "itemTag.h"
std::vector<itemStack> itemDrop::roll(const dropData& data, std::mt19937& randomToUse) const
{
	return std::vector<itemStack>({ itemStack(itemType,1, createItemTag(itemType)) });
}
