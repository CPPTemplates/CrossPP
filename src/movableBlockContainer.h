#pragma once
#include "tickableBlockContainer.h"
#include "arrayDataType.h"
#include "blockData.h"
#include "itemID.h"
#include "chunkLoadLevel.h"
#include "collisionDataCollection.h"
#include "constants.h"
#include "array/arraynd/arraynd.h"
#include "math/rectangle/rectangletn.h"
#include "math/vector/vectn.h"
#include "nbt/nbtSerializable.h"
#include "nbt/nbtSerializer.h"
#include "blockArrayContainer.h"

struct movableBlockContainer : tickableBlockContainer, public blockArrayContainer, nbtSerializable
{
	veci2 relativeAttachmentPosition = cveci2();
	veci2 arrayPos00Offset = veci2();//for if the array expands

	void* getArrayValuePointerUnsafe(cveci2& position, const arrayDataType& dataType, const chunkLoadLevel& minimalLoadLevel) override;
	bool inBounds(cveci2& position) const override;

	rectanglei2 getCurrentArrayRect() const;

	collisionDataCollection getHitboxCollisionData(crectangle2& box) override;
	bool cropRelativeLineToContainer(vec2& p0, vec2& p1) override;

	movableBlockContainer(cvect2<fsize_t>& size) :blockArrayContainer(size) {}
	void serializeValue(nbtSerializer& s) override;
	~movableBlockContainer() override;
};