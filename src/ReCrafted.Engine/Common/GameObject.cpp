// ReCrafted (c) 2016-2018 Always Too Late

#include "GameObject.h"
#include "GameObjectPool.h"
#include "Entities/MainWorld.h"

void GameObject::SetupEntity()
{
    var entity = MainWorld::GetWorld()->CreateEntity();

    // Add transform component
    cvar transform = &entity.AddComponent<TransformComponent>();
    m_components.Add(transform);

    m_entity = entity;
}

void GameObject::OnDestroy()
{
    // Destroy children game objects
    for(rvar gameObject : m_children)
    {
        Destroy(gameObject);
    }

    // Release this gameObject
    GameObjectPool::ReleaseGameObject(this);
}

GameObject::GameObject()
{
    SetupEntity();
}

void GameObject::AddChildren(GameObject* children, bool resetPosition, bool resetRotation)
{
    ASSERT(children->m_root == nullptr);
    ASSERT(children->m_parent == nullptr);

    children->m_root = m_root ? m_root : this;
    children->m_parent = this;

    m_children.Add(children);

    if (resetPosition)
        children->SetPosition(Vector3::Zero());

    if (resetRotation)
        children->SetRotation(Vector3::Zero());

    if(resetPosition && resetRotation)
        return;

    if(!resetPosition)
    {
        children->SetPosition(children->GetPosition() + GetPosition());
    }

    if (!resetRotation)
    {
        // TODO: Calculate relative rotation for children (we need to extend our math lib)
    }
}

void GameObject::RemoveChildren(GameObject* children)
{
    m_children.Remove(children);
}

void GameObject::SetActive(bool active)
{
    // TODO: Push
}
