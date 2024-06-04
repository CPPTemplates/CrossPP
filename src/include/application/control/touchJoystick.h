#pragma once
#include "control.h"

struct joystickEventArgs : controlEventArgs
{
    uint finger = 0;
    joystickEventArgs(control& sender, cuint& finger) : controlEventArgs(sender), finger(finger)
    {

    }
};

typedef eventHandler<joystickEventArgs> joystickEventHandler;

struct touchJoystick : public control {
    rectangle2 range;
    touchJoystick(crectangle2& range);
    vec2 value;
    joystickEventHandler onJoystickTouch;
    joystickEventHandler onJoystickTouchEnd;
    bool fingerDown = false;

    void mouseDown(cveci2& position, cmb& button) override;
    void drag(cveci2& originalPosition, cveci2& position, cmb& button) override;
    void drop(cveci2& originalPosition, cveci2& position, cmb& button) override;
    void render(cveci2& position, const texture& renderTarget) override;

};
