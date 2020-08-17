// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "PhysicsMaterial.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void PhysicsMaterial_Get_StaticFriction1(PhysicsMaterial* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "PhysicsMaterial");
        const auto _returnValue = instance->StaticFriction();
        *data = _returnValue;
    }

    static void PhysicsMaterial_Set_StaticFriction1(PhysicsMaterial* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "PhysicsMaterial");
        instance->StaticFriction(*data);
    }
    
    static void PhysicsMaterial_Get_DynamicFriction2(PhysicsMaterial* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "PhysicsMaterial");
        const auto _returnValue = instance->DynamicFriction();
        *data = _returnValue;
    }

    static void PhysicsMaterial_Set_DynamicFriction2(PhysicsMaterial* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "PhysicsMaterial");
        instance->DynamicFriction(*data);
    }
    
    static void PhysicsMaterial_Get_Restitution3(PhysicsMaterial* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "PhysicsMaterial");
        const auto _returnValue = instance->Restitution();
        *data = _returnValue;
    }

    static void PhysicsMaterial_Set_Restitution3(PhysicsMaterial* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "PhysicsMaterial");
        instance->Restitution(*data);
    }
    
    static void PhysicsMaterial_Get_Default4(MonoObject** data) 
    {
        MAIN_THREAD_ONLY();
        const auto _fRetValue = PhysicsMaterial::Default();
        const auto _returnValue = _fRetValue != nullptr ? _fRetValue ->ToManaged() : nullptr;
        *data = _returnValue;
    }
    
    static Object* PhysicsMaterial_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'PhysicsMaterial' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<PhysicsMaterial>();
    }
};

void PhysicsMaterial::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::PhysicsMaterial_CreateObject);
    
    MONO_REGISTER_OBJECT_TYPE(PhysicsMaterial);
    API_BIND("ReCrafted.API.Physics.PhysicsMaterial::Get_InternalStaticFriction", &APIProxy::PhysicsMaterial_Get_StaticFriction1);
    API_BIND("ReCrafted.API.Physics.PhysicsMaterial::Set_InternalStaticFriction", &APIProxy::PhysicsMaterial_Set_StaticFriction1);
    API_BIND("ReCrafted.API.Physics.PhysicsMaterial::Get_InternalDynamicFriction", &APIProxy::PhysicsMaterial_Get_DynamicFriction2);
    API_BIND("ReCrafted.API.Physics.PhysicsMaterial::Set_InternalDynamicFriction", &APIProxy::PhysicsMaterial_Set_DynamicFriction2);
    API_BIND("ReCrafted.API.Physics.PhysicsMaterial::Get_InternalRestitution", &APIProxy::PhysicsMaterial_Get_Restitution3);
    API_BIND("ReCrafted.API.Physics.PhysicsMaterial::Set_InternalRestitution", &APIProxy::PhysicsMaterial_Set_Restitution3);
    API_BIND("ReCrafted.API.Physics.PhysicsMaterial::Get_InternalDefault", &APIProxy::PhysicsMaterial_Get_Default4);
}

const char* PhysicsMaterial::Fullname() 
{
    return "ReCrafted.API.Physics.PhysicsMaterial";
}

const char* PhysicsMaterial::Name() 
{
    return "PhysicsMaterial";
}

const char* PhysicsMaterial::Namespace() 
{
    return "ReCrafted.API.Physics";
}