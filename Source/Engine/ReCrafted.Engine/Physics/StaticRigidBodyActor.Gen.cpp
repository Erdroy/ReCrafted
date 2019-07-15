// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "StaticRigidBodyActor.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static Object* StaticRigidBodyActor_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'StaticRigidBodyActor' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<StaticRigidBodyActor>();
    }
};

void StaticRigidBodyActor::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::StaticRigidBodyActor_CreateObject);
    
    MONO_REGISTER_OBJECT_TYPE(StaticRigidBodyActor);
}

const char* StaticRigidBodyActor::Fullname() 
{
    return "ReCrafted.API.Physics.StaticRigidBodyActor";
}

const char* StaticRigidBodyActor::Name() 
{
    return "StaticRigidBodyActor";
}

const char* StaticRigidBodyActor::Namespace() 
{
    return "ReCrafted.API.Physics";
}