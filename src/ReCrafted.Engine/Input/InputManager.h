// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "ReCrafted.h"
#include "Core/EngineComponent.h"
#include "Input/InputDevice.h"
#include "Input/NullDevice.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

#include <spp.h>

class InputManager : public EngineComponent<InputManager>
{
    friend class EngineMain;

private:
    SCRIPTING_API_IMPL()

private:
    spp::sparse_hash_map<int, InputDevice*> m_deviceMap;
    int m_deviceCount;

private:
    NullDevice m_nullDevice;
    Keyboard m_keyboard;
    Mouse m_mouse;

protected:
    void OnInit() override;
    void OnDispose() override;

    void UpdateInput();

public: /* -- Action Maps -- */

public: /* -- Basic Input -- */

public: /* -- Devices -- */
    /**
     * \brief Finds device of given id.
     * \param deviceId The unique device identificator.
     * \return The device reference.
     * 
     * \note When there is no device with specified id, this function throws an std::exception.
     */
    static InputDevice& GetDevice(int deviceId);

    /**
     * \brief Finds device of given id and casts it into TDevice type.
     * \tparam TDevice The target device type. Eg.: Keyboard, Gamepad etc.
     * \param deviceId The unique device identificator.
     * \return The device reference.
     *
     * \note When there is no device with specified id, this function throws an std::exception.
     */
    template<typename TDevice>
    static TDevice& GetDevice(int deviceId);

    /**
     * \brief Finds default device of given device type. 
     * \param deviceType The device type.
     * \return The default device reference.
     * 
     * \note This function guarantees that it will always return proper device for
     * DeviceType::Keyboard and DeviceType::Mouse.
     * 
     * \note When there is no device with specified type (other than Mouse and Keyboard) 
     * this function throws an std::exception.
     */
    static InputDevice& GetDevice(DeviceType deviceType);

    /**
     * \brief Finds device of given type and casts it into TDevice type.
     * \tparam TDevice The target device type. Eg.: Keyboard, Gamepad etc.
     * \param deviceType The device type.
     * \return The device reference.
     * 
     * \note This function guarantees that it will always return proper device for
     * DeviceType::Keyboard and DeviceType::Mouse.
     *
     * \note When there is no device with specified type (other than Mouse and Keyboard) 
     * this function throws an std::exception.
     */
    template<typename TDevice>
    static TDevice& GetDevice(DeviceType deviceType);

    /**
     * \brief Gets device count.
     * \return The current device count.
     */
    static int GetDeviceCount();
};

template <typename TDevice>
TDevice& InputManager::GetDevice(const int deviceId)
{
    return static_cast<TDevice&>(GetDevice(deviceId));
}

template <typename TDevice>
TDevice& InputManager::GetDevice(const DeviceType deviceType)
{
    return static_cast<TDevice&>(GetDevice(deviceType));
}

#endif // INPUTMANAGER_H
