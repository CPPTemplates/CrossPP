#pragma once
#include "structureFeature.h"
struct netherFortressFeature : structureFeature
{
	virtual void placeStructurePart(structure* s, tickableBlockContainer* containerIn, cveci2& pos00, cbool& flipX, cint& levels = 0, cbool& includeEntities = true, cbool& replaceStructureVoid = true) override;
};