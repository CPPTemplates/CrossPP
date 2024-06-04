#include "itemData.h"
void itemData::setEatingValues(cint& foodPoints, cfp& extraSaturation)
{
	canEat = true;
	this->foodPoints = foodPoints;
	this->extraSaturation = extraSaturation;
}



bool itemData::compare(const itemID& itemToCompare) const
{
	return itemToCompare == identifier;
}



itemData::~itemData()
{

}