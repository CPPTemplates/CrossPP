#include "pollen.h"
#include "idAnalysis.h"
#include "pollenData.h"
#include "tickableBlockContainer.h"
#include "math/vector/vectorrandom.h"
#include "include/math/rectangle/rectanglefunctions.h"
void pollenData::randomTick(tickableBlockContainer* containerIn, cveci2& position)
{
	const blockID& spreadingBlock = containerIn->getBlockID(position);
	if (randChance(currentRandom, getPollenChance(spreadingBlock)))
	{
		pollen* p = (pollen*)summonEntity(entityID::pollen, containerIn, getrandomPosition(currentRandom, crectangle2(position, cvec2(1)), cvec2(pollenSize)) - relativePollenHitbox.pos0);
		p->blockToSpread = getBlockToSpread(spreadingBlock);
	}
}