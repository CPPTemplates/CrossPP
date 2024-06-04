#include "idAnalysis.h"
#include "brewingStandData.h"
#include "rectangularSlotContainer.h"
#include "potionData.h"
#include "soundList.h"
#include "nbt/nbtSerializer.h"

brewingstandData::brewingstandData() {
    blazePowderSlot = new rectangularSlotContainer(cveci2(1));
    ingredientSlot = new rectangularSlotContainer(cveci2(1));
    for (int i = 0; i < brewingStandPotionCapacity; i++) {
        potionSlots[i] = new rectangularSlotContainer(cveci2(1));
    }
}

bool brewingstandData::tick(tickableBlockContainer *containerIn, cveci2 &position) {
    if (ingredientSlot->slots[0].count) {
        for (int i = 0; i < brewingStandPotionCapacity; i++) {
            if (potionSlots[i]->slots[0].count) {
                goto wantsToBrew;
            }
        }
        return false;
        wantsToBrew:

        if (ticksBrewed) {
            if (ticksBrewed >= brewingBatchTime) {
                const itemID ingredientItem = ingredientSlot->slots[0].stackItemID;
                for (int i = 0; i < brewingStandPotionCapacity; i++) {
                    if (potionSlots[i]->slots[0].count) {
                        const itemID currentItem = potionSlots[i]->slots[0].stackItemID;
                        potionData *currentData = canAddEffects(currentItem)
                                                  ? (potionData *) potionSlots[i]->slots[0].data
                                                  : nullptr;
                        if ((ingredientItem == itemID::gunpowder) &&
                            (currentItem == itemID::potion)) {
                            potionSlots[i]->slots[0].stackItemID = itemID::splash_potion;
                        } else if ((ingredientItem == itemID::dragon_breath) &&
                                   (currentItem == itemID::splash_potion)) {
                            potionSlots[i]->slots[0].stackItemID = itemID::lingering_potion;
                        } else if (((ingredientItem == itemID::redstone) && currentData) &&
                                   currentData->effectsToAdd.size()) {
                            //increase duration to maximum length for all effects
                            for (statusEffect &effectToMakeLonger: currentData->effectsToAdd) {
                                if (effectToMakeLonger.potency < 2) {
                                    switch (effectToMakeLonger.identifier) {
                                        case statusEffectID::speed:
                                            effectToMakeLonger.ticksDuration =
                                                    8 * 60 * ticksPerRealLifeSecond;
                                            break;
                                        case statusEffectID::slowness:
                                            effectToMakeLonger.ticksDuration =
                                                    4 * 60 * ticksPerRealLifeSecond;
                                            break;
                                        case statusEffectID::jumpBoost:
                                            effectToMakeLonger.ticksDuration =
                                                    8 * 60 * ticksPerRealLifeSecond;
                                            break;
                                        case statusEffectID::strength:
                                            effectToMakeLonger.ticksDuration =
                                                    8 * 60 * ticksPerRealLifeSecond;
                                            break;
                                        case statusEffectID::poison:
                                            effectToMakeLonger.ticksDuration =
                                                    90 * ticksPerRealLifeSecond;
                                            break;
                                        case statusEffectID::regeneration:
                                            effectToMakeLonger.ticksDuration =
                                                    90 * ticksPerRealLifeSecond;
                                            break;
                                        case statusEffectID::fireResistance:
                                            effectToMakeLonger.ticksDuration =
                                                    8 * 60 * ticksPerRealLifeSecond;
                                            break;
                                        case statusEffectID::waterBreathing:
                                            effectToMakeLonger.ticksDuration =
                                                    8 * 60 * ticksPerRealLifeSecond;
                                            break;
                                        case statusEffectID::nightVision:
                                            effectToMakeLonger.ticksDuration =
                                                    8 * 60 * ticksPerRealLifeSecond;
                                            break;
                                        case statusEffectID::invisibility:
                                            effectToMakeLonger.ticksDuration =
                                                    8 * 60 * ticksPerRealLifeSecond;
                                            break;
                                        case statusEffectID::slowFalling:
                                            effectToMakeLonger.ticksDuration =
                                                    4 * 60 * ticksPerRealLifeSecond;
                                            break;
                                        case statusEffectID::weakness:
                                            effectToMakeLonger.ticksDuration =
                                                    4 * 60 * ticksPerRealLifeSecond;
                                            break;
                                        default:
                                            break;
                                    }
                                }
                            }
                        } else {
                            //add ingredient to potion
                            const bool isWaterBottle = (currentItem == itemID::potion) &&
                                                       (currentData->effectsToAdd.size() == 0);
                            if (isWaterBottle) {
                                potionSlots[i]->slots[0].add(-1);
                                itemID resultingBottle;
                                switch ((int)ingredientItem) {
                                    case (int)blockID::nether_wart: {
                                        resultingBottle = itemID::awkward_potion;
                                    }
                                        break;
                                    case (int)itemID::fermented_spider_eye: {
                                        resultingBottle = itemID::potion;
                                    }
                                        break;
                                    default: {
                                        resultingBottle = itemID::mundane_potion;
                                    }
                                        break;
                                }
                                itemStack modifiedPotionStack = itemStack(resultingBottle, 1);

                                if (ingredientItem == itemID::fermented_spider_eye) {
                                    ((potionData *) (modifiedPotionStack.data))->effectsToAdd.push_back(
                                            statusEffect(statusEffectID::weakness,
                                                         90 * ticksPerRealLifeSecond));
                                }

                                potionSlots[i]->addStack(modifiedPotionStack);
                            } else {
                                if ((currentItem == itemID::awkward_potion) ||
                                    (canAddEffects(currentItem) &&
                                     (currentItem != itemID::potion) &&
                                     (currentData->effectsToAdd.size() == 0))) {
                                    if (currentItem == itemID::awkward_potion) {
                                        itemStack modifiedPotionStack;
                                        modifiedPotionStack = itemStack(itemID::potion, 1,
                                                                        new potionData());
                                        potionSlots[i]->slots[0].add(-1);
                                        potionSlots[i]->slots[0].addStack(modifiedPotionStack);
                                        currentData = (potionData *) potionSlots[i]->slots[0].data;
                                    }

                                    //apply the correct effect
                                    switch (ingredientItem) {
                                        case itemID::blaze_powder: {
                                            currentData->effectsToAdd.push_back(
                                                    statusEffect(statusEffectID::strength,
                                                                 5 * 60 * ticksPerRealLifeSecond));
                                        }
                                            break;
                                        case itemID::ghast_tear: {
                                            currentData->effectsToAdd.push_back(
                                                    statusEffect(statusEffectID::regeneration,
                                                                 45 * ticksPerRealLifeSecond));
                                        }
                                            break;
                                        case itemID::golden_carrot: {
                                            currentData->effectsToAdd.push_back(
                                                    statusEffect(statusEffectID::nightVision,
                                                                 5 * 60 * ticksPerRealLifeSecond));
                                        }
                                            break;
                                        case itemID::magma_cream: {
                                            currentData->effectsToAdd.push_back(
                                                    statusEffect(statusEffectID::fireResistance,
                                                                 3 * 60 * ticksPerRealLifeSecond));
                                        }
                                            break;
                                        case itemID::glistering_melon_slice: {
                                            currentData->effectsToAdd.push_back(
                                                    statusEffect(statusEffectID::instantHealth));
                                        }
                                            break;
                                        case itemID::phantom_membrane: {
                                            currentData->effectsToAdd.push_back(
                                                    statusEffect(statusEffectID::slowFalling,
                                                                 90 * ticksPerRealLifeSecond));
                                        }
                                            break;
                                        case itemID::rabbit_foot: {
                                            currentData->effectsToAdd.push_back(
                                                    statusEffect(statusEffectID::jumpBoost,
                                                                 3 * ticksPerRealLifeMinute));
                                        }
                                            break;
                                        default: {

                                        }
                                            break;
                                    }
                                }
                            }
                        }
                    }
                }
                brewingFinishSound->playRandomSound(containerIn, cvec2(position) + cvec2(0.5));
                ingredientSlot->slots[0].add(-1);
                ticksBrewed = 0;
            } else {
                ticksBrewed++;
            }
        } else {
            if (fuelLeft == 0) {
                if (blazePowderSlot->slots[0].count == 0) {
                    return false;
                }
                blazePowderSlot->slots[0].add(-1);
                fuelLeft = blazePowderBrewingBatchCount;
            }
            ticksBrewed = 1;
        }
        return true;
    } else {
        ticksBrewed = 0;
        return false;
    }
}

void brewingstandData::onBlockRemove(tickableBlockContainer *containerIn, cveci2 &position) {
    //convert slots to floating slots
    cvec2 floatingSlotPosition = (cvec2) position + cvec2(0.5, 0);
    blazePowderSlot->dropContent(containerIn, floatingSlotPosition);
    ingredientSlot->dropContent(containerIn, floatingSlotPosition);
    for (int i = 0; i < brewingStandPotionCapacity; i++) {
        potionSlots[i]->dropContent(containerIn, floatingSlotPosition);
    }
}

brewingstandData::~brewingstandData() {
    delete blazePowderSlot;
    delete ingredientSlot;
    for (int i = 0; i < brewingStandPotionCapacity; i++) {
        delete potionSlots[i];
    }
}

void brewingstandData::serializeValue(nbtSerializer &s) {
    blockData::serializeValue(s);
    s.serializeValue(std::wstring(L"fuel left"), fuelLeft);
    s.serializeValue(std::wstring(L"ticks brewed"), ticksBrewed);
    blazePowderSlot->serialize(s, std::wstring(L"input slot"));
    ingredientSlot->serialize(s, std::wstring(L"output slot"));
    if (s.push<nbtDataTag::tagList>(std::wstring(L"potion slots"))) {
        if (s.write) {
            for (int i = 0; i < brewingStandPotionCapacity; i++) {
                if (s.push<nbtDataTag::tagCompound>()) {
                    potionSlots[i]->serializeValue(s);
                    s.pop();
                }
            }
        } else {
            const std::vector<nbtData *> &potionDataList = s.getChildren();
            int i = 0;
            //mob types
            for (nbtData *serializedPotionSlot: potionDataList) {
                if (s.push(serializedPotionSlot)) {
                    potionSlots[i]->serializeValue(s);
                    i++;
                    s.pop();
                }
            }
        }
        s.pop();
    }
}