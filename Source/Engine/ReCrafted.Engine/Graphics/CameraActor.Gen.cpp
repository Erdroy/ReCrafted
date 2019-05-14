// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "CameraActor.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
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
}

const char* CameraActor::Fullname() 
{
    return "ReCrafted.API.Graphics.CameraActor";
}

const char* CameraActor::Name() 
{
    return "CameraActor";
}

const char* CameraActor::Namespace() 
{
    return "ReCrafted.API.Graphics";
}