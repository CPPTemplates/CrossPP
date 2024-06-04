#pragma once
#include "particleID.h"
enum class entityID :int
{
	human,
	skeleton,
	stray,
	husk,
	drowned,
	zombie_villager,
	zombie,
	zombiefied_piglin,
	enderman,

	slime,
	magma_cube,
	creeper,

	wolf,

	pig,
	cow,
	sheep,
	chicken,

	cod,

	shulker,

	blaze,
	ghast,
	ender_dragon,

	boat,
	minecart,

	item,
	experience_orb,
	particle,

	snowball = (int)particle + (int)particleID::count,
	egg,
	ender_pearl,
	ender_eye,
	fishing_bobber,
	arrow,

	ghast_fireball,
	dragon_fireball,
	small_Fireball,

	tnt,
	end_crystal,

	falling_block,
	piston,

	firework_rocket,
	pollen,

	count,
};
constexpr int mobTypeCount = (int)entityID::ender_dragon + 1;