// ReCrafted (c) 2016-2018 Always Too Late

#include "ActorBase.h"
#include "Scene/SceneManager.h"

void ActorBase::OnAcquire()
{
    // Add to scene
    SceneManager::GetInstance()->AddActor(this);

    // Call awake event
    OnAwake();
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

void ActorBase::Start()
{
    for (rvar script : m_scripts)
    {
        if (script->GetEnabled())
            script->Start();
    }

    for (rvar child : m_children)
    {
        if (child->IsActive())
            child->Start();
    }

    OnStart();
}

void ActorBase::Update()
{
    if (m_firstFrame)
    {
        Start();
        m_firstFrame = false;
    }

    for (rvar script : m_scripts)
    {
        if (script->GetEnabled())
            script->Update();
    }

    for (rvar child : m_children)
    {
        if (child->IsActive())
            child->Update();
    }

    OnUpdate();
}

void ActorBase::LateUpdate()
{
    for (rvar script : m_scripts)
    {
        if (script->GetEnabled())
            script->LateUpdate();
    }

    for (rvar child : m_children)
    {
        if (child->IsActive())
            child->Update();
    }

    OnLateUpdate();
}

void ActorBase::Simulate()
{
    for (rvar script : m_scripts)
    {
        if (script->GetEnabled())
            script->Simulate();
    }

    for (rvar child : m_children)
    {
        if (child->IsActive())
            child->Update();
    }

    OnSimulate();
}

void ActorBase::SetParent(ActorBase* newParent)
{
    MAIN_THREAD_ONLY();
    ASSERT(newParent);

    if (m_parent)
    {
        // Remove this from current parent
        m_parent->m_children.Remove(this);
        
        // Call remove child event
        m_parent->OnRemovedChild(this);

        // Return to the scene
        SceneManager::GetInstance()->AddActor(this);

        // Set parent as null
        m_parent = nullptr;
    }

    if(newParent)
    {
        // Set new parent of this actor
        m_parent = newParent;

        // Add this to the new parent
        m_parent->m_children.Add(this);

        // Call added child event
        m_parent->OnAddedChild(this);

        // Remove from the scene
        SceneManager::GetInstance()->RemoveActor(this);
    }

    // Call parent change event
    OnParentChange(m_parent);

    // TODO: Update transform
}

void ActorBase::AddChild(ActorBase* child)
{
    MAIN_THREAD_ONLY();
    ASSERT(child);
    ASSERT(child->m_parent == nullptr);

    // Set this actor as parent for the child
    child->SetParent(this);
}

void ActorBase::RemoveChild(ActorBase* child)
{
    MAIN_THREAD_ONLY();
    ASSERT(child);
    ASSERT(child->m_parent == this);

    // Reset parent to null pointer
    child->SetParent(nullptr);
}

void ActorBase::AddScript(Script* script)
{
    MAIN_THREAD_ONLY();
    ASSERT(script->GetActor() == nullptr);

    script->Awake();
    script->SetActor(this);
    m_scripts.Add(script);
}

void ActorBase::RemoveScript(Script* script)
{
    MAIN_THREAD_ONLY();
    ASSERT(script->GetActor() == this);

    script->SetActor(nullptr);
    m_scripts.Remove(script);

    // Destroy script
    Object::Destroy(script);
}

void ActorBase::SetActive(const bool active)
{
    MAIN_THREAD_ONLY();

    if (m_active == active)
        return;

    // Call enable or disable event
    if (m_active)
        OnEnable();
    else
        OnDisable();

    // Set state
    m_active = active;
    
    // Call children event
    for (rvar child : m_children)
    {
        if (child->IsActive())
            child->OnParentActiveChange(active);
    }
}

