#pragma once
#include "application/control/form/form.h"
#include "application/control/label.h"
#include "application/control/textBox.h"
#include "application/control/button.h"
#include "constants.h"
struct worldCreator : form
{
	world* worldCreating;
	worldCreator();
	label* worldNameLabel = new label(L"World Name");
	textBox* worldNameTextBox = new textBox();
	label* willBeSavedInLabel = new label();
	label* worldSeedLabel = new label(L"Seed For The World Generator");
	textBox* worldSeedTextBox = new textBox();
	button* createButton = new button(L"Create New World");
	button* cancelButton = new button(L"Cancel");
	button* cheatsButton = new button();
	void onClick(const controlEventArgs & args);
	virtual void keyPress(cvk& key) override;
	virtual void layout(crectanglei2& newRect) override;
	void render(cveci2& position, const texture& renderTarget) override;
	void cancelWorldCreation();
	virtual ~worldCreator() override;
};
extern worldCreator* currentWorldCreator;