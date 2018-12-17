// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef INPUTDEVICE_H
#define INPUTDEVICE_H

#include "ReCrafted.h"

enum class DeviceType
{
    Unknown,

    Keyboard,
    Mouse,
    Gamepad,

    Count
};

class InputDevice
{
protected:
    virtual ~InputDevice() = default;

public:
    /**
     * \brief The name of this device.
     */
    virtual const char* Name() = 0;

    /**
     * \brief The type of this device.
     */
    virtual DeviceType Type() = 0;
};

#endif // INPUTDEVICE_H
