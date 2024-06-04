#include "decayedStructureFeature.h"
#include "structureFeature.h"
#include "gameControl.h"
#include "folderList.h"
#include "structure.h"
#include "structureBlockLoadModeUI.h"
#include "constants.h"
#include "human.h"
#include "dimension.h"
#include "structureBlockData.h"
void structureBlockLoadModeUI::layout(crectanglei2& newRect)
{
	form::layout(newRect);
	cint showBoundingBoxWidth = (int)defaultTheme().font->measureStringSize(measureStringMaximumSize, showBoundingBoxLabel->text).x + defaultTheme().borderSize * 2;
	cint includeEntitiesLabelWidth = (int)defaultTheme().font->measureStringSize(measureStringMaximumSize, includeEntitiesLabel->text).x + defaultTheme().borderSize * 2;
	cint smallRightButtonSize = buttonSize.y * 4;
	cint smallRightButtonX = rect.w - smallRightButtonSize;
	//cint modeButtonSize = rect.w - smallRightButtonSize;

	cint w3 = rect.w / 3;
	int currentY = rect.h - buttonSize.y;

	relativePositionLabel->layout(crectanglei2(0, currentY, rect.w, buttonSize.y));
	showBoundingBoxLabel->layout(crectanglei2(0 + rect.w - showBoundingBoxWidth, currentY, showBoundingBoxWidth, buttonSize.y));
	currentY -= buttonSize.y;
	relativeXTextbox->layout(crectanglei2(0, currentY, rect.w / 3, buttonSize.y));
	relativeYTextbox->layout(crectanglei2(0 + rect.w / 3, currentY, rect.w / 3, buttonSize.y));
	showBoundingBoxButton->layout(crectanglei2(smallRightButtonX, currentY, smallRightButtonSize, buttonSize.y));
	currentY -= buttonSize.y;
	integrityAndSeedLabel->layout(crectanglei2(0, currentY, rect.w, buttonSize.y));
	currentY -= buttonSize.y;
	integritySlider->layout(crectanglei2(0, currentY, rect.w / 2, buttonSize.y));
	seedTextbox->layout(crectanglei2(rect.w / 2, currentY, w3, buttonSize.y));
	currentY -= buttonSize.y * 2;
	includeEntitiesLabel->layout(crectanglei2(rect.w - includeEntitiesLabelWidth, currentY, includeEntitiesLabelWidth, buttonSize.y));
	currentY -= buttonSize.y;
	includeEntitiesButton->layout(crectanglei2(smallRightButtonX, currentY, smallRightButtonSize, buttonSize.y));
	currentY -= buttonSize.y;
	mirrorYButton->layout(crectanglei2(0, currentY, smallRightButtonSize, buttonSize.y));
	rotationSlider->layout(crectanglei2(smallRightButtonSize, currentY, smallRightButtonX - smallRightButtonSize, buttonSize.y));
	loadButton->layout(crectanglei2(smallRightButtonX, currentY, smallRightButtonSize, buttonSize.y));
}

void structureBlockLoadModeUI::mouseDown(cveci2& position, cmb& button)
{
	human* currentHuman = ((gameControl*)parent->parent)->player;
	control* highestChild = getHighestChild(position);
	structureBlockUI* structureBlockOptions = (structureBlockUI*)parent;
	if (highestChild == loadButton)
	{
		structure* s = getStructureByPath(((gameControl*)parent)->structureBlockOptions->temporaryStructureBlockData->structureName);
		if (s)
		{
			structureFeature featureToUse;
			if (integritySlider->value < 1)
			{
				featureToUse = decayedStructureFeature(&currentRandom, integritySlider->value, 0, false);
			}
			else
			{
				featureToUse = structureFeature(false);
			}

			structureBlockOptions->temporaryStructureBlockData->structureSize.x = (int)s->blockIDArray.size.x;
			structureBlockOptions->temporaryStructureBlockData->structureSize.y = (int)s->blockIDArray.size.y;
			structureBlockOptions->saveUI->sizeXTextbox->text = std::to_wstring(s->blockIDArray.size.x);
			structureBlockOptions->saveUI->sizeYTextbox->text = std::to_wstring(s->blockIDArray.size.y);

			featureToUse.placeStructurePart(s, currentHuman->selectedContainerContainer,
				currentHuman->selectedContainerPosition + structureBlockOptions->temporaryStructureBlockData->relativeStructurePosition,
				structureBlockOptions->temporaryStructureBlockData->mirrorX, 0,
				structureBlockOptions->temporaryStructureBlockData->includeEntities, false);
		}
	}
	else if (highestChild == includeEntitiesButton)
	{
		structureBlockOptions->temporaryStructureBlockData->includeEntities = !structureBlockOptions->temporaryStructureBlockData->includeEntities;
	}
	form::mouseDown(position, button);
}

void structureBlockLoadModeUI::render(cveci2& position, const texture& renderTarget)
{
	human* currentHuman = ((gameControl*)parent->parent)->player;
	structureBlockUI* structureBlockOptions = (structureBlockUI*)parent;
	mirrorYButton->text = std::wstring(L"Mirror Y:") + std::wstring(structureBlockOptions->temporaryStructureBlockData->mirrorX ? std::wstring(L"On") : std::wstring(L"Off"));
	structureBlockOptions->temporaryStructureBlockData->structureIntegrity = integritySlider->value;
	structureBlockOptions->temporaryStructureBlockData->structureSeed = std::hash<std::wstring>()(seedTextbox->text);
	includeEntitiesButton->text = structureBlockOptions->temporaryStructureBlockData->includeEntities ? std::wstring(L"On") : std::wstring(L"Off");
	veci2 newRelativePosition = veci2();
	if (convertToInt(relativeXTextbox->text, newRelativePosition.x) &&
		convertToInt(relativeYTextbox->text, newRelativePosition.y))
	{
		structureBlockOptions->temporaryStructureBlockData->relativeStructurePosition = newRelativePosition;
	}
	rotationSlider->setValue(math::floor((int)rotationSlider->value, 90));
	structure* s = getStructureByPath(structureBlockOptions->temporaryStructureBlockData->structureName);
	if (s)
	{
		cveci2 pos00 = structureBlockOptions->temporaryStructureBlockData->relativeStructurePosition + currentHuman->selectedContainerPosition;
		crectangle2 boundingBoxRect = rectangle2(pos00, s->blockIDArray.size);

		//renderBlockRect(boundingBoxRect, renderTarget);
	}
	renderChildren(position, renderTarget);
}

structureBlockLoadModeUI::structureBlockLoadModeUI()
{
	relativePositionLabel->text = std::wstring(L"Relative Position");
	integrityAndSeedLabel->text = std::wstring(L"Structure Integrity and Seed");
	includeEntitiesLabel->text = std::wstring(L"Include Entities");
	showBoundingBoxLabel->text = std::wstring(L"Show boundig box");
	loadButton->text = std::wstring(L"Load");
	integritySlider->text = std::wstring(L"Integrity");

	addChildren({
		relativePositionLabel,
		showBoundingBoxLabel,
		relativeXTextbox ,
		relativeYTextbox ,
		showBoundingBoxButton,
		integrityAndSeedLabel,
		integritySlider,
		seedTextbox,
		includeEntitiesLabel,
		includeEntitiesButton,
		mirrorYButton ,
		rotationSlider ,
		loadButton
		});
	
}

void structureBlockLoadModeUI::serializeWithTemporaryData(cbool& write)
{
	structureBlockUI* structureBlockOptions = (structureBlockUI*)parent;
	serialize(integritySlider->value, structureBlockOptions->temporaryStructureBlockData->structureIntegrity, write);
	if (write)
	{
		structureBlockOptions->temporaryStructureBlockData->structureRotation = (int)rotationSlider->value;
		long seed;
		if (convertToLong(seedTextbox->text, seed))
		{
			structureBlockOptions->temporaryStructureBlockData->structureSeed = seed;
		}
	}
	else
	{
		rotationSlider->setValue(structureBlockOptions->temporaryStructureBlockData->structureRotation);
		seedTextbox->text = std::to_wstring(structureBlockOptions->temporaryStructureBlockData->structureSeed);
		relativeXTextbox->text = std::to_wstring(structureBlockOptions->temporaryStructureBlockData->relativeStructurePosition.x);
		relativeYTextbox->text = std::to_wstring(structureBlockOptions->temporaryStructureBlockData->relativeStructurePosition.y);
	}
}