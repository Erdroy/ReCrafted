// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "NullDevice.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static MonoString* NullDevice_DeviceName(NullDevice* instance) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        const auto _returnValue = instance->DeviceName();
        return MONO_STRING_FROM_CSTR(_returnValue);
    }
    
    static DeviceType NullDevice_Type(NullDevice* instance) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance);
        const auto _returnValue = instance->Type();
        return _returnValue;
    }
    
    static Object* NullDevice_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'NullDevice' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<NullDevice>();
    }
};

void NullDevice::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::NullDevice_CreateObject);
    
    API_BIND("ReCrafted.API.Input.NullDevice::InternalDeviceName", &APIProxy::NullDevice_DeviceName);
    API_BIND("ReCrafted.API.Input.NullDevice::InternalType", &APIProxy::NullDevice_Type);
}

const char* NullDevice::Fullname() 
{
    return "ReCrafted.API.Input.NullDevice";
}

const char* NullDevice::Name() 
{
    return "NullDevice";
}

const char* NullDevice::Namespace() 
{
    return "ReCrafted.API.Input";
}