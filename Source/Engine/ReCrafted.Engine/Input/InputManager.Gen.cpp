// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "InputManager.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static bool InputManager_IsButton(Button button) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::IsButton(button);
        return _returnValue;
    }
    
    static bool InputManager_IsButtonDown(Button button) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::IsButtonDown(button);
        return _returnValue;
    }
    
    static bool InputManager_IsButtonUp(Button button) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::IsButtonUp(button);
        return _returnValue;
    }
    
    static void InputManager_SetCursorPosition(const Vector2& position) 
    {
        MAIN_THREAD_ONLY();
        InputManager::SetCursorPosition(position);
    }
    
    static Vector2 InputManager_GetCursorPosition() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::GetCursorPosition();
        return _returnValue;
    }
    
    static Vector2 InputManager_GetCursorDelta() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::GetCursorDelta();
        return _returnValue;
    }
    
    static float InputManager_GetScrollDelta() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::GetScrollDelta();
        return _returnValue;
    }
    
    static void InputManager_SetShowCursor(bool showCursor) 
    {
        MAIN_THREAD_ONLY();
        InputManager::SetShowCursor(showCursor);
    }
    
    static bool InputManager_GetShowCursor() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::GetShowCursor();
        return _returnValue;
    }
    
    static void InputManager_SetLockCursor(bool lockCursor) 
    {
        MAIN_THREAD_ONLY();
        InputManager::SetLockCursor(lockCursor);
    }
    
    static bool InputManager_GetLockCursor() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::GetLockCursor();
        return _returnValue;
    }
    
    static bool InputManager_IsKey(Key key) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::IsKey(key);
        return _returnValue;
    }
    
    static bool InputManager_IsKeyDown(Key key) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::IsKeyDown(key);
        return _returnValue;
    }
    
    static bool InputManager_IsKeyUp(Key key) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::IsKeyUp(key);
        return _returnValue;
    }
};

void InputManager::InitRuntime() 
{
    API_BIND("ReCrafted.API.Input.InputManager::InternalIsButton", &APIProxy::InputManager_IsButton);
    API_BIND("ReCrafted.API.Input.InputManager::InternalIsButtonDown", &APIProxy::InputManager_IsButtonDown);
    API_BIND("ReCrafted.API.Input.InputManager::InternalIsButtonUp", &APIProxy::InputManager_IsButtonUp);
    API_BIND("ReCrafted.API.Input.InputManager::InternalSetCursorPosition", &APIProxy::InputManager_SetCursorPosition);
    API_BIND("ReCrafted.API.Input.InputManager::InternalGetCursorPosition", &APIProxy::InputManager_GetCursorPosition);
    API_BIND("ReCrafted.API.Input.InputManager::InternalGetCursorDelta", &APIProxy::InputManager_GetCursorDelta);
    API_BIND("ReCrafted.API.Input.InputManager::InternalGetScrollDelta", &APIProxy::InputManager_GetScrollDelta);
    API_BIND("ReCrafted.API.Input.InputManager::InternalSetShowCursor", &APIProxy::InputManager_SetShowCursor);
    API_BIND("ReCrafted.API.Input.InputManager::InternalGetShowCursor", &APIProxy::InputManager_GetShowCursor);
    API_BIND("ReCrafted.API.Input.InputManager::InternalSetLockCursor", &APIProxy::InputManager_SetLockCursor);
    API_BIND("ReCrafted.API.Input.InputManager::InternalGetLockCursor", &APIProxy::InputManager_GetLockCursor);
    API_BIND("ReCrafted.API.Input.InputManager::InternalIsKey", &APIProxy::InputManager_IsKey);
    API_BIND("ReCrafted.API.Input.InputManager::InternalIsKeyDown", &APIProxy::InputManager_IsKeyDown);
    API_BIND("ReCrafted.API.Input.InputManager::InternalIsKeyUp", &APIProxy::InputManager_IsKeyUp);
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