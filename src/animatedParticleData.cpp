#include "animatedParticleData.h"
#include "folderList.h"
#include "resourcePack.h"
animatedParticleData::animatedParticleData(const std::wstring& name) :INamable(name)
{
	size_t i = 0;
	while (true)
	{
		const stdPath& path = particleTextureFolder / (name + std::to_wstring(i));
		if (getResourceLocations(path).size())
		{
			animatedTextures.push_back(loadTextureFromResourcePack(path));
		}

		i++;
	}
}