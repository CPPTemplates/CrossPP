#include "array/arrayFunctions/arrayFunctions.h"
#include "statusEffectID.h"
#include "itemID.h"
#include "biomeID.h"
#include "noteTypeID.h"
#include "arrayDataType.h"
#include "armorTier.h"
#include "armorType.h"
#include "itemID.h"
#include "colorID.h"
#include "constants.h"
#include "entityID.h"
#include "harvestTier.h"
#include "globalFunctions.h"
#include "optimization/optimization.h"
#include "levelID.h"
#include "lightLevelID.h"
#include "particleID.h"
#include "toolTypeID.h"
#include "treeItemTypeID.h"
#include "woodtypeID.h"
#include "gameModeID.h"

#pragma once

// declarations
constexpr bool isSerializable(const entityID &identifier) noexcept;

constexpr bool isFire(const blockID &identifier) noexcept;

constexpr bool isSponge(const blockID &identifier) noexcept;

constexpr bool hasFluidData(const blockID &identifier) noexcept;

constexpr bool hasFluidData(const blockID &identifier) noexcept;

constexpr bool isTreeType(const blockID &identifier) noexcept;

constexpr bool isMushroom(const blockID &identifier) noexcept;

constexpr bool diesWithoutLight(const blockID &identifier) noexcept;

constexpr bool isMushroomBlock(const blockID &identifier) noexcept;

constexpr woodTypeID getWoodType(const blockID &identifier) noexcept;

constexpr treeItemTypeID getTreeItemType(const blockID &identifier) noexcept;

constexpr blockID getTreeBlock(const woodTypeID &typeID, const treeItemTypeID &itemTypeID);

constexpr bool isTrapDoor(const blockID &identifier) noexcept;

constexpr bool isDoor(const blockID &identifier) noexcept;

constexpr bool isStairs(const blockID &identifier) noexcept;

constexpr bool isSlab(const blockID &identifier) noexcept;

constexpr bool isFence(const blockID &identifier) noexcept;

constexpr bool isWall(const blockID &identifier) noexcept;

constexpr bool isConnectingBlock(const blockID &identifier) noexcept;

constexpr bool isFenceGate(const blockID &identifier) noexcept;

constexpr bool isGlazedTerracotta(const blockID &identifier) noexcept;

constexpr bool isSapling(const blockID &identifier) noexcept;

constexpr bool isBed(const blockID &identifier) noexcept;

constexpr bool isFlower(const blockID &identifier) noexcept;

constexpr bool isDoubleBlockWhenPlaced(const blockID &identifier) noexcept;

constexpr bool isDoubleBlock(const blockID &identifier) noexcept;

constexpr bool isSand(const blockID &identifier) noexcept;

constexpr bool isFallingBlock(const blockID &identifier) noexcept;

constexpr bool needsBlockUpdate(const blockID &identifierlock) noexcept;

constexpr bool isBoat(const itemID &item) noexcept;

constexpr bool isDispenser(const blockID &identifier) noexcept;

constexpr bool isPiston(const blockID &identifier) noexcept;

constexpr bool isBoatOrMinecart(const itemID &item) noexcept;

constexpr bool hasFacingData(const blockID &identifier) noexcept;

constexpr bool attachedToBottomBlock(const blockID &identifier) noexcept;

constexpr bool hasAttachedBlockData(const blockID &identifier) noexcept;

constexpr bool hasPollenData(const blockID &identifier) noexcept;

constexpr bool hasGrassBlockData(const blockID &identifier) noexcept;

constexpr bool isGrassBlock(const blockID &identifier) noexcept;

constexpr bool isNylium(const blockID &identifier) noexcept;

constexpr bool isChest(const blockID &identifier) noexcept;

constexpr bool isGrass(const blockID &identifier) noexcept;

constexpr bool isOverworldLeaves(const blockID &identifier) noexcept;

constexpr bool isRoots(const blockID &identifier) noexcept;

constexpr bool isFungus(const blockID &identifier) noexcept;

constexpr bool isShortGrass(const blockID &identifier) noexcept;

constexpr bool isTallGrass(const blockID &identifier) noexcept;

constexpr bool hasChestData(const blockID &identifier) noexcept;

constexpr bool hasAgeData(const blockID &identifier);

constexpr bool keepsFireBurning(const blockID &identifier);

constexpr bool isNetherForest(const biomeID &identifier) noexcept;

constexpr bool isForest(const biomeID &identifier) noexcept;

constexpr bool hasOpenData(const blockID &identifier) noexcept;

constexpr bool hasUpsideDownData(const blockID &identifier) noexcept;

constexpr bool invertPlacementDirection(const blockID &identifier) noexcept;

constexpr bool isRail(const blockID &identifier) noexcept;

constexpr bool canDig(const blockID &identifier);

constexpr bool isFluid(const blockID &identifier);

constexpr bool canFaceUp(const blockID &identifier);

constexpr bool isPillar(const blockID &identifier);

constexpr bool hasBlockDependantGUI(const blockID &identifier);

constexpr bool isFurnace(const blockID &identifier);

constexpr bool hasGUI(const blockID &identifier);

constexpr bool hasCustomGUI(const blockID &identifier);

constexpr bool isPressurePlate(const blockID &identifier);

constexpr bool isWoodenButton(const blockID &identifier);

constexpr bool isButton(const blockID &identifier);

constexpr bool isTorch(const blockID &identifier);

constexpr bool hasAttachmentDirectionData(const blockID &identifier);

constexpr bool isFarmlandCrop(const blockID &identifier);

constexpr bool isCrop(const blockID &identifier);

constexpr bool isStemPlant(const blockID &identifier);

constexpr bool hasPowerLevelData(const blockID &identifier);

constexpr bool needsSpreadValueUpdate(const blockID &identifier);

constexpr bool isBlockItem(const itemID &identifier);

constexpr bool isThrowable(const itemID &id) noexcept;

constexpr bool isThrowableEntity(const entityID &identifier) noexcept;

constexpr bool isFireBall(const entityID &identifier) noexcept;

constexpr bool isSpawnEgg(const itemID &identifier) noexcept;

constexpr bool isMinecartItem(const itemID &identifier) noexcept;

constexpr bool canAddEffects(const itemID &identifier) noexcept;

constexpr bool isPotion(const itemID &identifier) noexcept;

constexpr bool canDrink(const itemID &identifier) noexcept;

constexpr bool isPotionModifier(const itemID &identifier) noexcept;

constexpr bool isMusicDisc(const itemID &identifier) noexcept;

constexpr bool canReplaceByTree(const blockID &identifier) noexcept;

constexpr entityID getThrowableEntityID(const itemID &id);

constexpr bool isRepairItem(const itemID &itemToRepair, const itemID &repairWith);

// constexpr itemID getRepairItem(const itemID& identifier);

constexpr int getPollenChance(const blockID &identifier) noexcept;

constexpr blockID getBlockToSpread(const blockID &identifier) noexcept;

constexpr bool canSpreadOn(const blockID &identifier, const blockID &spreadOn) noexcept;

constexpr blockID getGrassToGrowOn(const blockID &identifier) noexcept;

bool canBeDestroyedByFluids(const blockID &identifier);

constexpr bool isBossMob(const entityID &entityType) noexcept;

constexpr bool isFish(const entityID &entityType) noexcept;

constexpr bool isGrassEatingMob(const entityID &entityType) noexcept;

constexpr bool isPassiveMob(const entityID &entityType) noexcept;

constexpr bool isHostileMob(const entityID &entityType) noexcept;

constexpr bool isSkeleton(const entityID &entityType) noexcept;

constexpr bool isZombie(const entityID &entityType) noexcept;

constexpr bool isUndeadMob(const entityID &entityType) noexcept;

constexpr bool isImmuneToFire(const entityID &entityType) noexcept;

constexpr bool isSlime(const entityID &entityType) noexcept;

constexpr bool isMob(const entityID &entityType);

// constexpr bool isStonePressurePlate(const blockID& identifier);
inline constexpr bool isStonePressurePlate(const blockID &identifier)
{
    return (identifier == blockID::stone_pressure_plate) ||
           (identifier == blockID::polished_blackstone_pressure_plate);
}

constexpr bool canHit(const entityID &entityType);

constexpr bool isHumanoid(const entityID &entityType) noexcept;

constexpr bool canOnlySpawnInWater(const entityID &entityType) noexcept;

constexpr bool canSpawnInWater(const entityID &entityType) noexcept;

constexpr bool canSpawnOutsideWater(const entityID &entityType) noexcept;

constexpr bool isRidable(const entityID &entityType) noexcept;

constexpr bool hasBowl(const itemID &identifier) noexcept;

constexpr bool isParticle(const entityID &entityType) noexcept;

constexpr bool isAnimatedParticle(const entityID &entityType) noexcept;

constexpr bool isGoldenApple(const itemID &identifier) noexcept;

constexpr bool canEatIfFull(const itemID &identifier) noexcept;

constexpr bool isMaterialArmor(const itemID &identifier) noexcept;

constexpr itemID getMaterialArmor(const armorTypeID &type, const armorTierID &tier) noexcept;

constexpr armorTypeID getMaterialArmorType(const itemID &identifier) noexcept;

constexpr armorTierID getMaterialArmorTier(const itemID &identifier) noexcept;

constexpr armorTypeID getArmorType(const itemID &identifier) noexcept;

constexpr armorTierID getArmorTier(const itemID &identifier) noexcept;

constexpr harvestTierID getMaterialToolTier(const itemID &identifier) noexcept;

constexpr toolTypeID getMaterialToolType(const itemID &identifier) noexcept;

constexpr itemID getMaterialTool(const toolTypeID &type, const harvestTierID &tier) noexcept;

constexpr bool isMaterialTool(const itemID &identifier) noexcept;

constexpr bool hasDurability(const itemID &identifier) noexcept;

constexpr bool isConcretePowder(const blockID &identifier) noexcept;

constexpr bool hasSand(const blockID &identifier) noexcept;

constexpr bool isIce(const blockID &identifier) noexcept;

constexpr bool isStainedGlass(const blockID &identifier) noexcept;

constexpr bool isGlass(const blockID &identifier) noexcept;

constexpr bool multiplyByBiomeColor(const blockID &identifier);

constexpr noteTypeID getNote(const blockID &blockBelow);

constexpr bool collidesWithThrowables(const entityID &identifier);

constexpr bool isLevelDataType(const arrayDataType &type);

constexpr bool isLightLevel(const levelID &identifier);

constexpr bool isSerializable(const entityID &identifier) noexcept
{
    return !is_in(identifier, entityID::human, entityID::particle);
}

constexpr bool willBeNoticedByMobs(const gameModeID &identifier) noexcept
{
    return identifier == gameModeID::survival || identifier == gameModeID::adventure;
}

constexpr bool collidesWithThrowables(const entityID &identifier)
{
    return isMob(identifier) || isThrowableEntity(identifier) ||
           is_in(identifier, entityID::end_crystal, entityID::boat);
}

constexpr bool isLevelDataType(const arrayDataType &type)
{
    return ((int)type >= (int)arrayDataType::levelType) &&
           ((int)type < ((int)arrayDataType::levelType + (int)levelID::count));
}

constexpr bool isLightLevel(const levelID &identifier)
{
    return ((int)identifier >= (int)levelID::light) &&
           ((int)identifier < ((int)levelID::light + (int)lightLevelID::count));
}

constexpr bool needsBlockUpdate(const blockID &block) noexcept
{
    return block == blockID::water || block == blockID::lava;
}

constexpr statusEffectID getNegativeStatusEffect(const statusEffectID &identifier)
{
    switch (identifier)
    {
    case statusEffectID::speed:
    case statusEffectID::jumpBoost:
        return statusEffectID::slowness;
    case statusEffectID::instantHealth:
    case statusEffectID::poison:
        return statusEffectID::instantDamage;
    case statusEffectID::nightVision:
        return statusEffectID::invisibility;
    default:
        break;
    }
    return identifier;
}

constexpr bool isBed(const blockID &identifier) noexcept
{
    return (identifier >= blockID::bed) &&
           (identifier < (blockID)((int)blockID::bed + (int)colorID::count));
}

constexpr bool isFlower(const blockID &identifier) noexcept
{
    return (identifier >= blockID::dandelion) &&
           ((int)identifier < ((int)blockID::dandelion + flowerTypeCount));
}

constexpr bool isFenceGate(const blockID &identifier) noexcept
{
    return (identifier >= blockID::wood_fence_gate) &&
           (identifier < (blockID)((int)blockID::wood_fence_gate + woodTypeCount));
}

constexpr bool isGlazedTerracotta(const blockID &identifier) noexcept
{
    return (identifier >= blockID::glazed_terracotta) &&
           (identifier < (blockID)((int)blockID::glazed_terracotta + (int)colorID::count));
}

constexpr bool isSapling(const blockID &identifier) noexcept
{
    return (identifier >= blockID::wood_sapling) &&
           (identifier < (blockID)((int)blockID::wood_sapling + woodTypeCount));
}

constexpr bool isTrapDoor(const blockID &identifier) noexcept
{
    return (identifier >= blockID::iron_trapdoor) &&
           ((int)identifier < ((int)blockID::wood_trapdoor + woodTypeCount));
}

constexpr bool isDoor(const blockID &identifier) noexcept
{
    return (identifier >= blockID::iron_door) &&
           ((int)identifier < ((int)blockID::wood_door + woodTypeCount));
}

constexpr bool isStairs(const blockID &identifier) noexcept
{
    return (identifier >= blockID::wood_stairs) &&
           (identifier < (blockID)((int)blockID::wood_stairs + stairTypeCount));
}

constexpr bool isSlab(const blockID &identifier) noexcept
{
    return (identifier >= blockID::wood_slab) &&
           (identifier < (blockID)((int)blockID::wood_slab + slabTypeCount));
}

constexpr bool isFence(const blockID &identifier) noexcept
{
    return (identifier >= blockID::wood_fence) &&
           (identifier < (blockID)((int)blockID::wood_fence + fenceTypeCount));
}

constexpr bool isWall(const blockID &identifier) noexcept
{
    return (identifier >= blockID::end_stone_brick_wall) &&
           (identifier < (blockID)((int)blockID::end_stone_brick_wall + wallTypeCount));
}

constexpr bool isChest(const blockID &identifier) noexcept
{
    return (identifier == blockID::chest) || (identifier == blockID::trapped_chest);
}

constexpr bool isRoots(const blockID &identifier) noexcept
{
    return (identifier == blockID::crimson_roots) || (identifier == blockID::warped_roots);
}

constexpr bool isFungus(const blockID &identifier) noexcept
{
    return ((int)identifier >= ((int)blockID::wood_sapling + normalTreeTypeCount)) &&
           ((int)identifier < ((int)blockID::wood_sapling + woodTypeCount));
}

constexpr bool isShortGrass(const blockID &identifier) noexcept
{
    return (identifier == blockID::grass) || (identifier == blockID::fern);
}

constexpr bool isTallGrass(const blockID &identifier) noexcept
{
    return (identifier == blockID::tall_grass) || (identifier == blockID::large_fern);
}

constexpr bool hasChestData(const blockID &identifier) noexcept
{
    return isChest(identifier) || (identifier == blockID::barrel);
}

constexpr bool isStemPlant(const blockID &identifier)
{
    return (identifier >= blockID::melon_stem) &&
           (identifier < (blockID)((int)blockID::melon_stem + stemPlantTypeCount));
}

constexpr bool hasAgeData(const blockID &identifier)
{
    return identifier == blockID::fire || isCrop(identifier);
}

constexpr bool keepsFireBurning(const blockID &identifier)
{
    return (identifier == blockID::netherrack) || (identifier == blockID::soul_sand) ||
           (identifier == blockID::magma_block) || (identifier == blockID::bedrock);
}

constexpr bool isNetherForest(const biomeID &identifier) noexcept
{
    return identifier == biomeID::warped_forest || identifier == biomeID::crimson_forest;
}

constexpr bool isForest(const biomeID &identifier) noexcept
{
    return isNetherForest(identifier) ||
           (identifier == biomeID::forest) || (identifier == biomeID::dark_forest) ||
           (identifier == biomeID::jungle) || (identifier == biomeID::taiga);
}

constexpr bool hasOpenData(const blockID &identifier) noexcept
{
    return isDoor(identifier) || isFenceGate(identifier);
}

constexpr bool hasUpsideDownData(const blockID &identifier) noexcept
{
    return isStairs(identifier) || isTrapDoor(identifier);
}

constexpr bool isBoat(const itemID &item) noexcept
{
    return (item >= itemID::wood_boat) &&
           ((int)item < ((int)itemID::wood_boat + normalTreeTypeCount));
}

constexpr bool isDispenser(const blockID &identifier) noexcept
{
    return (identifier == blockID::dispenser) || (identifier == blockID::dropper);
}

constexpr bool isPiston(const blockID &identifier) noexcept
{
    return (identifier == blockID::piston) || (identifier == blockID::sticky_piston);
}

constexpr bool isFire(const blockID &identifier) noexcept
{
    return (identifier == blockID::fire) || (identifier == blockID::soul_fire);
}

constexpr bool isSponge(const blockID &identifier) noexcept
{
    return (identifier == blockID::sponge) || (identifier == blockID::wet_sponge);
}

constexpr bool isTreeType(const blockID &identifier) noexcept
{
    return (identifier >= blockID::wood_block) && (identifier <
                                                   (blockID)((int)blockID::wood_block +
                                                             ((int)treeItemTypeID::count *
                                                              (int)woodTypeCount)));
}

constexpr bool isMushroom(const blockID &identifier) noexcept
{
    return (identifier == blockID::red_mushroom) || (identifier == blockID::brown_mushroom);
}

constexpr bool isMushroomBlock(const blockID &identifier) noexcept
{
    return ((int)identifier >= (int)blockID::mushroom_stem) &&
           ((int)identifier < ((int)blockID::mushroom_stem + mushroomBlockTypeCount));
}

constexpr woodTypeID getWoodType(const blockID &identifier) noexcept
{
    return (woodTypeID)(((int)identifier - (int)blockID::wood_block) /
                        (int)treeItemTypeID::count);
}

constexpr treeItemTypeID getTreeItemType(const blockID &identifier) noexcept
{
    return (treeItemTypeID)(((int)identifier - (int)blockID::wood_block) %
                            (int)treeItemTypeID::count);
}

constexpr blockID getTreeBlock(const woodTypeID &typeID, const treeItemTypeID &itemTypeID)
{
    return (blockID)((int)blockID::wood_block + ((int)typeID * (int)treeItemTypeID::count) +
                     (int)itemTypeID);
}

constexpr itemID getMaterialTool(const toolTypeID &type, const harvestTierID &tier) noexcept
{
    return (itemID)((int)itemID::material_tool + (((int)tier - 1) * (int)toolTypeID::count) +
                    (int)type);
}

constexpr bool isConnectingBlock(const blockID &identifier) noexcept
{
    return isFence(identifier) || isWall(identifier) || isFenceGate(identifier) ||
           identifier == blockID::iron_bars;
}

constexpr bool isGrass(const blockID &identifier) noexcept
{
    return isShortGrass(identifier) || isTallGrass(identifier);
}

constexpr bool diesWithoutLight(const blockID &identifier) noexcept
{
    return (identifier == blockID::grass_block) || (identifier == blockID::podzol) ||
           isGrass(identifier) || isFlower(identifier);
}

constexpr bool isDoubleBlockWhenPlaced(const blockID &identifier) noexcept
{
    return isTallGrass(identifier) || isDoor(identifier) || isBed(identifier) ||
           // pistons aren't always double blocks
           // double flowers
           identifier == blockID::sunflower || identifier == blockID::lilac ||
           identifier == blockID::rose_bush || identifier == blockID::peony;
}

constexpr bool isDoubleBlock(const blockID &identifier) noexcept
{
    return isDoubleBlockWhenPlaced(identifier) || isPiston(identifier);
}

constexpr bool isSand(const blockID &identifier) noexcept
{
    return identifier == blockID::sand || identifier == blockID::red_sand ||
           identifier == blockID::soul_sand;
}

constexpr bool isSnow(const blockID& identifier) noexcept
{
    return is_in(identifier, blockID::snow, blockID::snow_block);
}

constexpr bool isFallingBlock(const blockID &identifier) noexcept
{
    return isSand(identifier) || isConcretePowder(identifier) || (identifier == blockID::gravel) ||
           (identifier == blockID::dragon_egg) || (identifier == blockID::snow) ||
           (identifier == blockID::anvil);
}

constexpr bool isMob(const entityID &entityType)
{
    return isPassiveMob(entityType) || isHostileMob(entityType) || (entityType == entityID::wolf) ||
           (entityType == entityID::human);
}

constexpr bool canHit(const entityID &entityType)
{
    return isMob(entityType) || (entityType == entityID::minecart) ||
           (entityType == entityID::boat) || (entityType == entityID::end_crystal);
}

constexpr bool isHumanoid(const entityID &entityType) noexcept
{
    return isZombie(entityType) || isSkeleton(entityType) ||
           (entityType == entityID::zombiefied_piglin) || (entityType == entityID::human) ||
           (entityType == entityID::enderman);
}

constexpr bool canOnlySpawnInWater(const entityID &entityType) noexcept
{
    return (entityType == entityID::drowned) || (entityType == entityID::cod);
}

constexpr bool canSpawnInWater(const entityID &entityType) noexcept
{
    return canOnlySpawnInWater(entityType);
}

constexpr bool canSpawnOutsideWater(const entityID &entityType) noexcept
{
    return !canOnlySpawnInWater(entityType);
}

constexpr bool isRidable(const entityID &entityType) noexcept
{
    return (entityType == entityID::boat) || (entityType == entityID::minecart);
}

constexpr bool hasBowl(const itemID &identifier) noexcept
{
    return (identifier == itemID::beetroot_soup) || (identifier == itemID::mushroom_stew) ||
           (identifier == itemID::rabbit_stew);
}

constexpr bool isParticle(const entityID &entityType) noexcept
{
    return ((int)entityType >= (int)entityID::particle) &&
           ((int)entityType < ((int)entityID::particle + (int)particleID::count));
}

constexpr bool isAnimatedParticle(const entityID &entityType) noexcept
{
    const particleID &particleType = (particleID)((int)entityType - (int)entityID::particle);
    return (particleType == particleID::large_smoke) ||
           (particleType == particleID::bubble_pop) ||
           (particleType == particleID::effect) ||
           (particleType == particleID::explosion) ||
           (particleType == particleID::enchant) ||
           (particleType == particleID::soul) ||
           (particleType == particleID::sweep_attack);
}

constexpr bool isGoldenApple(const itemID &identifier) noexcept
{
    return (identifier == itemID::golden_apple) || (identifier == itemID::enchanted_golden_apple);
}

constexpr bool canEatIfFull(const itemID &identifier) noexcept
{
    return isGoldenApple(identifier) || (identifier == itemID::chorus_fruit);
}

constexpr armorTypeID getMaterialArmorType(const itemID &identifier) noexcept
{
    cint materialArmorIndex = ((int)identifier - (int)itemID::material_armor);
    return (armorTypeID)(1 + (materialArmorIndex % armorTypeCount));
}

constexpr armorTierID getMaterialArmorTier(const itemID &identifier) noexcept
{
    cint materialArmorIndex = ((int)identifier - (int)itemID::material_armor);
    return (armorTierID)(leatherArmorTier + (materialArmorIndex / armorTypeCount));
}

constexpr armorTierID getArmorTier(const itemID &identifier) noexcept
{
    if (isMaterialArmor(identifier))
    {
        return getMaterialArmorTier(identifier);
    }
    else if (identifier == itemID::turtle_helmet)
    {
        return turtleArmorTier;
    }
    else
    {
        return noArmorTier;
    }
}

constexpr harvestTierID getMaterialToolTier(const itemID &identifier) noexcept
{
    return (harvestTierID)(woodHarvestTier + ((int)identifier - (int)itemID::material_tool) /
                                                 (int)toolTypeID::count);
}

constexpr toolTypeID getMaterialToolType(const itemID &identifier) noexcept
{
    return (toolTypeID)(((int)identifier - (int)itemID::material_tool) %
                        (int)toolTypeID::count);
}

constexpr armorTypeID getArmorType(const itemID &identifier) noexcept
{
    if (isMaterialArmor(identifier))
    {
        return getMaterialArmorType(identifier);
    }
    else if (identifier == itemID::turtle_helmet)
    {
        return helmetArmorType;
    }
    else if (identifier == itemID::elytra)
    {
        return chestplateArmorType;
    }
    else if (identifier == (itemID)blockID::carved_pumpkin)
    {
        return helmetArmorType;
    }
    else
    {
        return noArmorType;
    }
}

constexpr bool isMaterialArmor(const itemID &identifier) noexcept
{
    return (identifier >= itemID::material_armor) && (int)identifier <
                                                         ((int)itemID::material_armor +
                                                          ((armorTierCount - 1) * armorTypeCount));
}

constexpr itemID getMaterialArmor(const armorTypeID &type, const armorTierID &tier) noexcept
{
    return (itemID)((int)itemID::material_armor +
                    ((int)type - (int)armorTypeID::bootsArmorType) +
                    (((int)tier - (int)armorTierID::leatherArmorTier) * armorTypeCount));
}

constexpr bool isMaterialTool(const itemID &identifier) noexcept
{
    return (identifier >= itemID::material_tool) && ((int)identifier <
                                                     ((int)itemID::material_tool +
                                                      (toolTierCount * materialToolTypeCount)));
}

constexpr bool hasDurability(const itemID &identifier) noexcept
{
    return isMaterialTool(identifier) || isMaterialArmor(identifier) ||
           (identifier == itemID::fishing_rod) || (identifier == itemID::flint_and_steel) ||
           (identifier == itemID::bow) || (identifier == itemID::elytra) ||
           (identifier == itemID::shears);
}

constexpr bool isConcretePowder(const blockID &identifier) noexcept
{
    return (identifier >= blockID::concrete_powder) &&
           ((int)identifier < ((int)blockID::concrete_powder + (int)colorID::count));
}

constexpr bool hasSand(const blockID &identifier) noexcept
{
    return isSand(identifier) || isConcretePowder(identifier) || (identifier == blockID::gravel);
}

constexpr bool isIce(const blockID &identifier) noexcept
{
    return (identifier == blockID::frosted_ice) || (identifier == blockID::ice) ||
           (identifier == blockID::packed_ice) || (identifier == blockID::blue_ice);
}

constexpr bool isStainedGlass(const blockID &identifier) noexcept
{
    return (identifier >= blockID::stained_glass) &&
           ((int)identifier < ((int)blockID::stained_glass + (int)colorID::count));
}

constexpr bool isGlass(const blockID &identifier) noexcept
{
    return isStainedGlass(identifier) || (identifier == blockID::glass);
}

constexpr bool hasFacingData(const blockID &identifier) noexcept
{
    return canFaceUp(identifier) || isDoor(identifier) || isStairs(identifier) ||
           isFenceGate(identifier) || isTrapDoor(identifier) || isBed(identifier) ||
           isGlazedTerracotta(identifier) || (identifier == blockID::repeater) || (identifier == blockID::comparator) ||
           (identifier == blockID::end_portal_frame) || (identifier == blockID::lectern);
}

constexpr bool attachedToBottomBlock(const blockID &identifier) noexcept
{
    return isFarmlandCrop(identifier) || isFlower(identifier) || isGrass(identifier) ||
           isSapling(identifier) || isMushroom(identifier) ||
           is_in(identifier, blockID::repeater, blockID::comparator, blockID::redstone_wire, blockID::sugar_cane, blockID::cactus);
}

constexpr bool hasAttachedBlockData(const blockID &identifier) noexcept
{
    return hasAttachmentDirectionData(identifier) || attachedToBottomBlock(identifier) || isFire(identifier);
}

constexpr bool hasPollenData(const blockID &identifier) noexcept
{
    return isGrass(identifier) || isFlower(identifier) || isRoots(identifier) ||
           isFungus(identifier) || isOverworldLeaves(identifier);
}

constexpr bool hasLeavesData(const blockID &identifier) noexcept
{
    return isOverworldLeaves(identifier);
}

constexpr bool isGrassBlock(const blockID &identifier) noexcept
{
    return ((int)identifier >= (int)blockID::grass_block) &&
           ((int)identifier < ((int)blockID::grass_block + grassBlockTypeCount));
}

constexpr bool isNylium(const blockID &identifier) noexcept
{
    return (identifier == blockID::crimson_nylium) || (identifier == blockID::warped_nylium);
}

constexpr bool hasGrassBlockData(const blockID &identifier) noexcept
{
    return isGrassBlock(identifier) && (identifier != blockID::mycelium);
}

constexpr bool isOverworldLeaves(const blockID &identifier) noexcept
{
    return isTreeType(identifier) && (getTreeItemType(identifier) == treeItemTypeID::leaves) &&
           ((int)getWoodType(identifier) < normalTreeTypeCount);
}

constexpr bool isBoatOrMinecart(const itemID &item) noexcept
{
    return isBoat(item) || isMinecartItem(item);
}

constexpr bool invertPlacementDirection(const blockID &identifier) noexcept
{
    return isDispenser(identifier) || isPiston(identifier);
}

constexpr bool isRail(const blockID &identifier) noexcept
{
    return identifier >= blockID::rail && (int)identifier < ((int)blockID::rail + railTypeCount);
}

constexpr bool hasPowerLevelData(const blockID &identifier)
{
    return (identifier == blockID::redstone_lamp) || (identifier == blockID::note_block);
}

constexpr bool needsSpreadValueUpdate(const blockID &identifier)
{
    return (identifier == blockID::note_block) || (identifier == blockID::jukebox) ||
           (identifier == blockID::tnt) || (identifier == blockID::iron_door) ||
           (identifier == blockID::iron_trapdoor) || (identifier == blockID::repeater) ||
           (identifier == blockID::comparator) || (identifier == blockID::redstone_torch) ||
           isDispenser(identifier) || hasPowerLevelData(identifier) || isPiston(identifier);
}

constexpr bool isBlockItem(const itemID &identifier)
{
    return (int)identifier < blockIDCount;
}

// definitions
constexpr bool renderAsBlock(const itemID &identifier)
{
    return isBlockItem(identifier) && !(isDoubleBlock((blockID)identifier) || is_in((blockID)identifier, blockID::wheat, blockID::beetroots, blockID::nether_wart));
}

constexpr bool canDig(const blockID &identifier)
{
    return identifier != blockID::air && !isFluid(identifier);
}

constexpr bool isFluid(const blockID &identifier)
{
    return identifier == blockID::water || identifier == blockID::lava;
}

constexpr bool canFaceUp(const blockID &identifier)
{
    if (isTreeType(identifier))
    {
        treeItemTypeID item = getTreeItemType(identifier);
        return item == treeItemTypeID::log || item == treeItemTypeID::strippedLog;
    }
    else
    {
        return isDispenser(identifier) || isPillar(identifier) || isPiston(identifier) ||
               (identifier == blockID::haybale) || (identifier == blockID::jigsaw) ||
               (identifier == blockID::bone_block);
    }
}

constexpr bool isPillar(const blockID &identifier)
{
    return (identifier == blockID::purpur_pillar) || (identifier == blockID::quartz_pillar);
}

constexpr bool hasBlockDependantGUI(const blockID &identifier)
{
    return isFurnace(identifier) || isDispenser(identifier) || hasChestData(identifier) ||
           (identifier == blockID::enchanting_table) || (identifier == blockID::brewing_stand) ||
           (identifier == blockID::jigsaw) || (identifier == blockID::structure_block);
}

constexpr bool isFurnace(const blockID &identifier)
{
    return (identifier >= blockID::furnace) &&
           ((int)identifier < ((int)blockID::furnace + furnaceTypeCount));
}

constexpr bool hasGUI(const blockID &identifier)
{
    return (identifier == blockID::crafting_table) || (identifier == blockID::smithing_table) ||
           (identifier == blockID::anvil) || (identifier == blockID::loom) ||
           (identifier == blockID::ender_chest) || hasBlockDependantGUI(identifier);
}

constexpr bool hasCustomGUI(const blockID &identifier)
{
    return (identifier == blockID::jigsaw) || (identifier == blockID::structure_block);
}

// constexpr bool isStonePressurePlate(const blockID& identifier)
//{
//	return (identifier == blockID::stone_pressure_plate) || (identifier == blockID::polished_blackstone_pressure_plate);
// }
constexpr bool isPressurePlate(const blockID &identifier)
{
    return (identifier >= blockID::wood_pressure_plate) &&
           ((int)identifier < ((int)blockID::wood_pressure_plate + pressurePlateTypeCount));
}

constexpr bool isWoodenButton(const blockID &identifier)
{
    return (identifier >= blockID::wood_button) &&
           ((int)identifier < ((int)blockID::wood_button + woodTypeCount));
}

constexpr bool isButton(const blockID &identifier)
{
    return (identifier >= blockID::wood_button) &&
           ((int)identifier < ((int)blockID::wood_button + buttonTypeCount));
}

constexpr bool isTorch(const blockID &identifier)
{
    return (identifier == blockID::torch) || (identifier == blockID::redstone_torch);
}

constexpr bool hasAttachmentDirectionData(const blockID &identifier)
{
    return isTorch(identifier) || isButton(identifier) || (identifier == blockID::lever) ||
           (identifier == blockID::end_rod);
}

constexpr bool isFarmlandCrop(const blockID &identifier)
{
    return (identifier == blockID::wheat) ||
           (identifier == blockID::carrots) ||
           (identifier == blockID::potatoes) ||
           (identifier == blockID::beetroots);
}

constexpr int getPollenChance(const blockID &identifier) noexcept
{
    return isTreeType(identifier) ? 0x20 : isDoubleBlock(identifier) ? 0x8
                                                                     : 0x4;
}

constexpr blockID getBlockToSpread(const blockID &identifier) noexcept
{
    return (identifier == blockID::tall_grass) ? blockID::grass_block : (identifier == blockID::large_fern) ? blockID::podzol
                                                                    : isOverworldLeaves(identifier)         ? (blockID)((int)blockID::wood_sapling + (int)getWoodType(identifier))
                                                                    : isRoots(identifier)                   ? (blockID)((int)blockID::crimson_nylium + ((int)identifier - (int)blockID::crimson_roots))
                                                                                                            : identifier;
}

constexpr bool canSpreadOn(const blockID &identifier, const blockID &spreadOn) noexcept
{
    return isNylium(identifier) ? (spreadOn == blockID::netherrack) : isGrassBlock(identifier) ? (spreadOn == blockID::dirt)
                                                                  : isFungus(identifier)       ? (spreadOn == (blockID)((int)blockID::crimson_nylium + ((int)getWoodType(identifier) - (int)woodTypeID::crimson)))
                                                                  : isSapling(identifier)      ? ((spreadOn == blockID::dirt) || (spreadOn == blockID::grass_block) || (spreadOn == blockID::podzol))
                                                                                               : (spreadOn == blockID::grass_block);
}

constexpr blockID getGrassToGrowOn(const blockID &identifier) noexcept
{
    switch (identifier)
    {
    case blockID::grass_block:
        return blockID::grass;
    case blockID::podzol:
        return blockID::fern;
    case blockID::crimson_nylium:
        return blockID::crimson_roots;
    case blockID::warped_nylium:
        return blockID::warped_roots;
    default:
        return blockID();
    }
}

constexpr bool isBossMob(const entityID &entityType) noexcept
{
    return entityType == entityID::ender_dragon;
}

constexpr bool isFish(const entityID &entityType) noexcept
{
    return (entityType == entityID::cod);
}

constexpr bool isGrassEatingMob(const entityID &entityType) noexcept
{
    return (entityType == entityID::sheep) || (entityType == entityID::cow) ||
           (entityType == entityID::pig) || (entityType == entityID::chicken);
}

constexpr bool isPassiveMob(const entityID &entityType) noexcept
{
    return isFish(entityType) || isGrassEatingMob(entityType);
}

constexpr bool isSkeleton(const entityID &entityType) noexcept
{
    return (entityType == entityID::skeleton) || (entityType == entityID::stray);
}

constexpr bool isZombie(const entityID &entityType) noexcept
{
    return (entityType == entityID::zombie) || (entityType == entityID::drowned) ||
           (entityType == entityID::zombie_villager) || (entityType == entityID::husk);
}

constexpr bool isUndeadMob(const entityID &entityType) noexcept
{
    return isZombie(entityType) || isSkeleton(entityType) ||
           (entityType == entityID::zombiefied_piglin) || (entityType == entityID::ghast);
}

constexpr bool isImmuneToFire(const entityID &entityType) noexcept
{
    return (entityType == entityID::zombiefied_piglin) || (entityType == entityID::ghast) ||
           (entityType == entityID::blaze) || (entityType == entityID::magma_cube);
}

constexpr bool isSlime(const entityID &entityType) noexcept
{
    return (entityType == entityID::slime) || (entityType == entityID::magma_cube);
}

constexpr bool isHostileMob(const entityID &entityType) noexcept
{
    return isUndeadMob(entityType) || isSlime(entityType) || (entityType == entityID::creeper) ||
           (entityType == entityID::enderman) || (entityType == entityID::blaze) ||
           (entityType == entityID::shulker) || isBossMob(entityType);
}

constexpr bool isThrowable(const itemID &id) noexcept
{
    return id == itemID::snowball || id == itemID::egg || id == itemID::fishing_rod ||
           id == itemID::bow || id == itemID::ender_pearl || id == itemID::ender_eye;
}

constexpr bool isFireBall(const entityID &identifier) noexcept
{
    return (identifier == entityID::small_Fireball) ||
           (identifier == entityID::ghast_fireball || (identifier == entityID::dragon_fireball));
}

constexpr bool isThrowableEntity(const entityID &identifier) noexcept
{
    return isFireBall(identifier) || (identifier == entityID::snowball) ||
           (identifier == entityID::egg) || (identifier == entityID::fishing_bobber) ||
           (identifier == entityID::ender_pearl) || (identifier == entityID::arrow);
}

constexpr bool isSpawnEgg(const itemID &identifier) noexcept
{
    return (identifier >= itemID::spawn_egg) &&
           ((int)identifier < ((int)itemID::spawn_egg + mobTypeCount));
}

constexpr bool isMinecartItem(const itemID &identifier) noexcept
{
    return (identifier >= itemID::minecart) &&
           ((int)identifier < ((int)itemID::minecart + mineCartItemTypeCount));
}

constexpr bool canAddEffects(const itemID &identifier) noexcept
{
    return (identifier == itemID::potion) || (identifier == itemID::splash_potion) ||
           (identifier == itemID::lingering_potion);
}

constexpr bool isPotion(const itemID &identifier) noexcept
{
    return canAddEffects(identifier) || (identifier == itemID::awkward_potion) ||
           (identifier == itemID::mundane_potion) || (identifier == itemID::thick_potion);
}

constexpr bool canDrink(const itemID &identifier) noexcept
{
    return (identifier == itemID::potion) || (identifier == itemID::honey_bottle) ||
           (identifier == itemID::dragon_breath) || (identifier == itemID::awkward_potion) ||
           (identifier == itemID::mundane_potion) || (identifier == itemID::thick_potion) ||
           (identifier == itemID::milk_bucket);
}

constexpr bool isPotionModifier(const itemID &identifier) noexcept
{
    return (identifier == (itemID)blockID::nether_wart) || (identifier == itemID::blaze_powder) ||
           (identifier == itemID::redstone) || (identifier == itemID::gunpowder) ||
           (identifier == itemID::ghast_tear) || (identifier == itemID::golden_carrot);
}

constexpr bool isMusicDisc(const itemID &identifier) noexcept
{
    return (identifier >= itemID::music_disc) &&
           ((int)identifier < ((int)itemID::music_disc + musicDiscTypeCount));
}

constexpr bool canReplaceByTree(const blockID &identifier) noexcept
{
    return (identifier == blockID::air) || isGrass(identifier) ||
           (isTreeType(identifier) && (getTreeItemType(identifier) == treeItemTypeID::leaves)) ||
           isSapling(identifier) || isMushroom(identifier) || (identifier == blockID::dead_bush);
}

constexpr entityID getThrowableEntityID(const itemID &id)
{
    switch (id)
    {
    case itemID::snowball:
        return entityID::snowball;
    case itemID::egg:
        return entityID::egg;
    case itemID::fishing_rod:
        return entityID::fishing_bobber;
    case itemID::ender_pearl:
        return entityID::ender_pearl;
    case itemID::arrow:
        return entityID::arrow;
    case itemID::ender_eye:
        return entityID::ender_eye;
    default:
        assumeInRelease(false, L"not a throwable");
        // handleError(std::wstring(L"not a throwable"));
        return (entityID)0;
        break;
    }
}

constexpr bool isRepairItem(const itemID &itemToRepair, const itemID &repairWith)
{
    if (itemToRepair == itemID::elytra)
    {
        return repairWith == itemID::phantom_membrane;
    }
    else if (itemToRepair == itemID::turtle_helmet)
    {
        return repairWith == itemID::scute;
    }
    else
    {
        if (isMaterialArmor(itemToRepair))
        {
            const armorTierID &tier = getMaterialArmorTier(itemToRepair);
            switch (tier)
            {
            case armorTierID::leatherArmorTier:
                return repairWith == itemID::leather;
            case armorTierID::goldArmorTier:
                return repairWith == itemID::gold_ingot;
            case armorTierID::ironArmorTier:
                return repairWith == itemID::iron_ingot;
            case armorTierID::diamondArmorTier:
                return repairWith == itemID::diamond;
            case armorTierID::netheriteArmorTier:
                return repairWith == itemID::netherite_ingot;
            case turtleArmorTier:
                return repairWith == itemID::scute;
                break;
            case noArmorTier:
                break;
            case chainmailArmorTier:
                break;
            }
        }
        else if (isMaterialTool(itemToRepair))
        {
            const harvestTierID &tier = getMaterialToolTier(itemToRepair);
            switch (tier)
            {
            case harvestTierID::woodHarvestTier:
                return isTreeType((blockID)repairWith) &&
                       getTreeItemType((blockID)repairWith) == treeItemTypeID::planks;
            case harvestTierID::goldHarvestTier:
                return repairWith == itemID::gold_ingot;
            case harvestTierID::stoneHarvestTier:
                return (repairWith == (itemID)blockID::cobblestone) ||
                       (repairWith == (itemID)blockID::blackstone);
            case harvestTierID::ironHarvestTier:
                return repairWith == itemID::iron_ingot;
            case harvestTierID::diamondHarvestTier:
                return repairWith == itemID::diamond;
            case harvestTierID::netheriteHarvestTier:
                return repairWith == itemID::netherite_ingot;
            case noHarvestTier:
                break;
            }
        }
    }
    return false;
}

constexpr bool hasFluidData(const blockID &identifier) noexcept
{
    return isFluid(identifier) || (identifier == blockID::wet_sponge);
}

constexpr noteTypeID getNote(const blockID &blockBelow)
{
    if (isTreeType(blockBelow) && (getTreeItemType(blockBelow) != treeItemTypeID::leaves))
    {
        return noteTypeID::bass;
    }
    else if (hasSand(blockBelow))
    {
        return noteTypeID::snare;
    }
    else if (isGlass(blockBelow) || (blockBelow == blockID::sea_lantern))
    {
        return noteTypeID::hat;
    }
    else if (blockBelow == blockID::stone)
    {
        return noteTypeID::bd;
    }
    else if (blockBelow == blockID::gold_block)
    {
        return noteTypeID::bell;
    }
    else if (blockBelow == blockID::clay)
    {
        return noteTypeID::flute;
    }
    else if (isIce(blockBelow))
    {
        return noteTypeID::flute;
    }
    else
    {
        return noteTypeID::harp;
    }
}

constexpr bool isCrop(const blockID &identifier)
{
    return isStemPlant(identifier) ||
           isFarmlandCrop(identifier) ||
           (identifier == blockID::nether_wart) ||
           (identifier == blockID::chorus_flower);
}

constexpr bool multiplyByBiomeColor(const blockID &identifier)
{
    return (isGrass(identifier) || is_in(identifier, blockID::vine, blockID::grass_block) ||
            (isTreeType(identifier) && ((int)getWoodType(identifier) < normalTreeTypeCount) &&
             getTreeItemType(identifier) == treeItemTypeID::leaves));
}

// dynamic functions

blockID getBlockToPlace(const itemID &identifier);

// defined in biome for performance (else the tables have to be moved)
fp getItemWeakness(const itemID &identifier);
