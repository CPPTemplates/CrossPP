#include "additiveDrop.h"
additiveDrop::additiveDrop(const std::vector<lootTable*>& drops)
{
	this->drops = std::vector<lootTable*>(drops);
}

additiveDrop::~additiveDrop()
{
	for (lootTable* const& drop : drops)
	{
		delete drop;
	}
}

std::vector<itemStack> additiveDrop::roll(const dropData& data, std::mt19937& randomToUse) const
{
	std::vector<itemStack> slots = std::vector<itemStack>();
	for (size_t i = 0; i < drops.size(); i++)
	{
		const std::vector<itemStack> additionalSlots = drops[i]->roll(data, randomToUse);
		slots.insert(slots.end(), additionalSlots.begin(), additionalSlots.end());
	}
	return slots;
}