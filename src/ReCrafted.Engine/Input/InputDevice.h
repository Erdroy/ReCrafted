// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef INPUTDEVICE_H
#define INPUTDEVICE_H

#include "ReCrafted.h"
#include "Scripting/Object.h"

enum class DeviceType : byte
{
    Unknown,

    Keyboard,
    Mouse,
    Gamepad,

    Count
};

class InputDevice : public Object
{
    friend class InputManager;

public:
    InputDevice() = default;
    InputDevice(InputDevice& other) = delete;
    InputDevice(InputDevice&& other) = delete;

protected:
    virtual ~InputDevice() = default;

protected:
    virtual void Update() { }
    virtual void LateUpdate() { }

public:
    /**
     * \brief The name of this device.
     */
    virtual const char* Name() = 0;

    /**
     * \brief The type of this device.
     */
    virtual DeviceType Type() = 0;

public:
    InputDevice& operator=(const InputDevice& other) = delete;
};

#endif // INPUTDEVICE_H
