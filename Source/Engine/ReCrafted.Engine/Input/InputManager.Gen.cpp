// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "InputManager.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static bool InputManager_IsButton1(Button button) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::IsButton(button);
        return _returnValue;
    }
    
    static bool InputManager_IsButtonDown2(Button button) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::IsButtonDown(button);
        return _returnValue;
    }
    
    static bool InputManager_IsButtonUp3(Button button) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::IsButtonUp(button);
        return _returnValue;
    }
    
    static void InputManager_SetCursorPosition4(const Vector2& position) 
    {
        MAIN_THREAD_ONLY();
        InputManager::SetCursorPosition(position);
    }
    
    static bool InputManager_IsKey5(Key key) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::IsKey(key);
        return _returnValue;
    }
    
    static bool InputManager_IsKeyDown6(Key key) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::IsKeyDown(key);
        return _returnValue;
    }
    
    static bool InputManager_IsKeyUp7(Key key) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::IsKeyUp(key);
        return _returnValue;
    }
    
    static MonoObject* InputManager_GetDevice8(int deviceId) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::GetDevice(deviceId);
        return _returnValue != nullptr ? _returnValue->ToManaged() : nullptr;
    }
    
    static MonoObject* InputManager_GetDevice9(DeviceType deviceType) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::GetDevice(deviceType);
        return _returnValue != nullptr ? _returnValue->ToManaged() : nullptr;
    }
    
    static void InputManager_Get_CursorPosition1(Vector2* data) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::GetCursorPosition();
        *data = _returnValue;
    }
    
    static void InputManager_Get_CursorDelta2(Vector2* data) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::GetCursorDelta();
        *data = _returnValue;
    }
    
    static void InputManager_Get_ScrollDelta3(float* data) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::GetScrollDelta();
        *data = _returnValue;
    }
    
    static void InputManager_Get_ShowCursor4(bool* data) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::GetShowCursor();
        *data = _returnValue;
    }

    static void InputManager_Set_ShowCursor4(bool* data) 
    {
        MAIN_THREAD_ONLY();
        InputManager::SetShowCursor(*data);
    }
    
    static void InputManager_Get_LockCursor5(bool* data) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::GetLockCursor();
        *data = _returnValue;
    }

    static void InputManager_Set_LockCursor5(bool* data) 
    {
        MAIN_THREAD_ONLY();
        InputManager::SetLockCursor(*data);
    }
    
    static void InputManager_Get_DeviceCount6(int* data) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::GetDeviceCount();
        *data = _returnValue;
    }
};

void InputManager::InitRuntime() 
{
    MONO_REGISTER_OBJECT_TYPE(InputManager);
    API_BIND("ReCrafted.API.Input.InputManager::InternalIsButton", &APIProxy::InputManager_IsButton1);
    API_BIND("ReCrafted.API.Input.InputManager::InternalIsButtonDown", &APIProxy::InputManager_IsButtonDown2);
    API_BIND("ReCrafted.API.Input.InputManager::InternalIsButtonUp", &APIProxy::InputManager_IsButtonUp3);
    API_BIND("ReCrafted.API.Input.InputManager::InternalSetCursorPosition", &APIProxy::InputManager_SetCursorPosition4);
    API_BIND("ReCrafted.API.Input.InputManager::InternalIsKey", &APIProxy::InputManager_IsKey5);
    API_BIND("ReCrafted.API.Input.InputManager::InternalIsKeyDown", &APIProxy::InputManager_IsKeyDown6);
    API_BIND("ReCrafted.API.Input.InputManager::InternalIsKeyUp", &APIProxy::InputManager_IsKeyUp7);
    API_BIND("ReCrafted.API.Input.InputManager::InternalGetDevice", &APIProxy::InputManager_GetDevice8);
    API_BIND("ReCrafted.API.Input.InputManager::InternalGetDevice", &APIProxy::InputManager_GetDevice9);
    API_BIND("ReCrafted.API.Input.InputManager::Get_InternalCursorPosition", &APIProxy::InputManager_Get_CursorPosition1);
    API_BIND("ReCrafted.API.Input.InputManager::Get_InternalCursorDelta", &APIProxy::InputManager_Get_CursorDelta2);
    API_BIND("ReCrafted.API.Input.InputManager::Get_InternalScrollDelta", &APIProxy::InputManager_Get_ScrollDelta3);
    API_BIND("ReCrafted.API.Input.InputManager::Get_InternalShowCursor", &APIProxy::InputManager_Get_ShowCursor4);
    API_BIND("ReCrafted.API.Input.InputManager::Set_InternalShowCursor", &APIProxy::InputManager_Set_ShowCursor4);
    API_BIND("ReCrafted.API.Input.InputManager::Get_InternalLockCursor", &APIProxy::InputManager_Get_LockCursor5);
    API_BIND("ReCrafted.API.Input.InputManager::Set_InternalLockCursor", &APIProxy::InputManager_Set_LockCursor5);
    API_BIND("ReCrafted.API.Input.InputManager::Get_InternalDeviceCount", &APIProxy::InputManager_Get_DeviceCount6);
}

const char* InputManager::Fullname() 
{
    return "ReCrafted.API.Input.InputManager";
}

const char* InputManager::Name() 
{
    return "InputManager";
}

const char* InputManager::Namespace() 
{
    return "ReCrafted.API.Input";
}