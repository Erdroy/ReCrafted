// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef RIGIDBODYACTOR_H
#define RIGIDBODYACTOR_H

#include "ReCrafted.h"
#include "Common/Actors/Actor.h"
#include "Physics/Collision.h"
#include "Physics/IPhysicsActor.h"

/**
 * \brief RigidBody physics actor.
 * 
 * \note To create actor of this type use RigidBodyActor::Create* functions.
 */
class RigidBodyActor final : public Actor<RigidBodyActor>
{
private:
    SCRIPTING_API_IMPL()

private:
    IPhysicsActor* m_actor = nullptr;
    Collision* m_collision = nullptr;
    bool m_dynamic = true;

private:
    void Initialize();

    void OnStart() override;
    void OnUpdate() override;
    void OnDestroy() override;

    const char* GetObjectName() const override
    {
        return "RigidBodyActor";
    }

public:
    void AttachCollision(Collision* collision);
    void DetachCollision();

public:
    void SetPosition(const Vector3& position) override;
    void SetRotation(const Quaternion& rotation) override;

    void SetVelocity(const Vector3& velocity);
    Vector3 GetVelocity();

public:
    /**
     * \brief Creates and initializes new dynamic actor.
     * \return The newly created dynamic actor.
     */
    static RigidBodyActor* Create();

    /**
     * \brief Creates and initializes new dynamic actor.
     * \return The newly created dynamic actor.
     */
    static RigidBodyActor* CreateDynamic();

    /**
     * \brief Creates and initializes new static actor.
     * \return The newly created static actor.
     */
    static RigidBodyActor* CreateStatic();
};

#endif // RIGIDBODYACTOR_H