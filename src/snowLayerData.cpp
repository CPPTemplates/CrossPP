#include "snowLayerData.h"
#include "nbt/nbtSerializer.h"

snowLayerData::snowLayerData(cfp& layerThickness)
{
	this->layerThickness = layerThickness;
}

void snowLayerData::serializeValue(nbtSerializer& s)
{
	blockData::serializeValue(s);
	fallingBlockData::serializeValue(s);
	s.serializeValue(std::wstring(L"layer thickness"), layerThickness);
}
