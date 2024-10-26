#pragma once
#include "blockContainer.h"
#include "blockArrayContainer.h"
struct renderBlockContainer : public blockContainer, blockArrayContainer {
	renderBlockContainer(cvect2<fsize_t>& size) :blockArrayContainer(size) {}

	// Inherited via blockContainer
	bool inBounds(cveci2& position) const override;
	void* getArrayValuePointerUnsafe(cveci2& position, const arrayDataType& dataType, const chunkLoadLevel& minimalLoadLevel) override;
};