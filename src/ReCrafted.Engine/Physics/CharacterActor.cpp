// ReCrafted (c) 2016-2018 Always Too Late

#include "CharacterActor.h"
#include "PhysicsManager.h"

void CharacterActor::Initialize()
{
    m_scene = PhysicsManager::Scene();

    // Create controller
    m_character = m_scene->CreateCharacter(0.35f, 1.75f, 0.08f, 35.0f, 0.01f);

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
    m_character->SetRotation(rotation);
}

CharacterActor* CharacterActor::Create()
{
    cvar actor = Super::Create();
    actor->Initialize();
    return actor;
}
