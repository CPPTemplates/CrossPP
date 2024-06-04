#include "netherFortressFeature.h"
#include "tickableBlockContainer.h"
void netherFortressFeature::placeStructurePart(structure* s, tickableBlockContainer* containerIn, cveci2& pos00, cbool& flipX, cint& levels, cbool& includeEntities, cbool& replaceStructureVoid)
{
	//repeat the lowest block all the way to the bottom
	structureFeature::placeStructurePart(s, containerIn, pos00, flipX, levels, includeEntities, replaceStructureVoid);
	cmat3x3i structureToWorldTransform = s->getBlocksToWorldTransform(pos00, flipX);
	for (int i = 0; i < (int)s->blockIDArray.size.x; i++)
	{
		blockID b = s->blockIDArray.getValue(cveci2(i, 0));
		if (b != blockID::structure_void)
		{
			cint worldX = structureToWorldTransform.multPointMatrix(cveci2(i, 0)).x;
			//repeat all the way to the bottom
			containerIn->setBlockRange(cveci2(worldX, pos00.y - 0x20), cveci2(worldX, pos00.y - 1), b);
		}
	}
}