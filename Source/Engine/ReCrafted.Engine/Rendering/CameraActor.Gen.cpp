// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "CameraActor.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void CameraActor_Get_Camera1(CameraActor* instance, MonoObject** data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "CameraActor");
        const auto _fRetValue = instance->GetCamera();
        const auto _returnValue = _fRetValue != nullptr ? _fRetValue ->ToManaged() : nullptr;
        *data = _returnValue;
    }
    
    static Object* CameraActor_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'CameraActor' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<CameraActor>();
    }
};

void CameraActor::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::CameraActor_CreateObject);
    
    MONO_REGISTER_OBJECT_TYPE(CameraActor);
    API_BIND("ReCrafted.API.Rendering.CameraActor::Get_InternalCamera", &APIProxy::CameraActor_Get_Camera1);
}

const char* CameraActor::Fullname() 
{
    return "ReCrafted.API.Rendering.CameraActor";
}

const char* CameraActor::Name() 
{
    return "CameraActor";
}

const char* CameraActor::Namespace() 
{
    return "ReCrafted.API.Rendering";
}