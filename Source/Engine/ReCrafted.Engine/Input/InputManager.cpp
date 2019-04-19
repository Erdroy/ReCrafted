// ReCrafted (c) 2016-2019 Always Too Late

#include "InputManager.h"
//#include "Common/Profiler/Profiler.h"

void InputManager::DispatchInput()
{
    //CPU_PROFILE_SCOPE(0, __FUNCTION__);

    m_frameInputLock.LockNow();
    for (auto&& input : m_frameInput)
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
    auto& lock2 = GetInstance()->m_actionMapsLock;
    {
        ScopeLock(lock2);

        for (auto& actionMap : m_actionMaps)
            actionMap.second->Update();
    }
}

void InputManager::Initialize()
{
    auto& lock = GetInstance()->m_deviceMapLock;
    ScopeLock(lock);

    // Create default devices
    m_nullDevice = new NullDevice();// Object::CreateInstance<NullDevice>("ReCrafted.API.Input", "NullDevice");
    m_keyboard = new Keyboard(); //Object::CreateInstance<Keyboard>("ReCrafted.API.Input", "Keyboard");
    m_mouse = new Mouse(); //Object::CreateInstance<Mouse>("ReCrafted.API.Input", "Mouse");
    
    // Register default devices
    m_deviceMap.insert(std::make_pair(0, m_nullDevice)); m_deviceCount++;
    m_deviceMap.insert(std::make_pair(1, m_keyboard)); m_deviceCount++;
    m_deviceMap.insert(std::make_pair(2, m_mouse)); m_deviceCount++;
    
    // Reserve some memory for frame input (32 entries is more than enough).
    m_frameInput.reserve(32);

    // TODO: Look for gamepad devices using XInput
}

void InputManager::Shutdown()
{
    delete m_nullDevice;//Object::Destroy(m_nullDevice);
    delete m_keyboard;//Object::Destroy(m_keyboard);
    delete m_mouse;//Object::Destroy(m_mouse);
    

    m_nullDevice = nullptr;
    m_keyboard = nullptr;
    m_mouse = nullptr;

    m_frameInput.clear();
    m_frameInput.resize(0);
    m_frameInput.shrink_to_fit();

    m_deviceMap.clear();
    m_deviceMap.resize(0);

    m_actionMaps.clear();
    m_actionMaps.resize(0);
}

void InputManager::OnLateUpdate()
{
    auto& lock = GetInstance()->m_deviceMapLock;
    ScopeLock(lock);

    // LateUpdate devices
    for (auto& device : m_deviceMap)
        device.second->LateUpdate();

    // Clear frame input
    GetInstance()->BeginEmitInput();
    GetInstance()->m_frameInput.clear();
    GetInstance()->EndEmitInput();
}

void InputManager::UpdateInput()
{
    //CPU_PROFILE_SCOPE(0, __FUNCTION__);

    // Update devices
    auto& lock1 = GetInstance()->m_deviceMapLock;
    {
        ScopeLock(lock1);

        for (auto& device : m_deviceMap)
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
    auto& lock = GetInstance()->m_actionMapsLock;
    ScopeLock(lock);

    // Create new action map
    auto actionMap = new ActionMap(); // Object::CreateInstance<ActionMap>("ReCrafted.API.Input", "ActionMap");

    // Set action map name
    actionMap->SetName(name);

    // Register map
    GetInstance()->m_actionMaps.insert(std::make_pair(std::string(name), actionMap));
    GetInstance()->m_actionMapCount++;

    return *actionMap;
}

ActionMap* InputManager::GetActionMap(const char* name)
{
    auto& lock = GetInstance()->m_actionMapsLock;
    ScopeLock(lock);

    const auto it = GetInstance()->m_actionMaps.find(std::string(name));
    if (it == GetInstance()->m_actionMaps.end())
        return nullptr;
    return it->second;
}

void InputManager::DestroyActionMap(ActionMap*& actionMap)
{
    auto& lock = GetInstance()->m_actionMapsLock;
    ScopeLock(lock);

    // Remove ActionMap from map
    const auto it = GetInstance()->m_actionMaps.find(actionMap->GetName());
    if (it == GetInstance()->m_actionMaps.end())
        return;
    GetInstance()->m_actionMaps.erase(it);

    // Destroy ActionMap object
    delete actionMap;//Object::Destroy(actionMap);
    actionMap = nullptr;
}

bool InputManager::IsButton(const Button button)
{
    return GetInstance()->m_mouse->IsButton(button);
}

bool InputManager::IsButtonDown(const Button button)
{
    return GetInstance()->m_mouse->IsButtonDown(button);
}

bool InputManager::IsButtonUp(const Button button)
{
    return GetInstance()->m_mouse->IsButtonUp(button);
}

void InputManager::SetCursorPosition(const Vector2& position)
{
    return GetInstance()->m_mouse->SetCursorPosition(position);
}

Vector2 InputManager::GetCursorPosition()
{
    return GetInstance()->m_mouse->GetCursorPosition();
}

Vector2 InputManager::GetCursorDelta()
{
    return GetInstance()->m_mouse->GetCursorDelta();
}

float InputManager::GetScrollDelta()
{
    return GetInstance()->m_mouse->GetScrollDelta();
}

void InputManager::SetShowCursor(const bool show)
{
    GetInstance()->m_mouse->SetShowCursor(show);
}

bool InputManager::GetShowCursor()
{
    return GetInstance()->m_mouse->GetShowCursor();
}

void InputManager::SetLockCursor(const bool lock)
{
    GetInstance()->m_mouse->SetLockCursor(lock);
}

bool InputManager::GetLockCursor()
{
    return GetInstance()->m_mouse->GetLockCursor();
}

bool InputManager::IsKey(const Key key)
{
    return GetInstance()->m_keyboard->IsKey(key);
}

bool InputManager::IsKeyDown(const Key key)
{
    return GetInstance()->m_keyboard->IsKeyDown(key);
}

bool InputManager::IsKeyUp(const Key key)
{
    return GetInstance()->m_keyboard->IsKeyUp(key);
}

InputDevice& InputManager::GetDevice(const int deviceId)
{
    auto& lock = GetInstance()->m_deviceMapLock;
    ScopeLock(lock);

    const auto it = GetInstance()->m_deviceMap.find(deviceId);
    if (it == GetInstance()->m_deviceMap.end())
        throw std::exception("Input device not found");

    return *it->second;
}

InputDevice& InputManager::GetDevice(const DeviceType deviceType)
{
    switch(deviceType)
    {
    case DeviceType::Mouse:
        return *GetInstance()->m_mouse;
    case DeviceType::Keyboard:
        return *GetInstance()->m_keyboard;

    case DeviceType::Gamepad:
    case DeviceType::Unknown:
    case DeviceType::Count:
    default: break;
    }

    throw std::exception("Input device not found");
}

int InputManager::GetDeviceCount()
{
    return GetInstance()->m_deviceCount;
}
