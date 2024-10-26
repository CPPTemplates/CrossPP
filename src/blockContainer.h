#pragma once
#include "arrayDataType.h"
#include "chunkLoadLevel.h"
#include "blockData.h"
#include "collisionDataCollection.h"
#include <set>
#include "block.h"
#include <list>
struct blockContainer
{
	virtual bool inBounds(cveci2& position) const = 0;
	virtual void* getArrayValuePointerUnsafe(cveci2& position, const arrayDataType& dataType, const chunkLoadLevel& minimalLoadLevel) = 0;

	std::vector<veci2> getBlockPositions(crectanglei2& rectToSearch, const std::vector<blockID>& blocksToFind);

	blockID getBlockID(cveci2& position, const chunkLoadLevel& minimalLoadLevel = chunkLoadLevel::worldGenerationLoaded);
	blockData* getBlockData(cveci2& position, const chunkLoadLevel& minimalLoadLevel = chunkLoadLevel::worldGenerationLoaded);
	lightLevel getInternalSunLightLevel(cveci2& position, const chunkLoadLevel& minimalLoadLevel = chunkLoadLevel::worldGenerationLoaded);
	lightLevel getBlockLightLevel(cveci2& position, const chunkLoadLevel& minimalLoadLevel = chunkLoadLevel::worldGenerationLoaded);
	powerLevel getPowerLevel(cveci2& position, const chunkLoadLevel& minimalLoadLevel = chunkLoadLevel::worldGenerationLoaded);
	virtual collisionDataCollection getBlockCollisionData(cveci2& position);
	block* getBlock(cveci2& pos, const chunkLoadLevel& minimalLoadLevel = chunkLoadLevel::worldGenerationLoaded);

	bool connectionBetween(cvect2<veci2>& positions);

	virtual void setBlockWithData(cveci2& position, const blockID& block, blockData* const& data, const chunkLoadLevel& minimalLoadLevel = chunkLoadLevel::worldGenerationLoaded);
	virtual void setBlockID(cveci2& pos, const blockID& block, const chunkLoadLevel& minimalLoadLevel = chunkLoadLevel::worldGenerationLoaded);

	virtual void removeBlock(cveci2& position, const blockID& oldBlockID, blockData* const& oldData, const chunkLoadLevel& minimalLoadLevel = chunkLoadLevel::worldGenerationLoaded);

	bool blockRangeContains(cveci2& pos00, cveci2& pos11, const std::vector<blockID>& checkList);
	bool blockRangeContainsOnly(cveci2& pos00, cveci2& pos11, const std::vector<blockID>& checkList);

	void setBlockRange(cveci2& pos0, cveci2& pos1, const blockID& block, const chunkLoadLevel& minimalLoadLevel = chunkLoadLevel::worldGenerationLoaded);
	void replaceBlockRange(cveci2& pos0, cveci2& pos1, const blockID& block, const std::vector<blockID>& replaceList, const chunkLoadLevel& minimalLoadLevel = chunkLoadLevel::worldGenerationLoaded);

	bool blockContains(cveci2& position, const std::vector<blockID>& checkList);

	void replaceCircleCentered(cvec2& pos, cvec2& size, const blockID& block, const std::vector<blockID>& replaceList);
	void replaceCircle(crectangle2& rect, const blockID& block, const std::vector<blockID>& replaceList);

	bool replaceBlock(cint& x, cint& y, const blockID& block, const std::vector<blockID>& replaceList, const chunkLoadLevel& minimalLoadLevel = chunkLoadLevel::worldGenerationLoaded);
	bool replaceBlock(cveci2& pos, const blockID& block, const std::vector<blockID>& replaceList, const chunkLoadLevel& minimalLoadLevel = chunkLoadLevel::worldGenerationLoaded);

	void setBlockData(cveci2& position, blockData* const& data, const chunkLoadLevel& minimalLoadLevel = chunkLoadLevel::worldGenerationLoaded);
	void setInternalSunLightLevel(cveci2& position, lightLevel const& level, const chunkLoadLevel& minimalLoadLevel = chunkLoadLevel::worldGenerationLoaded);
	void setBlockLightLevel(cveci2& position, lightLevel const& level, const chunkLoadLevel& minimalLoadLevel = chunkLoadLevel::worldGenerationLoaded);
	void setPowerLevel(cveci2& position, powerLevel const& level, const chunkLoadLevel& minimalLoadLevel = chunkLoadLevel::worldGenerationLoaded);


	bool canConnect(cveci2& from, cveci2& relativeCheckPosition);
	vecb2 railTopConnection(cveci2& pos);
	int getEmittanceLevel(cveci2& position, const levelID& arrayLevelDataTypeIndex);

	int getDecayStrengthExitingFrom(cveci2& positionFrom, cveci2& positionTo, const levelID& arrayLevelDataTypeIndex);
	int getDecayStrength(cveci2& positionFrom, cveci2& positionTo, const levelID& arrayLevelDataTypeIndex);

	void addOres(cveci2& pos, const blockID& b, cint& oreCount, std::mt19937& randomToUse, const std::vector<blockID>& replaceList);
	void addOres(cveci2& pos, const blockID& b, cint& oreCount, std::mt19937& randomToUse, const std::vector<blockID>& replaceList, const std::vector<veci2>& checkPositions);

	template<typename checkFunction>
	void addPool(cveci2& pos, const blockID& block, cint& maxPoolSize, checkFunction function);

	void addPool(cveci2& pos, const blockID& block, cint& maxPoolSize);
	void addFluidPool(cveci2& pos, const blockID& block, cint& maxPoolSize);

	//CAUTION: WILL NOT EXPAND WITH CHECK MARGIN
	virtual collisionDataCollection getHitboxCollisionData(crectangle2& box);
	collisionDataCollection getHitboxCollisionData(crectangle2& box, cvec2& hitboxTranslate);
	collisionTypeID getHitboxCollisionType(crectangle2& box);

	bool hasFullEdge(cveci2& position, const directionID& edgeToCheck);
	bool canAttachTo(cveci2& attachmentPosition, const directionID& edgeToCheck);

	bool fitExpandingHitbox(crectangle2& relativeHitbox, cvec2& positionToExpandFrom, vec2& hitboxPosition);
	bool fitExpandingHitbox(crectangle2& relativeHitbox, crectangle2& checkArea, cvec2& positionToExpandFrom, vec2& hitboxPosition);

	void moveTileToContainer(cveci2& sourcePosition, blockContainer& destinationContainer, cveci2& destinationPosition);

	template<typename t>
	t getArrayValue(cveci2& position, const arrayDataType& dataType, const chunkLoadLevel& minimalLoadLevel);
	template<typename t>
	void setArrayValue(cveci2& position, const t& value, const arrayDataType& dataType, const chunkLoadLevel& minimalLoadLevel);

	template<typename checkFunction>
	bool getAffectedPositions(cveci2& startPosition, cveci2& startDirection, csize_t& maxSize, checkFunction checkFunctionToUse, std::set<veci2>& affectedPositions, const std::vector<veci2>& relativeCheckPositions);

	template<typename checkFunction>
	std::vector<veci2> getAffectedPositions(crectanglei2& rectToSearch, checkFunction checkFunctionToUse);

	template<typename checkFunction>
	veci2 findBlock(cveci2& startPosition, cveci2& stepSize, cveci2& endPosition, checkFunction checkFunctionToUse);
};

template<typename t>
inline void blockContainer::setArrayValue(cveci2& position, const t& value, const arrayDataType& dataType, const chunkLoadLevel& minimalLoadLevel)
{
	if (inBounds(position))
	{
		*((t*)getArrayValuePointerUnsafe(position, dataType, minimalLoadLevel)) = value;
	}
}

template<typename t>
inline t blockContainer::getArrayValue(cveci2& position, const arrayDataType& dataType, const chunkLoadLevel& minimalLoadLevel)
{
	if (inBounds(position))
	{
		const t* val = (t*)getArrayValuePointerUnsafe(position, dataType, minimalLoadLevel);
		return val ? *val : t();
	}
	else
	{
		return t();
	}
}

template<typename checkFunction>

//check function : parameters: position, flowdirection
inline bool blockContainer::getAffectedPositions(cveci2& startPosition, cveci2& startDirection, csize_t& maxSize, checkFunction checkFunctionToUse, std::set<veci2>& affectedPositions, const std::vector<veci2>& relativeCheckPositions)
{
	if (!(inBounds(startPosition) && checkFunctionToUse(startPosition, startDirection)))
	{
		affectedPositions = std::set<veci2>();
	}
	else
	{
		const std::vector<veci2> startPositionContainer = { startPosition };
		affectedPositions = std::set<veci2>(startPositionContainer.begin(), startPositionContainer.end());
		std::list<veci2> floodQueue = std::list<veci2>({ startPosition });
		//6. While Q is not empty:
		while (floodQueue.size() > 0)
		{
			if (affectedPositions.size() > maxSize)
			{
				return false;//too large
			}
			//7. Set n equal to the first element of Q.
			cveci2& currentPosition = *(floodQueue.begin());

			for (size_t checkPosIndex = 0; checkPosIndex < relativeCheckPositions.size(); checkPosIndex++)
			{
				cveci2& absoluteCheckPos = currentPosition + relativeCheckPositions[checkPosIndex];
				if (inBounds(absoluteCheckPos) && checkFunctionToUse(absoluteCheckPos, relativeCheckPositions[checkPosIndex]))
				{
					const auto it = affectedPositions.lower_bound(absoluteCheckPos);
					if (it == affectedPositions.end() || *it != absoluteCheckPos)
					{
						affectedPositions.insert(it, absoluteCheckPos);
						floodQueue.push_back(absoluteCheckPos);
					}
				}
			}
			//8.     Remove first element from Q after it has been used
			floodQueue.pop_front();
			//13. Continue looping until Q is exhausted.
		}
	}
	return true;
}

template<typename checkFunction>
inline std::vector<veci2> blockContainer::getAffectedPositions(crectanglei2& rectToSearch, checkFunction checkFunctionToUse)
{
	std::vector<veci2> affectedPositions = std::vector<veci2>();
	cveci2& pos11 = rectToSearch.pos1();

	for (veci2 position = rectToSearch.pos0; position.y < pos11.y; position.y++)
	{
		for (position.x = rectToSearch.pos0.x; position.x < pos11.x; position.x++)
		{
			if (inBounds(position) && checkFunctionToUse(position))
			{
				affectedPositions.push_back(position);
			}
		}
	}
	return affectedPositions;
}

template<typename checkFunction>
inline veci2 blockContainer::findBlock(cveci2& startPosition, cveci2& stepSize, cveci2& endPosition, checkFunction checkFunctionToUse)
{
	for (veci2 pos = startPosition; pos != endPosition; pos += stepSize)
	{
		if (checkFunctionToUse(getBlockID(pos)))
		{
			return pos;
		}
	}
	return endPosition;
}

template<typename checkFunction>
inline void blockContainer::addPool(cveci2& pos, const blockID& block, cint& maxPoolSize, checkFunction function)
{
	std::set<veci2> positionsToFlood;
	if (getAffectedPositions(pos, cveci2(0, -1), maxPoolSize, function, positionsToFlood, { veci2(-1,0), veci2(1,0), veci2(0, -1) }))
	{
		for (veci2 pos : positionsToFlood)
		{
			setBlockID(pos, block);
		}
	}
}
void serializeBlocks(nbtSerializer& s, const array2d<blockID>& blockIDArray, const array2d<blockData*>& blockDataArray, const array2d<powerLevel>& powerLevelArray);