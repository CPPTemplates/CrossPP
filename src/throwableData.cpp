#include "throwableData.h"
throwableData::throwableData(const entityData& data, resolutionTexture* tex, cfp& throwPower) :
	entityData(data), tex(tex), throwPower(throwPower)
{
}