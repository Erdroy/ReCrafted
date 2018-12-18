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

bool InputManager::IsButton(const Button button)
{
    ASSERT(m_instance);
    return m_instance->m_mouse.IsButton(button);
}

bool InputManager::IsButtonDown(const Button button)
{
    ASSERT(m_instance);
    return m_instance->m_mouse.IsButtonDown(button);
}

bool InputManager::IsButtonUp(const Button button)
{
    ASSERT(m_instance);
    return m_instance->m_mouse.IsButtonUp(button);
}

void InputManager::SetMousePosition(const Vector2& position)
{
    ASSERT(m_instance);
    return m_instance->m_mouse.SetMousePosition(position);
}

Vector2 InputManager::GetMousePosition()
{
    ASSERT(m_instance);
    return m_instance->m_mouse.GetMousePosition();
}

Vector2 InputManager::GetMouseDelta()
{
    ASSERT(m_instance);
    return m_instance->m_mouse.GetMouseDelta();
}

int InputManager::GetMouseScrollDelta()
{
    ASSERT(m_instance);
    return m_instance->m_mouse.GetMouseScrollDelta();
}

void InputManager::SetShowCursor(const bool show)
{
    ASSERT(m_instance);
    m_instance->m_mouse.SetShowCursor(show);
}

bool InputManager::GetShowCursor()
{
    ASSERT(m_instance);
    return m_instance->m_mouse.GetShowCursor();
}

void InputManager::SetLockCursor(const bool lock)
{
    ASSERT(m_instance);
    m_instance->m_mouse.SetLockCursor(lock);
}

bool InputManager::GetLockCursor()
{
    ASSERT(m_instance);
    return m_instance->m_mouse.GetLockCursor();
}

bool InputManager::IsKey(const Key key)
{
    ASSERT(m_instance);
    return m_instance->m_keyboard.IsKey(key);
}

bool InputManager::IsKeyDown(const Key key)
{
    ASSERT(m_instance);
    return m_instance->m_keyboard.IsKeyDown(key);
}

bool InputManager::IsKeyUp(const Key key)
{
    ASSERT(m_instance);
    return m_instance->m_keyboard.IsKeyUp(key);
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
