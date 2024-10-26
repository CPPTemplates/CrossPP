#include "end.h"
#include "endGenerationData.h"
#include "chunk.h"
#include "world.h"
#include "structure.h"
#include "structureFeature.h"
#include "folderList.h"
#include "settings.h"
#include "textureList.h"
#include "nbt/nbtSerializer.h"
#include "math/graphics/brush/brushes/solidColorBrush.h"
#include "include/math/rectangle/rectanglefunctions.h"
#include "include/math/graphics/graphicsFunctions.h"
end::end() :dimension(dimensionID::end)
{

}

generationData* end::generateTerrain(chunk& generateIn)
{
	endGenerationData* data = new endGenerationData();


	crectanglei2& rect = crectanglei2(generateIn.worldPos, cveci2(chunkSize));
	for (cveci2& pos : rect)
	{
		cfp thickness = getThickness(cvec2(pos));
		cfp noiseValue = terrainNoise->evaluate(cvec2(pos));

		if (noiseValue < thickness)
		{
			setBlockID(pos, blockID::end_stone);
		}
	}

	return data;
}

void end::generateStructures(chunk& generateIn)
{
	endGenerationData* generationData = (endGenerationData*)generateIn.terrainData;

	crectanglei2& chunkRect = crectanglei2(generateIn.worldPos, veci2(chunkSize));
	for (cveci2& pos : chunkRect)
	{
		if (getBlockID(pos) == blockID::end_stone)
		{
			//on chunk borders, this could change if something is already placed, but it is only a small difference
			if (getBlockID(pos + cveci2(0, 1)) == blockID::air)
			{
				generationData->groundPositions.push_back(pos + cveci2(0, 1));
			}
		}
	}
	for (cveci2 pos : generationData->groundPositions)
	{
		currentWorld->biomeList[(int)getBiome(pos)]->attemptgenerateStructures(this, pos, generateIn.chunkRandom);
	}

	if (generateIn.chunkCoordinates == veci2())
	{
		//generate the structures

		cveci2& endPortalSearchPos = cveci2(0, mainEndIslandMaxRadius);

		cveci2& exitPortalPos = findBlock(endPortalSearchPos, cveci2(0, -1), cveci2(), [](const auto& a) {return a == blockID::end_stone; });
		exitPortalLevel = exitPortalPos.y + 1;


		//place portal bedrock
		structure* endExitPortalStructure = getStructureByPath(L"the_end/exit_portal");
		structureFeature().placeStructurePart(endExitPortalStructure, this, cveci2(0, exitPortalLevel) + cveci2(-(int)((endExitPortalStructure->blockIDArray.size.x - 1) / 2), 0), false);

		summonEntity(entityID::ender_dragon, this, cvec2(0, exitPortalLevel + 0x20));

		for (size_t i = 0; i < obsidianPillarCount; i++)
		{
			cint pillarX = i % 2 == 0 ? -obsidianPillarDistance : obsidianPillarDistance;

			cveci2& pillarSearchPos = cveci2(pillarX, mainEndIslandMaxRadius);
			cveci2& pillarPos = findBlock(pillarSearchPos, cveci2(0, -1), cveci2(pillarX, 0), [](const auto& a) {return a == blockID::end_stone; });
			obsidianPillarLevel[i] = pillarPos.y + 1;

			replaceBlockRange(cveci2(pillarX - obsidianPillarRadius[i], obsidianPillarLevel[i] - 1), cveci2(pillarX + obsidianPillarRadius[i], obsidianPillarLevel[i] + obsidianPillarHeight[i] - 1), blockID::obsidian, { blockID::air }, chunkLoadLevel::updateLoaded);

			setBlockID(cveci2(pillarX, obsidianPillarLevel[i] + obsidianPillarHeight[i]), blockID::bedrock);
			summonEntity(entityID::end_crystal, this, cvec2(pillarX + 0.5, obsidianPillarLevel[i] + obsidianPillarHeight[i] + 1 + math::fpepsilon));
		}
	}
}

void end::serializeValue(nbtSerializer& s)
{
	s.serializeValue(std::wstring(L"dragon alive"), dragonAlive);
	s.serializeValue(std::wstring(L"exit portal level"), exitPortalLevel);

	if (s.push<nbtDataTag::tagList>(std::wstring(L"obsidian pillar levels")))
	{
		for (size_t i = 0; i < obsidianPillarCount; i++)
		{
			s.serializeValue(std::wstring(), obsidianPillarLevel[i]);
		}
		s.pop();
	}

	dimension::serializeValue(s);
}

void end::renderSky(crectangle2& blockRect, crectangle2 & drawRect, const gameRenderData& targetData) const
{
	//do not render the end sky, as it is almost impossible to compress, especially with lighting
	//if (settings::videoSettings::multiplyBiomeColors)
	//{
	//	fillRepeatingTexture(drawRect, *endSkyTexture->scaledTextures[0], targetData.renderTarget);
	//}
	//else
	//{
	//	fillRectangle(targetData.renderTarget, ceilRectangle(drawRect), solidColorBrush(colorPalette::black));
	//}
	fillRectangle(targetData.renderTarget, ceilRectangle(drawRect), colorMixer(targetData.renderTarget, solidColorBrush(colorPalette::purple)));
}

biomeID end::getBiome(cvec2& position) const
{
	cfp& lengthSquared = position.lengthSquared();
	if (lengthSquared > math::squared(smallEndIslandsDistance))
	{
		return biomeID::end_lands;
	}
	else if (lengthSquared > math::squared(mainEndIslandMaxRadius))
	{
		return biomeID::end_void;
	}
	else
	{
		return biomeID::the_end;
	}
}

void end::initialize()
{
	const std::vector<fp>& octaveWeights
	{
		6,
		5,
		4,
		3,
		2,
		1,
	};

	cfp noiseHillWidth = 0x800;

	const std::vector<vec3>& octaveScales
	{
		{noiseHillWidth, noiseHillWidth / 0b100},
		{noiseHillWidth / 0b10, noiseHillWidth / 0b1000},
		{noiseHillWidth / 0b100, noiseHillWidth / 0b10000},
		{noiseHillWidth / 0b1000, noiseHillWidth / 0b100000},
		{noiseHillWidth / 0b10000, noiseHillWidth / 0b1000000},
		{noiseHillWidth / 0b100000, noiseHillWidth / 0b10000000},
	};


	terrainNoise = new layerNoiseSimplex(worldRandom, octaveWeights, octaveScales);

	for (size_t i = 0; i < obsidianPillarCount; i++)
	{
		obsidianPillarRadius[i] = rand(worldRandom, 3, 6);
		obsidianPillarHeight[i] = rand(worldRandom, 0x20, 0x28);
	}
}

end::~end()
{
	if (terrainNoise)
	{
		delete terrainNoise;
	}
}

vec2 end::getWindSpeed(cvec2& position)
{
	return vec2();
}

fp end::getThickness(cvec2& position) const
{
	const biomeID& currentBiome = getBiome(position);

	constexpr fp endLandsThickness = 0.3;

	switch (currentBiome)
	{
	case biomeID::the_end:
	{
		cfp& distance = ((position.lengthSquared()) / math::squared(mainEndIslandMaxRadius));
		return (1 - distance) * ((position.y < 0) ? 1 : (1 - position.y / mainEndIslandMaxRadius));
	}
	case biomeID::end_void:
	{
		return math::mapValue(position.lengthSquared(), (fp)math::squared(mainEndIslandMaxRadius), (fp)math::squared(smallEndIslandsDistance), (fp)0, endLandsThickness);
	}
	case biomeID::end_lands:
		return endLandsThickness;
	default:
		break;
	}

	return 1 - ((position.lengthSquared()) / math::squared(mainEndIslandMaxRadius));
}