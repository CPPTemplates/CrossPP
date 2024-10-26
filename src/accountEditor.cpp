#include "accountEditor.h"
#include "client.h"
#include "mainMenu.h"
#include <regex>
#include "gameControl.h"
#include "application/control/controlFunctions.h"

accountEditor::accountEditor()
{
	addChildren({ saveButton, playerNameTextBox });
	playerNameTextBox->text = currentClient->data.name;
}

void accountEditor::layout(crectanglei2& newRect)
{
	form::layout(newRect);
	layoutTableCentered(newRect, children);
	//rectanglei2 r = newRect.rectCentered(buttonSize);
	//saveButton->layout(r);
	//r.y += buttonSize.y + buttonMargin;
	//playerNameTextBox->layout(r);
	//
}

void accountEditor::mouseDown(cveci2& position, cmb& button)
{
	if (getHighestChild(position) == saveButton) {
		const std::wstring& newAccountName = playerNameTextBox->text;
		if (checkAccountName(newAccountName))//account name is valid and accepted
		{
			currentClient->data.name = newAccountName;
			currentClient->data.serialize(true);
			parent->switchVisibleChild(currentMainMenu);
		}
	}
	else {
		form::mouseDown(position, button);
	}
}

void accountEditor::renderBackGround(cveci2& position, const texture& renderTarget)
{
	renderOptionsBackGround(crectanglei2(position, rect.size), renderTarget);
}

bool checkAccountName(const std::wstring& n)
{
	return std::regex_match(n, std::wregex(L"^[A-Za-z0-9_]{3," + std::to_wstring(maxNameLetterCount) + L"}$"));
}
