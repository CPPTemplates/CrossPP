
#include "woodtypeID.h"
#include "treeItemTypeID.h"
#include "colorID.h"
#include "coralStateID.h"
#include "coralColorID.h"
#include "coralTypeID.h"
#include "sandstoneTypeID.h"
#include "sandstoneColorID.h"
#include "mushroomColorID.h"
#include "constants.h"
#include "entityID.h"
#include "constants.h"
#include "harvestTier.h"
#include "toolTypeID.h"
#include "armorTier.h"
#include "armorType.h"

//https://www.digminecraft.com/lists/item_id_list_pc.php

constexpr int normalTreeTypeCount = 6;
constexpr int netherTreeTypeCount = 2;

constexpr int musicDiscTypeCount = 13;
#pragma once
enum class itemID : int
{
	air,
	structure_void,
	barrier,
	water,
	lava,

	wood_block,

	coal_block = ((int)wood_block + (int)woodTypeID::count * (int)treeItemTypeID::count),
	iron_block,
	emerald_block,
	lapis_block,
	redstone_block,
	gold_block,
	diamond_block,
	quartz_block,

	coal_ore,
	iron_ore,
	emerald_ore,
	lapis_ore,
	redstone_ore,
	gold_ore,
	diamond_ore,
	nether_quartz_ore,
	nether_gold_ore,

	stone,

	cobblestone,
	smooth_stone,

	stone_bricks,
	chiseled_stone_bricks,

	end_stone,
	end_stone_bricks,

	obsidian,
	crying_obsidian,

	bone_block,

	netherrack,

	chiseled_quartz_block,
	quartz_pillar,
	smooth_quartz,
	quartz_bricks,

	prismarine,
	prismarine_bricks,
	darkprismarine,

	basalt,
	polishedbasalt,

	blackstone,
	polished_blackstone,
	chiseledpolished_blackstone,
	gilded_blackstone,

	magma_block,
	ancient_debris,

	polished_blackstone_bricks,
	crackedpolished_blackstone_bricks,

	bricks,

	nether_bricks,
	rednether_bricks,
	crackednether_bricks,
	chiselednether_bricks,

	purpur_block,
	purpur_pillar,

	mossy_cobblestone,
	mossy_stone_bricks,

	cracked_stone_bricks,

	infested_mossy_stone_bricks,
	infested_cracked_stone_bricks,
	infested_stone,
	infested_cobblestone,
	infested_stone_bricks,
	infested_chiseled_stone_bricks,

	end_stone_brick_wall,
	brick_wall,
	cobble_stone_wall,
	mossy_cobblestone_wall,
	nether_brick_wall,
	rednether_brick_wall,
	polished_blackstone_wall,
	andesite_wall,
	blackstone_wall,
	diorite_wall,
	granite_wall,
	polished_blackstone_bricks_wall,
	prismarine_wall,
	stone_brick_wall,
	mossystone_brick_wall,
	sandstone_wall,
	redsandstone_wall,

	wood_stairs,
	stonestairs = wood_stairs + (int)woodTypeID::count,
	granite_stairs,
	polished_granite_stairs,
	diorite_stairs,
	polished_diorite_stairs,
	andesite_stairs,
	polished_andesite_stairs,
	cobblestone_stairs,
	mossycobblestone_stairs,
	stone_brick_stairs,
	mossystone_brick_stairs,
	brick_stairs,
	end_stone_brick_stairs,
	nether_brick_stairs,
	rednether_brick_stairs,
	sandstone_stairs,
	smoothsandstone_stairs,
	redsandstone_stairs,
	smoothredsandstone_stairs,
	quartz_stairs,
	smooth_quartz_stairs,
	purpur_stairs,
	prismarine_stairs,
	prismarine_brick_stairs,
	darkprismarine_stairs,
	blackstone_stairs,
	polished_blackstone_stairs,
	polished_blackstone_brick_stairs,

	wood_slab,
	stone_slab = wood_slab + +(int)woodTypeID::count,
	smooth_stone_slab,
	granite_slab,
	polished_granite_slab,
	diorite_slab,
	polished_diorite_slab,
	andesite_slab,
	polished_andesite_slab,
	cobblestone_slab,
	mossycobblestone_slab,
	stone_brick_slab,
	mossystone_brick_slab,
	brick_slab,
	end_stone_brick_slab,
	nether_brick_slab,
	rednether_brick_slab,
	sandstone_slab,
	cutsandstone_slab,
	smoothsandstone_slab,
	redsandstone_slab,
	cutredsandstone_slab,
	smoothredsandstone_slab,
	quartz_slab,
	smooth_quartz_slab,
	purpur_slab,
	prismarine_slab,
	prismarine_brick_slab,
	darkprismarine_slab,
	blackstone_slab,
	polished_blackstone_slab,
	polished_blackstone_brick_slab,

	wool,
	terracotta = wool + (int)colorID::count,
	glazed_terracotta = terracotta + (int)colorID::count,

	glass = glazed_terracotta + (int)colorID::count,
	stained_glass,

	glass_pane = stained_glass + (int)colorID::count,
	stained_glass_pane,

	fire = stained_glass_pane + (int)colorID::count,
	soul_fire,

	coral,

	frosted_ice = coral + (int)coralStateID::count * (int)coralColorID::count * (int)coralTypeID::count,
	ice,
	packed_ice,
	blue_ice,

	sponge,
	wet_sponge,

	slime,
	honey,

	mushroom_stem,
	red_mushroom_block,
	brown_mushroom_block,
	red_mushroom,
	brown_mushroom,

	bamboo_shoot,
	bamboo,

	clay,
	dirt,
	coarse_dirt,
	bonebed_dirt,

	grass_path,
	grass_block,
	podzol,
	mycelium,
	crimson_nylium,
	warped_nylium,
	sea_lantern,
	glowstone,
	shroomlight,
	redstone_lamp,

	crafting_table,
	cartography_table,
	smithing_table,
	loom,

	furnace,
	blast_furnace,
	smoker,
	barrel,
	chest,
	trapped_chest,
	ender_chest,
	dispenser,
	dropper,
	enchanting_table,
	anvil,
	grindstone,

	fletchingtable,

	beacon,

	conduit,

	lectern,
	composter,
	cake,
	scaffolding,
	respawn_anchor,
	lodestone,
	target,
	bell,
	torch,
	soultorch,
	ladder,
	farmland,
	snow,
	snow_block,
	lantern,
	soul_lantern,
	sea_pickle,
	flower_pot,
	chain,
	turtle_egg,

	bookshelf,
	haybale,
	tnt,
	cobweb,
	brewing_stand,

	cauldron,

	hopper,

	redstone_wire,
	redstone_torch,

	repeater,
	comparator,

	wood_button,
	stone_button = wood_button + (int)woodTypeID::count,
	blackstone_button,

	wood_pressure_plate,
	stone_pressure_plate = wood_pressure_plate + (int)woodTypeID::count,
	polished_blackstone_pressure_plate,
	lightweighted_pressure_plate,
	heavy_pressure_plate,

	lever,

	daylight_detector,

	observer,

	note_block,
	jukebox,

	tripwire,
	tripwire_hook,

	piston,
	sticky_piston,

	rail,
	powered_rail,
	detector_rail,
	activator_rail,

	portal,
	end_portal,
	end_portal_frame,

	spawner,

	dead_bush,
	grass,
	fern,
	tall_grass,
	large_fern,

	crimson_roots,
	warped_roots,

	vine,

	weeping_vines,
	twisting_vines,

	dandelion,
	poppy,
	blue_orchid,
	allium,
	azure_bluet,
	red_tulip,
	pink_tulip,
	white_tulip,
	orange_tulip,
	oxeye_daisy,
	corn_flower,
	lily_of_the_valley,
	wither_rose,
	sunflower,
	lilac,
	rose_bush,
	peony,

	sandstone,

	bed = sandstone + (int)sandStoneTypeID::count * (int)sandStoneColorID::count,

	campfire = bed + (int)colorID::count,
	soul_campfire,

	end_rod,
	chorus_plant,
	chorus_flower,
	cactus,
	sugar_cane,
	kelp,
	dried_kelp_block,

	wheat,
	carrots,
	potatoes,
	beetroots,
	nether_wart,

	sweet_berry_bush,

	melon_stem,
	pumpkin_stem,

	melon,
	pumpkin,
	carved_pumpkin,
	jack_o_lantern,

	soul_soil,

	concrete,

	concrete_powder = concrete + (int)colorID::count,

	soul_sand = concrete_powder + (int)colorID::count,
	sand,
	red_sand,

	gravel,

	andesite,
	polished_andesite,
	diorite,
	polished_diorite,
	granite,
	polished_granite,

	wood_sapling,
	iron_trapdoor = wood_sapling + (int)woodTypeID::count,
	wood_trapdoor,

	iron_door = wood_trapdoor + (int)woodTypeID::count,
	wood_door = iron_door + 1,

	iron_bars = wood_door + (int)woodTypeID::count,

	wood_fence_gate,
	wood_fence = wood_fence_gate + (int)woodTypeID::count,
	nether_brick_fence = wood_fence + (int)woodTypeID::count,

	dragon_egg,
	jigsaw,
	structure_block,
	bedrock,
	//this is where the itemIDs start
	dye,
	music_disc = dye + (int)colorID::count,
	stick = music_disc + musicDiscTypeCount,
	charcoal,
	clay_ball,
	leather,
	paper,
	book,
	enchanted_book,
	blaze_rod,
	blaze_powder,
	gunpowder,
	ghast_tear,
	magma_cream,
	flint,
	feather,
	string,
	arrow,
	brick,
	nether_brick,
	clock,
	fermented_spider_eye,
	glowstone_dust,
	heart_of_the_sea,
	honey_comb,
	ink_sac,
	nautilus_shell,
	nether_star,
	netherite_ingot,
	netherite_scrap,
	phantom_membrane,
	popped_chorus_fruit,
	prismarine_crystals,
	prismarine_shard,
	rabbit_hide,
	rabbit_foot,
	scute,
	shulker_shell,
	slime_ball,
	firework_star,
	firework_rocket,

	glass_bottle,
	honey_bottle,
	experience_bottle,
	dragon_breath,

	awkward_potion,
	mundane_potion,
	thick_potion,

	potion,
	splash_potion,
	lingering_potion,

	bow,
	egg,
	fishing_rod,
	snowball,
	ender_pearl,
	ender_eye,

	end_crystal,

	wheat_seeds,
	carrot,
	potato,
	beetroot,

	melon_seeds,
	pumpkin_seeds,
	glistering_melon_slice,
	melon_slice,
	poisonous_potato,
	sweet_berries,
	chorus_fruit,
	dried_kelp,
	apple,
	beetroot_soup,
	baked_potato,
	rotten_flesh,
	golden_carrot,
	porkchop,
	cooked_porkchop,
	beef,
	cooked_beef,
	mutton,
	cooked_mutton,
	chicken,
	cooked_chicken,
	rabbit,
	cooked_rabbit,
	cod,
	cooked_cod,
	salmon,
	cooked_salmon,
	tropical_fish,
	pufferfish,
	spider_eye,
	bread,
	golden_apple,
	enchanted_golden_apple,
	mushroom_stew,
	rabbit_stew,

	bowl,
	bucket,
	water_bucket,
	lava_bucket,
	milk_bucket,
	flint_and_steel,

	bone,
	bone_meal,

	//ore drops respectively
	coal,
	iron_ingot,
	emerald,
	lapis_lazuli,
	redstone,
	gold_ingot,
	diamond,
	quartz,
	goldnugget,
	ironnugget,

	spawn_egg,

	minecart = spawn_egg + mobTypeCount,
	chest_minecart,
	furnace_minecart,
	hopper_minecart,
	tnt_minecart,
	command_block_minecart,

	wood_boat,
	material_tool = wood_boat + normalTreeTypeCount,//x: tool type, y: tool tier
	shears = material_tool + toolTierCount * materialToolTypeCount,
	elytra,
	turtle_helmet,
	material_armor,//x: armor type, y: armor tier
	count = material_armor + (armorTierCount - 1) * armorTypeCount
};

typedef itemID blockID;

constexpr int blockIDCount = (int)itemID::dye;

constexpr int stairTypeCount = (int)blockID::polished_blackstone_brick_stairs + 1 - (int)blockID::wood_stairs;
constexpr int slabTypeCount = (int)blockID::polished_blackstone_brick_slab + 1 - (int)blockID::wood_slab;
constexpr int fenceTypeCount = (int)blockID::nether_brick_fence + 1 - (int)blockID::wood_fence;
constexpr int flowerTypeCount = (int)blockID::peony + 1 - (int)blockID::dandelion;
constexpr int wallTypeCount = (int)blockID::sandstone_wall + 1 - (int)blockID::end_stone_brick_wall;
constexpr int railTypeCount = (int)blockID::activator_rail + 1 - (int)blockID::rail;
constexpr int pressurePlateTypeCount = (int)blockID::heavy_pressure_plate + 1 - (int)blockID::wood_pressure_plate;
constexpr int buttonTypeCount = (int)blockID::blackstone_button + 1 - (int)blockID::wood_button;
constexpr int stemPlantTypeCount = (int)blockID::pumpkin + 1 - (int)blockID::melon;
constexpr int furnaceTypeCount = (int)blockID::smoker + 1 - (int)blockID::furnace;
constexpr int mushroomBlockTypeCount = 1 + (int)mushroomColorID::count;
constexpr int grassBlockTypeCount = (int)blockID::warped_nylium + 1 - (int)blockID::grass_block;

constexpr int mineCartItemTypeCount = (int)itemID::command_block_minecart + 1 - (int)itemID::minecart;
