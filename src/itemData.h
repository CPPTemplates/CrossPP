#pragma once
#include "enchantmentID.h"
#include "harvestType.h"
#include "math/graphics/resolutiontexture.h"
#include "IItemComparable.h"
#include "interface/idestructable.h"
#include "idList.h"
struct itemData :IItemComparable, IDestructable
{
	itemData(const itemID& identifier, std::wstring name, resolutionTexture* tex, cint& burningTicks = -1, harvestTierID harvestTier = noHarvestTier, harvestTypeID harvestType = withHand, fp attackDamageModifier = 0, fp attackSpeedModifier = INFINITY, int maxStackSize = 0x40, bool hasHilt = false, armorTierID protectionTier = noArmorTier, armorTypeID protectionType = noArmorType, std::vector<enchantmentID> possibleEnchantments = std::vector<enchantmentID>(), int enchantability = 0) :
		//identifier(identifier), maxStackSize(maxStackSize), tex(tex), name(name), fuelTicks(burningTicks), attackDamageModifier(attackDamageModifier), attackSpeedModifier(attackSpeedModifier), harvestTier(harvestTier), harvestType(harvestType), hasHilt(hasHilt), protectionTier(protectionTier), protectionType(protectionType), possibleEnchantments(possibleEnchantments), enchantability(enchantability) {}
		identifier(identifier), maxStackSize(maxStackSize), tex(tex), name(name), attackDamageModifier(attackDamageModifier), attackSpeedModifier(attackSpeedModifier), harvestTier(harvestTier), harvestType(harvestType), protectionTier(protectionTier), protectionType(protectionType), hasHilt(hasHilt), fuelTicks(burningTicks), enchantability(enchantability), possibleEnchantments(possibleEnchantments) {}
	itemID identifier = (itemID)0;
	int maxStackSize = 0x40;
	resolutionTexture* tex = nullptr;
	std::wstring name = std::wstring();
	fp attackDamageModifier = 1.0;
	fp attackSpeedModifier = 1.0;
	harvestTierID harvestTier = noHarvestTier;
	harvestTypeID harvestType = withHand;
	armorTierID protectionTier = noArmorTier;
	armorTypeID protectionType = noArmorType;
	bool hasHilt = false;
	int fuelTicks = 0;//the amount of time this thing will burn when thrown in a fire

	bool canEat = false;
	int foodPoints = 0;
	fp extraSaturation = 0;
	int enchantability = 0;
	std::vector<enchantmentID> possibleEnchantments = std::vector<enchantmentID>();
	void setEatingValues(cint& foodPoints, cfp& extraSaturation);
	bool compare(const itemID& itemToCompare) const override;
	virtual ~itemData() override;
};
extern idList<itemData*, itemID> itemList;