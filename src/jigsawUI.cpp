#include "jigsawUI.h"
#include "jigsawData.h"
#include "human.h"
#include "dimension.h"
#include "gameControl.h"
#include "block.h"
#include "playerSocket.h"
void jigsawUI::layout(crectanglei2& rect)
{
	form::layout(rect);
	cint middle = rect.w / 2;
	int currentY = rect.h - buttonSize.y;
	cint jointTypeLabelWidth = (int)defaultTheme().font->measureStringSize(measureStringMaximumSize, jointTypeLabel->text).x + defaultTheme().borderSize * 2;
	cint w3 = rect.w / 3;
	targetPoolLabel->layout(crectanglei2(0, currentY, rect.w, buttonSize.y));
	currentY -= buttonSize.y;
	targetPoolTextbox->layout(crectanglei2(0, currentY, rect.w, buttonSize.y));
	currentY -= buttonSize.y;
	nameLabel->layout(crectanglei2(0, currentY, rect.w, buttonSize.y));
	currentY -= buttonSize.y;
	nameTextbox->layout(crectanglei2(0, currentY, rect.w, buttonSize.y));
	currentY -= buttonSize.y;
	targetNameLabel->layout(crectanglei2(0, currentY, rect.w, buttonSize.y));
	currentY -= buttonSize.y;
	targetNameTextbox->layout(crectanglei2(0, currentY, rect.w, buttonSize.y));
	currentY -= buttonSize.y;
	turnsIntoLabel->layout(crectanglei2(0, currentY, rect.w, buttonSize.y));
	currentY -= buttonSize.y;
	turnsIntoTextbox->layout(crectanglei2(0, currentY, rect.w, buttonSize.y));
	currentY -= buttonSize.y;
	jointTypeLabel->layout(crectanglei2(0, currentY, jointTypeLabelWidth, buttonSize.y));
	jointTypeButton->layout(crectanglei2(0 + jointTypeLabelWidth, currentY, jointTypeLabelWidth, buttonSize.y));
	currentY -= buttonSize.y;
	levelsSlider->layout(crectanglei2(0, currentY, w3, buttonSize.y));
	keepJigsawsButton->layout(crectanglei2(w3, currentY, w3, buttonSize.y));
	generateButton->layout(crectanglei2(w3 + w3, currentY, rect.w - w3 - w3, buttonSize.y));
	currentY -= buttonSize.y;
	doneButton->layout(crectanglei2(0, currentY, middle, buttonSize.y));
	cancelButton->layout(crectanglei2(0 + middle, currentY, rect.w - middle, buttonSize.y));
}
void jigsawUI::render(cveci2& position, const texture& renderTarget)
{
	keepJigsawsButton->text = std::wstring(L"Keep Jigsaws: ") + (temporaryJigsawData->keepJigsaws ? std::wstring(L"On") : std::wstring(L"Off"));
	jointTypeButton->text = temporaryJigsawData->rollable ? std::wstring(L"Rollable") : std::wstring(L"Aligned");
	levelsSlider->text = std::wstring(L"levels:") + std::to_wstring((int)levelsSlider->value);
	renderChildren(position, renderTarget);
}
void jigsawUI::mouseDown(cveci2& position, cmb& button)
{
	human* currentHuman = ((gameControl*)parent)->player;
	control* highestChild = getHighestChild(position);
	if (highestChild == jointTypeButton)
	{
		temporaryJigsawData->rollable = !temporaryJigsawData->rollable;
	}
	else if (highestChild == keepJigsawsButton)
	{
		temporaryJigsawData->keepJigsaws = !temporaryJigsawData->keepJigsaws;
	}
	else if (highestChild == doneButton)
	{
		//save
		serializeWithTemporaryData(true);
		*dynamic_cast<jigsawData*>(currentHuman->selectedContainerContainer->getBlockData(currentHuman->selectedContainerPosition)) = *temporaryJigsawData;
		delete temporaryJigsawData;
		visible = false;
		parent->focusChild(nullptr);
	}
	else if (highestChild == cancelButton)
	{
		delete temporaryJigsawData;
		visible = false;
		parent->focusChild(nullptr);
	}
	form::mouseDown(position, button);
}
jigsawUI::jigsawUI()
{
	targetPoolLabel->text = std::wstring(L"Target Pool");
	nameLabel->text = std::wstring(L"Name");
	targetNameLabel->text = std::wstring(L"Target Name");
	turnsIntoLabel->text = std::wstring(L"Turns Into");
	jointTypeLabel->text = std::wstring(L"Joint Type");
	generateButton->text = std::wstring(L"Generate");
	doneButton->text = std::wstring(L"Done");
	cancelButton->text = std::wstring(L"Cancel");
	addChildren({
		targetPoolLabel		,
		 targetPoolTextbox	,
		nameLabel  			,
		 nameTextbox 		,
		targetNameLabel		,
		 targetNameTextbox	,
		turnsIntoLabel 		,
		 turnsIntoTextbox	,
		jointTypeLabel		,
		 jointTypeButton	,
		 levelsSlider		,
		 keepJigsawsButton	,
		 generateButton		,
		 doneButton			,
		 cancelButton
		});
}

void jigsawUI::serializeWithTemporaryData(cbool& write)
{
	serialize(nameTextbox->text, temporaryJigsawData->jigsawName, write);
	serialize(targetNameTextbox->text, temporaryJigsawData->targetName, write);
	serialize(targetPoolTextbox->text, temporaryJigsawData->targetPoolName, write);
	if (write)
	{
		temporaryJigsawData->levels = (int)levelsSlider->value;
	}
	else
	{
		levelsSlider->setValue(temporaryJigsawData->levels);
	}
	if (write)
	{
		temporaryJigsawData->turnsInto = (blockID)blockList.getIDByName(turnsIntoTextbox->text);
		if (temporaryJigsawData->turnsInto == (blockID)-1)
		{
			temporaryJigsawData->turnsInto = blockID::air;
		}
	}
	else
	{
		turnsIntoTextbox->text = blockList[temporaryJigsawData->turnsInto]->name;
	}

}