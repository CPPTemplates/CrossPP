#pragma once
#include "blockData.h"
#include "structureBlockMode.h"
struct structureBlockData : blockData
{
	std::wstring structureName = std::wstring();
	veci2 relativeStructurePosition = cveci2(1, 0);
	veci2 structureSize = cveci2();
	bool includeEntities = false;
	bool mirrorX = false;
	int structureRotation = 0;
	fp structureIntegrity = 0.0;
	ull structureSeed = 0;
	bool showBoundingBox = false;
	structureBlockMode mode = structureBlockMode::saveMode;
	virtual void serializeValue(nbtSerializer& s) override;
};