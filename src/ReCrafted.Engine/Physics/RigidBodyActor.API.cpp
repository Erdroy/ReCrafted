// ReCrafted (c) 2016-2018 Always Too Late

#include "RigidBodyActor.h"
#include "Scripting/ScriptingAPI.h"
#include "PhysicsManager.h"

namespace Internal
{
    void GetRigidBodyVelocity(RigidBodyActor* actor, Vector3* value)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        *value = actor->GetVelocity();
    }

    void SetRigidBodyVelocity(RigidBodyActor* actor, Vector3* value)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        actor->SetVelocity(*value);
    }

    void GetRigidBodyAngularVelocity(RigidBodyActor* actor, Vector3* value)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        *value = actor->GetAngularVelocity();
    }

    void SetRigidBodyAngularVelocity(RigidBodyActor* actor, Vector3* value)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        actor->SetAngularVelocity(*value);
    }

    void GetRigidBodyCentreOfMass(RigidBodyActor* actor, Vector3* value)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        *value = actor->GetCentreOfMass();
    }

    void SetRigidBodyCentreOfMass(RigidBodyActor* actor, Vector3* value)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        actor->SetCentreOfMass(*value);
    }

    void AddForce(RigidBodyActor* actor, Vector3* force, int forceMode, const bool awake)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        actor->AddForce(*force, static_cast<ForceMode>(forceMode), awake);
    }

    void AddTorque(RigidBodyActor* actor, Vector3* torque, int forceMode, const bool awake)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        actor->AddTorque(*torque, static_cast<ForceMode>(forceMode), awake);
    }

    void AttachCollision(RigidBodyActor* actor, Collision* value)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        ASSERT(value);
        actor->AttachCollision(value);
    }

    void DetachCollision(RigidBodyActor* actor)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        actor->DetachCollision();
    }

    float GetRigidBodyMaxAngularVelocity(RigidBodyActor* actor)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        return actor->GetMaxAngularVelocity();
    }

    void SetRigidBodyMaxAngularVelocity(RigidBodyActor* actor, const float value)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        actor->SetMaxAngularVelocity(value);
    }

    float GetRigidBodyLinearDamping(RigidBodyActor* actor)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        return actor->GetLinearDamping();
    }

    void SetRigidBodyLinearDamping(RigidBodyActor* actor, const float value)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        actor->SetLinearDamping(value);
    }

    float GetRigidBodyAngularDamping(RigidBodyActor* actor)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        return actor->GetAngularDamping();
    }

    void SetRigidBodyAngularDamping(RigidBodyActor* actor, const float value)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        actor->SetAngularDamping(value);
    }

    float GetRigidBodyMass(RigidBodyActor* actor)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        return actor->GetMass();
    }

    void SetRigidBodyMass(RigidBodyActor* actor, const float value)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        actor->SetMass(value);
    }

    bool GetRigidBodyIsSleeping(RigidBodyActor* actor)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        return actor->IsSleeping();
    }

    void SetRigidBodyIsSleeping(RigidBodyActor* actor, const bool value)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        actor->IsSleeping(value);
    }

    bool GetRigidBodyIsKinematic(RigidBodyActor* actor)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        return actor->IsKinematic();
    }

    void SetRigidBodyIsKinematic(RigidBodyActor* actor, const bool value)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        actor->IsKinematic(value);
    }

    bool GetRigidBodyCCD(RigidBodyActor* actor)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        return actor->GetCCD();
    }

    void SetRigidBodyCCD(RigidBodyActor* actor, const bool value)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        actor->SetCCD(value);
    }

    bool GetRigidBodyIsDynamic(RigidBodyActor* actor)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        return actor->IsDynamic();
    }

    uint32_t GetRigidBodyCollisionLayer(RigidBodyActor* actor)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        return actor->GetCollisionLayer();
    }

    void SetRigidBodyCollisionLayer(RigidBodyActor* actor, const uint32_t value)
    {
        MAIN_THREAD_ONLY();
        ASSERT(actor);
        actor->SetCollisionLayer(value);
    }

    Object* CreateRigidBodyActor(bool createManagedInstance)
    {
        return RigidBodyActor::Create();
    }

    MonoObject* CreateDynamicRigidBodyActor()
    {
        return RigidBodyActor::CreateDynamic()->GetManagedPtr();
    }

    MonoObject* CreateStaticRigidBodyActor()
    {
        return RigidBodyActor::CreateStatic()->GetManagedPtr();
    }
}

const char* Actor<RigidBodyActor>::ManagedName = "RigidBodyActor";
const char* Actor<RigidBodyActor>::ManagedNamespace = "ReCrafted.API.Physics";

void RigidBodyActor::InitRuntime()
{
    API_REGISTER_OBJECT("ReCrafted.API.Physics.RigidBodyActor", &Internal::CreateRigidBodyActor);

    API_FILE("Physics/RigidBodyActor.Gen.cs");
    {
        API_USING("ReCrafted.API.Common");
        API_USING("ReCrafted.API.Common.Actors");
        API_USING("ReCrafted.API.Mathematics");

        API_COMMENT("RigidBodyActor actor");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Physics", "RigidBodyActor", "ActorBase", PARTIAL, NOCONSTRUCTOR);
        {
            API_METHOD(INTERNAL, REGULAR, "AddForce", NOPROXY, EXTERN);
            {
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_AddForce", &Internal::AddForce);

                API_PARAM("ref Vector3", "force");
                API_PARAM("int", "modeMode");
                API_PARAM("bool", "autoAwake");
            }
            API_METHOD_END();

            API_METHOD(INTERNAL, REGULAR, "AddTorque", NOPROXY, EXTERN);
            {
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_AddTorque", &Internal::AddForce);

                API_PARAM("ref Vector3", "torque");
                API_PARAM("int", "modeMode");
                API_PARAM("bool", "autoAwake");
            }
            API_METHOD_END();

            API_METHOD(INTERNAL, REGULAR, "AttachCollision", NOPROXY, EXTERN);
            {
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_AttachCollision", &Internal::AttachCollision);
                API_PARAM("IntPtr", "collisionNativePtr");
            }
            API_METHOD_END();

            API_METHOD(INTERNAL, REGULAR, "DetachCollision", NOPROXY, EXTERN);
            {
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_DetachCollision", &Internal::DetachCollision);
            }
            API_METHOD_END();

            API_METHOD(INTERNAL, STATIC, "CreateDynamic", EXTERN);
            {
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::CreateDynamic", &Internal::CreateDynamicRigidBodyActor);
                API_RETURN("RigidBodyActor");
            }
            API_METHOD_END();

            API_METHOD(INTERNAL, STATIC, "CreateStatic", EXTERN);
            {
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::CreateStatic", &Internal::CreateStaticRigidBodyActor);
                API_RETURN("RigidBodyActor");
            }
            API_METHOD_END();

            API_COMMENT("Gets or sets rigid body's current velocity.");
            API_PROPERTY(PUBLIC, REGULAR, "Vector3", "Velocity", GETSET, BY_REF);
            {
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_Velocity_Get", &Internal::GetRigidBodyVelocity);
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_Velocity_Set", &Internal::SetRigidBodyVelocity);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets rigid body's current angular velocity.");
            API_PROPERTY(PUBLIC, REGULAR, "Vector3", "AngularVelocity", GETSET, BY_REF);
            {
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_AngularVelocity_Get", &Internal::GetRigidBodyAngularVelocity);
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_AngularVelocity_Set", &Internal::SetRigidBodyAngularVelocity);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets rigid body's current centre of mass.");
            API_PROPERTY(PUBLIC, REGULAR, "Vector3", "CentreOfMass", GETSET, BY_REF);
            {
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_CentreOfMass_Get", &Internal::GetRigidBodyCentreOfMass);
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_CentreOfMass_Set", &Internal::SetRigidBodyCentreOfMass);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets rigid body's current centre of mass.");
            API_PROPERTY(PUBLIC, REGULAR, "float", "MaxAngularVelocity", GETSET);
            {
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_MaxAngularVelocity_Get", &Internal::GetRigidBodyMaxAngularVelocity);
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_MaxAngularVelocity_Set", &Internal::SetRigidBodyMaxAngularVelocity);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets rigid body's current linear damping.");
            API_PROPERTY(PUBLIC, REGULAR, "float", "LinearDamping", GETSET);
            {
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_LinearDamping_Get", &Internal::GetRigidBodyLinearDamping);
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_LinearDamping_Set", &Internal::SetRigidBodyLinearDamping);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets rigid body's current angular damping.");
            API_PROPERTY(PUBLIC, REGULAR, "float", "AngularDamping", GETSET);
            {
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_AngularDamping_Get", &Internal::GetRigidBodyAngularDamping);
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_AngularDamping_Set", &Internal::SetRigidBodyAngularDamping);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets rigid body's current mass.");
            API_PROPERTY(PUBLIC, REGULAR, "float", "Mass", GETSET);
            {
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_Mass_Get", &Internal::GetRigidBodyMass);
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_Mass_Set", &Internal::SetRigidBodyMass);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets rigid body's sleep state.");
            API_PROPERTY(PUBLIC, REGULAR, "bool", "IsSleeping", GETSET);
            {
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_IsSleeping_Get", &Internal::GetRigidBodyIsSleeping);
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_IsSleeping_Set", &Internal::SetRigidBodyIsSleeping);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets rigid body's kinematic state.");
            API_PROPERTY(PUBLIC, REGULAR, "bool", "IsKinematic", GETSET);
            {
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_IsKinematic_Get", &Internal::GetRigidBodyIsKinematic);
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_IsKinematic_Set", &Internal::SetRigidBodyIsKinematic);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets rigid body's CCD (Continuous Collision Detection) state.");
            API_PROPERTY(PUBLIC, REGULAR, "bool", "ContinuousCollisionEnabled", GETSET);
            {
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_ContinuousCollisionEnabled_Get", &Internal::GetRigidBodyCCD);
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_ContinuousCollisionEnabled_Set", &Internal::SetRigidBodyCCD);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets rigid body's dynamic state.");
            API_PROPERTY(PUBLIC, REGULAR, "bool", "IsDynamic", GET);
            {
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_IsDynamic_Get", &Internal::GetRigidBodyIsDynamic);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets rigid body's current collision layer.");
            API_PROPERTY(PUBLIC, REGULAR, "uint", "CollisionLayer", GETSET);
            {
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_CollisionLayer_Get", &Internal::GetRigidBodyCollisionLayer);
                API_BIND("ReCrafted.API.Physics.RigidBodyActor::Internal_CollisionLayer_Set", &Internal::SetRigidBodyCollisionLayer);
            }
            API_PROPERTY_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
