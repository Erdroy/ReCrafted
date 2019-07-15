// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "SphereCollider.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void SphereCollider_Get_Radius1(SphereCollider* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "SphereCollider");
        const auto _returnValue = instance->Radius();
        *data = _returnValue;
    }

    static void SphereCollider_Set_Radius1(SphereCollider* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "SphereCollider");
        instance->Radius(*data);
    }
    
    static Object* SphereCollider_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'SphereCollider' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<SphereCollider>();
    }
};

void SphereCollider::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::SphereCollider_CreateObject);
    
    MONO_REGISTER_OBJECT_TYPE(SphereCollider);
    API_BIND("ReCrafted.API.Physics.SphereCollider::Get_InternalRadius", &APIProxy::SphereCollider_Get_Radius1);
    API_BIND("ReCrafted.API.Physics.SphereCollider::Set_InternalRadius", &APIProxy::SphereCollider_Set_Radius1);
}

const char* SphereCollider::Fullname() 
{
    return "ReCrafted.API.Physics.SphereCollider";
}

const char* SphereCollider::Name() 
{
    return "SphereCollider";
}

const char* SphereCollider::Namespace() 
{
    return "ReCrafted.API.Physics";
}