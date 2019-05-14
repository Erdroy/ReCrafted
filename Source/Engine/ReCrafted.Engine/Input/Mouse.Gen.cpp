// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Mouse.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void Mouse_EmitInput1(Mouse* instance, Button button, ButtonState buttonState) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mouse");
        instance->EmitInput(button, buttonState);
    }
    
    static void Mouse_EmitCursor2(Mouse* instance, Vector2 position, Vector2 delta) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mouse");
        instance->EmitCursor(position, delta);
    }
    
    static void Mouse_EmitScroll3(Mouse* instance, float delta) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mouse");
        instance->EmitScroll(delta);
    }
    
    static MonoString* Mouse_DeviceName4(Mouse* instance) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mouse");
        const auto _returnValue = instance->DeviceName();
        return MONO_STRING_FROM_CSTR(_returnValue);
    }
    
    static DeviceType Mouse_Type5(Mouse* instance) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mouse");
        const auto _returnValue = instance->Type();
        return _returnValue;
    }
    
    static bool Mouse_IsButton6(Mouse* instance, Button button) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mouse");
        const auto _returnValue = instance->IsButton(button);
        return _returnValue;
    }
    
    static bool Mouse_IsButtonDown7(Mouse* instance, Button button) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mouse");
        const auto _returnValue = instance->IsButtonDown(button);
        return _returnValue;
    }
    
    static bool Mouse_IsButtonUp8(Mouse* instance, Button button) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mouse");
        const auto _returnValue = instance->IsButtonUp(button);
        return _returnValue;
    }
    
    static void Mouse_SetCursorPosition9(Mouse* instance, const Vector2& position) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mouse");
        instance->SetCursorPosition(position);
    }
    
    static Vector2 Mouse_GetCursorPosition10(Mouse* instance) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mouse");
        const auto _returnValue = instance->GetCursorPosition();
        return _returnValue;
    }
    
    static Vector2 Mouse_GetCursorDelta11(Mouse* instance) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mouse");
        const auto _returnValue = instance->GetCursorDelta();
        return _returnValue;
    }
    
    static float Mouse_GetScrollDelta12(Mouse* instance) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mouse");
        const auto _returnValue = instance->GetScrollDelta();
        return _returnValue;
    }
    
    static void Mouse_SetShowCursor13(Mouse* instance, bool cursorShow) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mouse");
        instance->SetShowCursor(cursorShow);
    }
    
    static bool Mouse_GetShowCursor14(Mouse* instance) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mouse");
        const auto _returnValue = instance->GetShowCursor();
        return _returnValue;
    }
    
    static void Mouse_SetLockCursor15(Mouse* instance, bool cursorLock) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mouse");
        instance->SetLockCursor(cursorLock);
    }
    
    static bool Mouse_GetLockCursor16(Mouse* instance) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mouse");
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
    
    MONO_REGISTER_OBJECT_TYPE(Mouse);
    API_BIND("ReCrafted.API.Input.Mouse::InternalEmitInput", &APIProxy::Mouse_EmitInput1);
    API_BIND("ReCrafted.API.Input.Mouse::InternalEmitCursor", &APIProxy::Mouse_EmitCursor2);
    API_BIND("ReCrafted.API.Input.Mouse::InternalEmitScroll", &APIProxy::Mouse_EmitScroll3);
    API_BIND("ReCrafted.API.Input.Mouse::InternalDeviceName", &APIProxy::Mouse_DeviceName4);
    API_BIND("ReCrafted.API.Input.Mouse::InternalType", &APIProxy::Mouse_Type5);
    API_BIND("ReCrafted.API.Input.Mouse::InternalIsButton", &APIProxy::Mouse_IsButton6);
    API_BIND("ReCrafted.API.Input.Mouse::InternalIsButtonDown", &APIProxy::Mouse_IsButtonDown7);
    API_BIND("ReCrafted.API.Input.Mouse::InternalIsButtonUp", &APIProxy::Mouse_IsButtonUp8);
    API_BIND("ReCrafted.API.Input.Mouse::InternalSetCursorPosition", &APIProxy::Mouse_SetCursorPosition9);
    API_BIND("ReCrafted.API.Input.Mouse::InternalGetCursorPosition", &APIProxy::Mouse_GetCursorPosition10);
    API_BIND("ReCrafted.API.Input.Mouse::InternalGetCursorDelta", &APIProxy::Mouse_GetCursorDelta11);
    API_BIND("ReCrafted.API.Input.Mouse::InternalGetScrollDelta", &APIProxy::Mouse_GetScrollDelta12);
    API_BIND("ReCrafted.API.Input.Mouse::InternalSetShowCursor", &APIProxy::Mouse_SetShowCursor13);
    API_BIND("ReCrafted.API.Input.Mouse::InternalGetShowCursor", &APIProxy::Mouse_GetShowCursor14);
    API_BIND("ReCrafted.API.Input.Mouse::InternalSetLockCursor", &APIProxy::Mouse_SetLockCursor15);
    API_BIND("ReCrafted.API.Input.Mouse::InternalGetLockCursor", &APIProxy::Mouse_GetLockCursor16);
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