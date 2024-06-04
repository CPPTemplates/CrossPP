#include "particleTypeData.h"
#include "resourcePack.h"
#include "filesystem/jsonReader.h"
#include <filesystem/textfile.h>
#include "folderList.h"

particleTypeData::particleTypeData(const std::wstring& name) : INamable(name), textures(std::vector<resolutionTexture*>())
{
	const auto locations = getResourceLocations(particleDataFolder / (name + L".json"));
	if (locations.size()) {
		const stdPath& path = locations[locations.size() - 1];
		jsonContainer container = readJson(stringToWString(readAllText(path)));
		if (container.children.size()) {
			//textures = std::vector<resolutionTexture*>(container.children[0].children.size());
			for (const auto& c : container.children[0].children) {
				textures.push_back(loadTextureFromResourcePack(particleTextureFolder / (removeNameSpace(c.value) + L".png")));
			}
		}
	}
}
