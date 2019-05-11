// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Keyboard.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void Keyboard_EmitInput1(Keyboard* instance, Key key, KeyState keyState) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Keyboard");
        instance->EmitInput(key, keyState);
    }
    
    static void Keyboard_EmitCharacter2(Keyboard* instance, Char character) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Keyboard");
        instance->EmitCharacter(character);
    }
    
    static MonoString* Keyboard_DeviceName3(Keyboard* instance) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Keyboard");
        const auto _returnValue = instance->DeviceName();
        return MONO_STRING_FROM_CSTR(_returnValue);
    }
    
    static DeviceType Keyboard_Type4(Keyboard* instance) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Keyboard");
        const auto _returnValue = instance->Type();
        return _returnValue;
    }
    
    static Object* Keyboard_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'Keyboard' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<Keyboard>();
    }
};

void Keyboard::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::Keyboard_CreateObject);
    
    API_BIND("ReCrafted.API.Input.Keyboard::InternalEmitInput", &APIProxy::Keyboard_EmitInput1);
    API_BIND("ReCrafted.API.Input.Keyboard::InternalEmitCharacter", &APIProxy::Keyboard_EmitCharacter2);
    API_BIND("ReCrafted.API.Input.Keyboard::InternalDeviceName", &APIProxy::Keyboard_DeviceName3);
    API_BIND("ReCrafted.API.Input.Keyboard::InternalType", &APIProxy::Keyboard_Type4);
}

const char* Keyboard::Fullname() 
{
    return "ReCrafted.API.Input.Keyboard";
}

const char* Keyboard::Name() 
{
    return "Keyboard";
}

const char* Keyboard::Namespace() 
{
    return "ReCrafted.API.Input";
}