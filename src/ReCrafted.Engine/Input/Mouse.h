// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef MOUSE_H
#define MOUSE_H

#include "ReCrafted.h"
#include "Input/InputDevice.h"
#include "Input/MouseButtons.h"

/**
 * \brief Mouse input device.
 */
class Mouse : public InputDevice
{
    friend class InputManager;

public:
    Mouse() = default;
    ~Mouse() override = default;

public:
    /**
     * \brief The name of this device.
     */
    const char* Name() override
    {
        return "Default Mouse";
    }

    /**
     * \brief The type of this device.
     */
    DeviceType Type() override
    {
        return DeviceType::Mouse;
    }

public:
    /**
     * \brief Returns true when specified button is being held for at least one frame.
     * \param button The button.
     */
    bool IsButton(Button button);

    /**
     * \brief Returns true when specified button is has been pressed this frame.
     * \param button The button.
     */
    bool IsButtonDown(Button button);

    /**
     * \brief Returns true when specified button is has been released this frame.
     * \param button The button.
     */
    bool IsButtonUp(Button button);
};

#endif // MOUSE_H
