// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
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
    virtual const char* DeviceName() = 0;

    /**
     * \brief The type of this device.
     */
    virtual DeviceType Type() = 0;

public:
    InputDevice& operator=(const InputDevice& other) = delete;
};

