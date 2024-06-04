#pragma once
#include "interface/idestructable.h"
#include "nbt/nbtSerializable.h"
#include "itemID.h"
#include "itemTag.h"
#include "enchantment.h"
#include "math/graphics/texture.h"
#include "math/rectangle/rectangletn.h"
#include "math/mattnxn.h"
#include "gameRenderData.h"
struct itemStack : IDestructable, nbtSerializable
{
	itemStack(itemID stackItemID = (itemID)0, cint& count = 0, itemTag* data = nullptr, std::vector<enchantment*> enchantments = std::vector<enchantment*>()) :
		stackItemID(stackItemID), count(count), data(data ? data : createItemTag(stackItemID)), enchantments(enchantments) {}

	itemStack(const itemStack& other);

	//can also be a negative value
	void add(cint& count);
	bool addStack(itemStack& s, int amount);
	bool addStack(itemStack& s);

	inline itemStack& operator=(itemStack copy)
	{
		this->swap(copy);
		return *this;
	}

	void swap(itemStack& with);

    //will return true if:
    //both stacks are empty
    //both stacks have the exact same stuff
	bool compare(const itemStack& other) const;

	void clearData();
	virtual ~itemStack() override;

	itemID stackItemID = (itemID)0;
	int count = 0;

	itemTag* data = nullptr;
	std::vector<enchantment*> enchantments = std::vector<enchantment*>();

	void render(crectangle2& rect, const gameRenderData& targetData) const;
	void renderSingleItem(const gameRenderData& targetData) const;

	int getEnchantmentLevel(const enchantmentID& identifier) const;
	void drawToolTips(cveci2& position, const texture& renderTarget) const;
	virtual void serializeValue(nbtSerializer& s) override;
};