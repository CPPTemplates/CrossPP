#include "gameSelector.h"
#include "world.h"
#include "saveFileControl.h"
#include "gameControl.h"
#include "gameForm.h"
#include "keyID.h"
#include "filesystem/filemanager.h"
#include "folderList.h"
#include "mainMenu.h"
#include "include/application/control/form/messageBox.h"
gameSelector::gameSelector() : form()
{
	// add gamelistcontrol first, so it will be at the bottom of the controls
	addChildren({gameListControl, openButton, newButton, deleteButton, refreshButton});
	// addEventHandler(&gameSelector::onClick,openButton->onClick);
	addEventHandlers(&gameSelector::onClick, openButton->onClick, newButton->onClick, deleteButton->onClick, refreshButton->onClick);
	// addEventHandlers(&gameSelector::onClick,openButton->onClick);
	// openButton->onClick.hook(&gameSelector::onClick, this);
}

void gameSelector::onClick(const controlEventArgs &args)
{
	if (&args.sender == openButton)
	{
		// open the selected world
		if (gameListControl->selectedChild)
		{
			openGame(gameListControl->selectedChild);
		}
	}
	else if (&args.sender == newButton)
	{
		// create a new world
		visible = false;
		addGame();
	}
	else if (&args.sender == refreshButton)
	{
		refresh();
	}
	else if (&args.sender == deleteButton)
	{
		// delete the selected world
		if (gameListControl->selectedChild)
		{
			// ask for confirmation
			// TODO: make a new messagebox function
			showMessageBox(L"Are You Sure You Want To Delete This " + gameTypeName + L"?", {L"Yes", L"Cancel"}, this, [this](const messageBoxEventArgs &args)
						   {
							if(args.buttonIndex == 0){	
								deleteGame(gameListControl->selectedChild);
								refresh();
								} });
			// const std::wstring& messageString = L"Are You Sure You Want To Delete This " + gameTypeName + L"?";
			// if (MessageBox(NULL, messageString.c_str(), gameName.c_str(), MB_OKCANCEL | MB_ICONWARNING) == IDOK)
			//{
			//	deleteGame(c);
			//	refresh();
			// }
		}
	}
}

void gameSelector::refresh()
{
	for (size_t i = 0; i < gameListControl->children.size; i++)
	{
		control *c = gameListControl->children[i];
		if (c != gameListControl->sideSlider)
		{
			delete c;
			gameListControl->children.erase(i);
		}
	}
	addGameControls();
	gameListControl->children.update();
	gameListControl->focusedChild = nullptr;
	if (hasBeenLayout())
	{
		gameListControl->reOrganizeChildControls();
	}
}

void gameSelector::keyDown(cvk &key)
{
	if (key == (vk)keyID::escape)
	{
		visible = false;
		currentMainMenu->visible = true;
		mainForm->focusedChild = currentMainMenu;
	}
}

void gameSelector::layout(crectanglei2 &newRect)
{
	form::layout(newRect);
	openButton->layout(crectanglei2(0, 0, rect.w / 2, (int)defaultTheme().font->fontSize + defaultTheme().borderSize * 2));
	newButton->layout(crectanglei2(openButton->rect.w, 0, rect.w - openButton->rect.w, openButton->rect.h));
	deleteButton->layout(crectanglei2(0, openButton->rect.h, rect.w / 2, (int)defaultTheme().font->fontSize + defaultTheme().borderSize * 2));
	refreshButton->layout(crectanglei2(openButton->rect.w, openButton->rect.h, rect.w - openButton->rect.w, openButton->rect.h));

	cint optionSpace = openButton->rect.h * 2;

	gameListControl->childSize = cveci2(rect.w / 2, rect.h / 5);
	crectanglei2 saveFolderListControlRect = crectanglei2((rect.w - gameListControl->childSize.x) / 2, optionSpace, gameListControl->childSize.x + gameListControl->sideSliderWidth, rect.h - optionSpace);
	gameListControl->layout(saveFolderListControlRect);
}

void gameSelector::render(cveci2 &position, const texture &renderTarget)
{
	renderOptionsBackGround(crectanglei2(position, rect.size), renderTarget);
	renderChildren(position, renderTarget);
}