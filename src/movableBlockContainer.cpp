#include "movableBlockContainer.h"
#include "idAnalysis.h"
#include <string>
#include "arrayDataType.h"
#include "blockContainer.h"
#include "blockData.h"
#include "itemID.h"
#include "chunkLoadLevel.h"
#include "collisionDataCollection.h"
#include "constants.h"
#include "array/arraynd/arraynd.h"
#include "globalFunctions.h"
#include "math/rectangle/rectangletn.h"
#include "math/vector/vectn.h"
#include "levelID.h"
#include "nbt/nbtSerializer.h"
#include "include/array/arraynd/arrayndFunctions.h"
#include "nbt/serializeColor.h"
void* movableBlockContainer::getArrayValuePointerUnsafe(cveci2& position, const arrayDataType& dataType, const chunkLoadLevel& minimalLoadLevel)
{
	veci2 movedPosition = position - arrayPos00Offset;
	//first expand to contain the point
	expandToContain(blockIDArray, movedPosition);
	expandToContain(blockDataArray, movedPosition);
	expandToContain(powerLevelArray, movedPosition);

	for (int i = 0; i < 2; i++)
	{
		if (movedPosition[i] < 0)
		{
			arrayPos00Offset[i] += movedPosition[i];
			movedPosition[i] = 0;
		}
	}

	if (!blockIDArray.inBounds(movedPosition))
	{
		throw "";
	}

	csize_t& arrayIndex = movedPosition.x + movedPosition.y * blockIDArray.size.x;
	switch (dataType)
	{
	case arrayDataType::blockIDType:
		return blockIDArray.baseArray + arrayIndex;
	case arrayDataType::blockDataType:
		return blockDataArray.baseArray + arrayIndex;
	default:
	{
		if (isLevelDataType(dataType))
		{
			const levelID& levelType = (levelID)((int)dataType - (int)arrayDataType::levelType);
			switch (levelType)
			{
			case levelID::powerLevel:
				return powerLevelArray.baseArray + arrayIndex;
			default:
				return nullptr;
			}
		}
		else
		{
			handleError(std::wstring(L"array not found"));
		}
		return nullptr;
	}
	}
}

bool movableBlockContainer::inBounds(cveci2& position) const
{
	//it will just expand
	return true;
}

rectanglei2 movableBlockContainer::getCurrentArrayRect() const
{
	return rectanglei2(arrayPos00Offset, (veci2)blockIDArray.size);
}

collisionDataCollection movableBlockContainer::getHitboxCollisionData(crectangle2& box)
{
	return blockContainer::getHitboxCollisionData(crectangle2(getCurrentArrayRect()).cropClientRectUnsafe(box));
}

bool movableBlockContainer::cropRelativeLineToContainer(vec2& p0, vec2& p1)
{
	return cropLine(p0, p1, crectangle2(getCurrentArrayRect()));
}

void movableBlockContainer::serializeValue(nbtSerializer& s)
{
	serializeNBTValue(s, std::wstring(L"relative attachment position"), relativeAttachmentPosition);
	serializeNBTValue(s, std::wstring(L"pos00 offset"), arrayPos00Offset);
	veci2 size = veci2(blockIDArray.size);
	if (serializeNBTValue(s, std::wstring(L"size"), size)) {
		if (size.volume()) {
			if (!s.write)
			{
				blockIDArray = array2d<blockID>(size);
				blockDataArray = array2d<blockData*>(size);
				powerLevelArray = array2d<powerLevel>(size);
			}
			serializeBlocks(s, blockIDArray, blockDataArray, powerLevelArray);
		}
	}
}

movableBlockContainer::~movableBlockContainer()
{
	for (fsize_t i = 0; i < blockDataArray.size.volume(); i++)
	{
		if (blockDataArray.baseArray[i])
		{
			delete blockDataArray.baseArray[i];
		}
	}
}
