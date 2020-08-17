// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "DynamicRigidBodyActor.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static Object* DynamicRigidBodyActor_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'DynamicRigidBodyActor' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<DynamicRigidBodyActor>();
    }
};

void DynamicRigidBodyActor::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::DynamicRigidBodyActor_CreateObject);
    
    MONO_REGISTER_OBJECT_TYPE(DynamicRigidBodyActor);
}

const char* DynamicRigidBodyActor::Fullname() 
{
    return "ReCrafted.API.Physics.DynamicRigidBodyActor";
}

const char* DynamicRigidBodyActor::Name() 
{
    return "DynamicRigidBodyActor";
}

const char* DynamicRigidBodyActor::Namespace() 
{
    return "ReCrafted.API.Physics";
}