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
    
    static void RigidBodyActor_Get_SyncMode2(RigidBodyActor* instance, RigidBodySyncMode* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->SyncMode();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_SyncMode2(RigidBodyActor* instance, RigidBodySyncMode* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->SyncMode(*data);
    }
    
    static void RigidBodyActor_Get_Position3(RigidBodyActor* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->Position();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_Position3(RigidBodyActor* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->Position(*data);
    }
    
    static void RigidBodyActor_Get_Rotation4(RigidBodyActor* instance, Quaternion* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->Rotation();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_Rotation4(RigidBodyActor* instance, Quaternion* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->Rotation(*data);
    }
    
    static void RigidBodyActor_Get_CollisionLayer5(RigidBodyActor* instance, uint32_t* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->CollisionLayer();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_CollisionLayer5(RigidBodyActor* instance, uint32_t* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->CollisionLayer(*data);
    }
    
    static void RigidBodyActor_Get_Velocity6(RigidBodyActor* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->Velocity();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_Velocity6(RigidBodyActor* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->Velocity(*data);
    }
    
    static void RigidBodyActor_Get_AngularVelocity7(RigidBodyActor* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->AngularVelocity();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_AngularVelocity7(RigidBodyActor* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->AngularVelocity(*data);
    }
    
    static void RigidBodyActor_Get_CentreOfMass8(RigidBodyActor* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->CentreOfMass();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_CentreOfMass8(RigidBodyActor* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->CentreOfMass(*data);
    }
    
    static void RigidBodyActor_Get_MaxAngularVelocity9(RigidBodyActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->MaxAngularVelocity();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_MaxAngularVelocity9(RigidBodyActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->MaxAngularVelocity(*data);
    }
    
    static void RigidBodyActor_Get_LinearDamping10(RigidBodyActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->LinearDamping();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_LinearDamping10(RigidBodyActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->LinearDamping(*data);
    }
    
    static void RigidBodyActor_Get_AngularDamping11(RigidBodyActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->AngularDamping();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_AngularDamping11(RigidBodyActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->AngularDamping(*data);
    }
    
    static void RigidBodyActor_Get_Mass12(RigidBodyActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->Mass();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_Mass12(RigidBodyActor* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->Mass(*data);
    }
    
    static void RigidBodyActor_Get_CCD13(RigidBodyActor* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->CCD();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_CCD13(RigidBodyActor* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->CCD(*data);
    }
    
    static void RigidBodyActor_Get_IsDynamic14(RigidBodyActor* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->IsDynamic();
        *data = _returnValue;
    }
    
    static void RigidBodyActor_Get_IsSleeping15(RigidBodyActor* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->IsSleeping();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_IsSleeping15(RigidBodyActor* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        instance->IsSleeping(*data);
    }
    
    static void RigidBodyActor_Get_IsKinematic16(RigidBodyActor* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "RigidBodyActor");
        const auto _returnValue = instance->IsKinematic();
        *data = _returnValue;
    }

    static void RigidBodyActor_Set_IsKinematic16(RigidBodyActor* instance, bool* data) 
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
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalSyncMode", &APIProxy::RigidBodyActor_Get_SyncMode2);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalSyncMode", &APIProxy::RigidBodyActor_Set_SyncMode2);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalPosition", &APIProxy::RigidBodyActor_Get_Position3);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalPosition", &APIProxy::RigidBodyActor_Set_Position3);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalRotation", &APIProxy::RigidBodyActor_Get_Rotation4);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalRotation", &APIProxy::RigidBodyActor_Set_Rotation4);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalCollisionLayer", &APIProxy::RigidBodyActor_Get_CollisionLayer5);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalCollisionLayer", &APIProxy::RigidBodyActor_Set_CollisionLayer5);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalVelocity", &APIProxy::RigidBodyActor_Get_Velocity6);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalVelocity", &APIProxy::RigidBodyActor_Set_Velocity6);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalAngularVelocity", &APIProxy::RigidBodyActor_Get_AngularVelocity7);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalAngularVelocity", &APIProxy::RigidBodyActor_Set_AngularVelocity7);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalCentreOfMass", &APIProxy::RigidBodyActor_Get_CentreOfMass8);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalCentreOfMass", &APIProxy::RigidBodyActor_Set_CentreOfMass8);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalMaxAngularVelocity", &APIProxy::RigidBodyActor_Get_MaxAngularVelocity9);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalMaxAngularVelocity", &APIProxy::RigidBodyActor_Set_MaxAngularVelocity9);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalLinearDamping", &APIProxy::RigidBodyActor_Get_LinearDamping10);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalLinearDamping", &APIProxy::RigidBodyActor_Set_LinearDamping10);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalAngularDamping", &APIProxy::RigidBodyActor_Get_AngularDamping11);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalAngularDamping", &APIProxy::RigidBodyActor_Set_AngularDamping11);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalMass", &APIProxy::RigidBodyActor_Get_Mass12);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalMass", &APIProxy::RigidBodyActor_Set_Mass12);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalCCD", &APIProxy::RigidBodyActor_Get_CCD13);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalCCD", &APIProxy::RigidBodyActor_Set_CCD13);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalIsDynamic", &APIProxy::RigidBodyActor_Get_IsDynamic14);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalIsSleeping", &APIProxy::RigidBodyActor_Get_IsSleeping15);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalIsSleeping", &APIProxy::RigidBodyActor_Set_IsSleeping15);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Get_InternalIsKinematic", &APIProxy::RigidBodyActor_Get_IsKinematic16);
    API_BIND("ReCrafted.API.Physics.RigidBodyActor::Set_InternalIsKinematic", &APIProxy::RigidBodyActor_Set_IsKinematic16);
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