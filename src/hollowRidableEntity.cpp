#include "hollowRidableEntity.h"
#include "fluidList.h"
#include "block.h"
fp hollowRidableEntity::getWeight() const
{
	cfp& airPart = 0.8;

	cvec2 hitboxSize = calculateHitBox().size;
	cfp& totalVolume = hitboxSize.volume() * hitboxSize.x;
	cfp& boatVolume = totalVolume * (1 - airPart);
	cfp& airVolume = totalVolume * airPart;


	cfp& boatWeight = boatVolume * getWeightPerCubicMeter();

	fp totalWeight = boatWeight;

	//check if the boat is sinking
	rectangle2 boatTopPart = rectangle2(mainBodyPart->translate.x, mainBodyPart->translate.y + mainBodyPart->size.y, mainBodyPart->size.x, 0);
	if (getFluidArea(boatTopPart, std::vector<blockID>(fluidList, fluidList + fluidCount)) > 0)
	{
		//add water weight to make the boat sink
		totalWeight += airVolume * blockList[blockID::water]->weightPerCubicMeter;
	}
	else
	{
		totalWeight += airVolume * airWeightPerCubicMeter;
	}
	return totalWeight;
}