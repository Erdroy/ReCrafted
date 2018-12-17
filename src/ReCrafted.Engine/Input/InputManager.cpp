// ReCrafted (c) 2016-2018 Always Too Late

#include "InputManager.h"

SINGLETON_IMPL(InputManager)

void InputManager::OnInit()
{
    // Register default devices
    m_deviceMap.insert(std::make_pair(0, &m_nullDevice)); m_deviceCount++;
    m_deviceMap.insert(std::make_pair(1, &m_keyboard)); m_deviceCount++;
    m_deviceMap.insert(std::make_pair(2, &m_mouse)); m_deviceCount++;

    // TODO: Look for gamepad devices using XInput
}

void InputManager::OnDispose()
{
    m_deviceMap.clear();
}

void InputManager::UpdateInput()
{
    // Update devices
    for(var& device : m_deviceMap)
        device.second->Update();

    // TODO: Update ActionMaps
}

InputDevice& InputManager::GetDevice(const int deviceId)
{
    DEBUG_ASSERT(m_instance);

    cvar it = m_instance->m_deviceMap.find(deviceId);

    if (it == m_instance->m_deviceMap.end())
        throw std::exception("Input device not found");

    return *it->second;
}

InputDevice& InputManager::GetDevice(const DeviceType deviceType)
{
    DEBUG_ASSERT(m_instance);

    switch(deviceType)
    {
    case DeviceType::Mouse:
        return m_instance->m_mouse;
    case DeviceType::Keyboard:
        return m_instance->m_keyboard;

    case DeviceType::Gamepad:
    case DeviceType::Unknown:
    case DeviceType::Count:
    default: break;
    }

    throw std::exception("Input device not found");
}

int InputManager::GetDeviceCount()
{
    DEBUG_ASSERT(m_instance);
    return m_instance->m_deviceCount;
}
