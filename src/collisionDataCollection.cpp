#include "collisionDataCollection.h"
#include "gameControl.h"
#include "block.h"
collisionDataCollection::collisionDataCollection()
{
	hitboxes = std::vector<collisionData>();
}

void collisionDataCollection::addCollisionData(const collisionDataCollection& extraData)
{
	hitboxes.insert(hitboxes.end(), extraData.hitboxes.begin(), extraData.hitboxes.end());
}

collisionData collisionDataCollection::getFirstCollision(cbool allowCollisionType[(size_t)collisionTypeID::count]) const
{
	collisionData nearestCollision = collisionData();
	for (size_t i = 0; i < (size_t)collisionTypeID::count; i++)
	{
		if (allowCollisionType[i] && firstCollisions[i] < nearestCollision)
		{
			nearestCollision = firstCollisions[i];
		}
	}
	return nearestCollision;
}

std::vector<collisionData> collisionDataCollection::getCollisions(const collisionTypeID& type)
{
	std::vector<collisionData> validCollisions = std::vector<collisionData>();
	for (const collisionData& d : hitboxes)
	{
		if (d.type == type)
		{
			validCollisions.push_back(d);
		}
	}
	return validCollisions;
}

collisionTypeID collisionDataCollection::getMaximumCollision() const
{
	collisionData nearestCollision = collisionData();
	for (size_t i = ((size_t)collisionTypeID::count - 1); i > 0; i--)
	{
		if (firstCollisions[i].axisCollided.x || firstCollisions[i].axisCollided.y)
		{
			return (collisionTypeID)i;
		}
	}
	return collisionTypeID::willNotCollide;
}

collisionEdgeData collisionDataCollection::getEdges(cfp& location, const directionID& checkDirection) const
{
	caxisID& checkAxis = getAxis(checkDirection);
	cbool positive = isPositive(checkDirection);
	caxisID& otherAxis = (axisID)(1 - (int)checkAxis);
	collisionEdgeData collisionEdges = collisionEdgeData();

	for (const collisionData& collision : hitboxes)
	{
		if (collision.type != collisionTypeID::willNotCollide)
		{
			if (positive)
			{
				if (collision.hitboxCollidingWith.pos0[checkAxis] == location)
				{
					collisionEdges.addEdge(collision.hitboxCollidingWith.pos0[otherAxis], collision.hitboxCollidingWith.size[otherAxis]);
				}
			}
			else
			{
				cfp positiveSide = collision.hitboxCollidingWith.pos0[checkAxis] + collision.hitboxCollidingWith.size[checkAxis];
				if (positiveSide == location)
				{
					collisionEdges.addEdge(collision.hitboxCollidingWith.pos0[otherAxis], collision.hitboxCollidingWith.size[otherAxis]);
				}
			}
		}
	}
	return collisionEdges;
}

void collisionDataCollection::evaluate(crectangle2& hitboxToTest, cvec2& hitboxSpeed, cfp& maxDuration)
{
	for (collisionData& data : hitboxes)
	{
		data.evaluate(hitboxToTest, hitboxSpeed, maxDuration);
		collisionData& currentData = firstCollisions[(size_t)data.type];
		if (data < currentData)
		{
			currentData = data;
		}
	}
}

void collisionDataCollection::render(const gameRenderData& targetData) const
{
	for (const collisionData& hitbox : hitboxes)
	{
		constexpr color collisionColor[(size_t)collisionTypeID::count]
		{
			colorPalette::orange,
			colorPalette::green,
			colorPalette::red,
		};

		renderBlockRect(hitbox.hitboxCollidingWith, targetData, collisionColor[(size_t)hitbox.type]);
		//render speed
		if (hitbox.speed.lengthSquared() > 0)
		{
			cvec2 pos0 = targetData.worldToRenderTargetTransform.multPointMatrix(hitbox.hitboxCollidingWith.pos0);
			cvec2 pos1 = targetData.worldToRenderTargetTransform.multPointMatrix(hitbox.hitboxCollidingWith.pos0 + hitbox.speed * secondsPerTick);
			fillLine(targetData.renderTarget, pos0, pos1, solidColorBrush(colorPalette::blue));
			//crectangle2& withSpeed = crectangle2(hitbox.hitboxCollidingWith.pos0 + hitbox.speed * secondsPerTick, hitbox.hitboxCollidingWith.size);
			//renderBlockRect(withSpeed, targetData, colorPalette::blue);
		}
	}
}