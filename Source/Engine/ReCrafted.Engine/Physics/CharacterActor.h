// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#ifndef CHARACTERACTOR_H
#define CHARACTERACTOR_H

#include <ReCrafted.h>
#include "Core/Actors/ActorBase.h"
#include "Physics/PhysX.h"
#include "Physics/PhysicsScene.h"

API_USING("ReCrafted.API.Core.Actors")
API_USING("ReCrafted.API.Mathematics")

enum class CharacterCollisionFlags
{
    None = 0,
    Sides = 1 << 0,
    Up = 1 << 1,
    Down = 1 << 2
};

/// <summary>
///     RigidBodyActor actor class.
/// </summary>
API_CLASS(public, sealed)
class CharacterActor final : public ActorBase
{
    API_CLASS_BODY()
    friend class PhysicsManager;
    friend class PhysicsScene;

private:
    uint32_t m_collisionLayer = 0u;

    PhysicsScene* m_scene = nullptr;
    PxCapsuleController* m_controller = nullptr;
    bool m_grounded = false;

    float m_height = 1.75f;
    float m_radius = 0.35f;
    float m_stepOffset = 0.08f;
    float m_slopeLimit = 0.0f;
    float m_contactOffset = 0.01f;

public:
    ACTOR_BODY(CharacterActor)
    ~CharacterActor();

protected:
    void OnAwake() override;
    void OnUpdate() override;

public:
    void Position(const Vector3& position) override;
    void Rotation(const Quaternion& rotation) override;

public:
    /// <summary>
    ///     Moves this character along given displacement vector.
    /// </summary>
    /// <param name="displacement">The displacement vector.</param>
    /// <returns>The collision flags.</returns>
    API_FUNCTION()
    CharacterCollisionFlags Move(const Vector3& displacement);

public:
    /// <summary>
    ///     Gets or sets the collision layer.
    /// </summary>
    API_PROPERTY()
    void CollisionLayer(uint32_t layer);

    /// <summary>
    ///     Gets or sets the collision layer.
    /// </summary>
    API_PROPERTY()
    uint32_t CollisionLayer() const;

    /// <summary>
    ///     Gets or sets the up direction.
    /// </summary>
    API_PROPERTY()
    void UpDirection(const Vector3& direction) const;

    /// <summary>
    ///     Gets or sets the up direction.
    /// </summary>
    API_PROPERTY()
    Vector3 UpDirection() const;

    /// <summary>
    ///     Gets or sets the height.
    /// </summary>
    API_PROPERTY()
    void Height(float height);

    /// <summary>
    ///     Gets or sets the height.
    /// </summary>
    API_PROPERTY()
    float Height() const;

    /// <summary>
    ///     Gets or sets the radius.
    /// </summary>
    API_PROPERTY()
    void Radius(float radius);

    /// <summary>
    ///     Gets or sets the radius.
    /// </summary>
    API_PROPERTY()
    float Radius() const;

    /// <summary>
    ///     Gets or sets the step offset.
    /// </summary>
    API_PROPERTY()
    void StepOffset(float stepOffset);
    
    /// <summary>
    ///     Gets or sets the step offset.
    /// </summary>
    API_PROPERTY()
    float StepOffset() const;

    /// <summary>
    ///     Gets or sets the contact offset.
    /// </summary>
    API_PROPERTY()
    void ContactOffset(float contactOffset);

    /// <summary>
    ///     Gets or sets the contact offset.
    /// </summary>
    API_PROPERTY()
    float ContactOffset() const;

    /// <summary>
    ///     Gets or sets the slope limit.
    /// </summary>
    API_PROPERTY()
    void SlopeLimit(float slopeLimit);

    /// <summary>
    ///     Gets or sets the slope limit.
    /// </summary>
    API_PROPERTY()
    float SlopeLimit() const;

    /// <summary>
    ///     Gets the velocity.
    /// </summary>
    API_PROPERTY()
    Vector3 Velocity() const;

public:
    bool IsGrounded() const;

};

#endif // CHARACTERACTOR_H
