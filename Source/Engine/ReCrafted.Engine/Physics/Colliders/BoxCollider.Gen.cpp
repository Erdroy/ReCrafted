// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "BoxCollider.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void BoxCollider_Get_Size1(BoxCollider* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "BoxCollider");
        const auto _returnValue = instance->Size();
        *data = _returnValue;
    }

    static void BoxCollider_Set_Size1(BoxCollider* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "BoxCollider");
        instance->Size(*data);
    }
    
    static Object* BoxCollider_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'BoxCollider' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<BoxCollider>();
    }
};

void BoxCollider::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::BoxCollider_CreateObject);
    
    MONO_REGISTER_OBJECT_TYPE(BoxCollider);
    API_BIND("ReCrafted.API.Physics.BoxCollider::Get_InternalSize", &APIProxy::BoxCollider_Get_Size1);
    API_BIND("ReCrafted.API.Physics.BoxCollider::Set_InternalSize", &APIProxy::BoxCollider_Set_Size1);
}

const char* BoxCollider::Fullname() 
{
    return "ReCrafted.API.Physics.BoxCollider";
}

const char* BoxCollider::Name() 
{
    return "BoxCollider";
}

const char* BoxCollider::Namespace() 
{
    return "ReCrafted.API.Physics";
}