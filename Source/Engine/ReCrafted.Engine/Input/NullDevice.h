// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Input/InputDevice.h"

/// <summary>
///     Null input device.
/// </summary>
API_CLASS(public)
class NullDevice final : public InputDevice
{
    friend class InputManager;
    API_CLASS_BODY()

public:
    NullDevice() = default;
    ~NullDevice() override = default;

public:
    /// <summary>
    ///     The name of this device.
    /// </summary>
    API_FUNCTION()
    const char* DeviceName() override
    {
        return "Null Device";
    }
    
    /// <summary>
    ///     The type of this device.
    /// </summary>
    API_FUNCTION()
    DeviceType Type() override
    {
        return DeviceType::Unknown;
    }
};
