#include "tntData.h"
#include "tickableBlockContainer.h"
bool tntData::tick(tickableBlockContainer* containerIn, cveci2& position)
{
	if (containerIn->getPowerLevel(position) > 0)
	{
		containerIn->fuseTNT(position);
	}
	return false;
}