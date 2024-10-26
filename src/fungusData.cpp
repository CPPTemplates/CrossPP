#include "fungusData.h"
void fungusData::randomTick(tickableBlockContainer* containerIn, cveci2& position)
{
	pollenData::randomTick(containerIn, position);
	saplingData::randomTick(containerIn, position);
}