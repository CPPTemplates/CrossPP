#include "doubleAttachedBlockData.h"
bool doubleAttachedBlockData::tick(tickableBlockContainer* containerIn, cveci2& position)
{
	return isPart0 ? attachedBlockData::tick(containerIn, position) : false;
}