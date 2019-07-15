// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Collider.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void Collider_Get_LocalPosition1(Collider* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Collider");
        const auto _returnValue = instance->LocalPosition();
        *data = _returnValue;
    }

    static void Collider_Set_LocalPosition1(Collider* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Collider");
        instance->LocalPosition(*data);
    }
    
    static void Collider_Get_LocalRotation2(Collider* instance, Quaternion* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Collider");
        const auto _returnValue = instance->LocalRotation();
        *data = _returnValue;
    }

    static void Collider_Set_LocalRotation2(Collider* instance, Quaternion* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Collider");
        instance->LocalRotation(*data);
    }
    
    static void Collider_Get_Material3(Collider* instance, MonoObject** data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Collider");
        const auto _fRetValue = instance->Material();
        const auto _returnValue = _fRetValue != nullptr ? _fRetValue ->ToManaged() : nullptr;
        *data = _returnValue;
    }

    static void Collider_Set_Material3(Collider* instance, PhysicsMaterial* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Collider");
        instance->Material(data);
    }
    
    static Object* Collider_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'Collider' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<Collider>();
    }
};

void Collider::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::Collider_CreateObject);
    
    MONO_REGISTER_OBJECT_TYPE(Collider);
    API_BIND("ReCrafted.API.Physics.Collider::Get_InternalLocalPosition", &APIProxy::Collider_Get_LocalPosition1);
    API_BIND("ReCrafted.API.Physics.Collider::Set_InternalLocalPosition", &APIProxy::Collider_Set_LocalPosition1);
    API_BIND("ReCrafted.API.Physics.Collider::Get_InternalLocalRotation", &APIProxy::Collider_Get_LocalRotation2);
    API_BIND("ReCrafted.API.Physics.Collider::Set_InternalLocalRotation", &APIProxy::Collider_Set_LocalRotation2);
    API_BIND("ReCrafted.API.Physics.Collider::Get_InternalMaterial", &APIProxy::Collider_Get_Material3);
    API_BIND("ReCrafted.API.Physics.Collider::Set_InternalMaterial", &APIProxy::Collider_Set_Material3);
}

const char* Collider::Fullname() 
{
    return "ReCrafted.API.Physics.Collider";
}

const char* Collider::Name() 
{
    return "Collider";
}

const char* Collider::Namespace() 
{
    return "ReCrafted.API.Physics";
}