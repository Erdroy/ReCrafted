// ReCrafted (c) 2016-2018 Always Too Late

#include "InputManager.h"
#include "Common/Profiler/Profiler.h"

SINGLETON_IMPL(InputManager)

void InputManager::DispatchInput()
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

    m_frameInputLock.LockNow();
    for (rvar input : m_frameInput)
    {
        switch (input.first)
        {
        case InputType::Key:
        {
            m_keyboard->EmitInput(input.second.key, input.second.keyState);
            break;
        }
        case InputType::Button:
        {
            m_mouse->EmitInput(input.second.button, input.second.buttonState);
            break;
        }
        case InputType::Axis:
        {
            // TODO: Axis implementation for pads
            break;
        }
        case InputType::Character:
        {
            m_keyboard->EmitCharacter(input.second.character);
            break;
        }
        case InputType::Cursor:
        {
            m_mouse->EmitCursor(
                input.second.axis2D,
                Vector2(input.second.axis3D.x, input.second.axis3D.y) // TODO: replace with .xy() when vector accessors will be available
            );
            break;
        }
        case InputType::Scroll:
        {
            m_mouse->EmitScroll(input.second.axis1D);
            break;
        }
        case InputType::Unknown:
        case InputType::Count:
        default: break;
        }
    }
    m_frameInputLock.UnlockNow();

    // Update ActionMaps
    rvar lock2 = m_instance->m_actionMapsLock;
    {
        ScopeLock(lock2);

        for (var& actionMap : m_actionMaps)
            actionMap.second->Update();
    }
}

void InputManager::OnInit()
{
    rvar lock = m_instance->m_deviceMapLock;
    ScopeLock(lock);

    // Create default devices
    m_nullDevice = Object::CreateInstance<NullDevice>("ReCrafted.API.Input", "NullDevice");
    m_keyboard = Object::CreateInstance<Keyboard>("ReCrafted.API.Input", "Keyboard");
    m_mouse = Object::CreateInstance<Mouse>("ReCrafted.API.Input", "Mouse");

    // Register default devices
    m_deviceMap.insert(std::make_pair(0, m_nullDevice)); m_deviceCount++;
    m_deviceMap.insert(std::make_pair(1, m_keyboard)); m_deviceCount++;
    m_deviceMap.insert(std::make_pair(2, m_mouse)); m_deviceCount++;

    // Reserve some memory for frame input (32 entries is more than enough).
    m_frameInput.reserve(32);

    // TODO: Look for gamepad devices using XInput
}

void InputManager::OnDispose()
{
    Object::Destroy(m_nullDevice);
    Object::Destroy(m_keyboard);
    Object::Destroy(m_mouse);

    m_deviceMap.clear();
}

void InputManager::LateUpdate()
{
    rvar lock = m_instance->m_deviceMapLock;
    ScopeLock(lock);

    // LateUpdate devices
    for (var& device : m_deviceMap)
        device.second->LateUpdate();

    // Clear frame input
    m_instance->BeginEmitInput();
    m_instance->m_frameInput.clear();
    m_instance->EndEmitInput();
}

void InputManager::UpdateInput()
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

    // Update devices
    rvar lock1 = m_instance->m_deviceMapLock;
    {
        ScopeLock(lock1);

        for (var& device : m_deviceMap)
            device.second->Update();
    }
}

void InputManager::BeginEmitInput()
{
    m_frameInputLock.LockNow();
    m_isEmitting = true;
}

void InputManager::EndEmitInput()
{
    m_isEmitting = false;
    m_frameInputLock.UnlockNow();
}

void InputManager::EmitInput(InputType type, InputData data)
{
    if (!m_isEmitting)
        throw std::exception("You must call BeginEmitInput before you call EmitInput.");

    m_frameInput.emplace_back(std::make_pair(type, data));
}

const std::vector<InputManager::FrameInputItem>& InputManager::GetFrameInput() const
{
    return m_frameInput;
}

ActionMap& InputManager::CreateActionMap(const char* name)
{
    ASSERT(m_instance);

    rvar lock = m_instance->m_actionMapsLock;
    ScopeLock(lock);

    // Create new action map
    var actionMap = Object::CreateInstance<ActionMap>("ReCrafted.API.Input", "ActionMap");

    // Set action map name
    actionMap->SetName(name);

    // Register map
    m_instance->m_actionMaps.insert(std::make_pair(std::string(name), actionMap));
    m_instance->m_actionMapCount++;

    return *actionMap;
}

ActionMap* InputManager::GetActionMap(const char* name)
{
    ASSERT(m_instance);

    rvar lock = m_instance->m_actionMapsLock;
    ScopeLock(lock);

    cvar it = m_instance->m_actionMaps.find(std::string(name));
    if (it == m_instance->m_actionMaps.end())
        return nullptr;
    return it->second;
}

void InputManager::DestroyActionMap(ActionMap*& actionMap)
{
    ASSERT(m_instance);

    rvar lock = m_instance->m_actionMapsLock;
    ScopeLock(lock);

    // Remove ActionMap from map
    cvar it = m_instance->m_actionMaps.find(actionMap->GetName());
    if (it == m_instance->m_actionMaps.end())
        return;
    m_instance->m_actionMaps.erase(it);

    // Destroy ActionMap object
    Object::Destroy(actionMap);
    actionMap = nullptr;
}

bool InputManager::IsButton(const Button button)
{
    ASSERT(m_instance);
    return m_instance->m_mouse->IsButton(button);
}

bool InputManager::IsButtonDown(const Button button)
{
    ASSERT(m_instance);
    return m_instance->m_mouse->IsButtonDown(button);
}

bool InputManager::IsButtonUp(const Button button)
{
    ASSERT(m_instance);
    return m_instance->m_mouse->IsButtonUp(button);
}

void InputManager::SetCursorPosition(const Vector2& position)
{
    ASSERT(m_instance);
    return m_instance->m_mouse->SetCursorPosition(position);
}

Vector2 InputManager::GetCursorPosition()
{
    ASSERT(m_instance);
    return m_instance->m_mouse->GetCursorPosition();
}

Vector2 InputManager::GetCursorDelta()
{
    ASSERT(m_instance);
    return m_instance->m_mouse->GetCursorDelta();
}

float InputManager::GetScrollDelta()
{
    ASSERT(m_instance);
    return m_instance->m_mouse->GetScrollDelta();
}

void InputManager::SetShowCursor(const bool show)
{
    ASSERT(m_instance);
    m_instance->m_mouse->SetShowCursor(show);
}

bool InputManager::GetShowCursor()
{
    ASSERT(m_instance);
    return m_instance->m_mouse->GetShowCursor();
}

void InputManager::SetLockCursor(const bool lock)
{
    ASSERT(m_instance);
    m_instance->m_mouse->SetLockCursor(lock);
}

bool InputManager::GetLockCursor()
{
    ASSERT(m_instance);
    return m_instance->m_mouse->GetLockCursor();
}

bool InputManager::IsKey(const Key key)
{
    ASSERT(m_instance);
    return m_instance->m_keyboard->IsKey(key);
}

bool InputManager::IsKeyDown(const Key key)
{
    ASSERT(m_instance);
    return m_instance->m_keyboard->IsKeyDown(key);
}

bool InputManager::IsKeyUp(const Key key)
{
    ASSERT(m_instance);
    return m_instance->m_keyboard->IsKeyUp(key);
}

InputDevice& InputManager::GetDevice(const int deviceId)
{
    DEBUG_ASSERT(m_instance);

    rvar lock = m_instance->m_deviceMapLock;
    ScopeLock(lock);

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
        return *m_instance->m_mouse;
    case DeviceType::Keyboard:
        return *m_instance->m_keyboard;

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
