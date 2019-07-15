// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "RigidBodyActor.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void RigidBodyActor_AddForce1(RigidBodyActor* instance, const Vector3& force, ForceMode forceMode, bool awake) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->AddForce(force, forceMode, awake);
    }
    
    static void RigidBodyActor_AddTorque2(RigidBodyActor* instance, const Vector3& torque, ForceMode forceMode, bool awake) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->AddTorque(torque, forceMode, awake);
    }
    
    static MonoObject* RigidBodyActor_Create3() 
    {
        const auto _returnValue = RigidBodyActor::Create();
        return _returnValue != nullptr ? _returnValue->ToManaged() : nullptr;
    }
    
    static MonoObject* RigidBodyActor_CreateDynamic4() 
    {
        const auto _returnValue = RigidBodyActor::CreateDynamic();
        return _returnValue != nullptr ? _returnValue->ToManaged() : nullptr;
    }
    
    static MonoObject* RigidBodyActor_CreateStatic5() 
    {
        const auto _returnValue = RigidBodyActor::CreateStatic();
        return _returnValue != nullptr ? _returnValue->ToManaged() : nullptr;
    }
    
    static void RigidBodyActor_Get_Scene1(RigidBodyActor* instance, MonoObject** data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _fRetValue = instance->Scene();
        const auto _returnValue = _fRetValue != nullptr ? _fRetValue ->ToManaged() : nullptr;
        *data = _returnValue;
    }
    
    static void RigidBodyActor_Get_Position2(RigidBodyActor* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->Position();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_Position2(RigidBodyActor* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->Position(*data);
    }
    
    static void RigidBodyActor_Get_Rotation3(RigidBodyActor* instance, Quaternion* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->Rotation();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_Rotation3(RigidBodyActor* instance, Quaternion* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->Rotation(*data);
    }
    
    static void RigidBodyActor_Get_CollisionLayer4(RigidBodyActor* instance, uint32_t* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->CollisionLayer();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_CollisionLayer4(RigidBodyActor* instance, uint32_t* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->CollisionLayer(*data);
    }
    
    static void RigidBodyActor_Get_Velocity5(RigidBodyActor* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->Velocity();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_Velocity5(RigidBodyActor* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->Velocity(*data);
    }
    
    static void RigidBodyActor_Get_AngularVelocity6(RigidBodyActor* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->AngularVelocity();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_AngularVelocity6(RigidBodyActor* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->AngularVelocity(*data);
    }
    
    static void RigidBodyActor_Get_CentreOfMass7(RigidBodyActor* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->CentreOfMass();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_CentreOfMass7(RigidBodyActor* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->CentreOfMass(*data);
    }
    
    static void RigidBodyActor_Get_MaxAngularVelocity8(RigidBodyActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->MaxAngularVelocity();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_MaxAngularVelocity8(RigidBodyActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->MaxAngularVelocity(*data);
    }
    
    static void RigidBodyActor_Get_LinearDamping9(RigidBodyActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->LinearDamping();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_LinearDamping9(RigidBodyActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->LinearDamping(*data);
    }
    
    static void RigidBodyActor_Get_AngularDamping10(RigidBodyActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->AngularDamping();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_AngularDamping10(RigidBodyActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->AngularDamping(*data);
    }
    
    static void RigidBodyActor_Get_Mass11(RigidBodyActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->Mass();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_Mass11(RigidBodyActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->Mass(*data);
    }
    
    static void RigidBodyActor_Get_CCD12(RigidBodyActor* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->CCD();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_CCD12(RigidBodyActor* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->CCD(*data);
    }
    
    static void RigidBodyActor_Get_IsDynamic13(RigidBodyActor* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->IsDynamic();
        *data = _returnValue;
    }
    
    static void RigidBodyActor_Get_IsSleeping14(RigidBodyActor* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->IsSleeping();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_IsSleeping14(RigidBodyActor* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->IsSleeping(*data);
    }
    
    static void RigidBodyActor_Get_IsKinematic15(RigidBodyActor* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->IsKinematic();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_IsKinematic15(RigidBodyActor* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->IsKinematic(*data);
    }
};

void RigidBodyActor::InitRuntime() 
{
    MONO_REGISTER_OBJECT_TYPE(RigidBodyActor);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::InternalAddForce", &APIProxy::RigidBodyActor_AddForce1);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::InternalAddTorque", &APIProxy::RigidBodyActor_AddTorque2);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::InternalCreate", &APIProxy::RigidBodyActor_Create3);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::InternalCreateDynamic", &APIProxy::RigidBodyActor_CreateDynamic4);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::InternalCreateStatic", &APIProxy::RigidBodyActor_CreateStatic5);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalScene", &APIProxy::RigidBodyActor_Get_Scene1);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalPosition", &APIProxy::RigidBodyActor_Get_Position2);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalPosition", &APIProxy::RigidBodyActor_Set_Position2);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalRotation", &APIProxy::RigidBodyActor_Get_Rotation3);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalRotation", &APIProxy::RigidBodyActor_Set_Rotation3);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalCollisionLayer", &APIProxy::RigidBodyActor_Get_CollisionLayer4);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalCollisionLayer", &APIProxy::RigidBodyActor_Set_CollisionLayer4);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalVelocity", &APIProxy::RigidBodyActor_Get_Velocity5);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalVelocity", &APIProxy::RigidBodyActor_Set_Velocity5);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalAngularVelocity", &APIProxy::RigidBodyActor_Get_AngularVelocity6);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalAngularVelocity", &APIProxy::RigidBodyActor_Set_AngularVelocity6);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalCentreOfMass", &APIProxy::RigidBodyActor_Get_CentreOfMass7);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalCentreOfMass", &APIProxy::RigidBodyActor_Set_CentreOfMass7);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalMaxAngularVelocity", &APIProxy::RigidBodyActor_Get_MaxAngularVelocity8);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalMaxAngularVelocity", &APIProxy::RigidBodyActor_Set_MaxAngularVelocity8);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalLinearDamping", &APIProxy::RigidBodyActor_Get_LinearDamping9);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalLinearDamping", &APIProxy::RigidBodyActor_Set_LinearDamping9);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalAngularDamping", &APIProxy::RigidBodyActor_Get_AngularDamping10);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalAngularDamping", &APIProxy::RigidBodyActor_Set_AngularDamping10);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalMass", &APIProxy::RigidBodyActor_Get_Mass11);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalMass", &APIProxy::RigidBodyActor_Set_Mass11);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalCCD", &APIProxy::RigidBodyActor_Get_CCD12);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalCCD", &APIProxy::RigidBodyActor_Set_CCD12);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalIsDynamic", &APIProxy::RigidBodyActor_Get_IsDynamic13);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalIsSleeping", &APIProxy::RigidBodyActor_Get_IsSleeping14);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalIsSleeping", &APIProxy::RigidBodyActor_Set_IsSleeping14);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalIsKinematic", &APIProxy::RigidBodyActor_Get_IsKinematic15);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalIsKinematic", &APIProxy::RigidBodyActor_Set_IsKinematic15);
}

const char* RigidBodyActor::Fullname() 
{
    return "ReCrafted.API.Physics.RigidBodyActor";
}

const char* RigidBodyActor::Name() 
{
    return "RigidBodyActor";
}

const char* RigidBodyActor::Namespace() 
{
    return "ReCrafted.API.Physics";
}