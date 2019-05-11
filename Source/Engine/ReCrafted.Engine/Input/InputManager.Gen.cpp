// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
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
    
    static Vector2 InputManager_GetCursorPosition5() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::GetCursorPosition();
        return _returnValue;
    }
    
    static Vector2 InputManager_GetCursorDelta6() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::GetCursorDelta();
        return _returnValue;
    }
    
    static float InputManager_GetScrollDelta7() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::GetScrollDelta();
        return _returnValue;
    }
    
    static void InputManager_SetShowCursor8(bool showCursor) 
    {
        MAIN_THREAD_ONLY();
        InputManager::SetShowCursor(showCursor);
    }
    
    static bool InputManager_GetShowCursor9() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::GetShowCursor();
        return _returnValue;
    }
    
    static void InputManager_SetLockCursor10(bool lockCursor) 
    {
        MAIN_THREAD_ONLY();
        InputManager::SetLockCursor(lockCursor);
    }
    
    static bool InputManager_GetLockCursor11() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::GetLockCursor();
        return _returnValue;
    }
    
    static bool InputManager_IsKey12(Key key) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::IsKey(key);
        return _returnValue;
    }
    
    static bool InputManager_IsKeyDown13(Key key) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::IsKeyDown(key);
        return _returnValue;
    }
    
    static bool InputManager_IsKeyUp14(Key key) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = InputManager::IsKeyUp(key);
        return _returnValue;
    }
};

void InputManager::InitRuntime() 
{
    API_BIND("ReCrafted.API.Input.InputManager::InternalIsButton", &APIProxy::InputManager_IsButton1);
    API_BIND("ReCrafted.API.Input.InputManager::InternalIsButtonDown", &APIProxy::InputManager_IsButtonDown2);
    API_BIND("ReCrafted.API.Input.InputManager::InternalIsButtonUp", &APIProxy::InputManager_IsButtonUp3);
    API_BIND("ReCrafted.API.Input.InputManager::InternalSetCursorPosition", &APIProxy::InputManager_SetCursorPosition4);
    API_BIND("ReCrafted.API.Input.InputManager::InternalGetCursorPosition", &APIProxy::InputManager_GetCursorPosition5);
    API_BIND("ReCrafted.API.Input.InputManager::InternalGetCursorDelta", &APIProxy::InputManager_GetCursorDelta6);
    API_BIND("ReCrafted.API.Input.InputManager::InternalGetScrollDelta", &APIProxy::InputManager_GetScrollDelta7);
    API_BIND("ReCrafted.API.Input.InputManager::InternalSetShowCursor", &APIProxy::InputManager_SetShowCursor8);
    API_BIND("ReCrafted.API.Input.InputManager::InternalGetShowCursor", &APIProxy::InputManager_GetShowCursor9);
    API_BIND("ReCrafted.API.Input.InputManager::InternalSetLockCursor", &APIProxy::InputManager_SetLockCursor10);
    API_BIND("ReCrafted.API.Input.InputManager::InternalGetLockCursor", &APIProxy::InputManager_GetLockCursor11);
    API_BIND("ReCrafted.API.Input.InputManager::InternalIsKey", &APIProxy::InputManager_IsKey12);
    API_BIND("ReCrafted.API.Input.InputManager::InternalIsKeyDown", &APIProxy::InputManager_IsKeyDown13);
    API_BIND("ReCrafted.API.Input.InputManager::InternalIsKeyUp", &APIProxy::InputManager_IsKeyUp14);
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