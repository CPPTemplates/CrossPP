#pragma once
#include "blockContainer.h"
#include "array/fastlist.h"
#include <list>
#include <vector>
#include "blockData.h"
#include "itemID.h"
#include "chunkLoadLevel.h"
#include "constants.h"
#include "entity.h"
#include "globalFunctions.h"
#include "interface/idestructable.h"
#include "math/mattnxn.h"
#include "math/rectangle/rectangletn.h"
#include "math/uuid.h"
#include "math/vector/vectn.h"
struct tickableBlockContainer : blockContainer, IDestructable
{
	std::vector<veci2> portalPositions = std::vector<veci2>();
	tickableBlockContainer* parentContainer = nullptr;
	mat3x3 childToParentTransform = mat3x3();
	dimension* rootDimension = nullptr;
	mat3x3 containerToRootTransform = mat3x3();
	//the amount of translate that will be added to the childToParentTransform in the next tick
	vec2 speedInParent = vec2();
	vec2 speedInRoot = vec2();

	std::vector<tickableBlockContainer*> childContainers = std::vector<tickableBlockContainer*>();
	void updateChildToParentTransform(cmat3x3& newTransform);
	void updateContainerToRootTransform();

	void updateSpeedInParent(cvec2& newSpeed);
	void updateSpeedInRoot();

	void addChildContainer(tickableBlockContainer* const& movingBlocks, cmat3x3& childToParentTransform);
	void removeChildContainer(tickableBlockContainer* const& movingBlocks);
	void setParentContainer(tickableBlockContainer* newParentContainer, cmat3x3& newTransform);
	tickableBlockContainer* getDeepestNestedContainer(cvec2& relativePosition);

	tickableBlockContainer() : blockContainer() {}
	//double ticks will be removed automatically
	fastList<veci2> blockUpdatePositions = fastList<veci2>();
	std::list<veci2> emittanceUpdateQueue = std::list<veci2>();

	//harvest the block (let the block drop as item)
	void harvestBlock(dropData data, cveci2& position, const chunkLoadLevel& minimalLoadLevel = chunkLoadLevel::worldGenerationLoaded);

	virtual void removeBlock(cveci2& position, const blockID& oldBlockID, blockData* const& oldData, const chunkLoadLevel& minimalLoadLevel = chunkLoadLevel::worldGenerationLoaded) override;

	//p0 and p1 will be cropped
	virtual bool cropRelativeLineToContainer(vec2& p0, vec2& p1);
	virtual bool canAddUpdates(cveci2& position);
	//if the update does not already exist
	void addBlockUpdatePositionsAround(cveci2& pos);
	virtual void addBlockUpdatePosition(cveci2& position);
	virtual void addLevelUpdatePosition(cveci2& position);
	void addUpdatePosition(cveci2& position);

	virtual std::vector<entity*> findNearEntities(cvec2& position, cfp& radius);
	virtual std::vector<entity*> findNearEntities(crectangle2& searchBox);

	//when no collision is found but a block other than air is found at the destination, the resultingcontainer is still set!
	bool findRaycastRecursive(cvec2& absolutePosition, cvec2& absoluteDestination, veci2& resultingBlockPosition, veci2& adjacentBlockPosition, vec2& exactBlockIntersection, tickableBlockContainer*& resultingContainer);

	virtual lightLevel getVisibleLightLevel(cveci2& pos);
	lightLevel getMaximumInternalLightLevel(cveci2& pos);

	virtual entity* findUUID(cvec2& position, cfp& range, const uuid& id);

	void reCalculateLevels();
	bool fillNetherPortal(cveci2& position);

	virtual void setBlockID(cveci2& pos, const blockID& block, const chunkLoadLevel& minimalLoadLevel = chunkLoadLevel::worldGenerationLoaded) override;
	virtual void setBlockWithData(cveci2& position, const blockID& block, blockData* const& data, const chunkLoadLevel& minimalLoadLevel = chunkLoadLevel::worldGenerationLoaded) override;

	void openDoorType(cveci2& position, cbool& open);

	void fuseTNT(cveci2& position, cbool& ignitedByExplosion = false);
	void createExplosion(cvec2& position, cfp& explosionPower, cbool& startFire = false);
	virtual void tick();
	~tickableBlockContainer();
};

lightLevel getSpreadedLightLevel(const lightLevel& spreadFrom, const lightLevel& decayStrength, const lightLevel& filterStrength) noexcept;