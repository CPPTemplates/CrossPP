#pragma once
#include "particleID.h"
#include "interface/inamable.h"
#include "idList.h"
#include "math/graphics/resolutiontexture.h"
struct particleTypeData : INamable
{
	particleTypeData(const std::wstring& name);
	std::vector<resolutionTexture*> textures;
};
extern idList<particleTypeData*, particleID> particleTypeDataList;