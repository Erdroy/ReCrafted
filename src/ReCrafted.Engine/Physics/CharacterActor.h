// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef CHARACTERACTOR_H
#define CHARACTERACTOR_H

#include "ReCrafted.h"
#include "Common/Actor.h"
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

public:
    CharacterCollisionFlags Move(const Vector3& displacement);

public:
    void SetUpDirection(const Vector3& direction) const
    {
        m_character->SetUpDirection(direction);
    }

    Vector3 GetUpDirection() const
    {
        return m_character->GetUpDirection();
    }

    void SetHeight(const float height) const
    {
        m_character->SetHeight(height);
    }
    float GetHeight() const
    {
        return m_character->GetHeight();
    }

    void SetRadius(const float radius) const
    {
        m_character->SetRadius(radius);
    }

    float GetRadius() const
    {
        return m_character->GetRadius();
    }

    void SetStepOffset(const float stepOffset) const
    {
        m_character->SetStepOffset(stepOffset);
    }

    float GetStepOffset() const
    {
        return m_character->GetStepOffset();
    }

    void SetContactOffset(const float contactOffset) const
    {
        m_character->SetContactOffset(contactOffset);
    }

    float GetContactOffset() const
    {
        return m_character->GetContactOffset();
    }

    void SetSlopeLimit(const float slopeAngle) const
    {
        m_character->SetSlopeLimit(slopeAngle);
    }

    float GetSlopeLimit() const
    {
        return m_character->GetSlopeLimit();
    }

    void SetPosition(const Vector3& position) override;
    void SetRotation(const Quaternion& rotation) override;

    Vector3 GetVelocity() const;

    IPhysicsCharacter* GetPhysicsCharacter() const
    {
        return m_character;
    }

public:
    bool IsGrounded() const
    {
        return m_grounded;
    }

public:
    static CharacterActor* Create();
};

#endif // CHARACTERACTOR_H
