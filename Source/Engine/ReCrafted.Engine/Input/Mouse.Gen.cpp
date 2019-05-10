// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Mouse.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void Mouse_EmitInput(Mouse* instance, Button button, ButtonState buttonState) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        instance->EmitInput(button, buttonState);
    }
    
    static void Mouse_EmitCursor(Mouse* instance, Vector2 position, Vector2 delta) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        instance->EmitCursor(position, delta);
    }
    
    static void Mouse_EmitScroll(Mouse* instance, float delta) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        instance->EmitScroll(delta);
    }
    
    static MonoString* Mouse_DeviceName(Mouse* instance) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        const auto _returnValue = instance->DeviceName();
        return MONO_STRING_FROM_CSTR(_returnValue);
    }
    
    static DeviceType Mouse_Type(Mouse* instance) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        const auto _returnValue = instance->Type();
        return _returnValue;
    }
    
    static bool Mouse_IsButton(Mouse* instance, Button button) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        const auto _returnValue = instance->IsButton(button);
        return _returnValue;
    }
    
    static bool Mouse_IsButtonDown(Mouse* instance, Button button) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        const auto _returnValue = instance->IsButtonDown(button);
        return _returnValue;
    }
    
    static bool Mouse_IsButtonUp(Mouse* instance, Button button) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        const auto _returnValue = instance->IsButtonUp(button);
        return _returnValue;
    }
    
    static void Mouse_SetCursorPosition(Mouse* instance, const Vector2& position) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        instance->SetCursorPosition(position);
    }
    
    static Vector2 Mouse_GetCursorPosition(Mouse* instance) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        const auto _returnValue = instance->GetCursorPosition();
        return _returnValue;
    }
    
    static Vector2 Mouse_GetCursorDelta(Mouse* instance) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        const auto _returnValue = instance->GetCursorDelta();
        return _returnValue;
    }
    
    static float Mouse_GetScrollDelta(Mouse* instance) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        const auto _returnValue = instance->GetScrollDelta();
        return _returnValue;
    }
    
    static void Mouse_SetShowCursor(Mouse* instance, bool cursorShow) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        instance->SetShowCursor(cursorShow);
    }
    
    static bool Mouse_GetShowCursor(Mouse* instance) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        const auto _returnValue = instance->GetShowCursor();
        return _returnValue;
    }
    
    static void Mouse_SetLockCursor(Mouse* instance, bool cursorLock) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        instance->SetLockCursor(cursorLock);
    }
    
    static bool Mouse_GetLockCursor(Mouse* instance) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        const auto _returnValue = instance->GetLockCursor();
        return _returnValue;
    }
    
    static Object* Mouse_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'Mouse' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<Mouse>();
    }
};

void Mouse::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::Mouse_CreateObject);
    
    API_BIND("ReCrafted.API.Input.Mouse::InternalEmitInput", &APIProxy::Mouse_EmitInput);
    API_BIND("ReCrafted.API.Input.Mouse::InternalEmitCursor", &APIProxy::Mouse_EmitCursor);
    API_BIND("ReCrafted.API.Input.Mouse::InternalEmitScroll", &APIProxy::Mouse_EmitScroll);
    API_BIND("ReCrafted.API.Input.Mouse::InternalDeviceName", &APIProxy::Mouse_DeviceName);
    API_BIND("ReCrafted.API.Input.Mouse::InternalType", &APIProxy::Mouse_Type);
    API_BIND("ReCrafted.API.Input.Mouse::InternalIsButton", &APIProxy::Mouse_IsButton);
    API_BIND("ReCrafted.API.Input.Mouse::InternalIsButtonDown", &APIProxy::Mouse_IsButtonDown);
    API_BIND("ReCrafted.API.Input.Mouse::InternalIsButtonUp", &APIProxy::Mouse_IsButtonUp);
    API_BIND("ReCrafted.API.Input.Mouse::InternalSetCursorPosition", &APIProxy::Mouse_SetCursorPosition);
    API_BIND("ReCrafted.API.Input.Mouse::InternalGetCursorPosition", &APIProxy::Mouse_GetCursorPosition);
    API_BIND("ReCrafted.API.Input.Mouse::InternalGetCursorDelta", &APIProxy::Mouse_GetCursorDelta);
    API_BIND("ReCrafted.API.Input.Mouse::InternalGetScrollDelta", &APIProxy::Mouse_GetScrollDelta);
    API_BIND("ReCrafted.API.Input.Mouse::InternalSetShowCursor", &APIProxy::Mouse_SetShowCursor);
    API_BIND("ReCrafted.API.Input.Mouse::InternalGetShowCursor", &APIProxy::Mouse_GetShowCursor);
    API_BIND("ReCrafted.API.Input.Mouse::InternalSetLockCursor", &APIProxy::Mouse_SetLockCursor);
    API_BIND("ReCrafted.API.Input.Mouse::InternalGetLockCursor", &APIProxy::Mouse_GetLockCursor);
}

const char* Mouse::Fullname() 
{
    return "ReCrafted.API.Input.Mouse";
}

const char* Mouse::Name() 
{
    return "Mouse";
}

const char* Mouse::Namespace() 
{
    return "ReCrafted.API.Input";
}