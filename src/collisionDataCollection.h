#pragma once
#include "collisionData.h"
#include "collisionEdgeData.h"
#include "gameRenderData.h"
#include <map>
struct collisionDataCollection
{
	collisionDataCollection();
	//std::map<collisionTypeID, collisionData> firstCollisions = std::map<collisionTypeID, collisionData>();
	collisionData firstCollisions[(size_t)collisionTypeID::count];
	void addCollisionData(const collisionDataCollection& extraData);
	collisionData getFirstCollision(cbool allowCollisionType[(size_t)collisionTypeID::count]) const;
	std::vector<collisionData> getCollisions(const collisionTypeID& type);
	std::vector<collisionData> hitboxes = std::vector<collisionData>();
	//DOES NOT CHECK DISTANCE
	collisionTypeID getMaximumCollision() const;

	//x : start
	//y : size
	collisionEdgeData getEdges(cfp& location, const directionID& checkDirection) const;
	void evaluate(crectangle2& hitboxToTest, cvec2& hitboxSpeed, cfp& maxDuration);
	void render(const gameRenderData& targetData) const;
};