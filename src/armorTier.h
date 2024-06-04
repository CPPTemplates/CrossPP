#pragma once
enum armorTierID :int
{
	noArmorTier,
	turtleArmorTier,
	leatherArmorTier,
	goldArmorTier,
	chainmailArmorTier,
	ironArmorTier,
	diamondArmorTier,
	netheriteArmorTier
};
//no tier does not count
constexpr int armorTierCount = netheriteArmorTier;