// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "PhysicsScene.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void PhysicsScene_AttachActor1(PhysicsScene* instance, RigidBodyActor* actor) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "PhysicsScene");
        instance->AttachActor(actor);
    }
    
    static void PhysicsScene_DetachActor2(PhysicsScene* instance, RigidBodyActor* actor) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "PhysicsScene");
        instance->DetachActor(actor);
    }
    
    static void PhysicsScene_Get_Enabled1(PhysicsScene* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "PhysicsScene");
        const auto _returnValue = instance->Enabled();
        *data = _returnValue;
    }

    static void PhysicsScene_Set_Enabled1(PhysicsScene* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "PhysicsScene");
        instance->Enabled(*data);
    }
    
    static Object* PhysicsScene_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'PhysicsScene' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<PhysicsScene>();
    }
};

void PhysicsScene::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::PhysicsScene_CreateObject);
    
    MONO_REGISTER_OBJECT_TYPE(PhysicsScene);
    API_BIND("ReCrafted.API.Physics.PhysicsScene::InternalAttachActor", &APIProxy::PhysicsScene_AttachActor1);
    API_BIND("ReCrafted.API.Physics.PhysicsScene::InternalDetachActor", &APIProxy::PhysicsScene_DetachActor2);
    API_BIND("ReCrafted.API.Physics.PhysicsScene::Get_InternalEnabled", &APIProxy::PhysicsScene_Get_Enabled1);
    API_BIND("ReCrafted.API.Physics.PhysicsScene::Set_InternalEnabled", &APIProxy::PhysicsScene_Set_Enabled1);
}

const char* PhysicsScene::Fullname() 
{
    return "ReCrafted.API.Physics.PhysicsScene";
}

const char* PhysicsScene::Name() 
{
    return "PhysicsScene";
}

const char* PhysicsScene::Namespace() 
{
    return "ReCrafted.API.Physics";
}