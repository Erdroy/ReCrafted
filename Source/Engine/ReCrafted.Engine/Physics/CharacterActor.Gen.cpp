// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "CharacterActor.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static CharacterCollisionFlags CharacterActor_Move1(CharacterActor* instance, const Vector3& displacement) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "CharacterActor");
        const auto _returnValue = instance->Move(displacement);
        return _returnValue;
    }
    
    static void CharacterActor_Get_CollisionLayer1(CharacterActor* instance, uint32_t* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "CharacterActor");
        const auto _returnValue = instance->CollisionLayer();
        *data = _returnValue;
    }

    static void CharacterActor_Set_CollisionLayer1(CharacterActor* instance, uint32_t* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "CharacterActor");
        instance->CollisionLayer(*data);
    }
    
    static void CharacterActor_Get_UpDirection2(CharacterActor* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "CharacterActor");
        const auto _returnValue = instance->UpDirection();
        *data = _returnValue;
    }

    static void CharacterActor_Set_UpDirection2(CharacterActor* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "CharacterActor");
        instance->UpDirection(*data);
    }
    
    static void CharacterActor_Get_Height3(CharacterActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "CharacterActor");
        const auto _returnValue = instance->Height();
        *data = _returnValue;
    }

    static void CharacterActor_Set_Height3(CharacterActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "CharacterActor");
        instance->Height(*data);
    }
    
    static void CharacterActor_Get_Radius4(CharacterActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "CharacterActor");
        const auto _returnValue = instance->Radius();
        *data = _returnValue;
    }

    static void CharacterActor_Set_Radius4(CharacterActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "CharacterActor");
        instance->Radius(*data);
    }
    
    static void CharacterActor_Get_StepOffset5(CharacterActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "CharacterActor");
        const auto _returnValue = instance->StepOffset();
        *data = _returnValue;
    }

    static void CharacterActor_Set_StepOffset5(CharacterActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "CharacterActor");
        instance->StepOffset(*data);
    }
    
    static void CharacterActor_Get_ContactOffset6(CharacterActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "CharacterActor");
        const auto _returnValue = instance->ContactOffset();
        *data = _returnValue;
    }

    static void CharacterActor_Set_ContactOffset6(CharacterActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "CharacterActor");
        instance->ContactOffset(*data);
    }
    
    static void CharacterActor_Get_SlopeLimit7(CharacterActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "CharacterActor");
        const auto _returnValue = instance->SlopeLimit();
        *data = _returnValue;
    }

    static void CharacterActor_Set_SlopeLimit7(CharacterActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "CharacterActor");
        instance->SlopeLimit(*data);
    }
    
    static void CharacterActor_Get_Velocity8(CharacterActor* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "CharacterActor");
        const auto _returnValue = instance->Velocity();
        *data = _returnValue;
    }
    
    static void CharacterActor_Get_IsGrounded9(CharacterActor* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "CharacterActor");
        const auto _returnValue = instance->IsGrounded();
        *data = _returnValue;
    }
    
    static Object* CharacterActor_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'CharacterActor' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<CharacterActor>();
    }
};

void CharacterActor::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::CharacterActor_CreateObject);
    
    MONO_REGISTER_OBJECT_TYPE(CharacterActor);
    API_BIND("ReCrafted.API.Physics.CharacterActor::InternalMove", &APIProxy::CharacterActor_Move1);
    API_BIND("ReCrafted.API.Physics.CharacterActor::Get_InternalCollisionLayer", &APIProxy::CharacterActor_Get_CollisionLayer1);
    API_BIND("ReCrafted.API.Physics.CharacterActor::Set_InternalCollisionLayer", &APIProxy::CharacterActor_Set_CollisionLayer1);
    API_BIND("ReCrafted.API.Physics.CharacterActor::Get_InternalUpDirection", &APIProxy::CharacterActor_Get_UpDirection2);
    API_BIND("ReCrafted.API.Physics.CharacterActor::Set_InternalUpDirection", &APIProxy::CharacterActor_Set_UpDirection2);
    API_BIND("ReCrafted.API.Physics.CharacterActor::Get_InternalHeight", &APIProxy::CharacterActor_Get_Height3);
    API_BIND("ReCrafted.API.Physics.CharacterActor::Set_InternalHeight", &APIProxy::CharacterActor_Set_Height3);
    API_BIND("ReCrafted.API.Physics.CharacterActor::Get_InternalRadius", &APIProxy::CharacterActor_Get_Radius4);
    API_BIND("ReCrafted.API.Physics.CharacterActor::Set_InternalRadius", &APIProxy::CharacterActor_Set_Radius4);
    API_BIND("ReCrafted.API.Physics.CharacterActor::Get_InternalStepOffset", &APIProxy::CharacterActor_Get_StepOffset5);
    API_BIND("ReCrafted.API.Physics.CharacterActor::Set_InternalStepOffset", &APIProxy::CharacterActor_Set_StepOffset5);
    API_BIND("ReCrafted.API.Physics.CharacterActor::Get_InternalContactOffset", &APIProxy::CharacterActor_Get_ContactOffset6);
    API_BIND("ReCrafted.API.Physics.CharacterActor::Set_InternalContactOffset", &APIProxy::CharacterActor_Set_ContactOffset6);
    API_BIND("ReCrafted.API.Physics.CharacterActor::Get_InternalSlopeLimit", &APIProxy::CharacterActor_Get_SlopeLimit7);
    API_BIND("ReCrafted.API.Physics.CharacterActor::Set_InternalSlopeLimit", &APIProxy::CharacterActor_Set_SlopeLimit7);
    API_BIND("ReCrafted.API.Physics.CharacterActor::Get_InternalVelocity", &APIProxy::CharacterActor_Get_Velocity8);
    API_BIND("ReCrafted.API.Physics.CharacterActor::Get_InternalIsGrounded", &APIProxy::CharacterActor_Get_IsGrounded9);
}

const char* CharacterActor::Fullname() 
{
    return "ReCrafted.API.Physics.CharacterActor";
}

const char* CharacterActor::Name() 
{
    return "CharacterActor";
}

const char* CharacterActor::Namespace() 
{
    return "ReCrafted.API.Physics";
}