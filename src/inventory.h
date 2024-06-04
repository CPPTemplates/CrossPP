#pragma once
#include "math/graphics/resolutiontexture.h"
#include "slotContainer.h"
#include "interface/idestructable.h"
#include "uiSlotContainer.h"
struct stackDivider;

constexpr rectanglei2 defaultUIRectangle = crectanglei2(0, 90, 176, 166);
struct inventory : slotContainer, IDestructable
{
	human* linkedPlayer = nullptr;
	resolutionTexture* uiTexture = nullptr;
	rectanglei2 uiTexturePartToDraw = defaultUIRectangle;//default

	std::vector<uiSlotContainer*> containers = std::vector<uiSlotContainer*>();
	uiSlotContainer* hotbarSlots = new uiSlotContainer(cveci2(0x8, 0x8), nullptr);
	uiSlotContainer* inventorySlots = new uiSlotContainer(cveci2(0x8, 30), nullptr);

    std::vector<itemStack*> slotsDividingOver = std::vector<itemStack*>();

	virtual void clickedOnItem(cmb& button, stackDivider& divider, uiSlotContainer* selectedSlotContainer, veci2 selectedSlot);
	void drawExtraData(crectanglei2& textureRect, cveci2& offset, cmat3x3& transform, const texture& renderTarget) const;
	virtual void drawExtraData(cmat3x3& transform, const texture& renderTarget);
	virtual ~inventory() override;
	virtual void mouseDown(cveci2& pixelPosition, cmb& button, stackDivider& divider);
    virtual void mouseMove(cveci2& pixelPosition, cmb& button, stackDivider& divider);
	virtual void drawToolTips(cveci2& mouseTexturePosition, cveci2& mousePositionPixels, const texture& renderTarget);
	virtual bool canAddStack(uiSlotContainer* containerToAddTo, stackDivider& s);

	uiSlotContainer* getSlotContainer(cveci2& mousePositionPixels, veci2& slotPosition);
};