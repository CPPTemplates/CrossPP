#include "enchantmentID.h"
#include <vector>
#pragma once
const std::vector<enchantmentID> incompatibleEnchantments[]
{
	{enchantmentID::sharpness, enchantmentID::smite, enchantmentID::baneOfArthropods },
	{enchantmentID::fortune, enchantmentID::silkTouch },
	{enchantmentID::protection, enchantmentID::blastProtection, enchantmentID::fireProtection, enchantmentID::projectileProtection },
	{enchantmentID::depthStrider, enchantmentID::frostWalker },
	{enchantmentID::infinity, enchantmentID::mending },
	{enchantmentID::multiShot, enchantmentID::piercing },
	{enchantmentID::loyalty, enchantmentID::riptide },
	{enchantmentID::channeling, enchantmentID::riptide },
	{enchantmentID::silkTouch, enchantmentID::looting },
	{enchantmentID::silkTouch, enchantmentID::luckOfTheSea },
};


bool canCombine(const enchantmentID& enchantment0, const enchantmentID& enchantment1);