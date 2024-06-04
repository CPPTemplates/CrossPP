#include "settingsForm.h"
#include "world.h"
#include "gameControl.h"
#include "worldSelector.h"
#include "gameForm.h"
#include "server.h"
settingsForm::settingsForm() :form()
{
	addChildren({ saveAndQuitButton, videoSettingsButton, soundSettingsButton });
}

void settingsForm::render(cveci2& position, const texture& renderTarget)
{
	//transparent
	form::renderChildren(position, renderTarget);
}

void settingsForm::mouseDown(cveci2& position, cmb& button)
{
	control* highestChild = getHighestChild(position);
	if (highestChild == saveAndQuitButton)
	{
		//do NOT disconnect in an uncontrolled way!
		//((gameControl*)parent)->socket.s.socket->disconnect();
		((gameControl*)parent)->socket.shouldDisconnect = true;
	}
	else if (highestChild == videoSettingsButton)
	{
		parent->switchVisibleChild(((gameControl*)parent)->videoOptions);
	}
	else if (highestChild == soundSettingsButton)
	{
		parent->switchVisibleChild(((gameControl*)parent)->soundOptions);
	}
	form::mouseDown(position, button);
}

void settingsForm::layout(crectanglei2& newRect)
{
	form::layout(newRect);
	saveAndQuitButton->layout(crectanglei2((rect.w - buttonSize.x) / 2, rect.h / 2, buttonSize.x, buttonSize.y));
	videoSettingsButton->layout(crectanglei2((rect.w - buttonSize.x) / 2, rect.h / 2 - buttonSize.y, buttonSize.x, buttonSize.y));
	soundSettingsButton->layout(crectanglei2((rect.w - buttonSize.x) / 2, rect.h / 2 - buttonSize.y * 2, buttonSize.x, buttonSize.y));
}