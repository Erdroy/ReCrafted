// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef CHARACTERACTOR_H
#define CHARACTERACTOR_H

#include "ReCrafted.h"
#include "Common/Actors/Actor.h"
#include "Physics/IPhysicsScene.h"
#include "Physics/IPhysicsCharacter.h"

enum class CharacterCollisionFlags
{
    None = 0,
    Sides = 1 << 0,
    Up = 1 << 1,
    Down = 1 << 2
};

class CharacterActor : public Actor<CharacterActor>
{
private:
    SCRIPTING_API_IMPL()

private:
    IPhysicsScene* m_scene = nullptr;
    IPhysicsCharacter* m_character = nullptr;

    bool m_grounded = false;

private:
    void Initialize();
    
    void OnStart() override;
    void OnUpdate() override;
    void OnDestroy() override;

    const char* GetObjectName() const override
    {
        return "CharacterActor";
    }

public:
    void SetPosition(const Vector3& position) override;
    void SetRotation(const Quaternion& rotation) override;

public:
    /**
     * \brief Moves this character along given displacement vector.
     * \param displacement The displacement vector.
     * \return The collision flags.
     */
    CharacterCollisionFlags Move(const Vector3& displacement);

public:
    void SetCollisionLayer(uint32_t layer);

    void SetUpDirection(const Vector3& direction) const;
    Vector3 GetUpDirection() const;

    void SetHeight(float height) const;
    float GetHeight() const;

    void SetRadius(float radius) const;
    float GetRadius() const;

    void SetStepOffset(float stepOffset) const;
    float GetStepOffset() const;

    void SetContactOffset(float contactOffset) const;
    float GetContactOffset() const;

    void SetSlopeLimit(float slopeAngle) const;
    float GetSlopeLimit() const;

    Vector3 GetVelocity() const;

    IPhysicsCharacter* GetPhysicsCharacter() const;

public:
    bool IsGrounded() const;

public:
    static CharacterActor* Create();
};

#endif // CHARACTERACTOR_H
