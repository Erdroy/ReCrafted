// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "ReCrafted.h"
#include "Input/InputDevice.h"
#include "Input/KeyboardKeys.h"

/**
 * \brief Keyboard input device.
 */
class Keyboard : public InputDevice
{
    friend class InputManager;

public:
    Keyboard() = default;
    ~Keyboard() override = default;

public:
    /**
     * \brief The name of this device.
     */
    const char* Name() override
    {
        return "Default Keyboard";
    }

    /**
     * \brief The type of this device.
     */
    DeviceType Type() override
    {
        return DeviceType::Keyboard;
    }

public:
    /**
     * \brief Returns true when specified key is being held for at least one frame.
     * \param key The key.
     */
    bool IsKey(Key key);

    /**
     * \brief Returns true when specified key is has been pressed this frame.
     * \param key The key.
     */
    bool IsKeyDown(Key key);

    /**
     * \brief Returns true when specified key is has been released this frame.
     * \param key The key.
     */
    bool IsKeyUp(Key key);
};

#endif // KEYBOARD_H
