// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Input/InputDevice.h"

/**
 * \brief Null input device.
 */
class NullDevice final : public InputDevice
{
    friend class InputManager;

public:
    NullDevice() = default;
    ~NullDevice() override = default;

public:
    /**
     * \brief The name of this device.
     */
    const char* Name() override
    {
        return "Null Device";
    }

    /**
     * \brief The type of this device.
     */
    DeviceType Type() override
    {
        return DeviceType::Unknown;
    }
};
