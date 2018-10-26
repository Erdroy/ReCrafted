// ReCrafted (c) 2016-2018 Always Too Late

#include "Actor.h"
#include "ActorPool.h"

void Actor::SetEntity(const Entity& entity)
{

}

void Actor::OnDestroy()
{
    // Destroy children actors
    for(rvar actor : m_children)
    {
        Destroy(actor);
    }

    // Release this actor
    ActorPool::ReleaseActor(this);
}

void Actor::AddChildren(Actor* children, bool resetPosition, bool resetRotation)
{
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

void Actor::RemoveChildren(Actor* children)
{
    m_children.Remove(children);
}

void Actor::SetActive(bool active)
{
    // TODO: Push
}
