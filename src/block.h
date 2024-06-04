#pragma once
#include <memory>
#include "interface/idestructable.h"
#include "interface/inamable.h"
#include "math/graphics/resolutiontexture.h"
#include "itemID.h"
#include "levelID.h"
#include "soundCollection.h"
#include "harvestType.h"
#include "harvestTier.h"
#include "collisionTypeID.h"
#include "experienceDrop.h"
#include "lootTable.h"
#include "idList.h"
#include "biomeData.h"
#include "facingData.h"
#include "idAnalysis.h"
#include "gameRenderData.h"
#include "collisionDataCollection.h"

struct blockContainer;

extern resolutionTexture *grassOverlay;

constexpr int noEmittance[(size_t)levelID::count]{

};

struct block : INamable, IDestructable
{
	block(blockID identifier, fp hardness, fp blastResistance, cfp &weightPerCubicMeter, resolutionTexture *tex, std::wstring name,
		  std::shared_ptr<soundCollection> fallSound,
		  std::shared_ptr<soundCollection> stepSound,
		  std::shared_ptr<soundCollection> hitSound,
		  std::shared_ptr<soundCollection> breakSound,
		  std::shared_ptr<soundCollection> placeSound, cint (&filterStrength)[(size_t)levelID::count], harvestTypeID bestTool = withHand, harvestTierID itemTier = noHarvestTier, collisionTypeID collisiontype = collisionTypeID::willCollide,
		  cint &fireEncouragement = 0, cint &flammability = 0, cbool &canCatchFireFromLava = false,
		  cbool &canReplaceBlock = false, cint (&emittanceLevel)[(size_t)levelID::count] = noEmittance, const experienceDrop &experienceWhenBroken = experienceDrop(), std::shared_ptr<soundCollection> ambientSound = nullptr);
	blockID identifier;

	// breaking data

	harvestTierID itemTier = noHarvestTier;
	harvestTypeID bestTool = withHand;

	fp hardness = 0;
	fp blastResistance = 0;

	int fireEncouragement = 0;
	int flammability = 0;
	bool canCatchFireFromLava = false;

	// other physical properties

	collisionTypeID blockCollisionType;
	int emittanceLevel[(size_t)levelID::count]{};
	int filterStrength[(size_t)levelID::count]{};
	fp weightPerCubicMeter = 0;

	// resources:

	// texture

	resolutionTexture *tex;

	// sound

	std::shared_ptr<soundCollection> fallSound = nullptr;
	std::shared_ptr<soundCollection> stepSound = nullptr;
	std::shared_ptr<soundCollection> hitSound = nullptr;
	std::shared_ptr<soundCollection> breakSound = nullptr;
	std::shared_ptr<soundCollection> placeSound = nullptr;

	std::shared_ptr<soundCollection> ambientSound = nullptr;

	bool canReplaceBlock = false; // wether you can place another block inside this block to replace it

	//loot

	std::shared_ptr<lootTable> dropsWhenHarvested = std::make_shared<lootTable>();
	experienceDrop experienceWhenBroken = experienceDrop(); // experience given when this block is broken

	bool correctTool(const itemID &tool) const;
	bool canHarvest(const itemID &tool);


	virtual bool canPlace(tickableBlockContainer *containerIn, cveci2 position);
	virtual void use();
	/// <summary>
	/// render a block
	/// </summary>
	/// <param name="rect"></param>
	/// <param name="targetData">
	/// the transform of targetData should transform rectangle2(blockPosition,cvec2(1))
	/// </param>
	/// <param name="data"></param>
	/// <param name="containerIn"></param>
	/// <param name="blockPosition"></param>
	virtual void render(const gameRenderData &targetData, blockData *const data, blockContainer *containerIn, cveci2 &blockPosition) const;
	template <typename brush0Type>
	void render(const brush0Type &b, rectangle2 brushRect, crectangle2 &blockRect, const gameRenderData &targetData, blockData *const data, blockContainer *containerIn, cbool &renderAnimation = false, const std::optional<vec2> &rotationCentreWorld = std::nullopt) const;

	static mat3x3 getTextureToWorldTransform(cvec2 &textureSize, cmat3x3 &blockToRenderTargetTransform, cvec2 &blockPosition);
	static mat3x3 getBrushRectToWorldTransform(crectangle2 &brushRect, cmat3x3 &blockToRenderTargetTransform, cvec2 &blockPosition);

	virtual collisionDataCollection getCollisionData(blockContainer *containerIn, cveci2 &position) const;
};

void renderTorch(cveci2 &blockPosition, cvec2 &relativeRotationCentre, cfp &angle, const resolutionTexture &tex, const gameRenderData &targetData);
void renderTexture(crectangle2 &rectangleToTransform, cmat3x3 &transform, cbool &hasTransparency, const directionID &directionFacing, const resolutionTexture &tex, const texture &renderTarget, cbool &renderAnimation);
void renderTexture(crectangle2 &drawRect, cbool &hasTransparency, const directionID &directionFacing, const resolutionTexture &tex, const texture &renderTarget, cbool &renderAnimation);
void renderBlockRect(crectangle2 &blockRect, const gameRenderData &targetData);
void renderBlockRect(crectangle2 &blockRect, const gameRenderData &targetData, const color &c);

// template<typename targetType>
// void renderBrewingStand(crectangle2& drawRect, cbool hasBottle[brewingStandPotionCapacity], const targetType& renderTarget);

template <typename brush0Type>
void renderCollisionData(blockContainer *containerIn, cveci2 &position, crectangle2 &brushRect, const brush0Type &b, const gameRenderData &targetData);
void renderCollisionData(blockContainer *containerIn, cveci2 &position, const resolutionTexture &tex, const gameRenderData &targetData);
void renderCollisionData(blockContainer *containerIn, cveci2 &position, const gameRenderData &targetData);

extern idList<biomeData *, biomeID> biomeDataList;

extern idList<block *, blockID> blockList;