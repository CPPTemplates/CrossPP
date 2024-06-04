#include "math/vector/vectn.h"
#include "shulker.h"
#include "taskList.h"
#include "teleportToAttachAI.h"
#include "dimension.h"
#include "math/vector/vectorrandom.h"
shulker::shulker(dimension* dimensionIn, cvec2& position) : mob(dimensionIn, position, entityID::shulker)
{
	tasks = new taskList(this, {
	new teleportToAttachAI(this)
		});

	head = new bodyPart2D(shulkerHeadTextureRect, nullptr, position, shulkerHeadSize, shulkerHeadSize * 0.5);
	mainBodyPart = head;

	bottomShell = new bodyPart2D(shulkerBottomShellTextureRect, head, cvec2(0, -0.5 + shulkerBottomShellSize.y * 0.5), shulkerBottomShellSize, shulkerBottomShellSize * 0.5, 0, 0, true);
	topShell = new bodyPart2D(shulkerTopShellTextureRect, head, cvec2(0, 0.5 - shulkerTopShellSize.y * 0.5), shulkerTopShellSize, shulkerTopShellSize * 0.5, 0, 0, true);

	head->children.push_back(bottomShell);
	head->children.push_back(topShell);
}

void shulker::teleportToAttachableBlock()
{
	constexpr int shulkerTeleportAttemptCount = 0x8;
	constexpr int maxShulkerTeleportDistance = 9;

	//cveci2 flooredPosition = floorVector(position);

	for (int i = 0; i < shulkerTeleportAttemptCount; i++)
	{
		cveci2& testPosition = floorVector(getRandomPointOnEllipse(currentRandom, crectangle2(position, cvec2()).expanded(maxShulkerTeleportDistance)));

		std::vector<int> possibleDirections = std::vector<int>();

		for (fsize_t newDirection = 0; newDirection < directionCount2D; newDirection++)
		{
			if (canAttachTo(testPosition, (directionID)newDirection))
			{
				possibleDirections.push_back(newDirection);
			}
		}
		if (possibleDirections.size())
		{
			teleportTo(dimensionIn, cvec2(testPosition) + 0.5, true);
			attachmentDirection = (directionID)possibleDirections[randIndex(currentRandom, possibleDirections.size())];
			return;
		}
	}
}

void shulker::updateBodyParts() const
{
	mainBodyPart->translate = position;

	mainBodyPart->angle = getAngle2D(flipDirection(attachmentDirection));
	mainBodyPart->changed = true;
}

fp shulker::getGravityForce() const
{
	return attached() ? fp() : mob::getGravityForce();
}

std::vector<vec3> shulker::getFrictions() const
{
	return attached() ? std::vector<vec3>() : mob::getFrictions();
}

bool shulker::attached() const
{
	return canAttachTo(floorVector(position), attachmentDirection);
}

bool shulker::canAttachTo(cveci2& testPosition, const directionID& testDirection) const
{
	if (dimensionIn->getBlockID(testPosition) != blockID::air)
	{
		return false;
	}
	cveci2& absoluteAttachmentPosition = testPosition + directionVectors2D[(int)testDirection];

	return dimensionIn->hasFullEdge(absoluteAttachmentPosition, flipDirection(testDirection));
}

void shulker::addSpeed(cvec2& additionalSpeed)
{
	if (!attached())
	{
		return mob::addSpeed(additionalSpeed);
	}
}