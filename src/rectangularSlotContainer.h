#pragma once
#include "itemStack.h"
#include "slotContainer.h"
//a rectangular area that contains a rectangle of slots
//derive to outputslotcontainer, inputslotcontainer
struct rectangularSlotContainer :public slotContainer, public IDestructable
{
	veci2 rowsAndColumns = veci2();
	itemStack* slots = nullptr;//array of slots
	rectangularSlotContainer(cveci2& rowsAndColumns);
	inline itemStack* getSlot(cveci2& pos) { return slots + pos.x + pos.y * rowsAndColumns.x; }
	void render(const gameRenderData& targetData, cvec2& pos, cfp& offset, cfp& itemDrawSize);
	bool getSlot(cveci2& mousePos, cveci2& drawPos, cint& offset, cint& itemDrawSize, veci2& slotPosition) const;
	bool addStack(itemStack& stack) override;
	int countAmount(const itemStack& stack) const;
	bool substractStack(itemStack& stack) override;

	virtual ~rectangularSlotContainer() override;
	//converts everything to floating slots
	void dropContent(tickableBlockContainer* containerIn, cvec2& dropPos, cfp& floatingSlotSpeed = maxFloatingSlotSpeed);

	//transfer everything to 'containerTo' or convert it to a floating slot if it does not fit
	void transfer(slotContainer* const containerTo, tickableBlockContainer* containerIn, cvec2& dropPos) const;
	//does not serialize the size!
	virtual void serializeValue(nbtSerializer& s) override;

	virtual void clearData() override;
};