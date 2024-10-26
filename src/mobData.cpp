#include "mobData.h"
#include "idAnalysis.h"
#include "chunk.h"
#include "dimension.h"
fp mobData::getSpawnWeight(dimension *dimensionIn, cveci2 &position)
{
	const biomeID biomeIn = dimensionIn->getBiome(cvec2(position));

	if (dimensionIn->identifier == dimensionID::overworld)
	{
		if (identifier == entityID::enderman)
		{
			return 1;
		}

		if (identifier == entityID::wolf)
		{
			if ((biomeIn == biomeID::taiga) || (biomeIn == biomeID::forest) || (biomeIn == biomeID::dark_forest))
			{
				return 1;
			}
		}

		if (identifier == entityID::slime)
		{
			if (biomeIn == biomeID::swamp)
			{
				return 0x4;
			}
			else if (position.y < minimumHillHeight)
			{
				return 0x1;
			}
		}

		if ((biomeIn == biomeID::taiga) || (biomeIn == biomeID::ice_spikes) || (biomeIn == biomeID::snowy_tundra))
		{
			if (identifier == entityID::stray)
			{
				return 0x4;
			}
		}
		else
		{
			if (identifier == entityID::skeleton)
			{
				return 0x4;
			}
		}

		if (biomeIn == biomeID::desert)
		{
			if (identifier == entityID::husk)
			{
				return 0x4;
			}
		}
		else
		{
			if (identifier == entityID::zombie)
			{
				return 0x4;
			}
		}

		if (biomeIn == biomeID::ocean)
		{
			if (identifier == entityID::drowned)
			{
				return 0x2;
			}
			else if (identifier == entityID::cod)
			{
				return 0x4;
			}
		}
		else if (identifier == entityID::creeper)
		{
			return 0x2;
		}
		else if ((biomeIn == biomeID::jungle) && (identifier == entityID::chicken))
		{
			return 0x2;
		}
		else if (isPassiveMob(identifier))
		{
			return 1;
		}
	}
	else if (dimensionIn->identifier == dimensionID::nether)
	{
		if (identifier == entityID::enderman)
		{
			if ((biomeIn == biomeID::nether_wastes) || (biomeIn == biomeID::warped_forest) || (biomeIn == biomeID::soulsand_valley))
			{
				return 0x1;
			}
		}
		else if (identifier == entityID::skeleton)
		{
			if (biomeIn == biomeID::soulsand_valley)
			{
				return 0x8;
			}
		}
		else if (identifier == entityID::magma_cube)
		{
			if (biomeIn == biomeID::nether_wastes)
			{
				return 0x1;
			}
			else if (biomeIn == biomeID::basalt_deltas)
			{
				return 0x8;
			}
		}
		else if (identifier == entityID::zombiefied_piglin)
		{
			if (biomeIn == biomeID::nether_wastes)
			{
				return 0x8;
			}
			else if (biomeIn == biomeID::crimson_forest)
			{
				return 0x4;
			}
		}
		else if (identifier == entityID::blaze)
		{
			const chunk *c = dimensionIn->loadChunkIfNotLoaded(getChunkCoordinates(cvec2(position)), chunkLoadLevel::notLoaded);
			if (std::find(c->generatedStructures.begin(), c->generatedStructures.end(), structureID::nether_fortress) != c->generatedStructures.end())
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else if (identifier == entityID::ghast)
		{
			if ((biomeIn == biomeID::nether_wastes) || (biomeIn == biomeID::basalt_deltas))
			{
				return 0.05;
			}
			else if (biomeIn == biomeID::soulsand_valley)
			{
				return 0.1;
			}
		}
	}
	else if (dimensionIn->identifier == dimensionID::end)
	{
		if (identifier == entityID::enderman)
		{
			return 0x8;
		}
	}
	return 0;
}
mobData *getMobData(const entityID &entityType)
{
	return (mobData *)entityDataList[(int)entityType];
}
mobData::~mobData()
{
	delete experienceWhenKilled;
}
mobData::mobData(const entityData &data, resolutionTexture *skin, std::shared_ptr<soundCollection> walkSound, std::shared_ptr<soundCollection> ambientSound, std::shared_ptr<soundCollection> hurtSound, std::shared_ptr<soundCollection> deathSound, cfp &walkingSpeed, const swingSynchronizer &legSwingSynchronizer, const color &spawnEggColor, const color &spawnEggOverlayColor, cfp &attackSpeed, cfp &attackDamage, experienceDrop *experienceWhenKilled, cfp &flyingSpeed) : entityData(data),
																																																																																																																					  walkingSpeed(walkingSpeed), flyingSpeed(flyingSpeed ? flyingSpeed : walkingSpeed), attackSpeed(attackSpeed), attackDamage(attackDamage),
																																																																																																																					  ambientSound(ambientSound), hurtSound(hurtSound), deathSound(deathSound), stepSound(walkSound),
																																																																																																																					  skin(skin), legSwingSynchronizer(legSwingSynchronizer),
																																																																																																																					  spawnEggColor(spawnEggColor), spawnEggOverlayColor(spawnEggOverlayColor),
																																																																																																																					  experienceWhenKilled(experienceWhenKilled)
{
}