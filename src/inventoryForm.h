#pragma once
#include "itemStack.h"
#include "application/control/form/form.h"
#include "stackDivider.h"

struct inventory;
struct inventoryForm : form
{
	fp scaleMultiplier = 1;
    stackDivider divider = stackDivider();
	veci2 holdingMousePos = cveci2();
	mb mouseButtonPressing = mb::ButtonCount;
	inventory* inventoryToDisplay = nullptr;
	inventoryForm();

	virtual void linkUp(inventory* newInventoryToDisplay);
	virtual void unLink();
	virtual void mouseDown(cveci2& position, cmb& button) override;
	virtual void mouseMove(cveci2& position, cmb& button) override;
    virtual void mouseUp(cveci2& position, cmb& button) override;
	virtual void render(cveci2& position, const texture& renderTarget) override;
	void updateScale();
};