// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef NULLDEVICE_H
#define NULLDEVICE_H

#include "ReCrafted.h"
#include "InputDevice.h"

/**
 * \brief Null input device.
 */
class NullDevice : public InputDevice
{
    friend class InputManager;

private:
    SCRIPTING_API_IMPL();

public:
    NullDevice() = default;
    ~NullDevice() override = default;

protected:
    void OnDestroy() override { }

    const char* GetObjectName() const override
    {
        return "NullDevice (InputDevice)";
    }

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

#endif // NULLDEVICE_H
