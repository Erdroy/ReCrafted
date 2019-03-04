// ReCrafted (c) 2016-2019 Always Too Late

#include "CharacterActor.h"
#include "PhysicsManager.h"
#include "Common/Profiler/Profiler.h"

void CharacterActor::Initialize()
{
    m_scene = PhysicsManager::Scene();

    // Create controller
    m_character = m_scene->CreateCharacter(0.35f, 1.75f, 0.08f, 0.0f, 0.01f);

    ASSERT(m_character);
}

void CharacterActor::OnStart()
{
    if (m_character == nullptr)
        Initialize();
}

void CharacterActor::OnUpdate()
{
    ASSERT(m_character);

    Super::SetPosition(m_character->GetPosition());
}

void CharacterActor::OnDestroy()
{
    ASSERT(m_scene);
    ASSERT(m_character);

    m_scene->ReleaseCharacter(m_character);
    m_character = nullptr;
    m_scene = nullptr;
}

CharacterCollisionFlags CharacterActor::Move(const Vector3& displacement)
{
    ASSERT(m_character);

    // Move the character
    cvar flags = m_character->Move(displacement);

    // Set grounded flag
    m_grounded = (flags & IPhysicsCharacter::Down) != 0;

    return static_cast<CharacterCollisionFlags>(flags);
}

void CharacterActor::SetCollisionLayer(const uint32_t layer)
{
    m_character->SetCollisionLayer(layer);
}

void CharacterActor::SetUpDirection(const Vector3& direction) const
{
    MAIN_THREAD_ONLY();
    m_character->SetUpDirection(direction);
}

Vector3 CharacterActor::GetUpDirection() const
{
    MAIN_THREAD_ONLY();
    return m_character->GetUpDirection();
}

void CharacterActor::SetHeight(const float height) const
{
    MAIN_THREAD_ONLY();
    m_character->SetHeight(height);
}

float CharacterActor::GetHeight() const
{
    MAIN_THREAD_ONLY();
    return m_character->GetHeight();
}

void CharacterActor::SetRadius(const float radius) const
{
    MAIN_THREAD_ONLY();
    m_character->SetRadius(radius);
}

float CharacterActor::GetRadius() const
{
    MAIN_THREAD_ONLY();
    return m_character->GetRadius();
}

void CharacterActor::SetStepOffset(const float stepOffset) const
{
    MAIN_THREAD_ONLY();
    m_character->SetStepOffset(stepOffset);
}

float CharacterActor::GetStepOffset() const
{
    MAIN_THREAD_ONLY();
    return m_character->GetStepOffset();
}

void CharacterActor::SetContactOffset(const float contactOffset) const
{
    MAIN_THREAD_ONLY();
    m_character->SetContactOffset(contactOffset);
}

float CharacterActor::GetContactOffset() const
{
    MAIN_THREAD_ONLY();
    return m_character->GetContactOffset();
}

void CharacterActor::SetSlopeLimit(const float slopeAngle) const
{
    MAIN_THREAD_ONLY();
    m_character->SetSlopeLimit(Math::Clamp(slopeAngle, 0.0f, 90.0f));
}

float CharacterActor::GetSlopeLimit() const
{
    MAIN_THREAD_ONLY();
    return m_character->GetSlopeLimit();
}

Vector3 CharacterActor::GetVelocity() const
{
    MAIN_THREAD_ONLY();
    return m_character->GetVelocity();
}

IPhysicsCharacter* CharacterActor::GetPhysicsCharacter() const
{
    MAIN_THREAD_ONLY();
    return m_character;
}

bool CharacterActor::IsGrounded() const
{
    MAIN_THREAD_ONLY();
    return m_grounded;
}

void CharacterActor::SetPosition(const Vector3& position)
{
    Super::SetPosition(position);

    // Update actor transform
    m_character->SetPosition(position);
}

void CharacterActor::SetRotation(const Quaternion& rotation)
{
    Super::SetRotation(rotation);

    // Update actor transform
    //m_character->SetRotation(rotation);
}

CharacterActor* CharacterActor::Create()
{
    MAIN_THREAD_ONLY();
    cvar actor = Super::Create();
    actor->Initialize();
    return actor;
}
