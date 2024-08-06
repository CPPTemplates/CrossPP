#include "touchJoystick.h"
#include "math/graphics/brush/brushes/colorMixer.h"
#include "math/graphics/brush/brushes/solidColorBrush.h"
#include "math/graphics/graphicsFunctions.h"

touchJoystick::touchJoystick(crectangle2 &range) : range(range)
{
    backGroundColor = colorPalette::transparent;
}

void touchJoystick::mouseDown(cveci2 &position, cmb &button)
{
    value = math::mapValue((vec2)position,
                           vec2(), (vec2)rect.size,
                           range.pos0, range.pos1());
    fingerDown = true;
    control::mouseDown(position, button);
    const joystickEventArgs &args = joystickEventArgs(*this, button);
    onJoystickTouch.invoke(args);
}

void touchJoystick::drag(cveci2 &originalPosition, cveci2 &position, cmb &button)
{
    value = math::mapValue((vec2)position,
                           vec2(), (vec2)rect.size,
                           range.pos0, range.pos1());
    control::drag(originalPosition, position, button);
    const joystickEventArgs &args = joystickEventArgs(*this, button);
    onJoystickTouch.invoke(args);
}

void touchJoystick::drop(cveci2 &originalPosition, cveci2 &position, cmb &button)
{
    fingerDown = false;
    value = math::mapValue((vec2)position,
                           vec2(), (vec2)rect.size,
                           range.pos0, range.pos1());
    control::drop(originalPosition, position, button);
    const joystickEventArgs &args = joystickEventArgs(*this, button);
    onJoystickTouchEnd.invoke(args);
}

void touchJoystick::render(cveci2 &position, const texture &renderTarget)
{
    // control::render(position, renderTarget);
    cvec2 &screenPos = math::mapValue(value, range.pos0, range.pos1(), (cvec2)position, (cvec2)position + rect.size);
    const auto &currentBrush = fingerDown ? brushes::green : brushes::orange;
    fillLine(renderTarget, (vec2)rect.getCenter(), screenPos, currentBrush);
    fillEllipseCentered(renderTarget,
                        rectangle2(screenPos,
                                   cvec2(rect.size * 0.2)),
                        currentBrush);

    fillEllipse(renderTarget, rect, colorMixer(solidColorBrush(color(colorPalette::white, 0x20)), renderTarget));
}
