// ReCrafted (c) 2016-2018 Always Too Late

#include "ActorBase.h"
#include "Scene/SceneManager.h"

void ActorBase::OnAcquire()
{
    // Add to scene
    SceneManager::GetInstance()->AddActor(this);
}

void ActorBase::OnRelease()
{
    // We need to unbind the managed object, because we don't want to get 
    // finalized being called on us, because it will also remove our 
    // unmanaged instance. And we don't want this, because we are assigning 
    // managed instances dynamically (when actor is acquired).
    UnbindManaged(this);

    // Remove from scene
    SceneManager::GetInstance()->RemoveActor(this);
}
