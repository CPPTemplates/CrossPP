#include "attachedBlockData.h"
#include "idAnalysis.h"
#include "dropData.h"
#include <string>
#include "itemID.h"
#include "chunkLoadLevel.h"
#include "constants.h"
#include "math/direction.h"
#include "math/vector/vectn.h"
#include "tickableBlockContainer.h"
directionID attachedBlockData::getAttachmentDirection(tickableBlockContainer *containerIn, cveci2 &position) const
{
	const blockID &block = containerIn->getBlockID(position);

	if (attachedToBottomBlock(block))
	{
		return directionID::negativeY;
	}
	else
	{
		return (directionID)-1;
	}
}

bool attachedBlockData::tick(tickableBlockContainer *containerIn, cveci2 &position)
{
	if (!attached(containerIn, position))
	{
		containerIn->harvestBlock(dropData(), position, chunkLoadLevel::updateLoaded);
		containerIn->setBlockID(position, blockID::air, chunkLoadLevel::updateLoaded);
	}
	return false;
}

bool attachedBlockData::attached(tickableBlockContainer *containerIn, cveci2 &position) const
{
	const directionID &attachmentDirection = getAttachmentDirection(containerIn, position);
	if (attachmentDirection == (directionID)-1)
	{
		//this block can attach to any direction!
		// seek for attachment points
		for (fsize_t i = 0; i < directionCount2D; i++)
		{
			//const veci2 &directionToCheck = directionVectors2D[i];
			if (containerIn->canAttachTo(position + directionVectors2D[i], flipDirection((directionID)i)))
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		return containerIn->canAttachTo(position + directionVectors2D[(size_t)attachmentDirection], flipDirection(attachmentDirection));
	}
}