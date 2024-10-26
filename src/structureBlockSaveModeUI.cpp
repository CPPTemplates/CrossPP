#include "structureBlockSaveModeUI.h"
#include "gameControl.h"
#include "structureBlockData.h"
#include "human.h"
#include "folderList.h"
#include "structure.h"
#include "jigsawPool.h"
#include "dimension.h"
#include "gameControl.h"
#include "playerControlledAI.h"
structureBlockSaveModeUI::structureBlockSaveModeUI()
{
	relativePositionLabel->text = std::wstring(L"Relative Position");
	showInvisibleBlocksLabel->text = std::wstring(L"Show Invisible Blocks");
	sizeLabel->text = std::wstring(L"Structure Size");
	detectLabel->text = std::wstring(L"Detect Structure Size And Position");
	detectButton->text = std::wstring(L"Detect");
	saveButton->text = std::wstring(L"Save");
	includeEntitiesLabel->text = std::wstring(L"Include Entities");
	addChildren({
		relativePositionLabel,
		showInvisibleBlocksLabel,
		relativeXTextbox,
		relativeYTextbox,
		showInvisibleBlocksButton,
		sizeLabel,
		detectLabel,
		sizeXTextbox,
		sizeYTextbox,
		detectButton,
		includeEntitiesLabel,
		includeEntitiesButton,
		saveButton });
	
}

void structureBlockSaveModeUI::serializeWithTemporaryData(cbool& write)
{
	structureBlockUI* structureBlockOptions = (structureBlockUI*)parent;
	if (!write)
	{
		relativeXTextbox->text = std::to_wstring(structureBlockOptions->temporaryStructureBlockData->relativeStructurePosition.x);
		relativeYTextbox->text = std::to_wstring(structureBlockOptions->temporaryStructureBlockData->relativeStructurePosition.y);

		sizeXTextbox->text = std::to_wstring(structureBlockOptions->temporaryStructureBlockData->structureSize.x);
		sizeYTextbox->text = std::to_wstring(structureBlockOptions->temporaryStructureBlockData->structureSize.y);
	}
}

void structureBlockSaveModeUI::layout(crectanglei2& newRect)
{
	form::layout(newRect);
	cint showInvisibleBlocksWidth = (int)defaultTheme().font->measureStringSize(measureStringMaximumSize, showInvisibleBlocksLabel->text).x + defaultTheme().borderSize * 2;
	cint detectLabelWidth = (int)defaultTheme().font->measureStringSize(measureStringMaximumSize, detectLabel->text).x + defaultTheme().borderSize * 2;
	cint includeEntitiesLabelWidth = (int)defaultTheme().font->measureStringSize(measureStringMaximumSize, includeEntitiesLabel->text).x + defaultTheme().borderSize * 2;
	cint smallRightButtonX = rect.w - smallRightButtonSize;
	int currentY = rect.h - buttonSize.y;

	relativePositionLabel->layout(crectanglei2(0, currentY, rect.w, buttonSize.y));
	showInvisibleBlocksLabel->layout(crectanglei2(0 + rect.w - showInvisibleBlocksWidth, currentY, showInvisibleBlocksWidth, buttonSize.y));
	currentY -= buttonSize.y;
	relativeXTextbox->layout(crectanglei2(0, currentY, rect.w / 3, buttonSize.y));
	relativeYTextbox->layout(crectanglei2(0 + rect.w / 3, currentY, rect.w / 3, buttonSize.y));
	showInvisibleBlocksButton->layout(crectanglei2(smallRightButtonX, currentY, smallRightButtonSize, buttonSize.y));
	currentY -= buttonSize.y;
	sizeLabel->layout(crectanglei2(0, currentY, rect.w, buttonSize.y));
	detectLabel->layout(crectanglei2(rect.w - detectLabelWidth, currentY, detectLabelWidth, buttonSize.y));
	currentY -= buttonSize.y;
	sizeXTextbox->layout(crectanglei2(0, currentY, rect.w / 3, buttonSize.y));
	sizeYTextbox->layout(crectanglei2(rect.w / 3, currentY, rect.w / 3, buttonSize.y));
	detectButton->layout(crectanglei2(smallRightButtonX, currentY, smallRightButtonSize, buttonSize.y));
	currentY -= buttonSize.y;
	includeEntitiesLabel->layout(crectanglei2(rect.w - includeEntitiesLabelWidth, currentY, includeEntitiesLabelWidth, buttonSize.y));
	currentY -= buttonSize.y;
	includeEntitiesButton->layout(crectanglei2(smallRightButtonX, currentY, smallRightButtonSize, buttonSize.y));
	currentY -= buttonSize.y;
	saveButton->layout(crectanglei2(smallRightButtonX, currentY, smallRightButtonSize, buttonSize.y));
}

void structureBlockSaveModeUI::mouseDown(cveci2& position, cmb& button)
{
	human* currentHuman = ((gameControl*)parent->parent)->player;
	structureBlockUI* structureBlockOptions = (structureBlockUI*)parent;
	tickableBlockContainer* containerIn = currentHuman->selectedContainerContainer;
	control* highestChild = getHighestChild(position);
	if (highestChild == saveButton)
	{
		const stdPath& path = structureBlockOptions->temporaryStructureBlockData->structureName;
		stdPath absolutePath = structureFolder / path;
		structure* s = new structure(absolutePath);
		cveci2 pos0 = currentHuman->selectedContainerPosition + structureBlockOptions->temporaryStructureBlockData->relativeStructurePosition;
		s->createStructure(*currentHuman->dimensionIn, crectanglei2(pos0, structureBlockOptions->temporaryStructureBlockData->structureSize), structureBlockOptions->temporaryStructureBlockData->includeEntities);
		s->serialize(absolutePath += nbtFileExtension, true);

		structure* oldStructure = getStructureByPath(s->pathWithoutExtension.wstring());
		if (oldStructure)
		{
			structureList.erase(std::find(structureList.begin(), structureList.end(), oldStructure));
			delete oldStructure;
		}
		structureList.push_back(s);
		reloadJigsawPools();
	}
	else if (highestChild == detectButton)
	{

		cint maxStructureSize = 0x100;

		cveci2 absoluteStartPosition = currentHuman->selectedContainerPosition + structureBlockOptions->temporaryStructureBlockData->relativeStructurePosition;

		std::vector<veci2> possibleStructureBlockPositions = containerIn->getAffectedPositions(crectanglei2(absoluteStartPosition, cveci2(maxStructureSize)),
			[containerIn, &nameToFind = structureBlockOptions->temporaryStructureBlockData->structureName](cveci2& pos)
			{
				if (containerIn->getBlockID(pos) == blockID::structure_block)
				{
					structureBlockData* data = dynamic_cast<structureBlockData*>(containerIn->getBlockData(pos));
					return (data->mode == structureBlockMode::cornerMode) && (data->structureName == nameToFind);
				}
				return false;
			});

		if (possibleStructureBlockPositions.size())
		{
			veci2 relativeNearestPosition = possibleStructureBlockPositions[0] - absoluteStartPosition;
			//select the nearest
			for (veci2 pos : possibleStructureBlockPositions)
			{
				cveci2& relativePosition = pos - absoluteStartPosition;
				if (relativePosition.lengthSquared() < relativeNearestPosition.lengthSquared())
				{
					relativeNearestPosition = relativePosition;
				}
			}
			sizeXTextbox->text = std::to_wstring(relativeNearestPosition.x);
			sizeYTextbox->text = std::to_wstring(relativeNearestPosition.y);
		}
	}
	else if (highestChild == includeEntitiesButton)
	{
		structureBlockOptions->temporaryStructureBlockData->includeEntities = !structureBlockOptions->temporaryStructureBlockData->includeEntities;
	}
	form::mouseDown(position, button);
}

void structureBlockSaveModeUI::render(cveci2& position, const texture& renderTarget)
{
	human* currentHuman = ((gameControl*)parent->parent)->player;
	structureBlockUI* structureBlockOptions = (structureBlockUI*)parent;
	showInvisibleBlocksButton->text = structureBlockOptions->showInvisibleBlocks ? std::wstring(L"On") : std::wstring(L"Off");
	includeEntitiesButton->text = structureBlockOptions->temporaryStructureBlockData->includeEntities ? std::wstring(L"On") : std::wstring(L"Off");
	veci2 newRelativePosition = veci2();
	if (convertToInt(relativeXTextbox->text, newRelativePosition.x) &&
		convertToInt(relativeYTextbox->text, newRelativePosition.y))
	{
		structureBlockOptions->temporaryStructureBlockData->relativeStructurePosition = newRelativePosition;
	}
	veci2 newSize = veci2();
	if (convertToInt(sizeXTextbox->text, newSize.x) &&
		convertToInt(sizeYTextbox->text, newSize.y))
	{
		structureBlockOptions->temporaryStructureBlockData->structureSize = newSize;
	}
	cveci2 pos00 = structureBlockOptions->temporaryStructureBlockData->relativeStructurePosition + currentHuman->selectedContainerPosition;
	crectangle2 boundingBoxRect = rectangle2(pos00, structureBlockOptions->temporaryStructureBlockData->structureSize);

	renderBlockRect(boundingBoxRect, ((gameControl*)parent->parent)->getRenderData(renderTarget));
	renderChildren(position, renderTarget);
}