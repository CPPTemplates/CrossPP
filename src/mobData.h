#pragma once
#include "math/swingsynchronizer.h"
#include "experienceDrop.h"
#include "lootTable.h"
#include "soundCollection.h"
#include "math/graphics/resolutiontexture.h"
#include "entityData.h"

struct mobData : public entityData
{
	mobData(const entityData& data, resolutionTexture* skin, std::shared_ptr<soundCollection> walkSound, std::shared_ptr<soundCollection> ambientSound, std::shared_ptr<soundCollection> hurtSound, std::shared_ptr<soundCollection> deathSound, cfp& walkingSpeed, const swingSynchronizer& legSwingSynchronizer, const color& spawnEggColor, const color& spawnEggOverlayColor, cfp& attackSpeed = 1.0, cfp& attackDamage = 1.0, experienceDrop* experienceWhenKilled = new experienceDrop(0, 0), cfp& flyingSpeed = 0);
	fp walkingSpeed = 0;
	fp flyingSpeed = 0;
	fp attackSpeed = 0;
	//at NORMAL difficulty
	fp attackDamage = 0;

	//appearance
	std::shared_ptr<soundCollection> ambientSound = std::shared_ptr<soundCollection>();
	std::shared_ptr<soundCollection> hurtSound = std::shared_ptr<soundCollection>();
	std::shared_ptr<soundCollection> deathSound = std::shared_ptr<soundCollection>();
	std::shared_ptr<soundCollection> stepSound = std::shared_ptr<soundCollection>();

	resolutionTexture* skin;
	swingSynchronizer legSwingSynchronizer = swingSynchronizer();

	color spawnEggColor = color();
	color spawnEggOverlayColor = color();


	//loot
	std::shared_ptr <lootTable> dropsWhenKilled = std::make_shared<lootTable>();
	experienceDrop* experienceWhenKilled = nullptr;


	virtual ~mobData() override;
	virtual fp getSpawnWeight(dimension* dimensionIn, cveci2& position);
};
mobData* getMobData(const entityID& entityType);