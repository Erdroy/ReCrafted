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
    if(m_parent)
    {
        // Remove from parent
        SetParent(nullptr);
    }

    // We need to unbind the managed object, because we don't want to get 
    // finalizer being called on us, because it will also remove our 
    // unmanaged instance. And we don't want this, because we are assigning 
    // managed instances dynamically (when actor is acquired).
    UnbindManaged(this);

    // ... and now, we can safely destroy the actor object.
    Object::Destroy(this);

    // Remove from scene
    SceneManager::GetInstance()->RemoveActor(this);
}

void ActorBase::Cleanup(const ActorId_t id)
{
    if (id != 0)
        m_id = id;

    m_static = false;
    m_active = true;
    m_firstFrame = true;
    m_parent = nullptr; 
    m_transform = Transform::Identity;
    m_localTransform = Transform::Identity;

    m_children.Clear();
    m_scripts.Clear();

    SetName(TEXT_CONST("Actor"));
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
            child->LateUpdate();
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
            child->Simulate();
    }

    OnSimulate();
}

void ActorBase::UpdateTransform()
{
    // We need to construct the world transform now.
    // If we have a parent, this gets a bit complicated, because we need to transform the local-space transform to world-space.
    // This is accomplished by using parent's world-space transform and ToWorld function.
    // Otherwise just use the local-space transform as world-space.
    // ... Then broadcast UpdateTransform to all children actors.

    if(m_parent)
    {
        // Transform to world-space by using parent's transform.
        m_transform = m_parent->GetTransform().ToWorld(m_localTransform);
    }
    else
    {
        // Just use the local-space transform as world-space.
        m_transform = m_localTransform;
    }

    // Raise transform change event
    OnTransformChange();

    // Broadcast UpdateTransform
    for(cvar child : m_children)
        child->UpdateTransform();
}

void ActorBase::SetParent(ActorBase* newParent)
{
    MAIN_THREAD_ONLY();

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

    UpdateTransform();

    // Call parent change event
    OnParentChange(m_parent);
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

    script->SetActor(this);
    script->Awake();
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

void ActorBase::SetPosition(const Vector3& position)
{
    MAIN_THREAD_ONLY();

    if (Vector3::NearEqual(m_transform.translation, position))
        return;

    if(m_parent)
    {
        m_localTransform.translation = m_parent->GetTransform().ToLocal(position);
    }
    else
    {
        m_localTransform.translation = position;
    }

    // Update transform
    UpdateTransform();
}

void ActorBase::SetLocalPosition(const Vector3& position)
{
    MAIN_THREAD_ONLY();

    if (Vector3::NearEqual(m_localTransform.translation, position))
        return;

    m_localTransform.translation = position;

    // Update transform
    UpdateTransform();
}

void ActorBase::SetRotation(const Quaternion& rotation)
{
    MAIN_THREAD_ONLY();

    if (Quaternion::NearEqual(m_transform.orientation, rotation))
        return;

    if(m_parent)
    {
        m_localTransform.orientation = m_parent->GetTransform().ToLocal(rotation);
    }
    else
    {
        m_localTransform.orientation = rotation;
    }

    // Update transform
    UpdateTransform();
}

void ActorBase::SetLocalRotation(const Quaternion& rotation)
{
    MAIN_THREAD_ONLY();

    if (Quaternion::NearEqual(m_localTransform.orientation, rotation))
        return;

    m_localTransform.orientation = rotation;

    // Update transform
    UpdateTransform();
}

void ActorBase::SetScale(const Vector3& scale)
{
    MAIN_THREAD_ONLY();

    if (Vector3::NearEqual(m_transform.scale, scale))
        return;

    if (m_parent)
    {
        m_localTransform.scale = scale / m_parent->GetScale();
    }
    else
    {
        m_localTransform.scale = scale;
    }

    // Update transform
    UpdateTransform();
}

void ActorBase::SetLocalScale(const Vector3& scale)
{
    MAIN_THREAD_ONLY();

    if (Vector3::NearEqual(m_localTransform.scale, scale))
        return;

    m_localTransform.scale = scale;

    // Update transform
    UpdateTransform();
}

void ActorBase::SetTransform(const Transform& transform)
{
    MAIN_THREAD_ONLY();

    if(m_parent)
    {
        m_localTransform = m_parent->GetTransform().ToLocal(transform);
    }
    else
    {
        m_localTransform = transform;
    }

    // Update transform
    UpdateTransform();
}
