#include "playerControlledAI.h"
#include "application/interaction.h"
#include "settings.h"
#include "math/collisions.h"
#include "boat.h"
#include "gameControl.h"
#include "idAnalysis.h"
#include "keyID.h"
#include "human.h"
#include "dimension.h"
#include "soundList.h"
#include "gameForm.h"
#include "rectangularSlotContainer.h"
#include "floatingSlot.h"
// to switch to flying or walking

void playerControlledAI::execute()
{
    human *connectedPlayer = (human *)connectedEntity;
    // currentGame->focused && !currentGame->focusedChild;

    cbool &worldFocus =
        connectedPlayer->screen.getWorldFocus();

    connectedPlayer->screen.updateTransforms(connectedPlayer->getHeadPosition());
    if (worldFocus)
    {
        // only update lookingat on world focus
        //  all values are set to false automatically
        if (connectedPlayer->screen.touchInput)
        {
            connectedPlayer->lookingAt = connectedPlayer->getHeadPosition() + connectedPlayer->screen.interactJoystick->value;
            if (connectedPlayer->screen.moveJoystick->fingerDown)
            {
                constexpr rectangle2 sneakRect = rectangle2(-1, -1, 2, 2);
                constexpr rectangle2 moveRect = rectangle2(-1, -2, 2, 4);
                constexpr rectangle2 sprintRect = rectangle2(-2, -2, 4, 4);
                if (sneakRect.contains(connectedPlayer->screen.moveJoystick->value))
                {
                    connectedPlayer->wantsToSneak = true;
                }
                else if (moveRect.contains(connectedPlayer->screen.moveJoystick->value))
                {
                    // do nothing, just walk normally
                }
                else if (sprintRect.contains(connectedPlayer->screen.moveJoystick->value))
                {
                    connectedPlayer->wantsToSprint = true;
                }
                fp directionTreshold = 0.2;
                connectedPlayer->wantsToGoRight =
                    connectedPlayer->screen.moveJoystick->value.x > directionTreshold;
                connectedPlayer->wantsToGoLeft =
                    connectedPlayer->screen.moveJoystick->value.x < -directionTreshold;
                connectedPlayer->wantsToGoUp =
                    connectedPlayer->screen.moveJoystick->value.y > directionTreshold;
                connectedPlayer->wantsToGoDown =
                    connectedPlayer->screen.moveJoystick->value.y < -directionTreshold;
                connectedPlayer->wantsToJump = connectedPlayer->wantsToGoUp;
            }
        }
        else
        {
            connectedPlayer->lookingAt = connectedPlayer->screen.worldToRenderTargetTransform.inverse().multPointMatrix(cvec2(connectedPlayer->screen.unFocusedMousePosition));
            connectedPlayer->wantsToSprint =
                connectedPlayer->screen.holdingDownKey((vk)keyID::sprint);
            connectedPlayer->wantsToJump =
                connectedPlayer->screen.holdingDownKey((vk)keyID::jump);
            connectedPlayer->wantsToGoLeft =
                connectedPlayer->screen.holdingDownKey((vk)keyID::left);
            connectedPlayer->wantsToGoRight =
                connectedPlayer->screen.holdingDownKey((vk)keyID::right);
            connectedPlayer->wantsToSneak =
                connectedPlayer->screen.holdingDownKey((vk)keyID::sneak);
            connectedPlayer->wantsToGoDown =
                connectedPlayer->screen.holdingDownKey((vk)keyID::down);
            connectedPlayer->wantsToGoUp =
                connectedPlayer->screen.holdingDownKey((vk)keyID::up);
            if (connectedPlayer->screen.pressedKey((vk)keyID::drop))
            {
                if (connectedPlayer->itemHolding->count)
                {
                    if (connectedPlayer->screen.holdingDownKey((vk)sf::Keyboard::Key::LControl))
                    {
                        // drop the whole stack
                        connectedPlayer->drop(*connectedPlayer->itemHolding);
                    }
                    else
                    {
                        connectedPlayer->drop(*connectedPlayer->itemHolding, 1);
                    }
                }

                // we don't have to clear toDrop or something
                //  connectedPlayer->itemHolding->clearData();
                //  floatingSlot *slot = (floatingSlot *)summonEntity(entityID::item, connectedPlayer->dimensionIn, connectedPlayer->getDropPosition());
                //  connectedPlayer->itemHolding.size_t amount = connectedPlayer->screen.holdingDownKey((vk)sf::Keyboard::Key::LControl) ?
            }
        }

        cbool &spectating = connectedPlayer->currentGameMode == gameModeID::spectator;

        if (spectating)
        {
            if (connectedPlayer->screen.scrollDelta &&
                !connectedPlayer->screen.holdingDownKey((vk)keyID::camera))
            {
                // scroll delta of 10 = 2x as fast
                connectedPlayer->spectatorSpeed *= pow(0.5,
                                                       connectedPlayer->screen.scrollDelta * 0.1);
            }
        }
        else
        {
            if (connectedPlayer->screen.touchInput)
            {
                connectedPlayer->wantsToHit = connectedPlayer->screen.touchStarted;
                connectedPlayer->wantsToDig = connectedPlayer->screen.touching;
            }
            else
            {
                connectedPlayer->wantsToHit = connectedPlayer->screen.clickedFocused[sf::Mouse::Left];
                connectedPlayer->wantsToDig = connectedPlayer->screen.holding[sf::Mouse::Left];
            }
        }

        if (connectedPlayer->screen.touchInput)
        {
            connectedPlayer->wantsToKeepUsing =
                connectedPlayer->screen.touching;
            connectedPlayer->wantsToStartUsing =
                connectedPlayer->screen.touchStarted;
            connectedPlayer->wantsToStopUsing =
                connectedPlayer->screen.touchEnded;
        }
        else
        {
            connectedPlayer->wantsToKeepUsing =
                connectedPlayer->screen.holding[sf::Mouse::Right];
            connectedPlayer->wantsToStartUsing =
                connectedPlayer->screen.clickedFocused[sf::Mouse::Right];
            connectedPlayer->wantsToStopUsing =
                connectedPlayer->screen.clickReleased[sf::Mouse::Right];
        }
    }

    if (connectedPlayer->wantsToJump)
    {
        ticksSinceStoppedJumping++;
        if ((connectedPlayer->currentGameMode == gameModeID::creative) &&
            (ticksSinceStoppedJumping <= ticksBetweenJumpsToFly))
        {
            connectedPlayer->flying = !connectedPlayer->flying;
        }
    }
    else
    {
        if (wasJumping)
        {
            ticksSinceStoppedJumping = 0;
        }
        else
        {
            ticksSinceStoppedJumping++;
        }
    }
    wasJumping = connectedPlayer->wantsToJump;

    if (connectedPlayer->UUIDRidingOn)
    {
        entity *entityRidingOn = connectedPlayer->dimensionIn->findUUID(connectedPlayer->position,
                                                                        ridingEntitySearchRadius,
                                                                        connectedPlayer->UUIDRidingOn);
        if (entityRidingOn)
        {
            if (entityRidingOn->entityType == entityID::boat)
            {
                boat *boatRidingOn = (boat *)entityRidingOn;
                if (entityRidingOn->fluidArea > 0)
                {
                    if (connectedPlayer->wantsToGoLeft ^ connectedPlayer->wantsToGoRight)
                    {
                        boatRidingOn->paddleRotations +=
                            0.5 * secondsPerTick; // each second forth and back
                        if (math::isSummitBetween(boatRidingOn->paddleRotations,
                                                  boatRidingOn->paddleRotations +
                                                      0.5 * secondsPerTick,
                                                  [](cfp &x)
                                                  {
                                                      return -boat::getPaddleAngle(x);
                                                  }))
                        {
                            waterPaddleSound->playRandomSound(boatRidingOn->dimensionIn,
                                                              boatRidingOn->position);
                        }
                        // cfp nextRotation = boatRidingOn->paddleRotations + 0.5 * secondsPerTick;

                        cfp &paddleForce = boatSpeed *
                                           getFrictionMultiplier(entityRidingOn->getWeight(),
                                                                 entityRidingOn->fluidArea *
                                                                     waterWeightPerCubicMeter);

                        if (connectedPlayer->wantsToGoLeft)
                        {
                            entityRidingOn->speed.x -= paddleForce;
                        }
                        else if (connectedPlayer->wantsToGoRight)
                        {
                            entityRidingOn->speed.x += paddleForce;
                        }
                    }
                }
            }
        }
    }
    if (worldFocus)
    {
        int scrollDelta = connectedPlayer->screen.scrollDelta;
        if (scrollDelta != 0)
        {
            if (connectedEntity->entityType == entityID::human)
            {
                if (connectedPlayer->screen.holdingDownKey((vk)keyID::camera))
                {
                    connectedPlayer->visibleRangeXWalk *= pow(0.95, scrollDelta);
                }
                connectedPlayer->desiredRightHandSlotIndex = math::mod(
                    connectedPlayer->desiredRightHandSlotIndex - scrollDelta,
                    StandardInventoryColumnCount);
            }
            // totalScrollDelta = 0;
        }
        connectedPlayer->rightHandSlotIndex = connectedPlayer->desiredRightHandSlotIndex;
        connectedPlayer->itemHolding = connectedPlayer->hotbarSlots->getSlot(cveci2(connectedPlayer->rightHandSlotIndex, 0));

        itemStack *selectedHotbarSlot = connectedPlayer->hotbarSlots->getSlot(
            cveci2(connectedPlayer->rightHandSlotIndex, 0));
        if (worldFocus && connectedPlayer->screen.clickedFocused[sf::Mouse::Middle] &&
            connectedPlayer->currentGameMode == gameModeID::creative)
        {
            // pick block
            selectedHotbarSlot->clearData();
            selectedHotbarSlot->stackItemID = (itemID)connectedPlayer->dimensionIn->getBlockID(
                connectedPlayer->selectedBlockPosition);
            selectedHotbarSlot->count = 1;
        }
        connectedPlayer->itemHolding = selectedHotbarSlot;
    }
    connectedPlayer->flipBodyToLookingDirection();
}

void playerControlledAI::serializeValue(nbtSerializer &s)
{
    // we don't have to serialize tickssincestoppedjumping, that would only cause surprise moments where you f.e. stop flying when you reconnect because you're holding space

    // s.serializeValue(std::wstring(L"visible range x"), visibleRange.x);
    // s.serializeValue(std::wstring(L"visible range when walking"), visibleRangeXWalk);
}
