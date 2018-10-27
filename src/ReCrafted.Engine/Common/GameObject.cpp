// ReCrafted (c) 2016-2018 Always Too Late

#include "GameObject.h"
#include "GameObjectPool.h"
#include "Entities/MainWorld.h"

void GameObject::SetupEntity()
{
    var entity = MainWorld::GetWorld()->CreateEntity();

    // Add transform component
    m_transform = &entity.AddComponent<TransformComponent>();

    // Set entity reference
    m_entity = entity;
}

void GameObject::Cleanup()
{
    // Cleanup transform
    *m_transform = TransformComponent();

    // Remove all scripts
    for(crvar script : m_scripts)
    {
        // TODO: Remove scripts
    }

    // Remove all components (beside transform)
    for(crvar component : m_components)
    {
        m_entity.RemoveComponent(component);
    }

    SetActive(false);

    m_parent = nullptr;
    m_root = nullptr;
}

void GameObject::OnDestroy()
{
    // Destroy children game objects
    for (rvar gameObject : m_children)
    {
        Destroy(gameObject);
    }

    // Deactivate entity
    m_entity.Deactivate();

    // Release this gameObject
    GameObjectPool::ReleaseGameObject(this);
}

void GameObject::OnParentChangeActive(const bool active)
{
    // Update children
    for (rvar child : m_children)
    {
        child->OnParentChangeActive(active);
    }

    if(!m_active)
        return;

    // Activate or Deactive ECS entity
    if(active)
        m_entity.Activate();

    if(!active)
        m_entity.Deactivate();
}

GameObject::GameObject()
{
    SetupEntity();
}

void GameObject::AddChildren(GameObject* gameObject, const bool resetPosition, const bool resetRotation)
{
    ASSERT(gameObject->m_root == nullptr);
    ASSERT(gameObject->m_parent == nullptr);

    gameObject->m_root = m_root ? m_root : this;
    gameObject->m_parent = this;

    m_children.Add(gameObject);

    if (resetPosition)
        gameObject->SetPosition(GetPosition());

    if (resetRotation)
        gameObject->SetRotation(GetRotation());

    if(resetPosition && resetRotation)
        return;

    if(!resetPosition)
    {
        gameObject->SetPosition(gameObject->GetPosition() + GetPosition());
    }

    if (!resetRotation)
    {
        // TODO: Calculate relative rotation for children (we need to extend our math lib)
    }
}

void GameObject::RemoveChildren(GameObject* gameObject)
{
    ASSERT(m_children.Contains(gameObject));
    m_children.Remove(gameObject);

    gameObject->m_root = nullptr;
    gameObject->m_parent = nullptr;
}

void GameObject::SetActive(const bool active)
{
    if (m_active == active)
        return;

    // Set new active state
    m_active = active;

    // Update children
    OnParentChangeActive(active);

    // Activate or deactivate ECS entity
    if (active)
        m_entity.Activate();
    else
        m_entity.Deactivate();
}

bool GameObject::IsActive() const
{
    return m_active;
}

TransformComponent* GameObject::GetTransform() const
{
    ASSERT(m_transform);
    return m_transform;
}

void GameObject::SetPosition(const Vector3& position)
{
    DEBUG_ASSERT(m_transform);

    if(!m_children.Empty())
    {
        // Update relative children position to this gameObject
        for (rvar child : m_children)
            child->SetPosition(child->GetPosition() + (position - GetPosition()));
    }

    // Set position
    m_transform->position = position;
}

const Vector3& GameObject::GetPosition() const
{
    DEBUG_ASSERT(m_transform);
    return m_transform->position;
}

void GameObject::SetRotation(const Vector3& rotation)
{
    // TODO: Update relative children rotation to this gameObject

    // Set rotation
    m_transform->rotation = rotation;
}

const Vector3& GameObject::GetRotation() const
{
    DEBUG_ASSERT(m_transform);
    return m_transform->rotation;
}
